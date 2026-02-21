/* rmr_baremetal_compat.c — Instância do arena de memória baremetal */
#define RMR_BAREMETAL_COMPAT_IMPL
#include "rmr_baremetal_compat.h"

/* Arena estático: 1MB alinhado a 64 bytes (cache line) */
__attribute__((aligned(64)))
uint8_t  rmr_arena[RMR_ARENA_SIZE];
uint32_t rmr_arena_ptr = 0u;

void rmr_baremetal_arena_reset(void) {
    rmr_arena_ptr = 0u;
    /* Não zera: performance — kernel_main deve zerar BSS inteiro */
}

uint32_t rmr_baremetal_arena_used(void) {
    return rmr_arena_ptr;
}
