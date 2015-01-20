#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "data.h"

int		main()
{
  key_t		key;
  int		shm_id;
  char		cwd[128];
  void		*addr;
  t_data	*received_data;

  if (getcwd(cwd, 64) == NULL)
    {
      printf("Unable to get the current directory\n");
      return (1);
    }
  key = ftok(cwd, 42);
  if (key == -1)
    {
      printf("Unable to get IPC key\n");
      return (1);
    }
  printf("key : %d\n", key);
  shm_id = shmget(key, sizeof(t_data), IPC_CREAT | SHM_R | SHM_W);
  if (shm_id == -1)
    {
      printf("Unable to create shared memory\n");
      return (1);
    }
  addr = shmat(shm_id, NULL, SHM_R | SHM_W);

  /*
  *if ((int)addr == -1)
  *  {
  *    printf("Unable to attach shared memory\n");
  *    return (1);
  *  }
  */

  received_data = (t_data *)addr;
  received_data->status = 0;
  while (1)
    {
      if (received_data->status == 1)
	{
	  switch (received_data->operator)
	    {
	    case ADD:
	      received_data->result = received_data->nbr1 + received_data->nbr2;
	      break;
	    case SUB:
	      received_data->result = received_data->nbr1 - received_data->nbr2;
	      break;
	    case MUL:
	      received_data->result = received_data->nbr1 * received_data->nbr2;
	      break;
	    case DIV:
	      received_data->result = received_data->nbr1 / received_data->nbr2;
	      break;
	    }
	  printf("operation caculated\n");
	  received_data->status = 0;
	}
      usleep(500);
    }
  return (0);
}
