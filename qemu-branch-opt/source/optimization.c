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

//#define HW_OPT_IBTC
#define HW_OPT_SHACK
//#define HW_OPT_DEBUG
/*
 * Shadow Stack
 */

static inline void shack_init(CPUState *env)
{
#ifdef HW_OPT_SHACK
	/* Create shadow stack */
	env->shack = malloc(SHACK_SIZE * sizeof(void *));
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
	p = (shack_slot *) malloc(sizeof(shack_slot));
	p->guest_eip = guest_eip;
	p->host_eip = NULL;
	p->next = env->shack_hash->head[h];
	env->shack_hash->head[h] = p;
	return p;
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
		const int IF1END = gen_new_label();
	/*
		tmp_shack_top = CPUState.shack_top
		tmp_shack_end = CPUState.shack_end
		IF1.BEGIN: tmp_shack_top == tmp_shack_end
			tmp_shack_top = CPUState.shack
			CPUState.shack_top = tmp_shack_top
		IF1.END:
		tmp_shack_top = CPUState.shack_top
		tmp_shack_slot = shack_hash_find(cpu_env, next_eip)
		tmp_shack_top += sizeof(void*);
		shack[tmp_shack_top] = tmp_shack_slot
	 */
		TCGv tmp_shack_top = tcg_temp_new_i32();
	tcg_gen_ld_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
		TCGv tmp_shack_end = tcg_temp_new_i32();
	tcg_gen_ld_i32(tmp_shack_end, cpu_env, offsetof(CPUState, shack_end));
	tcg_gen_brcond_i32(TCG_COND_NE, tmp_shack_top, tmp_shack_end, IF1END);
	/* */
		tmp_shack_top = tcg_temp_new_i32();
	/* */
	tcg_gen_ld_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack));
	tcg_gen_st_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
	gen_set_label(IF1END);
	/* */
		tcg_temp_free_i32(tmp_shack_end);
		tmp_shack_top = tcg_temp_new_i32();
	/* */
	tcg_gen_ld_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
	/* */
		TCGv tmp_shack_slot = tcg_const_ptr(shack_hash_find(env, next_eip));
	/* */
	tcg_gen_st_i32(tmp_shack_slot, tmp_shack_top, 0);
	/* */
		tcg_temp_free_i32(tmp_shack_slot);
	/* */
	tcg_gen_addi_i32(tmp_shack_top, tmp_shack_top, sizeof(void*));
	tcg_gen_st_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
	/* */
		tcg_temp_free_i32(tmp_shack_top);
	/* */
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
		const int IF1END = gen_new_label();
	/*
		tmp_shack_top = CPUState.shack_top
		tmp_shack_begin = CPUState.shack
		IF1.BEGIN tmp_shack_top != tmp_shack_begin
			tmp_shack_top -= sizeof(void*)
			tmp_shack_slot = *tmp_shack_top
			tmp_shack_eip = tmp_shack_slot.guest_eip
			IF2.BEGIN tmp_shack_eip == next_eip
				tmp_shack_eip = tmp_shack_slot.host_eip
				IF3.BEGIN tmp_shack_eip != NULL
					<set next jump>
				IF3.END
			IF2.END
		IF1.END
	*/
		TCGv tmp_next_eip = tcg_temp_local_new_i32();
	tcg_gen_mov_i32(tmp_next_eip, next_eip);
		TCGv tmp_shack_top = tcg_temp_local_new_i32();
	tcg_gen_ld_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
		TCGv tmp_shack_begin = tcg_temp_local_new_i32();
	tcg_gen_ld_i32(tmp_shack_begin, cpu_env, offsetof(CPUState, shack));
	tcg_gen_brcond_i32(TCG_COND_EQ, tmp_shack_top, tmp_shack_begin, IF1END);
		tcg_temp_free_i32(tmp_shack_begin);
	tcg_gen_subi_i32(tmp_shack_top, tmp_shack_top, sizeof(void*));
	tcg_gen_st_i32(tmp_shack_top, cpu_env, offsetof(CPUState, shack_top));
	tcg_gen_ld_i32(tmp_shack_top, tmp_shack_top, 0);
	/* */
		TCGv tmp_shack_eip = tcg_temp_local_new_i32();
	/* */
	tcg_gen_ld_i32(tmp_shack_eip, tmp_shack_top, offsetof(shack_slot, guest_eip));
	tcg_gen_brcond_i32(TCG_COND_NE, tmp_shack_eip, tmp_next_eip, IF1END);
		tcg_temp_free_i32(tmp_next_eip);
		tcg_temp_free_i32(tmp_shack_eip);
	tcg_gen_ld_i32(tmp_shack_top, tmp_shack_top, offsetof(shack_slot, host_eip));
	/* */
		TCGv tmp_NULL = tcg_const_ptr(NULL);
	/* */
	tcg_gen_brcond_i32(TCG_COND_EQ, tmp_shack_top, tmp_NULL, IF1END);
	*gen_opc_ptr++= INDEX_op_jmp;
	*gen_opparam_ptr++= tmp_shack_top;
	
	/* */
		tcg_temp_free_i32(tmp_shack_top);
	/* */
	gen_set_label(IF1END);
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
