#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int getch()
{
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main(int argc, char* argv[])
{
    // Argc validation
    if (argc < 3)
    {
        fprintf(stderr, "Error: Not enough arguments\n");
        return 1;
    }

    // Argv[2] validation
    char *endptr;
    int n = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || n < 0)
    {
        fprintf(stderr, "Error: 2nd argument must be natural number or 0\n");
        return 1;
    }

    // File opening, argv[1] validation
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "Error: Fail to open the file\n");
        return 1;
    }    

    int lineCounter = 0;
    char c;
    while ((c = getc(f)) != EOF)
    {
        // Printing
        putc(c, stdout);
        
        // Line counting
        if (c == '\n')
            lineCounter++;

        // Pause for confirmation
        if (n != 0 && lineCounter == n)
        {
            lineCounter = 0;
            printf("Continue? [n/any]");
            c = getch();
            printf("\033[2K\r"); // Erase previous line
            if (c == 'n')
                break;
        }
    }
    putc('\n', stdout);

    // File closing
    if(fclose(f) == EOF)
    {
        fprintf(stderr, "Error: Fail to close the file\n");
        return 1;
    }

    return 0;
}
