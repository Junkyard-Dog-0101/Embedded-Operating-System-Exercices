#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vesy_j");
MODULE_DESCRIPTION("Sys Info module");

int		file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size)
{
  mm_segment_t	oldfs;
  int		ret;

  oldfs = get_fs();
  set_fs(get_ds());
  ret = vfs_read(file, data, size, &offset);
  set_fs(oldfs);
  return (ret);
}

void	file_close(struct file* file)
{
  filp_close(file, NULL);
}

struct file	*file_open(const char *path, int flags, int rights)
{
  struct file	*filp;
  mm_segment_t	oldfs;
  int		err;

  filp = NULL;
  err = 0;
  oldfs = get_fs();
  set_fs(get_ds());
  filp = filp_open(path, flags, rights);
  set_fs(oldfs);
  if (IS_ERR(filp))
    {
      err = PTR_ERR(filp);
      return (NULL);
    }
  return (filp);
}

int	get_info_from_cpuinfo(char *src, char *buf, char *key)
{
  int	i;
  char	*ptr;

  memset(buf, 0, 64);
  ptr = strstr(src, key);
  if (ptr == NULL)
    return (1);
  i = 0;
  while (*ptr != '\n' && *ptr != '\0')
    {
      buf[i] = *ptr;
      ++i;
      ++ptr;
    }
  return (0);
}

/*int	get_usage_from_cpustat(char *src, char *buf, char *key)
{
}*/

static int		__init sysinfo_init(void)
{
  const char		*cpuinfo = "/proc/cpuinfo";
  //const char		*cpuusage = "/proc/stat";
  //const char		*networkinfo = "/proc/net/dev";
  struct sysinfo	si;
  struct file		*ffd;
  char			*buf;
  char			str[64];

  buf = kmalloc(1024, GFP_KERNEL);
  memset(buf, 0, 1024);
  ffd = file_open(cpuinfo, O_RDONLY, 0);
  file_read(ffd, ffd->f_pos, buf, 1023);
  get_info_from_cpuinfo(buf, str, "model name"); 
  printk(KERN_INFO "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cpu cores"); 
  printk(KERN_INFO "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cpu MHz"); 
  printk(KERN_INFO "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cache size"); 
  printk(KERN_INFO "%s\n", str);
  si_meminfo(&si);
  printk(KERN_INFO "total memory\t: %lu Mo\n", si.totalram);
  printk(KERN_INFO "free memory\t: %lu Mo\n", si.freeram);
  printk(KERN_INFO "shared memory\t: %lu Mo\n", si.sharedram);
  printk(KERN_INFO "uptime\t\t: %lu seconds\n", si.uptime);
  file_close(ffd);
  kfree(buf);
  return (0);
}

static void	__exit sysinfo_cleanup(void)
{
  printk(KERN_INFO "Cleaning up module.\n");
}

module_init(sysinfo_init);
module_exit(sysinfo_cleanup);
