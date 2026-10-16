# led_blink_hal —— CubeMX + HAL 版最小 LED 闪烁

走通**完整开发流程**的工程：CubeMX 生成 → 补业务代码 → EIDE 编译 → J-Link 下载 → Ozone 调试。

和 `../led_blink/`（裸机寄存器版）的区别：那一个是"不依赖任何库，证明工具链能跑"；
**这一个才是战队要求的真实流程**，和 `balance_car` 结构一致。

## 工程结构

```
led_blink_hal/
├── led_blink_hal.ioc             # ★ CubeMX 工程文件（改引脚/外设就改这里）
├── Core/
│   ├── Inc/main.h                # LED_Pin / LED_GPIO_Port 宏在这里
│   └── Src/main.c                # ★ 业务代码写在 USER CODE 区
│       stm32f1xx_it.c            # 中断服务函数
│       stm32f1xx_hal_msp.c       # 外设底层初始化
│       system_stm32f1xx.c        # 时钟系统初始化
│       syscalls.c / sysmem.c     # newlib 系统调用桩
├── Drivers/
│   ├── STM32F1xx_HAL_Driver/     # HAL 库（CubeMX 自动拷贝的）
│   └── CMSIS/                    # Cortex-M3 内核 + 寄存器定义
├── startup_stm32f103xb.s         # 启动文件（CubeMX 放在根目录）
├── STM32F103xx_FLASH.ld          # 链接脚本：64K Flash / 20K RAM
├── Makefile                      # CubeMX 生成的 Makefile（命令行备用）
├── led_blink_hal.jdebug          # ★ Ozone 调试工程
├── .eide/eide.json               # ★ EIDE 工程配置
├── .vscode/tasks.json            # Cmd+Shift+B 快捷任务
└── build/Debug/                  # 编译产物（.elf/.hex/.bin）
```

带 ★ 的是**你日常会动到的文件**，其余交给 CubeMX。

## 硬件配置（当前状态）

| 项目 | 配置 |
|---|---|
| 芯片 | STM32F103C8T6，LQFP48 |
| 时钟 | **HSI 内部 8MHz**，未开 PLL（还没接晶振） |
| LED | **PC13**，开漏输出（OD），初始高电平 = 灭 |
| SWD | PA13 / PA14 |
| PA9 | 配成了推挽输出（CubeMX 里误配的，不用可以删） |

LED 接在 3V3 和 PC13 之间，**低电平点亮** —— 所以开漏输出 + 拉低才亮。

## 代码在哪

`Core/Src/main.c` 的 `while(1)` 里：

```c
/* USER CODE BEGIN 3 */
HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
HAL_Delay(500);          // 500ms 翻转一次 = 1Hz 闪烁
/* USER CODE END 3 */
```

**改代码只准改 `USER CODE BEGIN/END` 之间的内容** —— CubeMX 重新生成时只保留这部分，
其他位置会被覆盖。改 500 这个数字就能调闪烁快慢。

## 完整工作流

### 1. CubeMX 改配置（改引脚、加外设时）

用 CubeMX 打开 `led_blink_hal.ioc`，改完点 `GENERATE CODE`。
**别在 VS Code 里手动改 `Core/`、`Drivers/` 下的非 USER CODE 区**，会被覆盖。

命令行也能生成（不用开 GUI）：

```bash
cat > /tmp/gen.txt <<'EOF'
config load /Users/macbook/Desktop/wyz/led_blink_hal/led_blink_hal.ioc
project generate
exit
EOF
/Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOS/STM32CubeMX -q /tmp/gen.txt
```

### 2. EIDE 编译

VS Code 打开本文件夹 → 侧边栏 **EIDE** 图标 → `Build`，或 `Cmd+Shift+B`。
输出在 `build/Debug/`：`led_blink_hal.elf` / `.hex` / `.bin`。

### 3. 下载（J-Link 到手后）

EIDE 面板 → `Flash`。当前配的是 **JLink + SWD + 4MHz**。

### 4. 调试（J-Link 到手后）

双击 `led_blink_hal.jdebug` 用 Ozone 打开 → `Download & Reset` → 打断点、单步、看变量。

## 编译结果

| | 占用 | 总量 | 占比 |
|---|---|---|---|
| FLASH | 3524 B | 64 KB | 5.38% |
| RAM | 1584 B | 20 KB | 7.73% |

已和 CubeMX 原生 Makefile 的产物**逐字节核对一致**，说明 EIDE 配置没有偏差。

## 几个坑（踩过的）

1. **CubeMX 生成的工程不能直接给 EIDE 用**。要在 `.eide/eide.json` 里
   手工指定 `srcDirs`、`incList`、`defineList`、链接脚本。
2. **startup 有两份**：根目录（CubeMX 放的）和
   `Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/`（CMSIS 包自带的）。
   EIDE 里用的是后者，并排除了其他 13 个型号，**否则符号重定义**。
3. **HAL 源文件要按需排除**。CubeMX 会把整个 HAL 库拷进来，但只用到十几个文件。
   EIDE 的 `excludeList` 里排掉了没用的外设（连 ADC/TIM/UART 都排了）。
   **以后在 CubeMX 里加了新外设，记得把对应的 HAL 源文件从排除列表里去掉**，
   否则链接时会报 `undefined reference`。
4. **`HAL_GPIO_TogglePin` 需要 PC13 是输出模式**，这个 CubeMX 配好了，不用手写寄存器。
