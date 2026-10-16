# led_blink —— 最小系统板 LED 闪烁

用来走通 EIDE 工作流的最小工程。**STM32F103C8T6**，板载 LED（PC13）闪烁。

## 工程结构

```
led_blink/
├── src/
│   ├── main.c                    # LED 闪烁逻辑
│   └── startup_stm32f103xb.s     # 启动文件（GCC 语法）
├── STM32F103C8Tx_FLASH.ld        # 链接脚本：64K Flash / 20K RAM
├── .eide/eide.json               # EIDE 工程配置
├── .eide.usr.ctx.json
├── .vscode/tasks.json            # build / flash / clean 快捷任务
└── build/Debug/                  # 编译产物（led_blink.elf / .hex / .bin）
```

原工程 `balance_car` 一个字都没动，这两套是独立的。

## 代码做了什么

全裸机，不依赖 HAL / 标准库，只碰 3 个寄存器：

1. **`RCC_APB2ENR`** —— 打开 GPIOC 的时钟（不开时钟，寄存器写了没反应）
2. **`GPIOC_CRH`** —— 把 PC13 配成推挽输出
3. **`GPIOC_BSRR`** —— 置高 / 拉低 PC13，循环翻转

时钟没有配 PLL，跑内部 HSI **8MHz**。

## 三个文件的分工（换个芯片时改这三个）

| 文件 | 作用 | 换芯片怎么改 |
|---|---|---|
| `startup_stm32f103xb.s` | 中断向量表 + 复位后初始化，最后跳到 `main` | 换对应型号的 startup |
| `STM32F103C8Tx_FLASH.ld` | 告诉链接器 Flash/RAM 的地址和大小、各段怎么摆 | 改 `MEMORY{}` 里的 ORIGIN/LENGTH |
| `.eide/eide.json` | 源码目录、宏、器件型号、下载方式 | 改 `deviceName` 和 `defineList` |

## 使用

1. VS Code 打开本文件夹
2. 侧边栏 EIDE 图标 → `Build`（或 `Cmd+Shift+B`）
3. 产物在 `build/Debug/`：`led_blink.elf` / `.hex` / `.bin`

有 J-Link 之后：

- **下载**：EIDE 面板 → `Flash`
- **调试**：用 Ozone 打开 `led_blink.jdebug`（需先生成）

## 编译结果

```
FLASH:   724 B / 64 KB  (1.10%)
RAM:    1568 B / 20 KB  (7.66%)
```

1526 字节里有 804 字节是启动文件和向量表，实际 `main` 就几百字节。

## 注意

- **板载 LED 在 PC13**。如果你的板子不是这个引脚，改 `main.c` 里的 `LED_PIN`
  和对应寄存器（`GPIOC_*` → 引脚所在端口的寄存器）即可。
- 闪烁快慢由 `delay_loop(200000)` 里的数字决定，越大越慢。
- 本工程是**裸机寄存器版**，纯粹为了演示流程。真要写业务代码还是用
  `balance_car` 那套 HAL 结构。
