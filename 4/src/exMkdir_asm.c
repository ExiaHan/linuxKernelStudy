/*
 * =====================================================================================
 *
 *       Filename:  exMkdir.c
 *
 *    Description:  Use syscall "mkdir" to create a folder in ASM
 *
 *        Version:  1.0
 *        Created:  2015年03月28日 17时46分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ExiaHan 
 *   Organization:  USTC
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char *filename = NULL;
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    int result = 0;

    if (argc == 1) {
        printf("Usage: %s [PATH/FolderName]\n", argv[0]);
        return -1;
    }
    filename = argv[1];
    __asm__ __volatile__(
        "movl $0x27, %%eax\n\t"
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"
        : "=m" (result)
        : "c" (mode), "b" (filename)
        : "eax"
    );
    if(!result) {
        printf("[I]: Success, now use \"ls [path/folder name]\" to see the result\n");
        return 0;
    }
    else {
        printf("[E]: Error ,can not create folder [%s] , Error Code [%d]\n", filename, result);
        return -1;
    }
}
