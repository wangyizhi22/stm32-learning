#!/usr/bin/env bash
# ============================================================================
#  eide-init.sh —— 给 CubeMX 工程生成 EIDE 工程配置
#
#  用法：
#     ~/Desktop/wyz/cubemx/_模板/eide-init.sh <CubeMX 工程目录>
#
#  芯片型号 / 容量宏 / 链接脚本 / 内核 / startup / Flash RAM 大小，
#  全部从 CubeMX 生成的 .ioc 和 Makefile 里自动读取 ——
#  所以换板子（哪怕换系列）之后重跑一遍就行，不用手改任何配置。
#
#  生成三个文件：
#     <工程>/.eide/eide.yml               EIDE 工程配置（EIDE 3.27 的新格式）
#     <工程>/.vscode/tasks.json           快捷键编译
#     <工程>/<工程名>.code-workspace      让 EIDE 发现这个工程（必须有）
#
#  前置条件：CubeMX 生成代码时 Toolchain/IDE 选 Makefile
# ============================================================================
set -euo pipefail

# ---------------------------------------------------------------- 参数检查
if [ $# -lt 1 ]; then
    echo "用法: $0 <CubeMX 工程目录>"
    echo "例:   $0 ~/Desktop/wyz/cubemx/my_new_project"
    exit 1
fi
[ -d "$1" ] || { echo "错误: 目录不存在 —— $1"; exit 1; }

PROJ="$(cd "$1" && pwd)"
NAME="$(basename "$PROJ")"
MK="$PROJ/Makefile"
IOC="$(ls "$PROJ"/*.ioc 2>/dev/null | head -1 || true)"
TPL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
die() { echo "错误: $1" >&2; exit 1; }

# ------------------------------------------------------- 1. 芯片型号
CHIP=""
[ -n "$IOC" ] && CHIP="$(grep -E '^ProjectManager\.DeviceId=' "$IOC" | head -1 | cut -d= -f2 | sed -E 's/[A-Z]x$//' || true)"
[ -n "$CHIP" ] || die "没能从 .ioc 里读出芯片型号，确认工程是用 CubeMX 生成的吗？"

# ------------------------------------------------------- 2. Makefile 提取
[ -f "$MK" ] || die "没找到 Makefile —— CubeMX 里 Toolchain/IDE 必须选 Makefile"

DEFS=()
while IFS= read -r d; do [ -n "$d" ] && DEFS+=("$d"); done < <(
    sed -n '/^C_DEFS/,/[^\\]$/p' "$MK" | grep -oE -- '-D[A-Za-z0-9_]+' | sed 's/^-D//' || true
)
[ ${#DEFS[@]} -gt 0 ] || DEFS=("USE_HAL_DRIVER")

LDSCRIPT="$(grep -E '^LDSCRIPT[[:space:]]*=' "$MK" | head -1 | sed -E 's/^[^=]*=[[:space:]]*//' | tr -d ' \r' || true)"
CPU_RAW="$(grep -E '^CPU[[:space:]]*=' "$MK" | head -1 | sed -E 's/.*-mcpu=([A-Za-z0-9_-]+).*/\1/' | tr -d ' \r' || true)"
CPU_TYPE="$(echo "$CPU_RAW" | sed -E 's/cortex-m([0-9]+).*/Cortex-M\1/')"
STARTUP="$(sed -n '/^ASM_SOURCES/,/[^\\]$/p' "$MK" | grep -oE 'startup_[A-Za-z0-9_]+\.s' | head -1 || true)"

# ------------------------------------------------------- 3. 目录结构
HAL_DIR="$(ls "$PROJ/Drivers" 2>/dev/null | grep -E '^STM32[0-9A-Z]+xx_HAL_Driver$' | head -1 || true)"
CMSIS_SERIES="$(ls "$PROJ/Drivers/CMSIS/Device/ST" 2>/dev/null | grep -E '^STM32' | head -1 || true)"
GCC_TPL="Drivers/CMSIS/Device/ST/$CMSIS_SERIES/Source/Templates/gcc"
[ -n "$HAL_DIR" ] || die "没找到 Drivers/STM32*xx_HAL_Driver 目录"

# ------------------------------------------------------- 4. Flash / RAM
LDF="$PROJ/$LDSCRIPT"
RAM_ADDR="0x20000000"; RAM_SIZE="0x5000"
ROM_ADDR="0x08000000"; ROM_SIZE="0x10000"
if [ -n "$LDSCRIPT" ] && [ -f "$LDF" ]; then
    # 注意: CubeMX 生成的 .ld 是 CRLF 换行, tr 必须连带删掉 \r,
    #       否则 "20K\r" 匹配不上 *[Kk], 会掉进兜底分支报 "value too great for base"
    to_hex()  { printf '0x%08X' "$(( $1 ))"; }
    to_size() {
        local v="${1//[[:space:]]/}"
        case "$v" in
            *[Kk]) printf '0x%X' "$(( ${v%[Kk]} * 1024 ))";;
            *[Mm]) printf '0x%X' "$(( ${v%[Mm]} * 1048576 ))";;
            *)     printf '0x%X' "$(( v ))";;
        esac
    }
    grab() { grep -iE "^[[:space:]]*$1[[:space:]]*\(" "$LDF" | head -1 || true; }
    attr() { sed -E "s/.*$2[[:space:]]*=[[:space:]]*([^,]+).*/\1/" <<< "$1" | tr -d ' \r'; }

    RL="$(grab RAM)"; FL="$(grab FLASH)"
    if [ -n "$RL" ]; then
        RAM_ADDR="$(to_hex "$(attr "$RL" ORIGIN)")"
        RAM_SIZE="$(to_size "$(attr "$RL" LENGTH)")"
    fi
    if [ -n "$FL" ]; then
        ROM_ADDR="$(to_hex "$(attr "$FL" ORIGIN)")"
        ROM_SIZE="$(to_size "$(attr "$FL" LENGTH)")"
    fi
fi

# ------------------------------------------------------- 5. 排除列表
#   a) HAL 里的 *_template.c 会和 Core/Src 下的实现撞车（重复定义）
#   b) CMSIS 模板目录里其他型号的 startup
EXCL=()
for f in "$PROJ/Drivers/$HAL_DIR/Src/"*_template.c; do
    [ -e "$f" ] || continue
    EXCL+=("Drivers/$HAL_DIR/Src/$(basename "$f")")
done
if [ -n "$STARTUP" ] && [ -d "$PROJ/$GCC_TPL" ]; then
    for f in "$PROJ/$GCC_TPL"/*.s; do
        [ -e "$f" ] || continue
        b="$(basename "$f")"
        [ "$b" = "$STARTUP" ] && continue
        EXCL+=("$GCC_TPL/$b")
    done
fi

# ------------------------------------------------------- 6. 组装 YAML 片段
yaml_list() {   # 用法: yaml_list <缩进> <值...>
    local indent="$1"; shift
    local v
    for v in "$@"; do printf '%s- %s\n' "$indent" "$v"; done
}

DEFS_Y="$(yaml_list '        ' "${DEFS[@]}")"
EXCL_Y="$(yaml_list '      ' "${EXCL[@]}")"
INC_Y="$(yaml_list '        ' \
    "Core/Inc" \
    "Drivers/$HAL_DIR/Inc" \
    "Drivers/$HAL_DIR/Inc/Legacy" \
    "Drivers/CMSIS/Device/ST/$CMSIS_SERIES/Include" \
    "Drivers/CMSIS/Include")"
# uid: 已存在就沿用旧的(EIDE 拿它做工程标识, 换了没必要还可能影响缓存)
UID_VAL=""
if [ -f "$PROJ/.eide/eide.yml" ]; then
    UID_VAL="$(grep -E '^[[:space:]]*uid:' "$PROJ/.eide/eide.yml" | head -1 | sed -E 's/.*uid:[[:space:]]*//' | tr -d ' \r' || true)"
fi
[ -n "$UID_VAL" ] || UID_VAL="$(uuidgen | tr -d '-' | tr 'A-Z' 'a-z')"

# ------------------------------------------------------- 7. 写文件
mkdir -p "$PROJ/.eide" "$PROJ/.vscode"
if [ -f "$PROJ/.eide/eide.yml" ]; then
    cp "$PROJ/.eide/eide.yml" "$PROJ/.eide/eide.yml.bak"
    echo "旧 eide.yml 已备份为 eide.yml.bak"
fi
rm -f "$PROJ/.eide/eide.json"      # 早先版本可能留下的 json 格式

cat > "$PROJ/.eide/eide.yml" <<EOF
version: "4.1"
name: $NAME
type: ARM
deviceName: $CHIP
packDir: null
srcDirs:
  - Core/Src
  - Drivers/$HAL_DIR/Src
  - $GCC_TPL
virtualFolder:
  name: <virtual_root>
  files: []
  folders: []
dependenceList: []
outDir: build
miscInfo:
  uid: $UID_VAL
targets:
  Debug:
    cppPreprocessAttrs:
      defineList:
$DEFS_Y
      incList:
$INC_Y
      libList: []
    excludeList:
$EXCL_Y
    settings:
      debugger: cortex-debug
    toolchain: GCC
    toolchainConfigMap:
      GCC:
        archExtensions: ""
        cpuType: $CPU_TYPE
        floatingPointHardware: default
        options:
          version: 5
          afterBuildTasks: []
          asm-compiler:
            ASM_FLAGS: ""
          beforeBuildTasks: []
          c/cpp-compiler:
            CXX_FLAGS: ""
            C_FLAGS: ""
            language-c: c11
            language-cpp: c++11
            one-elf-section-per-data: true
            one-elf-section-per-function: true
            optimization: level-debug
            warnings: all-warnings
          global:
            \$float-abi-type: softfp
            misc-control: --specs=nosys.specs --specs=nano.specs
            output-debug-info: enable
          linker:
            LD_FLAGS: ""
            LIB_FLAGS: -lm
            not-print-mem-usage: false
            output-format: elf
            remove-unused-input-sections: true
        scatterFilePath: $LDSCRIPT
        storageLayout:
          RAM:
            - id: 1
              isChecked: "1"
              mem:
                size: "$RAM_SIZE"
                startAddr: "$RAM_ADDR"
              noInit: false
              tag: IRAM
          ROM:
            - id: 1
              isChecked: "1"
              isStartup: true
              mem:
                size: "$ROM_SIZE"
                startAddr: "$ROM_ADDR"
              tag: IROM
        useCustomScatterFile: true
    uploadConfigMap:
      JLink:
        baseAddr: "$ROM_ADDR"
        bin: ""
        cpuInfo:
          cpuName: $CHIP
          vendor: ST
        otherCmds: ""
        proType: 1
        speed: 4000
    uploader: JLink
EOF

[ -f "$PROJ/.vscode/tasks.json" ] || cp "$TPL_DIR/tasks.json" "$PROJ/.vscode/tasks.json"
[ -f "$PROJ/$NAME.code-workspace" ] || cp "$TPL_DIR/workspace模板.code-workspace" "$PROJ/$NAME.code-workspace"

# ------------------------------------------------------- 8. 汇总
echo
echo "完成：$PROJ"
echo "   芯片      $CHIP   ($CPU_TYPE)"
echo "   内存      Flash $ROM_SIZE @ $ROM_ADDR   RAM $RAM_SIZE @ $RAM_ADDR"
echo "   宏        ${DEFS[*]}"
echo "   startup   ${STARTUP:-未识别}"
echo "   排除      ${#EXCL[@]} 个文件"
echo
echo "下一步：VSCode -> File -> Open Workspace from File -> $NAME.code-workspace"
