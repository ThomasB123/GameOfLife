#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"gol.h"

int main(int argc, char *argv[])
{
    struct universe v;
    FILE *inputFile = stdin;
    char *inFile = NULL;
    FILE *outputFile = stdout;
    char *outFile = NULL;
    int number_of_generations = 5;
    bool genDuplicate = false;
    bool stats = false;
    bool torus = false;
    int i = 0;
    while (++i < argc)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                default:
                    fprintf(stderr,"Unkown command line option\n");
                    return 1;
                case 'i':
                    if (strlen(argv[i]) != 2 || argc <= ++i)
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    if (inputFile != stdin && strcmp(inFile, &argv[i][0]))
                    {
                        fprintf(stderr, "Differing input file arguments\n");
                        return 1;
                    }
                    inputFile = fopen(&argv[i][0], "r");
                    inFile = &argv[i][0];
                    if (inputFile == NULL)
                    {
                        fprintf(stderr, "Couldn't open input file\n");
                        return 1;
                    }
                    break;
                case 'o':
                    if (strlen(argv[i]) != 2 || argc <= ++i)
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    if (outputFile != stdout && strcmp(outFile, &argv[i][0]))
                    {
                        fprintf(stderr, "Differing output file arguments\n");
                        return 1;
                    }
                    outputFile = fopen(&argv[i][0], "w");
                    outFile = &argv[i][0];
                    if (outputFile == NULL)
                    {
                        fprintf(stderr, "Couldn't open output file\n");
                        return 1;
                    }
                    break;
                case 'g':
                    if (strlen(argv[i]) != 2)
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    char *arg;
                    if (argc > ++i)
                    {
                        arg = &argv[i][0];
                    }
                    else
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    while (*arg)
                    {
                        char c = *arg;
                        if (c < '0' || c > '9')
                        {
                            fprintf(stderr, "Invalid number\n");
                            return 1;
                        }
                        arg++;
                    }
                    if (genDuplicate && number_of_generations != atoi(&argv[i][0]))
                    {
                        fprintf(stderr, "Differing generations argument\n");
                        return 1;
                    }
                    number_of_generations = atoi(&argv[i][0]);
                    genDuplicate = true;
                    break;
                case 's':
                    if (strlen(argv[i]) != 2)
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    stats = true;
                    break;
                case 't':
                    if (strlen(argv[i]) != 2)
                    {
                        fprintf(stderr, "Invalid argument\n");
                        return 1;
                    }
                    torus = true;
                    break;
            }
        }
        else
        {
            fprintf(stderr, "Invalid argument\n");
            return 1;
        }
    }
    read_in_file(inputFile,&v);
    fclose(inputFile);
    for (int i = 0; i < number_of_generations; i++)
    {
        if (torus)
        {
            evolve(&v,will_be_alive_torus);
        }
        else
        {
            evolve(&v,will_be_alive);
        }
    }
    write_out_file(outputFile,&v);
    if (stats)
    {
        print_statistics(&v);
    }
    fclose(outputFile);
    return 0;
}
