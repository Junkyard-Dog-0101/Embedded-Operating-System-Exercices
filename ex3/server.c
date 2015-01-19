#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include "data.h"

int	main(int argc, char *argv[])
{
  key_t	key;
  int	shm_id;
  char	cwd[128];

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
  shm_id = shmget(key, getpagesize(), IPC_CREAT | SHM_R | SHM_W);
  if (shm_id == -1)
    {
      printf("Unable to create Shared Memory\n");
    }
  shmctl(shm_id, IPC_RMID, NULL);
  return (0);
}
