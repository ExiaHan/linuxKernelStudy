/*
 * =====================================================================================
 *
 *       Filename:  exMkdir.c
 *
 *    Description:  Use syscall "mkdir" to create a folder
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
#include <errno.h>
#include <string.h>
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
    if(!(result = mkdir(filename, mode))) {
        printf("[I]: Success, now use \"ls [path/folder name]\" to see the result\n");
        return 0;
    }
    else {
        printf("[E]: Error, failed to create folder [%s], ErrorCode[%s]\n", argv[1], strerror(errno));
        return -1;
    }
}
