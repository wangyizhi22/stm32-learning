# STM32F103C8T6 蓝板工程

macOS (Apple Silicon) 上的 STM32 开发环境，**不依赖任何需要注册账号的工具**。

## 环境组成

| 组件 | 版本 | 位置 |
|---|---|---|
| ARM 工具链 | xPack GCC 15.2.1 | `~/stm32-tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/` |
| 调试器 | xPack OpenOCD 0.12.0 | `~/stm32-tools/xpack-openocd-0.12.0-7/` |
| CMSIS 头文件 | ST 官方 | `Drivers/CMSIS/` |
| HAL 驱动库 | ST 官方 | `Drivers/STM32F1xx_HAL_Driver/` |

工具链装在 `~/stm32-tools/`，**多个工程可以共用**，不用每个工程单独下一份。

## 目录结构

```
stm32-blink/
├── Makefile                        构建脚本
├── src/
│   ├── main.c                      ★ 你的代码写在这里
│   ├── syscalls.c                  标准库的空实现（不用管）
│   └── stm32f1xx_hal_conf.h        HAL 模块开关（决定启用哪些外设）
├── examples/
│   └── main-baremetal.c            裸机寄存器版 LED 闪烁，留作参考
├── startup/
│   └── startup_stm32f103xb.s       启动文件（中断向量表 + 复位入口）
├── linker/
│   └── STM32F103XB_FLASH.ld        链接脚本（规定代码/数据放哪）
├── Drivers/
│   ├── CMSIS/                      ST 官方头文件
│   └── STM32F1xx_HAL_Driver/       HAL 库源码
├── build/                          编译产物，可随时删
└── .vscode/                        VS Code 配置
```

## 日常使用

**在 VS Code 里**（推荐，不用敲命令）：

- `Cmd + Shift + B` — 编译
- `F5` — 编译 + 下载 + 进入调试（可以打断点、单步、看变量）
- 任务菜单里还有「烧录到板子」「探测芯片」

**在终端里**：

```bash
make          # 编译
make flash    # 编译 + 烧录
make probe    # 只检测能否连上芯片
make clean    # 清理
```

## 代码是 HAL 库写法

当前 `src/main.c` 用的是 ST 的 **HAL 库**，和网上正点原子 / 野火等教程的写法一致：

```c
HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
HAL_Delay(500);
```

如果想看**直接操作寄存器**的写法（能更清楚理解底层），见 `examples/main-baremetal.c`。
那个文件不在编译列表里，不会被编译，纯粹是参考。

## 硬件接线

| ST-Link | 蓝板 |
|---|---|
| 3V3 | 3V3 |
| SWDIO | SWDIO (PA13) |
| SWCLK | SWCLK (PA14) |
| GND | GND |

**不要接 5V。**

## 加新文件 / 用新外设

**加自己的 `.c` 文件**：放进 `src/` 之后，还要在 `Makefile` 的 `C_SOURCES` 里加一行。
（Makefile 不会自动扫描目录。）

**用新的外设**（比如 SPI 接屏幕、I2C 接传感器）：

1. 在 `Makefile` 的 `HAL_SOURCES` 里补上对应的 `stm32f1xx_hal_xxx.c`；
2. 确认 `src/stm32f1xx_hal_conf.h` 里那个模块的 `#define` 没有被注释掉。

比如用 SPI，就加 `$(HAL_DIR)/stm32f1xx_hal_spi.c`，并确保
`#define HAL_SPI_MODULE_ENABLED` 是打开的（当前已经是打开的）。

## 常见问题

**烧录报错 `Error: open failed`**
检查 ST-Link 是否插好、接线是否松动。跑 `make probe` 看能否识别芯片。

**报 `device id = 0x00000000` 或 ID 对不上**
可能是山寨 CH32 芯片，或者 SWD 线接反了。

**`HAL_Delay()` 延时时间明显不对**
通常是忘了在 `SystemClock_Config()` 最后重新调用 `HAL_InitTick(TICK_INT_PRIORITY)`。
改了主频之后 SysTick 必须重新初始化，否则延时按旧频率算。
本工程的 `main.c` 里已经处理好了。

**调试时断点打不上 / 变量显示 `<optimized out>`**
把 `Makefile` 里的 `OPT = -Og -g3` 改成 `-O0 -g3` 试试。

**改了 `stm32f1xx_hal_conf.h` 但没生效**
配置文件改了要重新编译：`make clean && make`。
