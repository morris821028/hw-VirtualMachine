#!/bin/bash

time build.qemu/i386-linux-user/qemu-i386 coremark_v1.0/coremark.exe  \
	0x0 0x0 0x66 0 7 1 2000
time build.qemu/i386-linux-user/qemu-i386 coremark_v1.0/coremark.exe  \
	0x3415 0x3415 0x66 0 7 1 2000
