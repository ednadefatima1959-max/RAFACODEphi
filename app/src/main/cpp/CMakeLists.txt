cmake_minimum_required(VERSION 3.22.1)
project(vectra_core_accel C)

set(RMR_UNIFIED_CHAIN_SRCS
    ../../../../engine/rmr/src/rmr_unified_kernel.c
    ../../../../engine/rmr/src/rmr_hw_detect.c
    ../../../../engine/rmr/src/rmr_policy_kernel.c
    ../../../../engine/rmr/src/bitraf.c
    ../../../../engine/rmr/src/rmr_corelib.c
    ../../../../engine/rmr/src/rmr_cycles.c
    ../../../../engine/rmr/src/rmr_ll_ops.c
    ../../../../engine/rmr/src/rmr_math_fabric.c
    ../../../../engine/rmr/src/rmr_ll_tuning.c)

set(RMR_UNIFIED_PROVIDER_LIB "" CACHE FILEPATH "External library that already provides RmR_UnifiedKernel_* symbols")

if(RMR_UNIFIED_PROVIDER_LIB)
    set(RMR_UNIFIED_LINK_DEPS ${RMR_UNIFIED_PROVIDER_LIB})
else()
    add_library(rmr_unified_chain STATIC ${RMR_UNIFIED_CHAIN_SRCS})
    target_include_directories(rmr_unified_chain PUBLIC ../../../../ ../../../../engine/rmr/include)
    target_compile_options(rmr_unified_chain PRIVATE -O3 -ffast-math -fvisibility=hidden)
    set(RMR_UNIFIED_LINK_DEPS rmr_unified_chain)
endif()

add_library(vectra_core_accel SHARED
    vectra_core_accel.c)

find_library(log-lib log)
if(NOT log-lib)
    message(WARNING "Android log library not found in host configure; proceeding without it")
    set(log-lib "")
endif()

target_compile_options(vectra_core_accel PRIVATE -O3 -ffast-math -fvisibility=hidden)

target_link_libraries(vectra_core_accel PRIVATE ${RMR_UNIFIED_LINK_DEPS} ${log-lib})

target_include_directories(vectra_core_accel PRIVATE ../../../../ ../../../../engine/rmr/include)
