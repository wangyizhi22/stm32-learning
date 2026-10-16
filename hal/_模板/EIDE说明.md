# EIDE 模板使用说明

配套文件：

| 文件 | 作用 |
|---|---|
| `eide-init.sh` | **主入口** —— 读 CubeMX 工程，自动生成下面三个文件 |
| `tasks.json` | 快捷键编译（脚本会复制过去） |
| `workspace模板.code-workspace` | 给 EIDE 发现工程用（脚本会复制过去） |
| `eide.json` | ⚠️ 旧格式模板，已由脚本取代，仅作参考 |

---

## 一、为什么 CubeMX 工程"接不上" EIDE

需要一个**两件套**，少任何一个 EIDE 都不认：

| 文件 | 作用 |
|---|---|
| `<工程>/.eide/eide.yml`（或旧版 `eide.json`） | 工程本身的内容：源文件、宏、头文件、编译烧录配置 |
| `<工程>/<工程名>.code-workspace` | 让 EIDE **发现**这个工程 |

CubeMX 不认识 EIDE，两样都不会生成 —— 结果 EIDE 侧边栏显示
**"该工作区中没有任何项目！"**（下面只有"打开项目 / 新建项目 / 导入项目"三个链接），
状态栏的编译、烧录按钮也不出现。

⚠️ **第二个文件最容易漏。** EIDE 发现项目的源码逻辑是这样的：

```js
for (const dir of workspaceList) {
    const found = dir.GetList([/.code-workspace$/i], EXCLUDE_ALL_FILTER)
    found.length > 0 && detectProject(dir) && openProject(found[0])
}
```

它**先找 `.code-workspace` 文件**（`GetList` 的头一个参数按文件名过滤），
找不到就直接 `continue`，连 `detectProject()`（检查 `.eide/eide.yml`）都不会执行。
所以**光有 `eide.yml` 是不够的** —— 必须两个都在。

PlatformIO 能用是因为 `platformio.ini` 一个文件就够了；EIDE 这边需要这一对。

---

## 二、标准流程：CubeMX 生成之后做什么

### 第一次建工程

1. **CubeMX** 里新建工程、选芯片、配时钟和外设
2. **Project Manager → Toolchain / IDE 选 `Makefile`** ← 关键，选错后面全废
3. 点 **GENERATE CODE**
4. 回终端跑脚本：

   ```bash
   ~/Desktop/wyz/cubemx/_模板/eide-init.sh ~/Desktop/wyz/cubemx/你的工程名
   ```

5. VSCode 里 **File → Open Workspace from File**，选 `<工程名>.code-workspace`
6. 完事。状态栏会出现编译 / 烧录按钮，`Cmd+Shift+B` 也能编译

### 以后改代码

**什么都不用做。** 直接改 `Core/Src/`、`Core/Inc/` 里的代码，然后编译。
即使回 CubeMX 重新 GENERATE CODE（改了外设、改了时钟），
`Core/` 和 `Drivers/` 被重写，脚本生成的那三个文件也**不会被动**，不用重跑。

> 什么时候需要重跑脚本：只有**芯片本身变了**（见第三节）。
> 加个串口、改个引脚、调个时钟 —— 都不用重跑。

---

## 三、换板子怎么操作

### 情况 A：同系列换型号（F103C8 → F103RC / F103ZE ...）

1. CubeMX 打开工程 → 换 Device → GENERATE CODE
2. 重跑一遍脚本
3. VSCode 重新打开 `.code-workspace`（已经打开的话 `Cmd+Shift+P` → `Developer: Reload Window`）

**就这三步。** 理由：脚本里所有跟芯片有关的东西全是**读出来的**，不是写死的 ——

| 要改的东西 | 脚本从哪读 |
|---|---|
| 芯片型号 `deviceName` | `.ioc` 里的 `ProjectManager.DeviceId` |
| 容量宏 `STM32F103xB` | `Makefile` 的 `C_DEFS` |
| 内核 `Cortex-M3` | `Makefile` 的 `CPU = -mcpu=...` |
| 链接脚本 `scatterFilePath` | `Makefile` 的 `LDSCRIPT` |
| Flash / RAM 大小和地址 | 直接解析 `.ld` 里的 `ORIGIN` / `LENGTH` |
| 保留哪个 startup | `Makefile` 的 `ASM_SOURCES` |
| HAL 目录名 | 扫 `Drivers/` 下实际存在的目录 |

所以 F103C8 的 64K/20K 会自动变成 F103RC 的 256K/48K，不用你查表。

**唯一要自己动手的**：CubeMX 里换芯片后，原来用的外设引脚会失效，
要重新配一遍引脚；`Core/Src` 里自己写的业务代码也可能要跟着调。

### 情况 B：换系列（F1 → F4 / F0 / G0 ...）

同样跑一遍脚本，**目录名和 CMSIS 路径它也能自动跟上**（F1 是
`STM32F1xx_HAL_Driver`，F4 就是 `STM32F4xx_HAL_Driver`，脚本扫 `Drivers/` 得到）。

但要额外注意两点：

- **`cpuType`**：脚本从 `-mcpu` 推，F4 会得到 `Cortex-M4`。**如果芯片带浮点单元**
  （F4 的 `F` 后缀，如 STM32F407），EIDE 里还要手动把
  `floatingPointHardware` 从 `default` 改成 `fpv4-sp-d16`，`$float-abi-type` 改成 `hard`，
  否则浮点走软件模拟、慢很多。脚本默认写的是安全的 softfp。
- **外设代码不一样**：F1 和 F4 的 HAL 头文件、寄存器操作都不同，
  `Core/Src` 里手写的东西基本要重来。CubeMX 生成的骨架没问题。

### 情况 C：换板子但芯片一样

（比如两块都是 F103C8T6 的蓝药丸，只是换了块板）

**什么都不用改**，编译出来直接烧。

---

## 四、脚本做了什么

`eide-init.sh` 生成 / 覆盖这些东西：

```
<工程>/.eide/eide.yml              EIDE 工程配置
<工程>/.vscode/tasks.json          快捷键定义（已存在则不动）
<工程>/<工程名>.code-workspace     让 EIDE 发现工程（已存在则不动）
```

行为细节：

- `eide.yml` **已存在时会先备份成 `eide.yml.bak`**，不会静默覆盖
- `uid` 沿用旧的（EIDE 拿它做工程标识，换新值没必要）
- 顺带删掉早先遗留的 `.eide/eide.json`（EIDE 会自动把 json 迁移成 yml，别留两份）

> ⚠️ **重跑脚本会覆盖你在 EIDE 界面里手改的配置**（比如换过烧录器、
> 改过优化等级）。如果你在界面里调过东西，重跑前先想清楚 ——
> 或者干脆养成习惯：**能写进脚本的就别在界面里改**。

**格式说明**：生成的是 `version: "4.1"` 的 YAML（EIDE 3.27 当前格式）。
早期版本的 EIDE 用 `eide.json`（v3.5），现在是 `eide.yml`。
本文档写的这份和 EIDE 自己写出来的**逐字节一致**（除了随机 uid），
所以不用担心格式兼容。

---

## 五、设计取舍：为什么模板"全编 HAL"

对比 `led_blink_hal/.eide/eide.json`，你会看到它的 `excludeList` 里排除了
几十个 HAL 文件（`stm32f1xx_hal_i2c.c`、`stm32f1xx_hal_spi.c` …）。

**那份配置是跟着那个工程的外设组合量身定制的，不能直接当模板用。**

如果新工程里 CubeMX 勾了 I2C，而模板把 `stm32f1xx_hal_i2c.c` 排除了，
编译能过、链接必炸：

```
undefined reference to `HAL_I2C_Init'
```

所以这份模板**只排除真正必须排除的文件**，其余 HAL 源码全部参与编译。
脚本排除两类：

| 文件 | 为什么必须排除 |
|---|---|
| `stm32f1xx_hal_msp_template.c` | 会和 `Core/Src/stm32f1xx_hal_msp.c` 重复定义 `HAL_MspInit` |
| `stm32f1xx_hal_timebase_tim_template.c` | 会和 `stm32f1xx_hal.c` 重复定义 `HAL_InitTick` 等 |
| `stm32f1xx_hal_timebase_rtc_alarm_template.c` | 同上 |
| 其余型号的 `startup_*.s` | CubeMX 把全系列 startup 都塞进 `Templates/gcc/`，只留自己那个，否则 `Reset_Handler` 重复定义 |

至于**没启用的外设模块**（如 CAN、SDIO），HAL 源码本身是这么写的：

```c
#include "stm32f1xx_hal.h"
#ifdef HAL_CAN_MODULE_ENABLED     // 没启用 → 整个文件编译成空的
...
#endif
```

所以全编只是**多花几秒编译时间**，换来的是"复制过去就能用，不管勾了什么外设"。
模板的价值就在这里。

---

## 六、烧录器

脚本里 `uploader: JLink`，`speed` 设 4000 kHz（SWD 4MHz，够快也够稳）。

### J-Link —— 你这台机器**现在就能用**

`/usr/local/bin/JLinkExe` 已装好，直接点 EIDE 的烧录按钮即可。

### ST-Link —— 需要先装软件

EIDE 的 `STLink` 烧录器**不是**直接跟硬件打交道，它调用的是
**STM32CubeProgrammer 的命令行 `STM32_Programmer_CLI`**。

这台机器上目前**没有这个程序**（`which STM32_Programmer_CLI` 找不到），
所以现在切成 ST-Link 会报找不到工具。要用 ST-Link 的话：

1. 装 [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)（含 CLI）
2. 装完后在 EIDE 里切换：`Cmd+Shift+P` → `eide: 切换烧录器`（Switch Uploader）→ 选 STLink

切换不用改 `eide.yml` —— EIDE 会把每种烧录器的配置分别存进 `uploadConfigMap`，
两边来回切都不会丢配置。

> 但注意：**重跑 `eide-init.sh` 会把 `uploader` 重置回 JLink**
> （脚本只写 JLink 那一份配置）。切换烧录器这种操作做完就别再重跑脚本了。

---

## 七、其他

- **调试**：EIDE 可以生成 cortex-debug 的 `launch.json`
  （`Cmd+Shift+P` → `eide: generate debug config` → JLink/OpenOCD/pyOCD）。
  你现在用的 Ozone（`.jdebug`）也能继续用 —— 它读的 `build/Debug/<工程名>.elf`
  就是 EIDE 编译出来的，路径对得上。
- **编译快捷键**：`Cmd+Shift+B`（`tasks.json` 里定义了 build / rebuild / clean / flash）。
- **头文件飘红**：如果 VSCode 里头文件报红但 EIDE 能编过，是 C/C++ 插件的
  `includePath` 没配 —— EIDE 工程树里点"生成 IntelliSense 配置"即可。
  （`.code-workspace` 里的 `C_Cpp.default.configurationProvider: "cl.eide"`
  就是让头文件路径自动跟着 EIDE 走的，所以要用"打开工作区"而不是"打开文件夹"。）
