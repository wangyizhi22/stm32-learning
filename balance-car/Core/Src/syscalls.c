/**
  ******************************************************************************
  * @file      syscalls.c
  * @brief     newlib 需要的系统调用桩函数。
  *
  * 单片机没有操作系统，这些函数只是为了让链接器不报错。
  * _write() 把 printf/vsprintf 的输出丢掉（本工程的串口打印走 My_USART_Printf）。
  *
  ******************************************************************************
  */

#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

/* 链接脚本符号，见 sysmem.c */
extern uint8_t _end;
extern uint8_t _estack;

/**
  * @brief  判断文件描述符是否是终端
  */
int _isatty(int fd)
{
  (void) fd;
  return 1;
}

/**
  * @brief  获取文件状态，单片机里只要返回 0 即可
  */
int _fstat(int fd, struct stat *st)
{
  (void) fd;
  st->st_mode = S_IFCHR; // 字符设备（串口）
  return 0;
}

/**
  * @brief  文件定位，单片机不支持
  */
int _lseek(int fd, int ptr, int dir)
{
  (void) fd;
  (void) ptr;
  (void) dir;
  return 0;
}

/**
  * @brief  读文件，单片机不支持
  */
int _read(int fd, char *ptr, int len)
{
  (void) fd;
  (void) ptr;
  (void) len;
  errno = EBADF;
  return -1;
}

/**
  * @brief  写文件，单片机没有文件系统，直接丢弃
  * @note   若想用 printf() 输出到串口2，就在这里调用 My_USART_Printf。
  */
int _write(int fd, char *ptr, int len)
{
  (void) fd;
  (void) ptr;
  return len;
}

/**
  * @brief  关闭文件
  */
int _close(int fd)
{
  (void) fd;
  errno = EBADF;
  return -1;
}

/**
  * @brief  获取进程号，固定返回 1
  */
int _getpid(void)
{
  return 1;
}

/**
  * @brief  发送信号，单片机不支持
  */
int _kill(int pid, int sig)
{
  (void) pid;
  (void) sig;
  errno = EINVAL;
  return -1;
}
