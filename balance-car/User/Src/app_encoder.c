/**
  ******************************************************************************
  * @file    app_encoder.c
  * @brief   编码器模块 —— 位置测量 + T法测速 + 占空比校准
  *
  * 引脚：左电机编码器 A相 PB14 (EXTI14)  B相 PB15 (普通输入)
  *       右电机编码器 A相 PB3  (EXTI3)   B相 PB4  (普通输入)
  *
  * 移植说明（标准库 -> HAL）：
  *   1. 引脚的时钟/模式/上下拉、EXTI 触发沿、NVIC 优先级全部搬到
  *      MX_GPIO_Init()（CubeMX 风格）。原来 Encoder_L_Init/Encoder_R_Init
  *      里那一大堆 RCC_APB2PeriphClockCmd + GPIO_Init + GPIO_EXTILineConfig
  *      + EXTI_Init + NVIC_Init 都不再需要。
  *   2. 注意 EXTI 的触发方式必须是「双边沿」(RISING_FALLING)：
  *      T法测速靠 A 相上升沿和下降沿之间的时间差算速度，少了任一边沿都会算错。
  *   3. 中断服务函数写在最下面（HAL 里叫 GPIO 外部中断，标志位通过
  *      EXTI->PR 管理，用 __HAL_GPIO_EXTI_GET_IT/CLEAR_IT 访问）。
  ******************************************************************************
  */

#include "app_encoder.h"
#include "delay.h"
#include "math.h"
#include "app_calibrator.h"

static volatile int64_t encoder_l = 0; // 左电机编码器的值
static volatile int64_t encoder_r = 0; // 右电机编码器的值
static volatile int8_t d0_l = 0, d1_l = 0; // 左电机旋转的方向，0 - 初始状态，+-1234对应4个阶段
static volatile int8_t d0_r = 0, d1_r = 0; // 右电机旋转的方向，0 - 初始状态，+-1234对应4个阶段
static volatile uint64_t t0_l = 0, t1_l = 0; // 左电机编码器发生变化的时间，单位us
static volatile uint64_t t0_r = 0, t1_r = 0; // 右电机编码器发生变化的时间，单位us
static float m_l[2], m_r[2];

// 校准用
static volatile int8_t calibrateFlag_l = 0; // 校准开始标志位，0-校准停止，1-校准准备，2-校准开始，-1 - 校准失败
static volatile uint64_t lastEdge_l = 0; // 用于记录上次边沿发生的时间（校准用）
static volatile uint64_t t1_cali_l = 0, t2_cali_l = 0;
static volatile uint16_t n1_cali_l = 0, n2_cali_l = 0;
// 校准用
static volatile int8_t calibrateFlag_r = 0; // 校准开始标志位，0-校准停止，1-校准准备，2-校准开始，-1 - 校准失败
static volatile uint64_t lastEdge_r = 0; // 用于记录上次边沿发生的时间（校准用）
static volatile uint64_t t1_cali_r = 0, t2_cali_r = 0;
static volatile uint16_t n1_cali_r = 0, n2_cali_r = 0;

//
// @简介：对编码器模块进行初始化
//
// @注意：引脚与 EXTI 的配置已经在 MX_GPIO_Init() 里做好了，
//        这里只需要根据校准结果算出两个「台阶高度」。
//
void App_Encoder_Init(void)
{
	// 从校准器中读出占空比
	float duty_l = App_Calibrator_GetResult()->encoder_duty_l;
	float duty_r = App_Calibrator_GetResult()->encoder_duty_r;

	// 计算台阶高度
	// 左编码器台阶高度
	// m_l[0] - 第1阶段（从A相上升沿到A相下降沿）的台阶高度
	// m_1[1] - 第2阶段（从A相下降沿到A相上升沿）的台阶高度
	m_l[0] = duty_l * 2; // m[]表示台阶高度，占空比*2，比如占空比为0.49，则m[0]=0.98,m[1]=1.02
	m_l[1] = 2 - duty_l * 2;

	// 右编码器台阶高度
	m_r[0] = duty_r * 2;
	m_r[1] = 2 - duty_r * 2;
}

//
// @简介：读取左轮胎旋转的角度，单位：度
//
float App_Encoder_GetPos_L(void)
{
	// return encoder_l / 22.0f / (30613.0f / 1500.0f) * 2*PI;
	return encoder_l * 0.01399402208920360588844895090594f;
}

//
// @简介：读取右轮胎旋转的角度，单位：度
//
float App_Encoder_GetPos_R(void)
{
	// return encoder_r / 22.0f / (30613.0f / 1500.0f) * 360.0f;
	return -encoder_r * 0.01399402208920360588844895090594f;
}

//
// @简介：读取左轮胎旋转的角速度，omega的值，单位是 度/s
//
float App_Encoder_GetSpeed_L(void)
{
	__disable_irq(); // 为了保持数据完整性，关闭单片机的总中断

	// 拷贝传感器数据
	int8_t d0_cpy = d0_l; // 上一个时刻编码器方向及阶段
	int8_t d1_cpy = d1_l; // 上上个时刻编码器方向及阶段
	uint64_t t0_cpy = t0_l; // 上一个时刻
	uint64_t t1_cpy = t1_l; // 上上个时刻

	__enable_irq(); // 开启单片机的总中断

	if(d0_cpy * d1_cpy <= 0) return 0.0f; // 方向改变时强制令速度为0

	uint64_t now = GetUs(); // 获取当前时间

	int8_t dnow; // 当前方向

	// 推算当前阶段和方向
	if(d0_cpy > 0)
	{
		dnow = d0_cpy % 2 + 1; // 1->1%2+1=2 2->2%2+1=1
	}
	else
	{
		dnow = -((-d0_cpy) % 2 +1);
	}

	float M,T; // M表示台阶高度，T表示台阶宽度

	float Mnow = dnow > 0 ? m_l[dnow-1]   : -m_l[-dnow-1];
	float M0 = d0_cpy > 0 ? m_l[d0_cpy-1] : -m_l[-d0_cpy-1];

	if((fabsf(Mnow)*(t0_cpy-t1_cpy)) < fabsf(M0)*(now - t0_cpy)) // 速度减慢
	{
		M = Mnow;
		T = (now - t0_cpy)  * 1.0e-6f;
	}
	else // 速度不变或者加快
	{
		M = M0;
		T = (t0_cpy-t1_cpy) * 1.0e-6f;
	}

	return M / T * 0.01399402208920360588844895090594f;
}


//
// @简介：读取右轮胎旋转的角速度，omega的值，单位是 度/s
//
float App_Encoder_GetSpeed_R(void)
{
	__disable_irq(); // 关闭单片机的总中断

	int8_t d0_cpy = d0_r;
	int8_t d1_cpy = d1_r;
	uint64_t t0_cpy = t0_r;
	uint64_t t1_cpy = t1_r;

	__enable_irq(); // 开启单片机的总中断

	if(d0_cpy * d1_cpy <= 0) return 0.0f; // 方向改变时强制令速度为0

	uint64_t now = GetUs();
	int8_t dnow; // 当前方向

	if(d0_cpy > 0)
	{
		dnow = d0_cpy % 2 + 1; // 1->1%2+1=2 2->2%2+1=1
	}
	else
	{
		dnow = -((-d0_cpy) % 2 +1);
	}

	float M,T;

	float Mnow = dnow > 0 ? m_r[dnow-1]   : -m_r[-dnow-1];
	float M0 = d0_cpy > 0 ? m_r[d0_cpy-1] : -m_r[-d0_cpy-1];

	if((fabsf(Mnow)*(t0_cpy-t1_cpy)) < fabsf(M0)*(now - t0_cpy))
	{
		M = Mnow;
		T = (now - t0_cpy)  * 1.0e-6f;
	}
	else
	{
		M = M0;
		T = (t0_cpy-t1_cpy) * 1.0e-6f;
	}

	return -M / T * 0.01399402208920360588844895090594f;
}

//
// @简介：EXTI3的中断响应函数，对应右编码器的A相
//
void EXTI3_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3); // 对标志位进行清零

	uint8_t a = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3); // A相的当前电压
	uint8_t b = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4); // B相的当前电压

	uint64_t now = GetUs();

	t1_r = t0_r;
	t0_r = now;
	d1_r = d0_r;

	if(a == GPIO_PIN_SET) // A相上升沿
	{
		if(b == GPIO_PIN_SET) // 正转，1
		{
			d0_r = 2;
			encoder_r++;

			if(calibrateFlag_r == 1) // 校准准备中
			{
				calibrateFlag_r = 2; // 遇到1号边沿即开始校准

				// 对所有数据清零
				t1_cali_r = 0; t2_cali_r = 0;
				n1_cali_r = 0; n2_cali_r = 0;
				lastEdge_r = now;
			}
			else if(calibrateFlag_r == 2) // 遇到1号边沿，累加t4
			{
				t2_cali_r += now - lastEdge_r;
				n2_cali_r++;
				lastEdge_r = now;
			}
		}
		else // 反转
		{
			d0_r = -2;
			encoder_r--;

			if(calibrateFlag_r == 1 || calibrateFlag_r == 2)
			{
				calibrateFlag_r = -1; // 遇电机反转则校准失败
			}
		}
	}
	else // A相下降沿
	{
		if(b == GPIO_PIN_RESET) // 正转，3
		{
			encoder_r++;
			d0_r = 1;

			if(calibrateFlag_r == 2) // 遇到1号边沿，累加t1
			{
				t1_cali_r += now - lastEdge_r;
				n1_cali_r++;
				lastEdge_r = now ;
			}
		}
		else // 反转
		{
			encoder_r--;
			d0_r = -1;

			if(calibrateFlag_r == 1 || calibrateFlag_r == 2)
			{
				calibrateFlag_r = -1; // 遇电机反转则校准失败
			}
		}
	}
}

//
// @简介：EXTI15_10的中断响应函数，对应左编码器的A相
//
void EXTI15_10_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != 0)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14); // 对标志位进行清零

		uint8_t a = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14); // A相的当前电压
		uint8_t b = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15); // B相的当前电压

		uint64_t now = GetUs();

		t1_l = t0_l;
		t0_l = now;
		d1_l = d0_l;

		if(a == GPIO_PIN_SET) // A相上升沿
		{
			if(b == GPIO_PIN_SET) // 正转，1
			{
				d0_l = 2;
				encoder_l++;

				if(calibrateFlag_l == 1) // 校准准备中
				{
					calibrateFlag_l = 2; // 遇到1号边沿即开始校准

					// 对所有数据清零
					t1_cali_l = 0; t2_cali_l = 0;
					n1_cali_l = 0; n2_cali_l = 0;
					lastEdge_l = now;
				}
				else if(calibrateFlag_l == 2) // 累加t2
				{
					t2_cali_l += now - lastEdge_l;
					n2_cali_l++;
					lastEdge_l = now;
				}
			}
			else // 反转
			{
				d0_l = -2;
				encoder_l--;

				if(calibrateFlag_l == 1 || calibrateFlag_l == 2)
				{
					calibrateFlag_l = -1; // 遇电机反转则校准失败
				}
			}
		}
		else // A相下降沿
		{
			if(b == GPIO_PIN_RESET) // 正转，3
			{
				encoder_l++;
				d0_l = 1;

				if(calibrateFlag_l == 2) // 累加t1
				{
					t1_cali_l += now - lastEdge_l;
					n1_cali_l++;
					lastEdge_l = now;
				}
			}
			else // 反转
			{
				encoder_l--;
				d0_l = -1;

				if(calibrateFlag_l == 1 || calibrateFlag_l == 2)
				{
					calibrateFlag_l = -1; // 遇电机反转则校准失败
				}
			}
		}
	}
}

void App_Encoder_StartCalibration(void)
{
	calibrateFlag_l = 1; // 准备校准
	calibrateFlag_r = 1; // 准备校准

	// 复位所有校准临时数据

	t1_cali_l = 0; t2_cali_l = 0;
	n1_cali_l = 0; n2_cali_l = 0;
	lastEdge_l = 0;

	t1_cali_r = 0; t2_cali_r = 0;
	n1_cali_r = 0; n2_cali_r = 0;
	lastEdge_r = 0;
}

int App_Encoder_EndCalibration(float *duty_l, float *duty_r)
{
	int ret = 0;

	__disable_irq();

	if(calibrateFlag_l < 0 || calibrateFlag_r < 0)
	{
		ret = -1;
	}

	calibrateFlag_l = 0; // 结束校准
	calibrateFlag_r = 0; // 结束校准

	__enable_irq();

	if(ret == 0)
	{
		float t1_l = ((float)t1_cali_l) / ((float)n1_cali_l);
		float t2_l = ((float)t2_cali_l) / ((float)n2_cali_l);

		float t1_r = ((float)t1_cali_r) / ((float)n1_cali_r);
		float t2_r = ((float)t2_cali_r) / ((float)n2_cali_r);

		*duty_l = t1_l / (t1_l + t2_l);
		*duty_r = t1_r / (t1_r + t2_r);
	}
	else
	{
		*duty_l = 0.5;
		*duty_r = 0.5;
	}

	return ret;
}
