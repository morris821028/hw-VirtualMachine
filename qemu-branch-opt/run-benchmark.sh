time test/indirect-branch/func_ptr 10000000
time build.qemu/i386-linux-user/qemu-i386 test/indirect-branch/func_ptr \
	10000000
time test/direct-br/func_ptr 10000000
time build.qemu/i386-linux-user/qemu-i386 test/direct-br/func_ptr \
	10000000
time test/recursion-fib/func_ptr 40
time build.qemu/i386-linux-user/qemu-i386 test/recursion-fib/func_ptr \
	40
