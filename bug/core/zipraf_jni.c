/* zipraf_jni.c — ZIPRAF JNI BRIDGE  ← ARQUIVO FALTANTE RESOLVIDO
 * ∆RAFAELIA_CORE·Ω
 * package: com.vectras.vm.core
 * Class:   ZiprafEngine
 *
 * Implements ALL native methods declared in ZiprafEngine.java:
 *   nativeOpen          → zr_open via zipraf_core.c
 *   nativeTripleComplete→ rmr_ll_triple_complete
 *   nativeCrc32cHw      → rmr_lowlevel_crc32c_hw
 *   nativeGeo4x4Trace   → rmr_ll_geo4x4_trace
 *   nativeVirtSize      → rmr_ll_virt_size
 * ─────────────────────────────────────────────────────── */
#include <jni.h>
#include "rmr_unified_kernel.h"
#include "rmr_lowlevel.h"

/* forward declarations from zipraf_core.c */
extern int   zr_open(void *container_out, const rmr_u8 *data, rmr_u32 len);
extern int   zr_triple_complete(rmr_u64 *off, rmr_u32 *len, rmr_u32 *crc,
                                 rmr_u32 valid,
                                 const rmr_u8 *data, rmr_u32 dlen);
extern rmr_u32 zr_crc32c(rmr_u32 seed, const rmr_u8 *data, rmr_u32 len);
extern rmr_u32 zr_geo4x4_trace(const rmr_u16 *cells, rmr_u32 count);
extern rmr_u64 zr_virt_size(rmr_u64 phys, rmr_u32 cells, rmr_u32 paths);

/* ── field/class cache ── */
static jfieldID fid_cellCount    = 0;
static jfieldID fid_virtualCount = 0;
static jfieldID fid_physSize     = 0;
static jfieldID fid_virtSize     = 0;
static jfieldID fid_globalParA   = 0;
static jfieldID fid_globalParB   = 0;

/* Triple fields */
static jfieldID fid_t_offset = 0;
static jfieldID fid_t_length = 0;
static jfieldID fid_t_crc32  = 0;
static jfieldID fid_t_valid  = 0;

static int s_fields_cached = 0;

static void s_cache_fields(JNIEnv *env, jobject container, jobject triple) {
    if (s_fields_cached) return;

    jclass cCls = (*env)->GetObjectClass(env, container);
    if (!cCls) return;
    fid_cellCount    = (*env)->GetFieldID(env, cCls, "cellCount",    "I");
    fid_virtualCount = (*env)->GetFieldID(env, cCls, "virtualCount", "I");
    fid_physSize     = (*env)->GetFieldID(env, cCls, "physSize",     "J");
    fid_virtSize     = (*env)->GetFieldID(env, cCls, "virtSize",     "J");
    fid_globalParA   = (*env)->GetFieldID(env, cCls, "globalParityA","I");
    fid_globalParB   = (*env)->GetFieldID(env, cCls, "globalParityB","I");

    if (triple) {
        jclass tCls = (*env)->GetObjectClass(env, triple);
        if (tCls) {
            fid_t_offset = (*env)->GetFieldID(env, tCls, "offset", "J");
            fid_t_length = (*env)->GetFieldID(env, tCls, "length", "I");
            fid_t_crc32  = (*env)->GetFieldID(env, tCls, "crc32",  "I");
            fid_t_valid  = (*env)->GetFieldID(env, tCls, "valid",  "I");
        }
    }
    s_fields_cached = 1;
}

/* ════════════════════════════════════════════════════════
 * nativeOpen(Container c, byte[] data) → int
 * ════════════════════════════════════════════════════════ */
JNIEXPORT jint JNICALL
Java_com_vectras_vm_core_ZiprafEngine_nativeOpen(
        JNIEnv *env, jclass clazz,
        jobject container, jbyteArray data_arr) {
    (void)clazz;
    if (!container || !data_arr) return -1;

    const jsize dlen = (*env)->GetArrayLength(env, data_arr);
    if (dlen <= 0) return -2;

    jbyte *raw = (*env)->GetPrimitiveArrayCritical(env, data_arr, (void*)0);
    if (!raw) return -3;

    /* ── Call native zipraf parser ── */
    /* We use a compact context struct to receive results */
    typedef struct {
        int cell_count;
        int virtual_count;
        long long virt_size;
        int parity_a;
        int parity_b;
    } zr_result_t;

    zr_result_t result = {0, 0, 0, 0, 0};
    int rc = zr_open(&result, (const rmr_u8*)raw, (rmr_u32)dlen);

    (*env)->ReleasePrimitiveArrayCritical(env, data_arr, raw, JNI_ABORT);

    if (rc <= 0) return (jint)rc;

    /* Cache field IDs on first call */
    s_cache_fields(env, container, (jobject)0);
    if (!fid_cellCount) return -4;

    /* ── Write back to Java Container object ── */
    if (fid_cellCount)    (*env)->SetIntField (env, container, fid_cellCount,    (jint)result.cell_count);
    if (fid_virtualCount) (*env)->SetIntField (env, container, fid_virtualCount, (jint)result.virtual_count);
    if (fid_virtSize)     (*env)->SetLongField(env, container, fid_virtSize,     (jlong)result.virt_size);
    if (fid_globalParA)   (*env)->SetIntField (env, container, fid_globalParA,   (jint)result.parity_a);
    if (fid_globalParB)   (*env)->SetIntField (env, container, fid_globalParB,   (jint)result.parity_b);

    return (jint)rc;
}

/* ════════════════════════════════════════════════════════
 * nativeTripleComplete(Triple t, byte[] data) → boolean
 * ════════════════════════════════════════════════════════ */
JNIEXPORT jboolean JNICALL
Java_com_vectras_vm_core_ZiprafEngine_nativeTripleComplete(
        JNIEnv *env, jclass clazz,
        jobject triple, jbyteArray data_arr) {
    (void)clazz;
    if (!triple || !data_arr) return JNI_FALSE;

    /* Cache field IDs if needed (need a dummy container class ref) */
    if (!fid_t_valid) {
        jclass tCls = (*env)->GetObjectClass(env, triple);
        if (!tCls) return JNI_FALSE;
        fid_t_offset = (*env)->GetFieldID(env, tCls, "offset", "J");
        fid_t_length = (*env)->GetFieldID(env, tCls, "length", "I");
        fid_t_crc32  = (*env)->GetFieldID(env, tCls, "crc32",  "I");
        fid_t_valid  = (*env)->GetFieldID(env, tCls, "valid",  "I");
    }
    if (!fid_t_valid) return JNI_FALSE;

    /* Read Triple fields */
    rmr_u64 off   = (rmr_u64)(*env)->GetLongField (env, triple, fid_t_offset);
    rmr_u32 len   = (rmr_u32)(*env)->GetIntField  (env, triple, fid_t_length);
    rmr_u32 crc   = (rmr_u32)(*env)->GetIntField  (env, triple, fid_t_crc32);
    rmr_u32 valid = (rmr_u32)(*env)->GetIntField  (env, triple, fid_t_valid);

    if (valid == 7u) return JNI_TRUE; /* already complete */

    const jsize dlen = (*env)->GetArrayLength(env, data_arr);
    jbyte *raw = (*env)->GetPrimitiveArrayCritical(env, data_arr, (void*)0);
    if (!raw) return JNI_FALSE;

    int rc = zr_triple_complete(&off, &len, &crc, valid,
                                (const rmr_u8*)raw, (rmr_u32)dlen);

    (*env)->ReleasePrimitiveArrayCritical(env, data_arr, raw, JNI_ABORT);

    if (rc != 0) return JNI_FALSE;

    /* Write back completed fields */
    (*env)->SetLongField(env, triple, fid_t_offset, (jlong)off);
    (*env)->SetIntField (env, triple, fid_t_length, (jint)len);
    (*env)->SetIntField (env, triple, fid_t_crc32,  (jint)crc);
    (*env)->SetIntField (env, triple, fid_t_valid,  (jint)7);

    return JNI_TRUE;
}

/* ════════════════════════════════════════════════════════
 * nativeCrc32cHw(int seed, byte[] data, int off, int len) → int
 * ════════════════════════════════════════════════════════ */
JNIEXPORT jint JNICALL
Java_com_vectras_vm_core_ZiprafEngine_nativeCrc32cHw(
        JNIEnv *env, jclass clazz,
        jint seed, jbyteArray data_arr, jint offset, jint length) {
    (void)clazz;
    if (!data_arr || offset < 0 || length < 0) return seed;

    const jsize total = (*env)->GetArrayLength(env, data_arr);
    if (offset > total || length > (total - offset)) return seed;

    jbyte *raw = (*env)->GetPrimitiveArrayCritical(env, data_arr, (void*)0);
    if (!raw) return seed;

    rmr_u32 result = rmr_lowlevel_crc32c_hw(
        (rmr_u32)seed,
        (const rmr_u8*)raw + (jsize)offset,
        (rmr_u32)length);

    (*env)->ReleasePrimitiveArrayCritical(env, data_arr, raw, JNI_ABORT);
    return (jint)result;
}

/* ════════════════════════════════════════════════════════
 * nativeGeo4x4Trace(short[] cells) → int
 * ════════════════════════════════════════════════════════ */
JNIEXPORT jint JNICALL
Java_com_vectras_vm_core_ZiprafEngine_nativeGeo4x4Trace(
        JNIEnv *env, jclass clazz, jshortArray cells_arr) {
    (void)clazz;
    if (!cells_arr) return 0;

    const jsize count = (*env)->GetArrayLength(env, cells_arr);
    if (count <= 0) return 0;

    jshort *raw = (*env)->GetPrimitiveArrayCritical(env, cells_arr, (void*)0);
    if (!raw) return 0;

    /* rmr_ll_geo4x4_trace expects rmr_u16* */
    rmr_u32 trace = zr_geo4x4_trace((const rmr_u16*)raw,
                                     (rmr_u32)(count > 16 ? 16 : count));

    (*env)->ReleasePrimitiveArrayCritical(env, cells_arr, raw, JNI_ABORT);
    return (jint)trace;
}

/* ════════════════════════════════════════════════════════
 * nativeVirtSize(long physSize, int cellCount, int pathCount) → long
 * ════════════════════════════════════════════════════════ */
JNIEXPORT jlong JNICALL
Java_com_vectras_vm_core_ZiprafEngine_nativeVirtSize(
        JNIEnv *env, jclass clazz,
        jlong phys_size, jint cell_count, jint path_count) {
    (void)env; (void)clazz;
    if (phys_size <= 0 || cell_count <= 0) return phys_size;

    rmr_u64 vs = zr_virt_size(
        (rmr_u64)phys_size,
        (rmr_u32)cell_count,
        (rmr_u32)(path_count > 0 ? path_count : 0));

    return (jlong)vs;
}
