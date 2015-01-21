#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/mm.h>      // included for __init and __exit macros
//#define __USE_GNU
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vesy_j");
MODULE_DESCRIPTION("Sys Info module");

static int __init sysinfo_init(void)
{
  struct sysinfo si;
  si_meminfo(&si);
  //  si_meminfo();/
  // char buf[4096];
  //int fd;
  //const char *memFile = "/proc/meminfo";


  //  fd = open(memFile, O_RDONLY);
  //memset(buf, 0, 4096);
  //read(fd, buf, 4096);
  //  printk(KERN_INFO "%s\n", buf);
  printk(KERN_INFO "total memory = %lu Mo, free memory = %lu Mo, shared memory = %lu Mo, uptime = %lu seconds\n", si.totalram, si.freeram, si.sharedram, si.uptime);
  return (0);
}

static void __exit sysinfo_cleanup(void)
{
  printk(KERN_INFO "Cleaning up module.\n");
}

module_init(sysinfo_init);
module_exit(sysinfo_cleanup);
