# 内核线程

## 一、实验介绍

在内核中，除了通常的进程外，还存在着一些“进程”，它们*始终在内核态下工作*，帮助内核完成一些运行时的子任务，比如，磁盘缓存脏块/脏页的定时写回等，我们称这样的“进程”为内核线程。在本实验中，你将亲自动手体验这一子功能，并帮助你完成一些简单的任务。

## 二、实验目的

1. 理解和掌握进程概念
2. 学会使用内核线程，理解它的创建与执行过程

## 三、实验要求

1. 创建指定的线程工作函数

## 四、实验步骤

我们的文件镜像中存在一个名为`password`的文件，你需要在此内核线程中读取此文件并输出。

```c
// src/mythread.c

// workder function
void mythread_fn(proc_t *me) {
    // Wait for fs initialization
    while (!fat) 
        yield();
    __sync_synchronize();

    // TODO:
    // 1. read the file '/password'
    // 2. output(print) the file content

    // Loop to avoid exit which is not supported now :(
    while (1)
        ;
}

// init called in `main`
void mythread_init() {
    // create thread with `kthread_create`
}
```

具体步骤如下：

1. 使用`kthread_create`在`mythread_init`中初始化我们的线程
2. 使用`namee`读取`password`所对应的文件目录项
3. 使用`reade`读取文件内容
4. 使用`kprintf`输出内容
