make
sudo insmod sysinfo.ko
dmesg | tail -1
sudo rmmod sysinfo.ko
make clean
