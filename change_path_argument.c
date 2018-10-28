#include "myshell.h"

/*
    CITS2002 Project 2 2017
    Name(s):             David Weight, Kieron Ho
    Student number(s):   21930719, 20500057
    Date:                
 */

//If there is no / character at the start of a command this program searches a path string to find the file or directory

void change_path_argument(SHELLCMD *t, char *path_test_string, int argument_to_test)  
{  
  char command_argument[100]; //Initialise array for command arguments to be tested
  char *path_to_test_list = (char *)malloc (strlen(path_test_string)); //Give the path_to_test variable the appropriate memory
  strcpy (path_to_test_list, path_test_string); // Give the path_to_test the string value of DEFAULT_PATH
  int continuecheck = 1; // Will specify whether to continue through the path_to_test_list for new TESTPATHS
  char *TESTPATH = (char *)malloc (strlen(path_test_string)); // This variable has a scope outside of the loop to be used each iteration

  while(continuecheck)
  {
    char  *pos = strchr(path_to_test_list, ':'); //Find location of : character

    if(!pos) // If there is no more : characters do this
    {
      continuecheck=0;
      char *test_path = path_to_test_list;
      sprintf(command_argument, "%s/%s", test_path, t->argv[argument_to_test]); // send final value to command_argument
    }
    else
    {
      *pos = '\0'; //terminate the string for the front test path
      char *test_path = path_to_test_list;
      sprintf(TESTPATH, "%s", test_path); // send a value to TESTPATH
      int remaining_position = pos - path_to_test_list; // find distance between start of list and : character
      char *remaining_paths_to_test = path_to_test_list + remaining_position + 1; //move the start of the remaining paths past the : character
      memcpy(path_to_test_list, remaining_paths_to_test, strlen(remaining_paths_to_test)); //copy the remaining paths to the new path_to_test_list
      char *end_cleaner = path_to_test_list + strlen(remaining_paths_to_test); //terminate the string at the new length
      *end_cleaner = '\0';
      sprintf(command_argument, "%s/%s", TESTPATH, t->argv[argument_to_test]);
    }
    // Check if command exists in path
    if (!access(command_argument, F_OK)) 
    { 
    //Change the contents pointed to by argv[argument_to_test] to the full path
    memcpy(t->argv[argument_to_test], command_argument, strlen(command_argument) + 1);
    continuecheck=0;
    } 
  }
}
