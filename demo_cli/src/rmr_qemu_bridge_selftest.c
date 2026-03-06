#include "rmr_qemu_bridge.h"
#include "rmr_unified_jni_base.h"
#include "rmr_unified_kernel.h"

#include <stdio.h>
#include <string.h>

static int expect(int cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "FAIL: %s\n", msg);
    return 1;
  }
  return 0;
}

int main(void) {
  int failed = 0;
  RmR_QemuPlan plan;
  RmR_HW_Info hw;
  memset(&hw, 0, sizeof(hw));
  hw.arch = 4u;
  hw.ptr_bits = 64u;
  hw.word_bits = 64u;
  hw.cache_hint_l2 = 1024u * 1024u;
  hw.cache_hint_l4 = 32u * 1024u * 1024u;

  RmR_QemuPlan_Autotune(&hw, RMR_GUEST_ARCH_X86_64, 8192u, 0u, &plan);
  failed += expect(plan.preset == RMR_QEMU_PRESET_PERFORMANCE, "preset performance");
  failed += expect(plan.vm_cpus >= 2u, "vm cpus >=2");

  char out[512];
  failed += expect(RmR_QemuPlan_BuildArgs(&plan, out, sizeof(out)) == 0, "build args");
  failed += expect(strstr(out, "-smp") != NULL, "args has smp");
  failed += expect(strstr(out, "-accel kvm") != NULL, "args has kvm");

  {
    RmR_QemuPlan with_l4;
    RmR_QemuPlan without_l4;
    RmR_HW_Info hw_no_l4 = hw;
    hw_no_l4.cache_hint_l4 = 0u;
    RmR_QemuPlan_Autotune(&hw, RMR_GUEST_ARCH_X86_64, 4096u, 0u, &with_l4);
    RmR_QemuPlan_Autotune(&hw_no_l4, RMR_GUEST_ARCH_X86_64, 4096u, 0u, &without_l4);
    failed += expect(with_l4.preset == RMR_QEMU_PRESET_PERFORMANCE, "l4 keeps performance preset");
    failed += expect(with_l4.host_cores >= without_l4.host_cores, "l4 increases or preserves host cores");
  }

  RmR_QemuPlan_Autotune(&hw, RMR_GUEST_ARCH_PPC, 2048u, 0u, &plan);
  failed += expect(plan.preset == RMR_QEMU_PRESET_COMPATIBILITY, "ppc preset compatibility");
  failed += expect(plan.use_virtio == 0u, "ppc disables virtio");
  failed += expect(plan.use_iothread == 0u, "ppc disables iothread");
  failed += expect(RmR_QemuPlan_BuildArgs(&plan, out, sizeof(out)) == 0, "build args ppc");
  failed += expect(strstr(out, "-drive if=ide") != NULL, "ppc uses ide fallback");
  failed += expect(strstr(out, "-device rtl8139,netdev=n0") != NULL, "ppc uses rtl8139 fallback");
  failed += expect(strstr(out, "virtio-scsi-pci") == NULL, "ppc avoids virtio-scsi device");

  RmR_QmpTelemetry tele;
  failed += expect(RmR_QmpTelemetry_Parse("{\"return\":{\"status\":\"running\"}}", &tele) == 0, "parse status");
  failed += expect(tele.running == 1u, "running state");

  failed += expect(RmR_QmpTelemetry_Parse("{\"return\":[{\"cpu-index\":0,\"halted\":false},{\"cpu-index\":1,\"halted\":true}]}", &tele) == 0, "parse cpus");
  failed += expect(tele.vcpu_count == 2u, "vcpu count");
  failed += expect(tele.running_count == 1u, "running count");

  failed += expect(RmR_QmpTelemetry_Parse("{\"return\":[{\"cpu-index\":0,\"halted\" : false},{\"cpu-index\":1,\"halted\" : true}]}", &tele) == 0, "parse cpus spaced halted");
  failed += expect(tele.vcpu_count == 2u, "vcpu count spaced halted");
  failed += expect(tele.halted_count == 1u, "halted count spaced halted");
  failed += expect(tele.running_count == 1u, "running count spaced halted");

  failed += expect(RmR_QmpTelemetry_Parse("{\"return\":{\"cpus\":999999999999}}", &tele) == 0, "parse cpus overflow clamp");
  failed += expect(tele.vcpu_count == 0xFFFFFFFFu, "cpus overflow clamps to u32 max");


  {
    rmr_legacy_kernel_t *legacy = NULL;
    rmr_legacy_kernel_init_desc_t init_desc;
    rmr_legacy_kernel_process_result_t process;
    rmr_legacy_kernel_route_result_t route_ref;
    rmr_legacy_kernel_route_result_t route_run;
    uint32_t i;

    memset(&init_desc, 0, sizeof(init_desc));
    init_desc.seed = 0xC0FFEE11u;
    failed += expect(rmr_legacy_kernel_init(&legacy, &init_desc) == RMR_STATUS_OK, "legacy init");
    memset(&process, 0, sizeof(process));
    process.cpu_pressure = 2411u;
    process.storage_pressure = 1777u;
    process.io_pressure = 1888u;
    process.matrix_determinant = 0x1234ABCD13572468LL;
    failed += expect(rmr_legacy_kernel_route(legacy, &process, &route_ref) == RMR_STATUS_OK, "legacy first route");

    for (i = 0u; i < 16u; ++i) {
      rmr_legacy_kernel_t *replay = NULL;
      failed += expect(rmr_legacy_kernel_init(&replay, &init_desc) == RMR_STATUS_OK, "legacy replay init");
      failed += expect(rmr_legacy_kernel_route(replay, &process, &route_run) == RMR_STATUS_OK, "legacy replay route");
      failed += expect(route_run.route_id == route_ref.route_id, "legacy route id stable");
      failed += expect(route_run.route_signature == route_ref.route_signature, "legacy route signature stable");
      failed += expect(route_run.toroidal.u == route_ref.toroidal.u, "legacy toroidal u stable");
      failed += expect(route_run.toroidal.v == route_ref.toroidal.v, "legacy toroidal v stable");
      failed += expect(route_run.toroidal.psi == route_ref.toroidal.psi, "legacy toroidal psi stable");
      failed += expect(route_run.toroidal.chi == route_ref.toroidal.chi, "legacy toroidal chi stable");
      failed += expect(route_run.toroidal.rho == route_ref.toroidal.rho, "legacy toroidal rho stable");
      failed += expect(route_run.toroidal.delta == route_ref.toroidal.delta, "legacy toroidal delta stable");
      failed += expect(route_run.toroidal.sigma == route_ref.toroidal.sigma, "legacy toroidal sigma stable");
      failed += expect(rmr_legacy_kernel_shutdown(&replay) == RMR_STATUS_OK, "legacy replay shutdown");
    }

    failed += expect(rmr_legacy_kernel_shutdown(&legacy) == RMR_STATUS_OK, "legacy shutdown");
  }

  {
    RmR_UnifiedKernel kernel;
    RmR_UnifiedConfig cfg;
    RmR_UnifiedProcessState process;
    RmR_UnifiedRouteState route_ref;
    RmR_UnifiedRouteState route_run;
    uint32_t i;

    memset(&kernel, 0, sizeof(kernel));
    cfg.seed = 0xC0FFEE11u;
    cfg.arena_bytes = 1024u * 1024u;
    failed += expect(RmR_UnifiedKernel_Init(&kernel, &cfg) == 0, "unified init");
    failed += expect(RmR_UnifiedKernel_Process(&kernel,
                                               9912312u,
                                               664433u,
                                               998877u,
                                               123456u,
                                               789012u,
                                               21,
                                               -7,
                                               5,
                                               34,
                                               &process) == 0,
                     "unified process");
    failed += expect(RmR_UnifiedKernel_Route(&kernel, &process, &route_ref) == 0, "unified first route");

    for (i = 0u; i < 16u; ++i) {
      RmR_UnifiedKernel replay;
      memset(&replay, 0, sizeof(replay));
      failed += expect(RmR_UnifiedKernel_Init(&replay, &cfg) == 0, "unified replay init");
      failed += expect(RmR_UnifiedKernel_Process(&replay,
                                                 9912312u,
                                                 664433u,
                                                 998877u,
                                                 123456u,
                                                 789012u,
                                                 21,
                                                 -7,
                                                 5,
                                                 34,
                                                 &process) == 0,
                       "unified replay process");
      failed += expect(RmR_UnifiedKernel_Route(&replay, &process, &route_run) == 0, "unified replay route");
      failed += expect(route_run.route_id == route_ref.route_id, "route id stable");
      failed += expect(route_run.route_tag == route_ref.route_tag, "route tag stable");
      failed += expect(route_run.toroidal.u == route_ref.toroidal.u, "toroidal u stable");
      failed += expect(route_run.toroidal.v == route_ref.toroidal.v, "toroidal v stable");
      failed += expect(route_run.toroidal.psi == route_ref.toroidal.psi, "toroidal psi stable");
      failed += expect(route_run.toroidal.chi == route_ref.toroidal.chi, "toroidal chi stable");
      failed += expect(route_run.toroidal.rho == route_ref.toroidal.rho, "toroidal rho stable");
      failed += expect(route_run.toroidal.delta == route_ref.toroidal.delta, "toroidal delta stable");
      failed += expect(route_run.toroidal.sigma == route_ref.toroidal.sigma, "toroidal sigma stable");
      failed += expect(RmR_UnifiedKernel_Shutdown(&replay) == 0, "unified replay shutdown");
    }

    failed += expect(RmR_UnifiedKernel_Shutdown(&kernel) == 0, "unified shutdown");
  }

  if (failed != 0) {
    fprintf(stderr, "rmr_qemu_bridge_selftest FAILED (%d)\n", failed);
    return 1;
  }
  printf("rmr_qemu_bridge_selftest OK\n");
  return 0;
}
