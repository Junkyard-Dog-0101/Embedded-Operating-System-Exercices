#!/bin/bash
make
sudo rmmod sysinfo.ko
sudo insmod sysinfo.ko
cat /proc/mysysinfo
