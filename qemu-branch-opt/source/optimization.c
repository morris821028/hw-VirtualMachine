/*
 *  (C) 2010 by Computer System Laboratory, IIS, Academia Sinica, Taiwan.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdlib.h>
#include "exec-all.h"
#include "tcg-op.h"
#include "helper.h"
#define GEN_HELPER 1
#include "helper.h"
#include "optimization.h"

extern uint8_t *optimization_ret_addr;

/*
 * Shadow Stack
 */
list_t *shadow_hash_list;

static inline void shack_init(CPUState *env)
{
	/* Create shadow stack */
	env->shack = (uint64_t *) calloc(SHACK_SIZE, sizeof(uint64_t));
	env->shack_top = env->shack;
	env->shack_end = env->shack + SHACK_SIZE;
	env->shadow_hash_list = (void *) calloc(TB_JMP_CACHE_SIZE, sizeof(list_t));
	env->shadow_ret_count = 0;
	env->shadow_ret_addr = 
				(unsigned long *) calloc(SHACK_SIZE, sizeof(unsigned long));
}

/*
 * shack_set_shadow()
 *  Insert a guest eip to host eip pair if it is not yet created.
 */
void shack_set_shadow(CPUState *env, target_ulong guest_eip, 
						unsigned long *host_eip)
{
}

/*
 * helper_shack_flush()
 *  Reset shadow stack.
 */
void helper_shack_flush(CPUState *env)
{
}

/*
 * push_shack()
 *  Push next guest eip into shadow stack.
 */
void push_shack(CPUState *env, TCGv_ptr cpu_env, target_ulong next_eip)
{
}

/*
 * pop_shack()
 *  Pop next host eip from shadow stack.
 */
void pop_shack(TCGv_ptr cpu_env, TCGv next_eip)
{
}


/*
 * Indirect Branch Target 
 */
__thread int update_ibtc;
/* Create Indirect Branch Target Cache */
struct ibtc_table ibtc_cache;

/*
 * helper_lookup_ibtc()
 *  Look up IBTC. Return next host eip if cache hit or
 *  back-to-dispatcher stub address if cache miss.
 */
void *helper_lookup_ibtc(target_ulong guest_eip)
{
	target_ulong hash = guest_eip & IBTC_CACHE_MASK;
	if (ibtc_cache.htable[hash].guest_eip == guest_eip)
		return ibtc_cache.htable[hash].tb->tc_ptr;
	update_ibtc = 1;
    return optimization_ret_addr;
}

/*
 * update_ibtc_entry()
 *  Populate eip and tb pair in IBTC entry.
 */
void update_ibtc_entry(TranslationBlock *tb)
{
	update_ibtc = 0;
	target_ulong hash = tb->pc & IBTC_CACHE_MASK;
	ibtc_cache.htable[hash].guest_eip = tb->pc;
	ibtc_cache.htable[hash].tb = tb;
}

/*
 * ibtc_init()
 *  Create and initialize indirect branch target cache.
 */
static inline void ibtc_init(CPUState *env)
{
	update_ibtc = 0;
}

/*
 * init_optimizations()
 *  Initialize optimization subsystem.
 */
int init_optimizations(CPUState *env)
{
    shack_init(env);
    ibtc_init(env);
    return 0;
}

/*
 * vim: ts=8 sts=4 sw=4 expandtab
 */
