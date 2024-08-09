// shell
// Authored by Omer Weinbrand

// ==Description==
// The program each time picks up a sentence from the user(command)and make a mini shell-the program divides the command into an array of pointers that each cell point to one word:
// afterwards the program use the fork process the son runs the command with exacvp and the dad wait for to do that and if the son Succeeded upadte the num of command and chars.
// when the user enterd "done" the program print how many command entered and the the num of the total chars and then the program is finished.
// also the program support history command
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define  len 512
#define TRUE 1
#define FALSE 0
void open_file_read(int*,FILE*,char[]);
void open_file_write(FILE*,int*,char const[]);
char** check_sentence(char [],char [] ,int*,int*,int,int*,int*,int*,int*);
void  Execution_command(char**,int*,int*,int*,int*,int*,int*,int*,char const[]);
void sons_code(char**);
void dads_code(char**,int*,int*,int*,int*,int*,int*,char const[]);
void free_array(char**,const int*);
void malloc_fail();

int main() {
    printf("Hello, World!\n");
    char currentdir[100] = "";
    getcwd(currentdir,100);
    currentdir[strlen(currentdir)] = '<';
    char sentence [len] ; // Stores the sentence
    char str1[len]; // store every word from the sentence (help to solve edge cases)
    int word = 0;
    int flag = TRUE;
    int numComands = 0;
    int character = 0;
    int total_char = 0;
    int len_of_argv = 0;
    int count = 0;
    FILE * fp = NULL;
    open_file_read(&count,fp,"");

    do {
        printf("%s",currentdir);
        fgets(sentence,len,stdin);
        sentence[strlen(sentence)-1] = '\0';
        check_sentence(sentence,str1,&word,&character,0,&len_of_argv,&count,&numComands,&total_char);
        char ** argv = check_sentence(sentence,str1,&word,&character,1,&len_of_argv,&count,&numComands,&total_char);
        if(argv!= NULL)
            Execution_command(argv,&word,&numComands,&character,&total_char,&len_of_argv,&flag,&count,sentence);

    } while (flag == TRUE);


    return 0;
}
// check if the sentence is proper, and if the command is the history command it acts accordingly.
// if it not the history command :
// if p == 1 : then divides the sentence into words(char **argv)
// if p == 0 : count how many words and chars the sentence contain
char** check_sentence(char sentence[],char str1 [],int* word,int* character,int p,int* len1,int* count,int* Numcommand,int *total_char)
{
    int k = 0;
    int i ;
    int j = 0;
    char ** argv = NULL;
    FILE *fp = NULL;
    if(p == 1 && *word !=0)
    {
        argv = (char**)malloc(sizeof(char*)*(*word+1));
        *len1 = *word+1;
        if(argv == NULL)
        {
            malloc_fail();

        }

    }
    else if(sentence[0]== ' ' || sentence[0] =='\0' || sentence[strlen(sentence)-1] == ' ')
    {
        if(p == 0)
            fprintf(stderr,"invalid input\n");
        return NULL;
    }
    else if(strcmp(sentence,"history") == 0)
    {
        if(p == 0)
        {
            if (*count == 0) {
                printf("no history yet\n");
                return NULL;
            }
            open_file_read(count, fp, "history");
            open_file_write(fp, count, sentence);
            *Numcommand+=1;
            *total_char+=7;
            *character=0;
            *word =0;
            return NULL;
        }
        else
        {
            return NULL;
        }
    }
    for(i = 0 ; i < strlen(sentence) ; i++)
    {
        if(sentence[i] == ' ' && sentence[i-1] == ' ');

        else if((sentence[i] == ' ' && sentence[i-1] != ' ') || (sentence[i] != ' ' && i+1 == strlen(sentence)))
        {
            if(p == 1)
            {
                if(k < (*word) )
                {
                    if(i + 1== strlen(sentence))
                    {
                        str1[j] = sentence[i];
                        j++;
                    }
                    str1[j] = '\0';
                    j++;
                    *(argv + k) = (char *) malloc(j);
                    if( *(argv + k) == NULL) {
                        malloc_fail();
                    }
                    strcpy(*(argv+k), str1);
                    k++;
                    j=0;
                    strcpy(str1,"");
                }


            }
            else
            {
                *word+=1;
                if(i+1 == strlen(sentence))
                    *character+=1;
            }
        }
        else if(sentence[i] != ' ')
        {
            if(p == 0)
             *character +=1;
            else {
                str1[j] = sentence[i];
                j++;
            }

        }
    }
    if(p==1)
         *(argv + *word) = NULL;
    return argv;

}
// check if the command is cd,or done and acts accordingly.
// otherwise, do the fork process and the sun runs the command with execvp and the dad check if the sun seceded.
void  Execution_command(char** argv,int* word ,int*Numcommand,int*character,int*total_char,int*len1,int* flag,int* count,char const sentence[])
{
    FILE *fp = NULL;

        if(strcmp(*(argv),"cd") == 0 && *word > 1)
        {
            fprintf(stderr,"invalid input\n");
            free_array(argv,len1);
            *word =0;
            *character = 0;
            return;
        }
        else if(strcmp(*(argv),"cd") == 0)
        {
            printf("command not supported (yet)\n");
            free_array(argv,len1);
            *Numcommand+=1;
            *total_char+=*character;
            *character=0;
            *word =0;
            return;
        }
        else if(strcmp(*(argv),"done") == 0 && *word ==1)
        {
            free_array(argv,len1);
            *Numcommand+=1;
            printf("Num of commands : %d\nTotal number of words in all commands: %d !\n",*Numcommand,*total_char);
            *flag = FALSE;
            open_file_write(fp,count,sentence);
            return;
        }
       pid_t x ;
        x = fork();
        if(x == 0)
        {

            sons_code(argv);

        }
        else if(x > 0)
        {
            dads_code(argv,Numcommand,total_char,word,character,len1,count,sentence);

        }
        else
        {
            perror("son is not created");
            exit(1);
        }
}
//runs the command with execvp
void sons_code(char** argv)
{
    execvp(*(argv),argv);
    perror("execvp() faild");
    exit (1);
}
// if the sun succeeded update the total command and total chars and write the command into file.
// in any case free the memory.
void dads_code(char** argv,int*Numcommand,int*total_char,int*word,int*character,int*len1,int* count,char const sentence[])
{
    FILE *fp = NULL;
    int status;
    wait(&status);
    int check_son =  WEXITSTATUS(status);

    if (check_son != 1)
    {
        *Numcommand+=1;
        *total_char+=*character;
        open_file_write(fp,count,sentence);
    }
    free_array(argv,len1);
    *word=0;
    *character=0;
}
// free the memory of argv(char**)
void free_array(char** argv,const int* len1)
{
    for(int i = 0 ; i < *len1; i++)
    {
        free(argv[i]);
        argv[i] = NULL;
    }
    free(argv);
    argv = NULL;
}
void malloc_fail()
{
    fprintf(stderr,"malloc faild");
    exit(1);
}
// open the file to read and do 2 task:
// 1) show the history of the commands 
// 2) update the counter how many command we have done
void open_file_read(int* p,FILE* fp,char str[])
{
    if(*p == 0 && strcmp(str,"history") == 0)
    {
        printf("no history yet\n");
        return;
    }
    fp = fopen("Desktop:\\file.txt","r");
    if(fp == NULL)
    {
        if(strcmp(str,"") == 0)
        {
            *p = 0;
            return;
        }
    }
    char temp1[len];

    while (fgets(temp1, len, fp)!=NULL)
    {
        if(strcmp(str,"history") == 0)
        {
            printf("%s",temp1);
        }
        else
        {
            *(p)+=1;
        }

    }
    fclose(fp);
}
// write command to the file
void open_file_write(FILE* fp,int *p,char const str[])
{
    fp = fopen("Desktop:\\file.txt","a");
    if(fp == NULL)
    {
        printf( "cant open file");
        exit(0);
    }
    fprintf(fp,"%d: %s\n",*p,str);
    *p+=1;
    fclose(fp);
}
