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

#define HW_OPT_IBTC
#define HW_OPT_SHACK
/*
 * Shadow Stack
 */
list_t *shadow_hash_list;

static inline void shack_init(CPUState *env)
{
#ifdef HW_OPT_SHACK
	/* Create shadow stack */
	env->shack = (uint64_t *) calloc(SHACK_SIZE, sizeof(uint64_t));
	env->shack_top = env->shack;
	env->shack_end = env->shack + SHACK_SIZE;
	env->shack_hash = (struct shack_hash*) calloc(1, sizeof(struct shack_hash));
#endif
}

/*
 * shack_set_shadow()
 *  Insert a guest eip to host eip pair if it is not yet created.
 */
static shack_slot* shack_hash_find(CPUState *env, target_ulong guest_eip) {
	target_ulong h = guest_eip & SHACK_MASK;
	shack_slot *p = env->shack_hash->head[h];
	for (; p != NULL; p = p->next) {
		if (p->guest_eip == guest_eip)
			return p;
	}
	p = (shack_slot *) calloc(1, sizeof(shack_slot));
	p->guest_eip = guest_eip;
	p->host_eip = NULL;
	p->next = env->shack_hash->head[h];
	return env->shack_hash->head[h] = p;
}
void shack_set_shadow(CPUState *env, target_ulong guest_eip, 
						unsigned long *host_eip)
{
#ifdef HW_OPT_SHACK
	shack_hash_find(env, guest_eip)->host_eip = host_eip;
#endif
}

/*
 * helper_shack_flush()
 *  Reset shadow stack.
 */
void helper_shack_flush(CPUState *env)
{
#ifdef HW_OPT_SHACK
	env->shack_top = env->shack;
#endif
}

/*
 * push_shack()
 *  Push next guest eip into shadow stack.
 */
void push_shack(CPUState *env, TCGv_ptr cpu_env, target_ulong next_eip)
{
#ifdef HW_OPT_SHACK
	/* TODO */
#endif
}

/*
 * pop_shack()
 *  Pop next host eip from shadow stack.
 */
void pop_shack(TCGv_ptr cpu_env, TCGv next_eip)
{
#ifdef HW_OPT_SHACK
	/* TODO */
#endif
}


/*
 * Indirect Branch Target 
 */
/* in cpu-exec.c to call update_ibtc_entry() */
__thread int update_ibtc;
/* Create Indirect Branch Target Cache */
struct ibtc_table *ibtc_cache;

/*
 * helper_lookup_ibtc()
 *  Look up IBTC. Return next host eip if cache hit or
 *  back-to-dispatcher stub address if cache miss.
 */
void *helper_lookup_ibtc(target_ulong guest_eip)
{
#ifdef HW_OPT_IBTC
	target_ulong hash = guest_eip & IBTC_CACHE_MASK;
	if (ibtc_cache->htable[hash].guest_eip == guest_eip)
		return ibtc_cache->htable[hash].tb->tc_ptr;
	update_ibtc = 1;
#endif
    return optimization_ret_addr;
}

/*
 * update_ibtc_entry()
 *  Populate eip and tb pair in IBTC entry.
 */
void update_ibtc_entry(TranslationBlock *tb)
{
#ifdef HW_OPT_IBTC
	update_ibtc = 0;
	target_ulong hash = tb->pc & IBTC_CACHE_MASK;
	ibtc_cache->htable[hash].guest_eip = tb->pc;
	ibtc_cache->htable[hash].tb = tb;
#endif
}

/*
 * ibtc_init()
 *  Create and initialize indirect branch target cache.
 */
static inline void ibtc_init(CPUState *env)
{
#ifdef HW_OPT_IBTC
	update_ibtc = 0;
	ibtc_cache = (struct ibtc_table *) calloc(1, sizeof(struct ibtc_table));
#endif
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
