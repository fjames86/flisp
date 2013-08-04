#!/bin/bash

losetup /dev/loop0 floppy.img
mount -t ext2 /dev/loop0 looptmp
cp kernel.bin looptmp/boot/grub/
umount looptmp
losetup -d /dev/loop0

