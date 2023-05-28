#include "common.h"
#include "fs/fs.h"

extern fat32_t *fat;

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

void mythread_init() {
    // create thread with `kthread_create`
}
