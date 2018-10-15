#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#define MEM_SIZE   30720

typedef unsigned char byte;

static byte memory[MEM_SIZE];
static byte *memptr = NULL;

static FILE *source = NULL;

static void die(const char *msg, ...);

static void init(const char *src);
static void run(int depth);
static void quit(void);

int
main(int argc, char *argv[])
{
    if (argc < 2)
        die("No input source file");

    init(argv[1]);

    run(0);

    quit();

    return EXIT_SUCCESS;
}

static void
die(const char *msg, ...)
{
    va_list args;

    fprintf(stderr, "ERROR: ");

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\n");

    quit();
    exit(EXIT_FAILURE);
}

static void
init(const char *src)
{
    memset(memory, 0, sizeof(byte) * MEM_SIZE);
    memptr = memory;

    source = fopen(src, "r");
    if (source == NULL)
        die("Unable to open \"%s\"", src);
}

static void
skip(void)
{
    int match = 1, symbol;
    size_t head = ftell(source) - 1;
    while (match > 0 && (symbol = fgetc(source)) != EOF) {
        match += symbol == '[';
        match -= symbol == ']';
    }
    
    if (match != 0)
        die("Unmatched opening parenthesis: %lu", head);
}

static void
run(int depth)
{
    int symbol;
    size_t head = ftell(source) - 1;
    while ((symbol = fgetc(source)) != EOF) {
        switch(symbol) {
            case '+': 
                (*memptr)++; 
                break;
            case '-': 
                (*memptr)--; 
                break;
            case '>': 
                memptr++;
                if (memptr > &memory[MEM_SIZE-1])
                    die("Overflow");
                break;
            case '<': 
                memptr--;
                if (memptr < memory)
                    die("Underflow");
                break;
            case '.': 
                putchar(*memptr); 
                break;
            case ',': 
                *memptr = getchar(); 
                break;
            case '[': 
                if (*memptr != 0) 
                    run(depth + 1);
                else 
                    skip();
                break;
            case ']': 
                if (depth == 0) 
                    die("Unmatched closing parenthesis: %lu", ftell(source));
                fseek(source, head, SEEK_SET);
                return;
            default:  
                break;
        }
    }

    if (depth != 0)
        die("Unmatched opening parenthesis: %lu", head);
}

static void
quit(void)
{
    if (source)
        fclose(source);
}

