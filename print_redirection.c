#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//Redirects the input or output to/from a supplied filename

int print_redirection(SHELLCMD *t, int exitstatus)
{
  if(t->infile != NULL)
  {
    int file_descriptor = open(t->infile, O_RDONLY);
    if(file_descriptor < 0)
    {
      fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
      exitstatus = EXIT_FAILURE; //If file open fails change exit status
      exit(EXIT_FAILURE); 
    }
    if(dup2(file_descriptor, 0) < 0)
    {
      fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
      exitstatus = EXIT_FAILURE; // If redirect fails change exitstatus
      exit(EXIT_FAILURE); 
    }
    close(file_descriptor);
  }
  if(t->outfile != NULL) 
  {
    if(t->append == false) //Truncate supplied file
    {
      int file_descriptor = open(t->outfile, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
      if(file_descriptor < 0)
      {
        fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
        exitstatus = EXIT_FAILURE; //If file open fails change exit status
        exit(EXIT_FAILURE); 
      }
      if(dup2(file_descriptor, 1) < 0)
      {
        fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
        exitstatus = EXIT_FAILURE; // If redirect fails change exitstatus
        exit(EXIT_FAILURE); 
      }
      close(file_descriptor);
    }
    else
    {
      int file_descriptor = open(t->outfile, O_CREAT|O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR);
      if(file_descriptor < 0)
      {
        fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
        exitstatus = EXIT_FAILURE; //If file open fails change exit status
        exit(EXIT_FAILURE); 
      }
      if(dup2(file_descriptor, 1) < 0)
      {
        fprintf(stderr, "\n%s: %s\n", argv0, strerror(errno));
        exitstatus = EXIT_FAILURE; // If redirect fails change exitstatus
        exit(EXIT_FAILURE); 
      }
      close(file_descriptor);
    }
  }
  exitstatus = EXIT_SUCCESS;
  return exitstatus;
}
