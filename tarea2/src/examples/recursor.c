#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char *argv[])
{
  char buffer[128];
  pid_t pid;
  int retval = 0;
  int i=0;
  printf("recursor\n");
  for(i=0; argv[i]!= NULL; i++)
  {
    printf("Arg %d in recursor: %s\n", i, argv[i]);
  }

  printf ("%s %s %s %s line19\n", argv[0], argv[1], argv[2], argv[3]);

  if (argc != 4) 
    {
      printf ("usage: recursor <string> <depth> <waitp>\n");
      exit (1);
    }

  /* Print args. */
  printf ("%s %s %s %s line19\n", argv[0], argv[1], argv[2], argv[3]);

  /* Execute child and wait for it to finish if requested. */
  if (atoi (argv[2]) != 0) 
    {
      snprintf (buffer, sizeof buffer,
                "recursor %s %d %s", argv[1], atoi (argv[2]) - 1, argv[3]);
      printf("**************buffer: %s***************\n", buffer);
      pid = exec (buffer);
      printf("if(%d)\n",atoi (argv[3]));
      if (atoi (argv[3])){
    	  printf("Wait\n");
    	  retval = wait (pid);
      }
    }
  
  /* Done. */
  printf ("%s %s: dying, retval=%d\n", argv[1], argv[2], retval);
  exit (retval);
}
