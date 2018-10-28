#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//Runs any simple commands once tokens have been removed

int command(SHELLCMD *t, int exitstatus)
{
  bool time_called = 0;
  struct timeval start_time, end_time;
  int elapsedTime;

  if(t != NULL) 
  {
    if(strcmp(*t->argv, "time") == 0)
    {
      gettimeofday(&start_time, NULL); //start timer
      t->argc--;             // skip time argument
      t->argv++;
      time_called = 1;
    }
    if(strcmp(*t->argv, "exit") == 0)
    {
      for(int i = 0; i < 500; i++)
      {
        if(child_pids[i]!=0)
        {
          printf("%i\n", child_pids[i]);
          kill(child_pids[i], SIGTERM); //Terminate any running pids
        }
      }
      if(t->argc == 2)
      {
        //If a second argument is provided, change exit status to this
        exitstatus = atoi(t->argv[1]);
      }
      printf("Exit status was %d\n", exitstatus); //This line can be removed when we have finished project, it just makes sure exit status is reported
      //Exit with the current value of exitstatus
      exit(exitstatus);
      }
      else if(strcmp(*t->argv, "cd") == 0)
      {
        if(t->argc == 1)
        {
          exitstatus = chdir(DEFAULT_HOME);
        }
        else
        {
          //If no / provided, check the DEFAULT_CDPATH for the given directory
          if(t->argv[1][0] != '/')
          {
            change_path_argument(t, DEFAULT_CDPATH, 1);
          }
          //Change to given directory, if not found exitstatus will be non 0
          if(chdir(t->argv[1]) != 0)
          {
            //If directory cannot be found print appropriate error
            fprintf(stderr, "\n%s: %s: %s: %s\n", argv0, t->argv[0], t->argv[1], strerror(errno));
            exitstatus = EXIT_FAILURE;
          }
          else
          {
            exitstatus = EXIT_SUCCESS;
          }
        }
      }
      else
      {
        //If the first character is not a / search through DEFAULT_PATH for correct command path
        if(t->argv[0][0] != '/')
        {
          change_path_argument(t, DEFAULT_PATH, 0);
        }
        if(t == NULL) 
        { 
          exitstatus  = EXIT_FAILURE;
          exit(exitstatus);
        }
        else 
        {
          //Create a new process, parent keeps running execute, child will become the called command
          int pid = fork();

          //Ensure the new process was created
          if(pid == -1) //Process creation failed
          {                             
            perror("The following error occurred");
            exitstatus = EXIT_FAILURE;
            exit(exitstatus);
          }
          //The new child process runs called command
          else if(pid == 0) 
          {
            if(t->infile != NULL || t->outfile != NULL)
            {
              print_redirection(t, exitstatus);
            }
            execv(*t->argv, t->argv);
            run_shellscript(t, exitstatus);
            exit(exitstatus);
          }
          //The parent process waits for the child to exit
          else 
          {
            int status;
            // capture (but ignore) child's exit status
            while( wait(&status) > 0) 
            {     
              /* nothing to do */ ;
            }
            //Set exitstatus to the Child process' exitstatus
            if (WIFEXITED(status)) 
            {
              exitstatus = WEXITSTATUS(status);
            }
          }
        }    
      }
      if(time_called)
      {
        // stop timer
        gettimeofday(&end_time, NULL);
        elapsedTime = (end_time.tv_sec - start_time.tv_sec) * 1000.0;       // sec to ms
        elapsedTime += (end_time.tv_usec - start_time.tv_usec) / 1000.0;    // us to ms

        fprintf (stderr, "%imsec\n", elapsedTime);
        t->argc++;  // remove the skip time argument so memory can be freed
        t->argv--;
    }
    free_shellcmd(t);
  }
  return exitstatus;
}
