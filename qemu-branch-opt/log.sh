#!/bin/bash

for i in {1..10};
do
	(time -p build.qemu/i386-linux-user/qemu-i386 $@ >/dev/null) 2>&1 \
		| awk 'NR==2{printf "%s ", $2}'
	(perf stat build.qemu/i386-linux-user/qemu-i386 $@ >/dev/null) 2>&1 \
		| awk 'NR==4{printf "%s\n", $1}'
done

