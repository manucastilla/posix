#include "mintest/macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>

int test1()
{
    test_printf("Hello! %d %f\n", 3, 3.14);
    return 0;
}

int loop_infinito()
{
    while (1)
    {
    }

    return 0;
}

int falha_seg()
{
    raise(SIGSEGV);
    // int vector[5];
    // vector[6] = 1;
    return 0;
}

int div_zero()
{
    return 1 / 0;
}

int muito_trabalho()
{
    int oi;
    for (int i = 0; i < 100; i++)
    {
        int x = 1 + i;
        for (int j = 0; j < 40; j++)
        {
            int y = j * i * 2;
            for (int z = 0; z < 10; z++)
            {
                oi = 5 + i + j + z;
            }
        }
    }

    return 0;
}

int falha_passa()
{
    test_assert(1 == 1, "This always pass.");
    test_assert(1 == 0, "This always fails!");

    return 0;
}

int varios_prints()
{
    for (int i = 0; i < 50; i++)
    {
        printf("oi, eu sou print num %d\n", i);
    }
}

int teste_devagar()
{
    for (int i = 0; i < 20; i++)
    {
        i++;
        sleep(2);
    }
}

int teste_rapido()
{
    for (int i = 0; i < 20; i++)
    {
        i++;
    }
}

test_list = {
    TEST(varios_prints),
    TEST(test1),
    TEST(falha_seg),
    TEST(falha_passa),
    TEST(div_zero),
    TEST(muito_trabalho),
    TEST(teste_devagar),
    TEST(loop_infinito),
    TEST(teste_rapido)

};
#include "mintest/runner.h"
