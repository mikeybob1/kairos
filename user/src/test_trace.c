#include "syscall.h"
#include "unistd.h"
#include "stdio.h"

int main() {
    // 监听172号(getpid)系统调用
    syscall(524, 172);
    // 第一次调用
    getpid();
    // fork产生子进程
    if (fork() == 0) {
        // 子进程调用
        getpid();
    } else {
        // 父进程取消监听
        syscall(524, -1);
        // 父进程调用
        getpid();
    }

    return 0;
}

// 输出：
// trace: PID X calls getpid
// trace: PID X+1 calls getpid
