#!/bin/bash

imagefile=floppy.img

# make the flat image
echo "Creating blank floppy image ..."
dd if=/dev/zero of=${imagefile} bs=512 count=2880

# set up the loop back device
losetup /dev/loop0 ${imagefile}

# create the file system
echo "Creating file system ..."
mkfs -t ext2 /dev/loop0

if [ ! -d looptmp ]; then
    mkdir looptmp
fi

# mount 
echo "Mounting ... "
mount -t ext2 /dev/loop0 looptmp

echo "Copying in files ... "
# add directories
mkdir looptmp/boot
mkdir looptmp/boot/grub

# copy files
cp /usr/lib/grub/i386-pc/stage1 looptmp/boot/grub
cp /usr/lib/grub/i386-pc/stage2 looptmp/boot/grub

echo "(fd0) /dev/loop0" > looptmp/boot/grub/device.map

echo "Installing grub ... "

echo "root (fd0)" > grub-tmp
echo "setup (fd0)" >> grub-tmp
grub --device-map=looptmp/boot/grub/device.map /dev/loop0 < grub-tmp
rm -f grub-tmp



cp menu.lst looptmp/boot/grub
cp kernel.bin looptmp/boot/grub

echo "Cleaning up ..."
umount looptmp
losetup -d /dev/loop0




