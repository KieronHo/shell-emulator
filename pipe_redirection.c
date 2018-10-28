#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//Redirects Standard out of command 1 into pipe and uses this as standard in of command 2

int pipe_redirection(SHELLCMD *t, int exitstatus)
{
  //Save a copy of standard in and out for restoration later
  int stdin_save = dup(0);
  int stdout_save = dup(1);

  //Initialise pipe descriptiors
  int pipe_connectors[2];

  if(pipe(pipe_connectors) < 0) //If pipe fails
  {
    fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
    exitstatus = EXIT_FAILURE;
    exit(exitstatus);
  }

  if(dup2(pipe_connectors[1], 1) < 0) //redirect Standard out into pipe
  {
    fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
    exitstatus = EXIT_FAILURE; // If redirect fails change exitstatus
    exit(exitstatus); 
  }

  exitstatus = execute_shellcmd(t->left, exitstatus);
  close(pipe_connectors[1]); //close the write end of the pipe
  dup2(stdout_save, 1); //restore standard out

  if(!exitstatus) //If the first command is succesful move to second command
  {
    if(dup2(pipe_connectors[0], 0) < 0) //redirect pipe to standard in
    {
      fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
      exitstatus = EXIT_FAILURE; // If redirect fails change exitstatus
      exit(exitstatus);
    }
    exitstatus = execute_shellcmd(t->right, exitstatus);
    close(pipe_connectors[0]); //close read end of pipe
    dup2(stdin_save, 0); //restore standard in
  }
  return exitstatus;
}
