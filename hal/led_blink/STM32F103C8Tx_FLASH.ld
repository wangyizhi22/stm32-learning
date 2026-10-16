/*
******************************************************************************
**  文件    : STM32F103C8Tx_FLASH.ld
**  说明    : STM32F103C8T6 链接脚本
**            64 KB Flash  @ 0x08000000
**            20 KB SRAM   @ 0x20000000
**
**  用途    : 给 EIDE / ARM GNU GCC 工具链做链接。
**            原 Keil 工程用的是 MDK-ARM/balance_car/balance_car.sct，
**            地址区间完全一致（Flash 0x10000 / RAM 0x5000），
**            换工具链不影响内存布局。
******************************************************************************
*/

/* 程序入口 —— 由 startup_stm32f103xb.s 提供 */
ENTRY(Reset_Handler)

/* 栈顶：SRAM 末尾，栈向下生长 */
_estack = 0x20005000;   /* 0x20000000 + 20K */

/* 堆栈余量检查：链接时若放不下会报错 */
_Min_Heap_Size  = 0x200;  /* 512 字节堆 */
_Min_Stack_Size = 0x400;  /* 1 K 字节栈 */

/* 存储器分布 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
  /* 中断向量表必须放在 Flash 最前面 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  /* 代码与只读数据 */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* .data 段在 RAM 中运行，但初值存在 Flash，加载后由启动代码搬运 */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT> FLASH

  /* .bss 段：启动代码负责清零 */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  PROVIDE ( end = _ebss );
  PROVIDE ( _end = _ebss );

  /* 堆栈保留区，用于检查 RAM 是否够用 */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
