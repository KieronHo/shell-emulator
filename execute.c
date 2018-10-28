#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

// -------------------------------------------------------------------

//  THIS FUNCTION SHOULD TRAVERSE THE COMMAND-TREE and EXECUTE THE COMMANDS
//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_shellcmd0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE

int execute_shellcmd(SHELLCMD *t, int exitstatus)
{
  if(t == NULL) 
  {
    printf("<nullcmd> ");
  }

  switch (t->type) 
  {
    case CMD_COMMAND :
    {
      exitstatus = command(t, exitstatus);
      break;
    }

    case CMD_SEMICOLON :
    {
      exitstatus = execute_shellcmd(t->left, exitstatus);
      exitstatus = execute_shellcmd(t->right, exitstatus);
      break;
    }

    case CMD_AND :
    {
      exitstatus = execute_shellcmd(t->left, exitstatus);
      if(!exitstatus)
      {
        exitstatus = execute_shellcmd(t->right, exitstatus);
      }
      break;
    }

    case CMD_OR :
    {
      exitstatus = execute_shellcmd(t->left, exitstatus);
        if(exitstatus)
        {
          exitstatus = execute_shellcmd(t->right, exitstatus);
        }
      break;
    }

    case CMD_SUBSHELL :
    {
      exitstatus = run_subshell(t, exitstatus);
      break;
    }

    case CMD_PIPE :
    {
      exitstatus = pipe_redirection(t, exitstatus);
      break;
    }

    case CMD_BACKGROUND :
    {
      exitstatus = background_execution(t->left, t->right, exitstatus);
      break;
    }

    default :
      fprintf(stderr, "%s: invalid CMDTYPE in print_shellcmd0()\n", argv0);
      exit(EXIT_FAILURE);
      break;
  }

  return exitstatus;
}
