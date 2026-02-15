#ifndef RMR_UNIFIED_KERNEL_H
#define RMR_UNIFIED_KERNEL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  RMR_STATUS_OK = 0,
  RMR_STATUS_ERR_ARG = -1,
  RMR_STATUS_ERR_STATE = -2,
  RMR_STATUS_ERR_NOMEM = -3,
  RMR_STATUS_ERR_VERIFY = -4,
  RMR_STATUS_ERR_ALREADY_INITIALIZED = -5,
  RMR_STATUS_ERR_ALREADY_SHUTDOWN = -6
} rmr_status_t;

typedef struct rmr_kernel rmr_kernel_t;

typedef struct {
  uint32_t seed;
} rmr_kernel_init_desc_t;

typedef struct {
  const uint8_t *data;
  size_t data_len;
} rmr_kernel_ingest_desc_t;

typedef struct {
  uint64_t cpu_cycles;
  uint64_t storage_read_bytes;
  uint64_t storage_write_bytes;
  uint64_t input_bytes;
  uint64_t output_bytes;
  int64_t matrix_m00;
  int64_t matrix_m01;
  int64_t matrix_m10;
  int64_t matrix_m11;
} rmr_kernel_process_desc_t;

typedef struct {
  uint32_t cpu_pressure;
  uint32_t storage_pressure;
  uint32_t io_pressure;
  int64_t matrix_determinant;
} rmr_kernel_process_result_t;

typedef struct {
  const uint8_t *data;
  size_t data_len;
  uint32_t expected_crc32c;
  uint64_t expected_bitraf_hash;
} rmr_kernel_verify_desc_t;

typedef struct {
  uint32_t route_id;
  uint64_t route_signature;
} rmr_kernel_route_result_t;

typedef struct {
  uint32_t crc32c;
  uint64_t bitraf_hash;
  uint32_t entropy_milli;
  uint32_t stage_counter;
} rmr_kernel_ingest_result_t;

typedef struct {
  uint32_t computed_crc32c;
  uint64_t computed_bitraf_hash;
  uint8_t crc_ok;
  uint8_t hash_ok;
} rmr_kernel_verify_result_t;

typedef struct {
  uint64_t audit_signature;
  uint32_t audit_code;
} rmr_kernel_audit_result_t;

typedef struct {
  uint32_t arch;
  uint32_t arch_hex;
  uint32_t word_bits;
  uint32_t ptr_bits;
  uint32_t is_little_endian;
  uint32_t has_cycle_counter;
  uint32_t has_asm_probe;
  uint32_t reg_signature_0;
  uint32_t reg_signature_1;
  uint32_t reg_signature_2;
  uint32_t feature_bits_0;
  uint32_t feature_bits_1;
  uint32_t cacheline_bytes;
  uint32_t cache_hint_l1;
  uint32_t cache_hint_l2;
  uint32_t cache_hint_l3;
  uint32_t page_bytes;
  uint32_t mem_bus_bits;
  uint32_t gpio_word_bits;
  uint32_t gpio_pin_stride;
  uint32_t align_bytes;
} rmr_kernel_capabilities_t;

rmr_status_t rmr_kernel_init(rmr_kernel_t **out_kernel, const rmr_kernel_init_desc_t *desc);
rmr_status_t rmr_kernel_shutdown(rmr_kernel_t **kernel);
rmr_status_t rmr_kernel_ingest(rmr_kernel_t *kernel,
                               const rmr_kernel_ingest_desc_t *desc,
                               rmr_kernel_ingest_result_t *out_result);
rmr_status_t rmr_kernel_process(rmr_kernel_t *kernel,
                                const rmr_kernel_process_desc_t *desc,
                                rmr_kernel_process_result_t *out_result);
rmr_status_t rmr_kernel_route(rmr_kernel_t *kernel,
                              const rmr_kernel_process_result_t *process,
                              rmr_kernel_route_result_t *out_result);
rmr_status_t rmr_kernel_verify(rmr_kernel_t *kernel,
                               const rmr_kernel_verify_desc_t *desc,
                               rmr_kernel_verify_result_t *out_result);
rmr_status_t rmr_kernel_audit(rmr_kernel_t *kernel,
                              const rmr_kernel_ingest_result_t *ingest,
                              const rmr_kernel_process_result_t *process,
                              const rmr_kernel_route_result_t *route,
                              const rmr_kernel_verify_result_t *verify,
                              rmr_kernel_audit_result_t *out_result);
rmr_status_t rmr_kernel_autodetect(rmr_kernel_capabilities_t *out_capabilities);
rmr_status_t rmr_kernel_get_capabilities(const rmr_kernel_t *kernel,
                                         rmr_kernel_capabilities_t *out_capabilities);

#ifdef __cplusplus
}
#endif

#endif
