/* vectra_cpu_safe.c — ANDROID-SAFE CPU DETECT WRAPPER
 * ∆RAFAELIA_CORE·Ω
 * Replaces unsafe *_EL1 direct reads with:
 *   ① getauxval(AT_HWCAP*) — always safe EL0
 *   ② /proc/cpuinfo fallback
 *   ③ SIGILL probe for optional MIDR_EL1 (non-fatal)
 *   ④ CTR_EL0 / CNTFRQ_EL0 (valid EL0 regs)
 *
 * Drop-in for vectra_cpu_detect.c on Android
 * ─────────────────────────────────────────────────────── */
#include <jni.h>
#include "vectra_cpu_detect.h"
#include "rmr_unified_kernel.h"

/* ── Lazy-initialized safe capabilities ── */
static vcpu_caps_t  g_safe_caps;
static int          g_safe_ready = 0;

/* ── Android-safe init (delegates to rmr_hw_detect) ── */
static int s_safe_detect(vcpu_caps_t *caps) {
    if (!caps) return -1;

    /* Use rmr layer — it already has SIGILL guard + getauxval */
    rmr_jni_capabilities_t rmr_caps = {0};
    int rc = rmr_hw_detect_fill(&rmr_caps);

    /* Map rmr_caps → vcpu_caps_t */
    caps->arch_id        = (vcpu_u8)rmr_caps.arch_id;
    caps->feature_mask   = rmr_caps.feature_mask;
    caps->feature_bits_hi= rmr_caps.feature_bits_hi;
    caps->midr_raw       = rmr_caps.midr_raw;
    caps->tsc_hz         = rmr_caps.tsc_hz;

    /* cache info */
    caps->cache_l1_data_bytes = rmr_caps.cache_line_bytes * 8u;  /* est. */
    caps->cache_line_bytes    = rmr_caps.cache_line_bytes;
    caps->page_bytes          = rmr_caps.page_bytes;

    /* reg map */
    caps->reg_map.sys_reg_access =
        (rmr_caps.midr_raw != 0u) ? 1u : 0u;  /* MIDR readable = EL1 accessible */

#if defined(__aarch64__)
    caps->reg_map.gpr_count       = 31u;
    caps->reg_map.gpr_width_bits  = 64u;
    caps->reg_map.simd_count      = 32u;
    caps->reg_map.simd_width_bits = 128u;
    caps->reg_map.fpr_count       = 32u;
    caps->reg_map.fpr_width_bits  = 64u;
    if (rmr_caps.feature_mask & RMR_FEAT_SVE) {
        caps->reg_map.sve_vl_bytes = 16u; /* 128-bit default; probe not done */
        caps->reg_map.simd_width_bits = 512u;
    }
    caps->reg_map.gpr_live_mask     = 0xFFFFFFFFu;
    caps->reg_map.callee_save_mask  = 0x0001FF00u; /* x19-x28, fp, lr */
    caps->reg_map.clobber_mask      = 0x000000FFu; /* x0-x7 */
#elif defined(__arm__)
    caps->reg_map.gpr_count       = 16u;
    caps->reg_map.gpr_width_bits  = 32u;
    caps->reg_map.simd_count       = 32u;
    caps->reg_map.simd_width_bits  = 64u;
#elif defined(__x86_64__)
    caps->reg_map.gpr_count       = 16u;
    caps->reg_map.gpr_width_bits  = 64u;
    caps->reg_map.simd_count       = 16u;
    caps->reg_map.simd_width_bits  = 128u;
    if (rmr_caps.feature_bits_hi & (1u<<0)) /* AVX2 */
        caps->reg_map.simd_width_bits = 256u;
    if (rmr_caps.feature_bits_hi & (1u<<1)) /* AVX512 */
        caps->reg_map.simd_width_bits = 512u;
#endif

    return rc;
}

/* ── Public vcpu_detect (replaces the original EL1-unsafe version) ── */
int vcpu_detect(vcpu_caps_t *caps) {
    if (!caps) return -1;
    if (!g_safe_ready) {
        int rc = s_safe_detect(&g_safe_caps);
        if (rc == 0) g_safe_ready = 1;
        else return rc;
    }
    /* copy */
    const vcpu_u8 *src = (const vcpu_u8*)&g_safe_caps;
    vcpu_u8 *dst = (vcpu_u8*)caps;
    for (vcpu_u32 i = 0; i < sizeof(vcpu_caps_t); i++) dst[i] = src[i];
    return 0;
}
