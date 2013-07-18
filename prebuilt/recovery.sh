#!/system/bin/sh

echo "boot-recovery" | /system/bin/dd of=/dev/block/mmcblk0p5 bs=1 count=13
