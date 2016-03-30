time test/indirect-branch/func_ptr.out 10000000
time build.qemu/i386-linux-user/qemu-i386 test/indirect-branch/func_ptr.out \
	10000000
time test/direct-br/basic.out 10000000
time build.qemu/i386-linux-user/qemu-i386 test/direct-br/basic.out \
	10000000
time test/recursion-fib/fib.out 40
time build.qemu/i386-linux-user/qemu-i386 test/recursion-fib/fib.out \
	40
