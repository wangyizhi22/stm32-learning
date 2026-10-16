/**
 * STM32F103C8T6 蓝板 —— LED 闪烁
 *
 * 板上 LED 接在 PC13，共阳极接法：PC13 拉低 = 点亮，拉高 = 熄灭。
 *
 * 这是最小可运行程序，目的是验证「工具链 → 编译 → ST-Link 烧录」整条链路。
 * 直接操作寄存器，不依赖 HAL 库。
 */

#include "stm32f1xx.h"

/* 粗糙的延时：空转 count 次。不精确，但足够看出 LED 在闪。 */
static void delay(volatile uint32_t count)
{
    while (count--) {
        __NOP();
    }
}

int main(void)
{
    /* 1. 打开 GPIOC 的时钟。
     *    默认所有外设时钟都是关的，不开时钟寄存器写不进去。 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 2. 把 PC13 配成「推挽输出，2MHz」。
     *    CRH 管 PC8~PC15，每个引脚占 4 位，PC13 对应第 20~23 位。
     *    0x2 = 输出模式、最大 2MHz；推挽由 CNF 位（0x0）决定。 */
    GPIOC->CRH &= ~(0xFUL << 20);   /* 先清零这 4 位 */
    GPIOC->CRH |=  (0x2UL << 20);   /* 再写入配置 */

    /* 3. 主循环：翻转 PC13 */
    while (1) {
        GPIOC->BSRR = (1UL << (13 + 16));   /* 高 16 位 = 复位，PC13 拉低 → 亮 */
        delay(200000);

        GPIOC->BSRR = (1UL << 13);          /* 低 16 位 = 置位，PC13 拉高 → 灭 */
        delay(200000);
    }
}
