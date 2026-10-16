/*
 * main.c —— STM32F103C8T6 最小系统板 板载 LED 闪烁
 *
 * 板载 LED 接在 PC13（蓝色药丸/最小系统板的通用接法），低电平点亮。
 * 这里用 BSRR 直接翻转，不关心极性 —— 无论高电平还是低电平点亮，
 * 都会看到 LED 在闪。
 *
 * 这是"裸机寄存器版"：不依赖 HAL、不依赖标准库外设驱动，
 * 只靠 4 个寄存器就能跑。目的是把 EIDE 的完整流程跑通。
 *
 * 时钟：没有配置 PLL，MCU 跑在内部 HSI 8MHz。
 * 所以能看到闪 —— 但闪烁频率不精确，只作演示。
 */

#include <stdint.h>

/* ---------------------------------------------------------------------------
 * 用到的几个寄存器（STM32F103 参考手册 RM0008）
 * 手工定义，省掉引入 CMSIS 头文件
 * ------------------------------------------------------------------------- */

/* RCC_APB2ENR —— APB2 外设时钟使能，偏移 0x18 */
#define RCC_APB2ENR     (*(volatile uint32_t *)0x40021018UL)
#define RCC_IOPCEN      (1UL << 4)          /* bit4：GPIOC 时钟使能 */

/* GPIOC_CRH —— 端口配置高寄存器，管 PC8~PC15，偏移 0x04 */
#define GPIOC_CRH       (*(volatile uint32_t *)0x40011004UL)

/* GPIOC_BSRR —— 位设置/清除寄存器，偏移 0x10
 * 低 16 位写 1 = 置高，高 16 位写 1 = 拉低，写 0 无影响 */
#define GPIOC_BSRR      (*(volatile uint32_t *)0x40011010UL)

#define LED_PIN         13                  /* 板载 LED 在 PC13 */

/* ---------------------------------------------------------------------------
 * SystemInit —— 启动文件（startup_stm32f103xb.s）在跳进 main 之前会调用它，
 * 正常是用来配置时钟树的（system_stm32f1xx.c 里实现）。
 * 这里给个空实现：我们用 HSI 默认 8MHz 就够了。
 * 不写这个函数会链接报错：undefined reference to `SystemInit'
 * ------------------------------------------------------------------------- */
void SystemInit(void)
{
}

/* 粗糙的软件延时，纯靠空转拖时间 */
static void delay_loop(volatile uint32_t count)
{
    while (count--) {
        __asm volatile("nop");
    }
}

int main(void)
{
    /* 1. 打开 GPIOC 的时钟。不开时钟的话寄存器写进去没有反应 */
    RCC_APB2ENR |= RCC_IOPCEN;

    /* 2. 把 PC13 配成推挽输出。
     *    CRH 里 PC13 占 bit[23:20]，其中 bit[21:20]=MODE，bit[23:22]=CNF
     *    MODE=10 → 输出模式 2MHz；CNF=00 → 通用推挽输出
     *    即 0x2 << 20 */
    GPIOC_CRH &= ~(0xFUL << 20);        /* 先清掉这 4 位 */
    GPIOC_CRH |=  (0x2UL << 20);        /* 再写入 0010 */

    /* 3. 主循环：亮 → 等 → 灭 → 等 */
    for (;;) {
        GPIOC_BSRR = (1UL << LED_PIN);              /* PC13 置高 → LED 灭 */
        delay_loop(200000);

        GPIOC_BSRR = (1UL << (LED_PIN + 16));       /* PC13 拉低 → LED 亮 */
        delay_loop(200000);
    }
}
