#include "utils.h"
#include "mm/page.h"
#include "fs/fat.h"
#include "str.h"
#include "kernel/proc.h"
#include "debug.h"
#include "bio.h"

#include "radix-tree.h"

// ref: lua fast get order 打表+快速移位x8
int luaO_log2(unsigned int x) {
    static const unsigned char log_2[256] = {
      0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
      8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
      8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
      8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    };

    int l = -1;
    while (x >= 256) { l += 8; x >>= 8; }
    return l + log_2[x];
}


void error(char* info, char* s, const char* s1, int d) {
    kprintf(rd("ERROR INFO:\n"));
    kprintf(rd("%s\n"), info);
    kprintf(rd("file: %s\t func: %s\t line: %d\r\n"), s, s1, d);
    enable_sum();
    for (;;);
}

void print_page_info(page_t* page) {
    kprintf("refcnt: %d\tpgnum: %d\taddr: 0x%x\tflags: 0x%x\tpg_pointer: 0x%x\n",
        page->refcnt, page - pages, NR_TO_ADDR(page - pages), page->flags, page);
}


// Empty implentation
uint64_t __sys_dummy(void) {
    return 0;
} 