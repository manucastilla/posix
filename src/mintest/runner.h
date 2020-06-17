#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

int status = 0;

void operacao_lenta()
{
    sleep(10);
}

void sigint_handler(int num)
{
    printf("\n Você deseja mesmo sair [s/n]? \n");
    char s_n;
    scanf("%c", &s_n);
    if (s_n == 's')
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    // clock:
    clock_t start, end;

    //handlers
    struct sigaction st;
    st.sa_handler = sigint_handler;
    sigemptyset(&st.sa_mask);
    st.sa_flags = 0;
    sigaction(SIGINT, &st, NULL);

    //relatório
    int size = sizeof(all_tests) / sizeof(test_data);
    printf("Running %d tests:\n", size);
    printf("=====================\n\n");
    int wt, pass_count = 0;

    pid_t testes[size];
    char buffer[50];
    int files[size];
    int saved_stdout;
    saved_stdout = dup(1);
    //rodar ao mesmo tempo
    for (int i = 0; i < size; i++)
    {
        sprintf(buffer, "./.temp%d", i);
        files[i] = open(buffer, O_RDWR | O_CREAT, 0600);

        pid_t temp = fork();
        if (temp == 0)
        {
            start = clock();
            dup2(files[i], 1);

            alarm(2);

            if (all_tests[i].function() == 0)
            {
                end = clock();
                printf("\033[0;32m %s: [PASS], (%lfs) \033[0m;\n", all_tests[i].name, (double)(end - start) / CLOCKS_PER_SEC);
                return 0;
            }
            return 1;
        }

        testes[i] = temp;
    }

    //imprimindo as informações (erro/sinais) de cada teste que falha
    int nomes = 0;
    while (waitpid(testes[nomes], &wt, 0) > 0)
    {
        dup2(files[nomes], 1);
        if (WIFEXITED(wt))
        {
            if (WEXITSTATUS(wt) == 0)
            {
                // wexistatus: If all the commands succeed, then the value in counter will be zero
                pass_count++;
            }
        }
        else
        {
            if (WTERMSIG(wt) == SIGALRM)
            {
                printf("\033[1;34m %s: [TIME] %s\033[0m;\n", all_tests[nomes].name, strsignal(WTERMSIG(wt)));
            }
            else
            {
                printf("\033[0;31m %s: [ERRO] %s \033[0m;\n", all_tests[nomes].name, strsignal(WTERMSIG(wt)));
            }
        }
        nomes++;
    }
    dup2(saved_stdout, 1);
    char c;
    for (int i = 0; i < size; i++)
    {
        lseek(files[i], 0, SEEK_SET);
        int bytes_read = read(files[i], &c, 1);

        while (bytes_read > 0)
        {
            write(1, &c, 1);
            bytes_read = read(files[i], &c, 1);
        }

        close(files[i]);
        sprintf(buffer, "./.temp%d", i);
        remove(buffer);
    }
    close(saved_stdout);

    printf("\n\n=====================\n");
    printf("%d/%d tests passed\n", pass_count, size);

    return 0;
}
