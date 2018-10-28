PROJECT =  myshell
HEADERS =  $(PROJECT).h
OBJ     =  myshell.o globals.o execute.o parser.o change_path_argument.o run_subshell.o print_redirection.o command.o pipe_redirection.o run_shellscript.o background_execution.o


C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror 


$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)