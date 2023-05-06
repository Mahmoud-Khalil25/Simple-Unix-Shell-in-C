#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#define BUFFERSIZE 80

void handle(int signum){
    FILE *fp;
    fp  = fopen ("data.txt", "a");
    fprintf(fp, "Child Process was terminated\n");
    fclose (fp);
}


int main()
{
    while(1)
    {
        fflush(stdout);
        char buffer[BUFFERSIZE];
        char buffer_copy[BUFFERSIZE]; // running strtok modifies the original string thus we need two copies

        printf("my shell-> ");
        fgets(buffer,BUFFERSIZE,stdin);
        buffer[strlen(buffer)-1]='\0'; //remove \n added by fgets
        strcpy(buffer_copy,buffer);

        //////////////////////////Tokenisation of string
        char* token=strtok(buffer," ");
        int token_num=0;
        //Tokenise first time to get how many strings are delimited by space

        while(token != NULL)
        {
            //assign elemnts to the array to be passed into excevp
            token=strtok(NULL," ");
            token_num++;
        }

        char** command=malloc((token_num+1) * sizeof(char *)); // this would be passed to excecp
        command[token_num]=NULL;

        char* token1=strtok(buffer_copy," ");
        int index=0;
        int size=0;
        while(token1 != NULL)
        {
            size=strlen(token1);
            command[index]=(char *)malloc(size+1);
            strcpy(command[index],token1);
            token1=strtok(NULL," ");
            index++;

        }

        //#############################################END OF Processing of the input#########################################################/

        int bgndFlag=0;
        if(!strcmp(command[0],"exit"))
            exit(0);

        if(!strcmp(command[token_num-1],"&"))
            bgndFlag=1; //hence dont wait for the child process

        pid_t pid=fork();
        signal(SIGCHLD,handle);
        if(pid<0)
        {
            perror("EXEC");
        }


        else if(pid==0)
        {
            //child process work
            if (execvp(command[0], command) < 0)
            {
                perror("Failed to execute process");
            }

        }

        else if(pid>0)
        {
            //parent process
            if(!bgndFlag)
                waitpid(pid,NULL,0);


        }


































        int count=0;
        for(count; count<=token_num; count++)
        {
            //printf("%s",command[count]);
            free(command[count]);
        }
        free(command);


    }









    return 0;
}
