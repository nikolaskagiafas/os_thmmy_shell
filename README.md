# Abstract
Creating my own command line interpreter (shell) working in both interactive and batch mode, Exercise in the course of Operating Systems

# Description
My shell has an interactive and a batch mode.


Many commands in one line can be executed

They are divided by ";" and "&&"

Redirection has also been implemented

There are no built-in commands

Pipe commands cannot be handled

Functions:
   -loop->the continuous recursion by the shell in interactive mode

   -read_command->take the command that has to be executed

   -commands_to_execute->division of a command in multiple commands

   -execute->single subcommand execution and quit if "&&" and error input command

   -split_command->splitting a single subcommand in its own arguments

   -batchfile->batchfile mode

   -quit->end of the program and exit of the shell

   -main->main function that decides in which mode the control of the program goes
