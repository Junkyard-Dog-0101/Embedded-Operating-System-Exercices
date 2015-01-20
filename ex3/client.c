#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "data.h"

int		main(int argc, char *argv[])
{
  key_t		key;
  int		shm_id;
  char		cwd[128];
  void		*addr;
  t_data	*received_data;
  char		buf[64];
  char		str[64];
  int		i;
  int		j;

  if (argc != 2)
    {
      printf("Usage : %s key\n", argv[0]);
      return (1);
    }
  key = atoi(argv[1]);
  if (key == -1)
    {
      printf("Unable to get IPC key\n");
      return (1);
    }
  shm_id = shmget(key, sizeof(t_data), SHM_R | SHM_W);
  if (shm_id == -1)
    {
      printf("Unable to get shared memory\n");
      return (1);
    }
  addr = shmat(shm_id, NULL, SHM_R | SHM_W);
  received_data = (t_data *)addr;
  while (1)
    {
      memset(buf, 0, 64);
      read(1, buf, 64);
      i = 0;
      while (buf[i] >= '0' && buf[i] <= '9')
	{
	  str[i] = buf[i];
	  i = i + 1;
	}
      str[i] = 0;
      received_data->nbr1 = atoi(str);
      switch (buf[i])
	{
	case '+':
	  received_data->operator = ADD;
	  break;
	case '-':
	  received_data->operator = SUB;
	  break;
	case '*':
	  received_data->operator = MUL;
	  break;
	case '/':
	  received_data->operator = DIV;
	  break;
	}
      i = i + 1;
      j = 0;
      while (buf[i] >= '0' && buf[i] <= '9')
	{
	  str[j] = buf[i];
	  i = i + 1;
	  j = j + 1;
	}
      str[j] = 0;
      received_data->nbr2 = atoi(str);
      received_data->status = 1;
      while (1)
	{
	  if (received_data->status == 0)
	    {
	      printf("%d\n", received_data->result);
	      break;
	    }
	}
    }
  return (0);
}
