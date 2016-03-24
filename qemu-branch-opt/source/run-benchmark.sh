make -C build.qemu
time test/indirect-branch/func_ptr 10000000
time build.qemu/i386-linux-user/qemu-i386 test/indirect-branch/func_ptr \
	10000000
