#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>

int	main(int argc, char *argv[])
{
  key_t	key;
  int	shm_id;
  int	cwd;

  cwd = get_current_dir_name();
  /*  if (cwd == NULL)
    {
      printf("Unable to get the current directory\n");
      return (1);
    }
  key = ftok(cwd, 42);
  if (key == -1)
    {
      printf("Unable to get IPC key from current directory\n");
      return (1);
    }
  printf("key : %d\n", key);
  shm_id = shmget(key, getpagesize(), SHM_R | SHM_W);
  if (shm_id == -1)
    {
      printf("Unable to get Shared Memory\n");
    }

    free(cwd);*/
  return (0);
}
