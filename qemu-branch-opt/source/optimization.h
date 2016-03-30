/*
 *  (C) 2010 by Computer System Laboratory, IIS, Academia Sinica, Taiwan.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef __OPTIMIZATION_H
#define __OPTIMIZATION_H

/* Comment the next line to disable optimizations. */
#define ENABLE_OPTIMIZATION

/*
 * Shadow Stack
 */
#define SHACK_SIZE		(1048576)
#define SHACK_BUCKET	(1U << 16)
#define SHACK_MASK		(SHACK_BUCKET - 1)

struct shack_slot 
{
	target_ulong guest_eip;	/* key */
	unsigned long *host_eip;			/* value */
	struct shack_slot *next;
};
typedef struct shack_slot shack_slot;

struct shack_hash
{
	shack_slot *head[SHACK_BUCKET];
};

#if TCG_TARGET_REG_BITS == 32
#define tcg_gen_st_ptr          tcg_gen_st_i32
#define tcg_gen_brcond_ptr      tcg_gen_brcond_i32
#define tcg_temp_free_ptr       tcg_temp_free_i32
#define tcg_temp_local_new_ptr  tcg_temp_local_new_i32
#else
#define tcg_gen_st_ptr          tcg_gen_st_i64
#define tcg_gen_brcond_ptr      tcg_gen_brcond_i64
#define tcg_temp_free_ptr       tcg_temp_free_i64
#define tcg_temp_local_new_ptr  tcg_temp_local_new_i64
#endif

#if TARGET_LONG_BITS == 32
#define TCGv TCGv_i32
#else
#define TCGv TCGv_i64
#endif

void shack_set_shadow(CPUState *env, target_ulong guest_eip, unsigned long *host_eip);
inline void insert_unresolved_eip(CPUState *env, target_ulong next_eip, unsigned long *slot);
unsigned long lookup_shadow_ret_addr(CPUState *env, target_ulong pc);
void push_shack(CPUState *env, TCGv_ptr cpu_env, target_ulong next_eip);
void pop_shack(TCGv_ptr cpu_env, TCGv next_eip);

/*
 * Indirect Branch Target Cache
 */
#define IBTC_CACHE_BITS     (16)
#define IBTC_CACHE_SIZE     (1U << IBTC_CACHE_BITS)
#define IBTC_CACHE_MASK     (IBTC_CACHE_SIZE - 1)

struct jmp_pair
{
    target_ulong guest_eip;
    TranslationBlock *tb;
};

struct ibtc_table
{
    struct jmp_pair htable[IBTC_CACHE_SIZE];
};

int init_optimizations(CPUState *env);
void update_ibtc_entry(TranslationBlock *tb);

#endif

/*
 * vim: ts=8 sts=4 sw=4 expandtab
 */
