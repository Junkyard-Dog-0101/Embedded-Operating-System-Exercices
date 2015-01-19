#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include "data.h"

int		main(int argc, char *argv[])
{
  key_t		key;
  int		shm_id;
  char		cwd[128];
  void		*addr;
  t_data	*received_data;

  key = atoi(argv[1]);
  if (key == -1)
    {
      printf("Unable to get IPC key\n");
      return (1);
    }
  printf("key : %d\n", key);
  shm_id = shmget(key, getpagesize(), SHM_R | SHM_W);
  printf("%d\n", shm_id);
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

  //  addr = malloc(sizeof(t_data));
  received_data = (t_data *)addr;
  received_data->status = 1;
  return (0);
}
