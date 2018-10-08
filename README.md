# irtimmer::AF_XDP

This plugin provides native AF_XDP support for Bro.

## Requirements

- Linux kernel 4.18+
- Linux kernel source files
- LLVM and CLang to compile eBPF
- Bro source files
- libbpf (included in the linux kernel source tree)

## Quick start

Compile the plugin.

``
./configure --bro-dist=BRO_SOURCE_PATH --with-kernel=KERNEL_SOURCE_PATH --with-bpf=LIBBPF_PATH --with-clang=CLANG_COMPILER_PATH --with-llc=LLVM_COMPILER_PATH
make
``

After installation of the plugin it can be used by using `af_xdp` as prefix for the interface name.

```
bro -i af_xdp::eth0
```

## Copyright

Copyright (c) 2018 by Iwan Timmer  
Distributed under the BSD-3 Clause license, except for the eBPF kernel parts which are licensed under the GPL-2.0.  
For more information about the licenses, see COPYING and COPYING.GPL.  
