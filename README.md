# STM32 学习记录

从 C 语言基础到 STM32 平衡小车的个人学习记录。硬件是 **STM32F103C8T6**（蓝药丸最小系统板），
开发环境是 **macOS + VS Code（EIDE / PlatformIO）+ ARM GCC + J-Link**。

这个仓库只放我自己写和改的代码与文档，用来留档学习轨迹，也欢迎后来的自学者参考。

---

## 目录结构

| 目录 | 内容 |
|---|---|
| [`c-language/`](c-language/) | C 语言基础练习。从变量、运算符、分支循环，到函数、指针、结构体、数组与排序，每个知识点一个 `.c` 文件，都带中文注释 |
| [`register-blink/`](register-blink/) | 裸机寄存器版点灯。不依赖 HAL，直接操作寄存器，用来理解「点灯到底发生了什么」 |
| [`hal/`](hal/) | CubeMX + HAL 库外设练习，一个功能一个工程：GPIO、按键消抖、自写延时、PWM 呼吸灯、SPI、I2C、ADC 定时器触发 / 规则通道转换等。附带 `_模板/`（EIDE 工程的初始化脚本和配置模板） |
| [`balance-car/`](balance-car/) | 平衡小车主项目（HAL 版）。含编码器测速、PWM 调速、电池 ADC 采集、串口通信、PID 等模块，以及[烧录说明](balance-car/烧录说明.md)和[学习路线图](balance-car/学习路线图.md) |

### `balance-car/` 内部结构

```
balance-car/
├── Core/           CubeMX 生成的骨架 + main.c 主逻辑
├── MyLib/      自写基础库：延时 delay、按键 button、定点数学 qmath
├── User/       业务层：app_bat 电池 / app_button 按键 / app_encoder 编码器
│                / app_pwm 电机调速 / app_usart2 串口
├── Test/       各模块的测试代码
├── balance_car_hal.ioc    CubeMX 工程配置（引脚、外设、时钟都在这里）
├── platformio.ini         PlatformIO 构建配置
└── 烧录说明.md / 学习路线图.md
```

---

## 学习进度

对照课程章节，做过一节勾一节：

- [x] 点灯（寄存器版 + HAL 版）
- [x] 自实现毫秒延时、按键消抖
- [x] PWM 呼吸灯
- [x] SPI / I2C 通信
- [x] ADC 采集（规则通道 + 定时器触发）
- [x] 电池电压采集与电源指示
- [x] 电机 PWM 调速
- [x] 编码器位置测量 → 减速比 → M 法测速 → T 法测速
- [ ] MPU6050 姿态解算
- [ ] PID 控制器
- [ ] 倒立摆串级 PID
- [ ] 速度环
- [ ] 遥控器

---

## 怎么编译

**`hal/` 里的工程**：用 STM32CubeMX 打开对应的 `.ioc` 重新生成代码，或用 Keil MDK 打开工程。

**`balance-car/`**：装好 PlatformIO 后，在 `balance-car/` 目录下：

```bash
pio run        # 编译
pio run -t upload   # 烧录（需要接好 ST-Link / J-Link）
```

**`c-language/`**：随便哪个 C 编译器都行，例如

```bash
gcc 冒泡排序.c -o 冒泡排序 && ./冒泡排序
```

---

## 关于 `Drivers/` 目录

这个仓库**不包含** `Drivers/`（ST 官方的 HAL 驱动和 CMSIS 库），原因有两个：

1. 它是厂商代码，不是我的学习内容，而且每份 60MB 以上，多个工程里完全重复；
2. 它可以随时重新获取——CubeMX 重新生成一下就有，PlatformIO 的 `framework = stm32cube` 也会自动下载。

所以 clone 下来想编译的话，先用 CubeMX 打开 `.ioc` 生成一遍即可。

---

## 声明

本仓库仅包含本人学习过程中编写和修改的代码与文档，
**不包含**任何课程课件、视频、配套资料压缩包或厂商库源码。
课程内容仅作个人学习参考。
