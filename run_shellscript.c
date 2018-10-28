#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//Runs a file as a shellscript if it is able to be found but not executed

void run_shellscript(SHELLCMD *t, int exitstatus)
{
  if (!access(*t->argv, F_OK)) 
  {
    FILE *shellscript = fopen(*t->argv, "r");

    if (shellscript == NULL)
    {
      fprintf(stderr, "Could not open file: %s\n", *t->argv);
      exitstatus = EXIT_FAILURE;
      exit(exitstatus);         // Exit indicating failure
    }
    while(!feof(shellscript)) 
    {
      SHELLCMD *t = parse_shellcmd(shellscript);
      exitstatus = execute_shellcmd(t, exitstatus);
    }
    if(interactive) 
    {
      fputc('\n', stdout);
    }
  }
  else
  {
    fprintf(stderr, "\n%s: %s: %s\n", argv0, *t->argv, strerror(errno));
    exitstatus = EXIT_FAILURE;
    exit(exitstatus);
  }
  exitstatus = EXIT_SUCCESS;
  exit(exitstatus);
}
