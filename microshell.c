
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>
#include <unistd.h>
#include <sys/wait.h>

void reset_pipes(int *prev_pipe,int *next_pipe, int pipes,int save_pipes)
{
    if (pipes != save_pipes)
    {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
    prev_pipe[0] = next_pipe[0];
    prev_pipe[1] = next_pipe[1];
}

int pipe_cmd(int i, int j, char **av, char **envp)
{
    int idx;
    int pipes;
    int save_pipes;
    int pid;
    int prev_pipe[2];
    int h;
    int next_pipe[2];
    char *new_av[j - i + 1];
    char    **sv;

    idx = i;
    pipes = 0;
    h = 0;
    while (idx < j)
    {
        if (!strcmp(av[idx], "|")) 
            pipes++;
        idx++;
    }
    if (!pipes)
        return (1);
    idx = i;
    while (idx < j) {
        new_av[h] = av[idx];
        idx++;
        h++;
    }
    new_av[h] = NULL;
    sv = new_av;
    save_pipes = pipes;
    while (pipes >= 0) 
    {
        idx = 0;
        if (pipes)
            pipe(next_pipe);
        pid = fork();
        if(pid == 0)
        {
            if(pipes)
            {
                dup2(next_pipe[1], 1);
                close(next_pipe[0]);
                close(next_pipe[1]);
            }
            if(save_pipes != pipes)
            {
                dup2(prev_pipe[0], 0);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }
            while (sv[idx]) 
            {
                if(!strcmp(sv[idx], "|"))
                    {
                        sv[idx] = NULL;
                        break ; 
                    }
                idx++;
            }
            if(execve(sv[0], sv, envp))
            {
                write(2, "error: cannot execute ", 22);
                write(2, sv[0], strlen(sv[0]));
                write(2, "\n", 1);
                exit(0);
            }
            // perror("error ");
        }
        else 
        {
             while (*sv)
            {
                if(!strcmp(*sv, "|"))
                    {
                        sv++;
                        break ;
                    }
                    sv++;
            }
            reset_pipes(prev_pipe,next_pipe,pipes,save_pipes);
            waitpid(pid, 0, 0);
        }
        pipes--;
    }
    return (0);
}

void semi_col(int i, int j, char **av, char **envp)
{
    int idx;
    int h;
    int pid;
    char *new_av[j - i + 1];

    if (!pipe_cmd(i, j, av, envp))
        return;
    h = 0;
    idx = i;
    while (idx < j) {
        new_av[h] = av[idx];
        idx++;
        h++;
    }
    new_av[h] = NULL;
    if(!new_av[0])
        return ;
    pid = fork();
    if (pid == 0)
    {
        execve(av[i], new_av, envp);
        perror("error ");
        exit(0);
    }
    else {
    waitpid(pid, 0, 0);
    }
}

int main(int ac, char **av, char **envp)
{
    int i;
    int j;

    i = 1;
    j = 1;
    if(ac > 1)
    {
        while(i < ac)
        {
            while (av[j] && strcmp(av[j], ";"))
                j++;
            if ((av[j] && !strcmp(av[j], ";")) || av[j] == NULL)
            {
                semi_col(i, j, av, envp);
                j++;
            }
            i = j;
        }
    }
    return (0);
}