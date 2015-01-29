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
#include <linux/cpumask.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <asm/cputime.h>
#include <linux/tick.h>

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

int	get_usage_from_cpustat(struct seq_file *p, void *v)
{
  int	i;
  u64	user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  user = nice = system = idle = iowait = irq = softirq = steal = guest = guest_nice = 0;
  for_each_possible_cpu(i)
  {
    user += kcpustat_cpu(i).cpustat[CPUTIME_USER];
    nice += kcpustat_cpu(i).cpustat[CPUTIME_NICE];
    system += kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM];
    irq += kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
    softirq += kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ];
    steal += kcpustat_cpu(i).cpustat[CPUTIME_STEAL];
    guest += kcpustat_cpu(i).cpustat[CPUTIME_GUEST];
    guest_nice += kcpustat_cpu(i).cpustat[CPUTIME_GUEST_NICE];
  }
  seq_puts(p, "cpu\t\t:");
  seq_puts(p, " user");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(user));
  seq_puts(p, " nice");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(nice));
  seq_puts(p, " sys");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(system));
  seq_puts(p, " irq");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(irq));
  seq_puts(p, " sirq");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(softirq));
  seq_puts(p, " steal");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(steal));
  seq_puts(p, " guest");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(guest));
  seq_puts(p, " gnice");
  seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(guest_nice));
  seq_putc(p, '\n');
  for_each_online_cpu(i)
  {
    user = kcpustat_cpu(i).cpustat[CPUTIME_USER];
    nice = kcpustat_cpu(i).cpustat[CPUTIME_NICE];
    system = kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM];
    irq = kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
    softirq = kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ];
    steal = kcpustat_cpu(i).cpustat[CPUTIME_STEAL];
    guest = kcpustat_cpu(i).cpustat[CPUTIME_GUEST];
    guest_nice = kcpustat_cpu(i).cpustat[CPUTIME_GUEST_NICE];
    seq_printf(p, "cpu%d\t\t:", i);
    seq_puts(p, " user");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(user));
    seq_puts(p, " nice");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(nice));
    seq_puts(p, " sys");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(system));
    seq_puts(p, " irq");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(irq));
    seq_puts(p, " sirq");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(softirq));
    seq_puts(p, " steal");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(steal));
    seq_puts(p, " guest");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(guest));
    seq_puts(p, " gnice");
    seq_put_decimal_ull(p, ' ', cputime64_to_clock_t(guest_nice));
    seq_putc(p, '\n');
  }
  return (0);
}

void	get_next_number_pos(char *buf, int *i)
{
  while (buf[*i] != ' ')
    {
      ++*i;
    }
  while (buf[*i] == ' ')
    {
      ++*i;
    }
}

int	get_network_from_netdev(char *buf, char tab[8][3][32])
{
  int	count;
  int	i;
  int	j;
  int	n;
  int	e;
  int	c;

  i = 0;
  count = 0;
  while (buf[i] != '\n' && buf[i] != '\0' && i <= 1024)
    {
      ++i;
    }
  ++i;
  while (buf[i] != '\n' && buf[i] != '\0' && i <= 1024)
    {
      ++i;
    }
  j = 0;
  while (buf[i] != '\0' && i <= 1024)
    {
      n = 0;
      while (buf[i] != '\n' && buf[i] != '\0' && i <= 1024)
	{
	  if (n == 0)
	    {
	      ++count;
	      while (buf[i] == ' ' && buf[i] != '\0' && i <= 1024)
		{
		  ++i;
		}
	      e = 0;
	      memset(tab[j][0], 0, 32);
	      while (buf[i] != ':' && buf[i] != '\0' && i <= 1024)
		{
		  tab[j][0][e] = buf[i];
		  ++e;
		  ++i;
		}
	      ++i;
	      while (buf[i] == ' ' && buf[i] != '\0' && i <= 1024)
		{
		  ++i;
		}
	      e = 0;
	      memset(tab[j][1], 0, 32);
	      while (buf[i] != ' ' && buf[i] != '\0' && i <= 1024)
		{
		  tab[j][1][e] = buf[i];
		  ++e;
		  ++i;
		}
	      while (buf[i] == ' ' && buf[i] != '\0' && i <= 1024)
		{
		  ++i;
		}
	      c = 2;
	      while (c != 9 && i <= 1024)
		{
		  get_next_number_pos(buf, &i);
		  ++c;
		}
	      e = 0;
	      memset(tab[j][2], 0, 32);
              while (buf[i] != ' ' && buf[i] != '\0' && i <= 1024)
                {
                  tab[j][2][e] = buf[i];
                  ++e;
                  ++i;
                }
	      --i;
	      n = 1;
	      ++j;
	    }
	  ++i;
	  
	}
      ++i;
    }
  return (count);
}

static int	show_stat(struct seq_file *p, void *v)
{
  const char		*cpuinfo = "/proc/cpuinfo";
  const char		*networkinfo = "/proc/net/dev";
  struct sysinfo	si;
  struct file		*ffd;
  char			*buf;
  char			str[64];
  char			tab[8][3][32];
  int			count;
  int			i;

  buf = kmalloc(1024, GFP_KERNEL);
  memset(buf, 0, 1024);
  ffd = file_open(cpuinfo, O_RDONLY, 0);
  file_read(ffd, ffd->f_pos, buf, 1023);
  seq_printf(p, "PROCESSOR\n");
  get_info_from_cpuinfo(buf, str, "model name"); 
  seq_printf(p, "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cpu cores"); 
  seq_printf(p, "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cpu MHz"); 
  seq_printf(p, "%s\n", str);
  get_info_from_cpuinfo(buf, str, "cache size");
  seq_printf(p, "%s\n", str);
  file_close(ffd);


  get_usage_from_cpustat(p, v);

  memset(buf, 0, 1024);
  ffd = file_open(networkinfo, O_RDONLY, 0);
  file_read(ffd, ffd->f_pos, buf, 1023);
  count = get_network_from_netdev(buf, tab);
  i = 0;

  seq_printf(p, "\nNETWORK\n");
  while (i != count)
    {
      seq_printf(p, "name\t\t: %s\nReceive\t\t: %s byte(s)\nTrasnmit\t: %s byte(s)\n", tab[i][0], tab[i][1], tab[i][2]);
      ++i;
    }
  file_close(ffd);

  seq_printf(p, "\nMEMORY\n");
  si_meminfo(&si);
  seq_printf(p, "total memory\t: %lu MB\n", si.totalram);
  seq_printf(p, "free memory\t: %lu MB\n", si.freeram);
  seq_printf(p, "shared memory\t: %lu MB\n", si.sharedram);
  seq_printf(p, "uptime\t\t: %lu seconds\n", si.uptime);
  kfree(buf);
  return (0);
}

static int	stat_open(struct inode *inode, struct file *file)
{
  int		res;

  res = single_open(file, show_stat, NULL);
  return (res);
}

static const struct file_operations	proc_stat_operations = {
  .open = stat_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int	__init sysinfo_init(void)
{
  proc_create("mysysinfo", 0, NULL, &proc_stat_operations);
  printk(KERN_INFO "sysinfo module started\n");
  return (0);
}

static void	__exit sysinfo_cleanup(void)
{
  remove_proc_entry("mystat",NULL);
  printk(KERN_INFO "sysinfo module ended\n");
}

module_init(sysinfo_init);
module_exit(sysinfo_cleanup);
