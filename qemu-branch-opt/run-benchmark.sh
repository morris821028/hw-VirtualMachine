#!/bin/bash

test=("indirect-branch/func_ptr.out 10000000" "direct-br/basic.out 10000000" "recursion-fib/fib.out 40")
for i in "${test[@]}";
do
	echo -e "\033[35m Test ${i} Origin\033[0m";
	(time test/${i}) 2>&1
	echo -e "\033[35m Test ${i} Qemu\033[0m";
	(time build.qemu/i386-linux-user/qemu-i386 test/${i}) 2>&1
	(perf stat build.qemu/i386-linux-user/qemu-i386 test/${i}) 2>&1
done

