#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#define BUFFERSIZE 512
#define TOKEN_BUFFERSIZE 512
#define TOKEN_DELIM ";&"
#define TOKEN_DELIMITER " \t\r\a\n"
#define REDIRECTION_DELIMITER " <>"


void loop(void);

char *read_command(void);

char **commands_to_execute(char *comm);

void execute(char **arguments, int w);

char **split_command(char *sub_com);

void batchfile(char *fp);

void quit(void){

    printf("hello from quit\n");

    exit(EXIT_FAILURE);

}


int main(int argc, char **argv){

    if(argv[1] != NULL){

         batchfile(argv[1]);

    }

    loop();

    return 0;

}

void loop(void){

    int array[512];

    char *command;

    char **sub_commands;

    char **arguments;

    char str[] = "quit";

    int i, j, k, u, v;

    while(1){

        for(i = 0; i < 512; i++){

             array[i] = 0;

        }

        i = 0;

        k = 0;

        j = 0;

        u = 0;

        v = 0;

        printf("kagiafas_8817> ");

        command = read_command();

        if(strncmp(command, str, strlen(str)) == 0){

            free(command);

            quit();

        }

        while(command[u] != '\0'){

            if(command[u] == 59){   

                 array[v] = 1;

                 v++;

            }

            if((command[u] == 38) && (command[u + 1] == 38)){

                 array[v] = 2;

                 v++;

            }

            u++;

        }

        sub_commands = commands_to_execute(command);

        while(j != 1){

            if(sub_commands[k] == NULL){

                 j++;

                 k--;

            }

            k++;

        }

        for(i = 0; i < k; i++){

              arguments = split_command(sub_commands[i]);

              execute(arguments, array[i]);

        }

    }

    free(command);

    free(sub_commands);

    free(arguments);

}


char *read_command(){

    char *buffer = malloc(BUFFERSIZE * sizeof(char));

    if(!buffer){

        fprintf(stderr, "error in memory allocation\n");

        quit();

    }

    int num = 512;

    fgets(buffer, num, stdin);

    return buffer;

}

char **commands_to_execute(char *comm){

    int buffersize = TOKEN_BUFFERSIZE;

    int parse_position = 0;

    char **tokens = malloc(buffersize * sizeof(char *));

    char *token;

    if(!tokens){

        fprintf(stderr, "error in memory allocation\n");

        quit();

    }

    token = strtok(comm, TOKEN_DELIM);

    while(token != NULL){

         tokens[parse_position] = token;

         parse_position++;

         token = strtok(NULL, TOKEN_DELIM);

    }

    tokens[parse_position] = NULL;

    return tokens;

}


char **split_command(char *sub_com){

    int buffer_size = TOKEN_BUFFERSIZE;

    int parsing_position = 0;

    char **tokens = malloc(buffer_size * sizeof(char *));

    char *token;

    if(!tokens){

         fprintf(stderr, "error in memory allocation\n");

         exit(EXIT_FAILURE);

    }

    token = strtok(sub_com, TOKEN_DELIMITER);

    while(token != NULL){

         tokens[parsing_position] = token;

         parsing_position++;

         token = strtok(NULL, TOKEN_DELIMITER);

    }

    tokens[parsing_position] = NULL;

    return tokens;

}


void execute(char **arguments, int w){

    pid_t pid;

    int status;

    int l = 0;

    FILE *fd0;

    FILE *fd1;

    int ret;

    char direction1[] = ">";

    char direction2[] = "<";

    int m = 0;

    int n = 0;

    while(arguments[l] != NULL){

              if(strncmp(arguments[l], direction1, strlen(direction1)) == 0){

                     fd1 = fopen(arguments[l + 1],"a");

                     if(fd1 < 0){

                            perror("open");

                            exit(EXIT_FAILURE);

                     }

                     m = 1;

               }

               if(strncmp(arguments[l], direction2, strlen(direction2)) == 0){

                      fd0 = fopen(arguments[l + 1], "r");

                      if(fd0 < 0){

                            perror("open");

                            exit(EXIT_FAILURE);

                     }

                     n = 1;

               }

               l++;

        }

    l = 0;

    pid = fork();

    if(pid == 0){

        while(arguments[l] != NULL){

              if(m == 1){

                     ret = dup2(fileno(fd1), 1);

                     if(ret < 0){

                            perror("dup2");

                            exit(EXIT_FAILURE);

                     }

                     m = 1;

               }

               if(n == 1){

                     ret = dup2(fileno(fd0), 0);

                     if(ret < 0){

                            perror("dup2");

                            exit(EXIT_FAILURE);

                     }

                     n = 1;

               }

               l++;

        }

        arguments[l - 2*n - 2*m] = NULL;

        if(execvp(arguments[0], arguments) == -1){

              printf("error in the input command\n");

              if(w == 2) {

                   kill(getppid(), 9);

                   quit();
              }

        }else{

              if((m == 1) & (n == 1)){

                    fclose(fd0);

                    fclose(fd1);

              }else if((m == 0) & (n == 1)){

                    fclose(fd0);

              }else if((m == 1) & (n == 0)){

                    fclose(fd1);

              }


         }

         exit(0);

    }else if(pid < 0){

        perror("lsh");

    }else{

        do{

            waitpid(pid, &status, WUNTRACED);

        }while(!WIFEXITED(status) && !WIFSIGNALED(status));

    }

}


void batchfile(char *fp){

     FILE* file = fopen(fp, "r");

     if(file < 0){

          perror("open");

          exit(EXIT_FAILURE);

     }

     char line[512];

     char **sub_commands;

     char **arguments;

     while(fgets(line, sizeof(line), file)){

          int array[512];

          char str[] = "quit";

          int i, j, k, u, v;

          for(i = 0; i < 512; i++){

               array[i] = 0;

          }

          i = 0;

          j = 0;

          k = 0;

          u = 0;

          v = 0;

          if(strncmp(line, str, strlen(str)) == 0){

            quit();

         }

         while(line[u] != '\0'){

            if(line[u] == 59){   

                 array[v] = 1;

                 v++;

            }

            if((line[u] == 38) && (line[u + 1] == 38)){

                 array[v] = 2;

                 v++;

            }

            u++;

        }

        sub_commands = commands_to_execute(line);

        while(j != 1){

            if(sub_commands[k] == NULL){

                 j++;

                 k--;

            }

            k++;

        }

        for(i = 0; i < k; i++){

              arguments = split_command(sub_commands[i]);

              execute(arguments, array[i]);

        }
          

     }

     free(sub_commands);

     free(arguments);

     quit();

}



