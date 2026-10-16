/**
  ******************************************************************************
  * @file      sysmem.c
  * @brief     newlib 需要的堆管理接口（_sbrk）。
  *
  * 本工程不用 malloc，但 vsprintf/字符串函数有可能间接引用，
  * 所以给出一个最小的实现，堆从 _end 一直到栈顶（见链接脚本 _Min_Stack_Size）。
  *
  ******************************************************************************
  */

#include <errno.h>
#include <stdint.h>

/* 由链接脚本提供：已初始化数据段的结束地址，也就是堆的起点 */
extern uint8_t _end;
/* 由链接脚本提供：栈顶（RAM 的最高地址） */
extern uint8_t _estack;
/* 栈保留大小，必须与 STM32F103C8TX_FLASH.ld 中的 _Min_Stack_Size 一致 */
#define _MIN_STACK_SIZE 0x400U

/**
  * @brief  分配堆内存（newlib 的底层实现）
  * @param  incr 本次需要增加的字节数
  * @retval 上一次堆顶的地址；失败返回 (void *) -1
  */
void *_sbrk(ptrdiff_t incr)
{
  static uint8_t *heap_end = 0;
  uint8_t *prev_heap_end;

  if (heap_end == 0)
  {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;

  if (heap_end + incr > (&_estack - _MIN_STACK_SIZE))
  {
    errno = ENOMEM;
    return (void *) -1;
  }

  heap_end += incr;
  return (void *) prev_heap_end;
}
