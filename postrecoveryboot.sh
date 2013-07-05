#!/sbin/sh

sleep 5
## Clear "boot-recovery...enable-wipe" flag
dd if=/dev/zero of=/dev/block/mmcblk0p5 bs=1 count=13

