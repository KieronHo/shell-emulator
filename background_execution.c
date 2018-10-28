#include "myshell.h"
int child_pids[500];//Initialise pid array to keep track of running processes

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//Moves first command to background and runs the second command immediately, background runs once completed

int background_execution(SHELLCMD *first_command, SHELLCMD *second_command, int exitstatus)
{
  //Check to see if commands need to be run before the command&command line
  if(!first_command->argc)
  {
  //run all commands to the left of the command&command
  execute_shellcmd(first_command->left, exitstatus);
  }

  //Fork off to start the left command
  int first_child_pid = fork();

  //Ensure the new process was created
  if(first_child_pid == -1) //Process creation failed
  {                             
    perror("The following error occurred");
    exitstatus = EXIT_FAILURE;
    exit(exitstatus);
  }
  //The new child process runs called command
  else if(first_child_pid == 0) 
  {
    fclose(stdin); // close child's stdin
    fopen("/dev/null", "r"); // open a new stdin that is always empty

    //Set signal receivers
    signal(SIGSTOP, SIG_DFL);
    signal(SIGCONT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);

    //If there was commands to the left of command&command run the 2 in from the right command else just run the left command
    if(!first_command->argc)
    {
      execute_shellcmd(first_command->right, exitstatus);
    }
    else
    {
    execute_shellcmd(first_command, exitstatus);
  } 
  kill(getppid(), SIGCHLD);
  exit(exitstatus);
  }
  //The parent process waits for the child to exit
  else 
  {
    //Save child pid to pid array for termination later
    printf("Process number %i started\n", first_child_pid);
    for(int i = 0; i < sizeof(child_pids) / sizeof(int); i++)
    {
      if(child_pids[i]==0)
      {
        child_pids[i]=first_child_pid;
        break;
      }
    }
    kill(first_child_pid, SIGSTOP); //Immediately stop the first command
    
    //Start the second command
    int second_child_pid = fork();

    //Ensure the new process was created
    if(second_child_pid == -1) //Process creation failed
    {                             
      perror("The following error occurred");
      exitstatus = EXIT_FAILURE;
      exit(exitstatus);
    }
    //The new child process runs called command
    else if(second_child_pid == 0) 
    {
      exitstatus = execute_shellcmd(second_command, exitstatus);
      kill(first_child_pid, SIGCONT); //send continue signal to first command
      exit(exitstatus);
    }
    else
    {
      int second_child_status;
      // capture (but ignore) child's exit status
      while( wait(&second_child_status) > 0) 
      {  
        //do nothing
      }
      //Set exitstatus to the Child process' exitstatus
      if (WIFEXITED(second_child_status)) 
      {
        exitstatus = WEXITSTATUS(second_child_status);
      }
    }
  }
  //Once child termination signal received, report it
  if(signal(SIGCHLD, SIG_DFL) == 0)
  {
    printf("Process number %i completed\n", first_child_pid);
    for(int i = 0; i < sizeof(child_pids) / sizeof(int); i++)
    {
      //remove completed pid from array
      if(child_pids[i]==first_child_pid)
      {
        child_pids[i]=0;
        break;
      }
    } 
  }
  return exitstatus;
}
