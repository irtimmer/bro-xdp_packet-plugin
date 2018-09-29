# - Try to find LibBPF headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibBPF)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LIBBPF_ROOT_DIR  Set this variable to the root installation of
#                      LibBPF if the module has problems finding
#                      the proper installation path.
#
# Variables defined by this module:
#
#  LIBBPF_FOUND         System has LibBPF libs/headers
#  LIBBPF_LIBRARIES     The LibBPF libraries
#  LIBBPF_INCLUDE_DIR   The location of LibBPF headers

find_path(LIBBPF_ROOT_DIR
    NAMES include/bpf/libbpf.h
)

find_path(LIBBPF_INCLUDE_DIR
    NAMES bpf/libbpf.h
    HINTS ${LIBBPF_ROOT_DIR}/include
)

find_library(LIBBPF_LIBRARY
    NAMES bpf
    PATHS ${LIBBPF_ROOT_DIR}/lib ${LIBBPF_ROOT_DIR}/lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    LibBPF
    DEFAULT_MSG
    LIBBPF_LIBRARY
    LIBBPF_INCLUDE_DIR
)

mark_as_advanced(
    LIBBPF_ROOT_DIR
    LIBBPF_INCLUDE_DIR
    LIBBPF_LIBRARY
)
