#include "string.h"
#include "syscall.h"

// 你的系统调用号
#define SYS_ECHO 666

int main(int argc, char **argv) {
	char *str = "Test String";
    int len = strlen(str);
    syscall(SYS_ECHO, str, len);
    return 0; 
}