cmake_minimum_required(VERSION 3.0)

project(BroPluginaf_xdp)

include(BroPlugin)

find_package(KernelHeaders REQUIRED)
find_package(LibBPF REQUIRED)

find_program(CLANG_COMPILER clang)
find_program(LLVM_COMPILER llc)

add_custom_command(OUTPUT af_xdp_bpf.ll COMMAND ${CLANG_COMPILER} -I${KERNELHEADERS_ROOT_DIR} -emit-llvm -c "${CMAKE_CURRENT_SOURCE_DIR}/src/bpf/bpf.c" -o af_xdp_bpf.ll)
add_custom_command(OUTPUT af_xdp_bpf.o COMMAND ${LLVM_COMPILER} -march=bpf -filetype=obj -o af_xdp_bpf.o af_xdp_bpf.ll DEPENDS af_xdp_bpf.ll)
add_custom_target(af_xdp_bpf ALL DEPENDS af_xdp_bpf.o)

bro_plugin_begin(itimmer af_xdp)
bro_plugin_cc(src/Plugin.cc)
bro_plugin_cc(src/AF_XDP.cc)
bro_plugin_cc(src/XDP_BPF.cc)
bro_plugin_bif(src/af_xdp.bif)
bro_plugin_dist_files(README CHANGES COPYING VERSION af_xdp_bpf.o)
bro_plugin_link_library(${LIBBPF_LIBRARY} elf)
list(APPEND _plugin_deps af_xdp_bpf)
bro_plugin_end()

include_directories(${KERNELHEADERS_ROOT_DIR})

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION LIMIT_COUNT 1)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    # Allows building rpm/deb packages via "make package" in build dir.
    include(ConfigurePackaging)
    ConfigurePackaging(${VERSION})
endif ()
