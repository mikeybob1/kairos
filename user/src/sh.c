#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

void execute_command(char *command) {
    char *args[MAX_ARGUMENTS];
    int arg_count = 0;

    // 解析命令
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < MAX_ARGUMENTS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    // 检查是否有管道符号 "|"
    int pipe_index = -1;
    for (int i = 0; i < arg_count; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index >= 0) {
        // 存在管道符号，执行管道命令

        // 分割命令，得到两个子命令
        char *cmd1[MAX_ARGUMENTS];
        char *cmd2[MAX_ARGUMENTS];

        for (int i = 0; i < pipe_index; i++) {
            cmd1[i] = args[i];
        }
        cmd1[pipe_index] = NULL;

        int cmd2_index = 0;
        for (int i = pipe_index + 1; i < arg_count; i++) {
            cmd2[cmd2_index++] = args[i];
        }
        cmd2[cmd2_index] = NULL;

        // 创建管道
        int fd[2];
        if (pipe(fd) == -1) {
            printf("pipe failure\n");
            return;
        }

        // 创建子进程1，执行第一个子命令
        pid_t pid1 = fork();
        if (pid1 == 0) {
            // 将标准输出重定向到管道写入端
            dup2(fd[1], STDOUT);
            close(fd[0]);
            close(fd[1]);

            execve(cmd1[0], cmd1, NULL);
            printf("execvp failure\n");
            exit(-1);
        } else if (pid1 < 0) {
            printf("fork failure\n");
            return;
        }

        // 创建子进程2，执行第二个子命令
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // 将标准输入重定向到管道读取端
            dup2(fd[0], STDIN);
            close(fd[0]);
            close(fd[1]);

            execve(cmd2[0], cmd2, NULL);
            printf("execvp");
            exit(-1);
        } else if (pid2 < 0) {
            printf("fork");
            return;
        }

        close(fd[0]);
        close(fd[1]);

        // 等待子进程结束
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

    } else {
        // 没有管道符号，直接执行命令
        pid_t pid = fork();
        if (pid == 0) {
            execve(args[0], args, NULL);
            printf("execvp failure\n");
            exit(-1);
        } else if (pid < 0) {
            printf("fork failure\n");
            return;
        }

        // 等待子进程结束
        waitpid(pid, NULL, 0);
    }
}

void shell() {
    char command[MAX_COMMAND_LENGTH];
    char cwd[MAX_COMMAND_LENGTH];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            printf("getcwd failure\n");
            exit(-1);
        }

        printf("%s$ ", cwd);

        // 读取用户输入的命令
        if (read(STDIN, command, sizeof(command)) == -1) {
            break;
        }

        // 移除换行符
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;  // 如果输入 exit 则退出 shell
        } else if (strcmp(command, "") == 0) {
            continue;  // 如果输入为空，则继续等待输入
        } else if (strncmp(command, "cd", 2) == 0) {
            // 处理内置指令 cd
            char *path = command + 3;  // 跳过 "cd "，获取路径
            if (chdir(path) != 0) {
                printf("cd failure\n");
            }
        } else {
            // 执行其他命令
            execute_command(command);
        }
    }
}

int main() {
    shell();
    return 0;
}
