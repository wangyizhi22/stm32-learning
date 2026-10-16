/**
 * 系统调用桩函数
 *
 * 单片机没有操作系统，标准库（newlib）里那些「打开文件」「读硬盘」之类的
 * 函数本来无处可去。这个文件给它们提供空实现，让链接器闭嘴。
 *
 * 如果以后要用 printf 往串口输出，改 _write 函数即可（把数据丢给 USART）。
 */

#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>

/* 写数据到文件描述符。裸机环境直接丢弃，返回「已写完」。 */
int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    return len;
}

/* 读数据。没有输入源，返回 0（文件尾）。 */
int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;   /* 假装是个字符设备 */
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

/* 堆区末尾。_sbrk 用到，动态内存分配（malloc）才需要。 */
void _exit(int status)
{
    (void)status;
    while (1) { }
}
