#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"gol.h"

void read_in_file(FILE *infile, struct universe *u)
{
    u->columns = 0;
    u->rows = 0;
    u->matrix = malloc(513);
    char *currentPosition = u->matrix;
    size_t maximumLength = 513;
    size_t length = 0;
    int character;
    if (currentPosition == NULL)
    {
        free(u->matrix);
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    bool first = true;
    int columns = 0;
    while (!feof(infile))
    {
        character = fgetc(infile);
        if (character == EOF)
        {
            if (columns != 0)
            {
                free(u->matrix);
                fprintf(stderr, "No line break at end of input\n");
                exit(1);
            }
            else if (columns != 0)
            {
                u->rows++;
            }
            break;
        }
        if (character == '\n')
        {
            if (first)
            {
                first = false;
            }
            u->rows++;
            if (columns != u->columns)
            {
                free(u->matrix);
                fprintf(stderr, "Invalid universe size\n");
                exit(1);
            }
            columns = 0;
        }
        else
        {
            if (character != '.' && character != '*')
            {
                free(u->matrix);
                fprintf(stderr, "Invalid character in universe\n");
                exit(1);
            }
            if (++length >= maximumLength)
            {
                char *newBuffer = realloc(u->matrix, maximumLength += u->columns);
                if (newBuffer == NULL)
                {
                    free(u->matrix);
                    fprintf(stderr, "Memory error\n");
                    exit(1);
                }
                currentPosition = newBuffer + (currentPosition - u->matrix);
                u->matrix = newBuffer;
            }
            *currentPosition++ = character;
            if (first)
            {
                u->columns++;
                if (u->columns > 512)
                {
                    free(u->matrix);
                    fprintf(stderr, "More than 512 columns\n");
                    exit(1);
                }
            }
            columns++;
        }
    }
    if (u->rows == 0)
    {
        free(u->matrix);
        fprintf(stderr,"Invalid universe input\n");
        exit(1);
    }
    *currentPosition = '\0';
    u->alive = 0;
    u->total = 0;
    for (int i = 0; i < u->columns*u->rows; i++)
    {
        if (u->matrix[i] == '*')
        {
            u->alive += 1;
        }
        u->total += 1;
    }
}

void write_out_file(FILE *outfile, struct universe *u)
{
    for (int i = 0; i < u->rows; i++)
    {
        for (int j = 0; j < u->columns; j++)
        {
            fprintf(outfile, "%c", u->matrix[u->columns * i + j]);
        }
        fprintf(outfile, "\n");
    }
}

int is_alive(struct universe *u, int row, int column)
{
    if (u->matrix[row * u->columns + column] == '*')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int will_be_alive(struct universe *u, int row, int column)
{
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (!(i < 0 || i >= u->rows || j < 0 || j >= u->columns))
            {
                count += is_alive(u,i,j);
            }
        }
    }
    if (count == 3 || (count == 4 && is_alive(u,row,column)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int will_be_alive_torus(struct universe *u, int row, int column)
{
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            count += is_alive(u,(i+u->rows)%u->rows,(j+u->columns)%u->columns);
        }
    }
    if (count == 3 || (count == 4 && is_alive(u,row,column)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int row, int column))
{
    char *newMatrix;
    newMatrix = malloc(u->rows*u->columns);
    for (int i = 0; i < u->rows; i++)
    {
        for (int j = 0; j < u->columns; j++)
        {
            if (rule(u,i,j))
            {
                newMatrix[i * u->columns + j] = '*';
                u->alive++;
            }
            else
            {
                newMatrix[i * u->columns + j] = '.';
            }
            u->total++;
        }
    }
    strcpy(u->matrix, newMatrix);
    free(newMatrix);
}

void print_statistics(struct universe *u)
{
    int currentAlive = 0;
    int currentTotal = 0;
    for (int i = 0; i < u->rows; i++)
    {
        for (int j = 0; j < u->columns; j++)
        {
            if (u->matrix[i * u->columns + j] == '*')
            {
                currentAlive++;
            }
            currentTotal++;
        }
    }
    float val1 = (float)currentAlive/currentTotal * 100;
    float percentage = roundf(val1 * 1000) / 1000;
    fprintf(stdout, "%.3f%% of cells currently alive\n", percentage);
    float val2 = (float)u->alive / u->total * 100;
    float average = roundf(val2 * 1000) / 1000;
    fprintf(stdout, "%.3f%% of cells alive on average\n", average);
}
