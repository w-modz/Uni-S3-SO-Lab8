#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include <process.h>
#include <sys/types.h>
//#include <unistd.h>

bool is_number(const char* str)
{
    for (int i = 0; i < strlen(str); i++) 
    {
        if (isdigit(str[i]) == 0) return false;
    }
    return true;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
    }
    else if (strlen(argv[1]) > 25  || !is_number(argv[1]))
    {
        fprintf(stderr, "Invalid argument\n");
    }
    else if (strlen(argv[1]) == 1)
    {
        return atoi(argv[1]);
    }
    else
    {
        char *str = argv[1];
        int length = strlen(str);
        int length1 = floor(length / 2);
        int length2 = length - length1;
        char* str1 = (char*)malloc(length1+1);
        memcpy(str1, str, length1);
        str1[length1] = '\0';
        char* str2 = (char*)malloc(length2+1);
        memcpy(str2, str+length1, length2);
        str2[length2] = '\0';
        //printf("%s %s ",str1,str2);

        const char* executable_path = "C:/Users/vitom/OneDrive/Dokumenty/GitHub/Uni-S3-SO-Lab8/SO-Lab8/x64/Debug/SO-Lab8.exe";
        char* args1[] = { (char*)executable_path, str1, NULL };
        char* args2[] = { (char*)executable_path, str2, NULL };
        free(str1);
        free(str2);

        pid_t parent_pid = getpid();
        pid_t child1_pid = fork();
        if (child1_pid == -1)
        {
            perror("Failed to fork");
            exit(1);
        }
        else if (child1_pid == 0)
        {
            _execv(executable_path, args1);
        }
        else
        {
            pid_t child2_pid = fork();
            if (child2_pid == -1)
            {
                perror("Failed to fork");
                exit(1);
            }
            else if (child2_pid == 0)
            {
                _execv(executable_path, args2);
            }
            else
            {
                int child1_status, child2_status;

                if (waitpid(child1_pid, &child1_status, 0) == -1) {
                    perror("Waitpid failed");
                    exit(1);
                }
                if (waitpid(child2_pid, &child2_status, 0) == -1) {
                    perror("Waitpid failed");
                    exit(1);
                }
                fprintf(stdout, "%d %d %s %d\n", parent_pid, child1_pid, str1, child1_status);
                fprintf(stdout, "%d %d %s %d\n", parent_pid, child2_pid, str2, child2_status);
                return child1_status + child2_status;
            }
        }
    }
}