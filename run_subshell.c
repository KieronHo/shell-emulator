#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//If the ( command ) is used this program runs a subshell with the commands inside parenthesis

int run_subshell(SHELLCMD *t, int exitstatus) 
{
	//Create a new process, parent keeps running execute, child will become the called command
    int pid = fork();

    //Ensure the new process was created
    if(pid == -1) //Process creation failed
    {                             
    	perror("The following error occurred");
        exit(EXIT_FAILURE);
    }
    //The new child process runs called command
    else if(pid == 0) 
    {
    	if(t->infile != NULL || t->outfile != NULL)
        {
          print_redirection(t, exitstatus);
        }
        exitstatus = execute_shellcmd(t->left, exitstatus);
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
    return exitstatus;
}
