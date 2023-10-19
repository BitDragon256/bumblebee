#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#include "string.h"

const char* kFilenameInputMacro = "i";

typedef struct
{
      const char* filename;
} input_arguments;

input_arguments get_arguments(const int argc, const char** argv, input_arguments* args)
{
      for (int i = 1; (i+1) < argc; i += 2)
      {
            if (same_str(argv[i], kFilenameInputMacro))
            {
                  args->filename = argv[i + 1];
            }
      }
}

int main(int argc, const char** argv)
{
      input_arguments args;
      get_arguments(argc, argv, &args);

      FILE* sourceFile = fopen(args.filename, "r");
      if (!sourceFile)
      {
            perror("failed to open file");
            printf("file: %s", args.filename);
            return 0;
      }

      const int kMaxSource = 1000;
      char* sourceCode = malloc(kMaxSource * sizeof(const char));
      int runner = 0;
      int ch;
      while ((ch = fgetc(sourceFile)) != EOF)
      {
            sourceCode[runner] = ch;
            runner++;
            if (runner == kMaxSource)
                  break;
      }

      // test brainfuck program
      const int kCells = 30000;
      uint32_t* cells = malloc(kCells * sizeof(uint32_t));
      for (int i = 0; i < kCells; i++) {
            cells[i] = 0;
      }
      uint8_t pointer = 2;
      int execPtr = 0;
      int jump = 0;
      int indent = 0;

      while (pointer < kCells && execPtr < kMaxSource)
      {
            if (sourceCode[execPtr] == '\0')
                  break;

            ch = sourceCode[execPtr];
            if (!strchr("<>+-.[]", ch))
            {
                  if (jump == -1)
                        execPtr--;
                  else
                        execPtr++;
                  continue;
            }

            if (jump == 1)
            {
                  if (ch == '[')
                  {
                        indent++;
                  }
                  if (ch == ']')
                  {
                        if (indent == 0)
                        {
                              jump = 0;
                        }
                        else
                        {
                              indent--;
                        }
                  }
            }
            else if (jump == -1)
            {
                  if (ch == ']')
                  {
                        indent++;
                  }
                  if (ch == '[')
                  {
                        if (indent == 0)
                        {
                              jump = 0;
                        }
                        else
                        {
                              indent--;
                        }
                  }
            }
            else
            {
                  switch (ch)
                  {
                  case '>':
                        pointer++;
                        break;
                  case '<':
                        pointer--;
                        break;
                  case '+':
                        cells[pointer]++;
                        break;
                  case '-':
                        cells[pointer]--;
                        break;
                  case '.':
                        printf("%c", cells[pointer]);
                        break;
                  case '[':
                        if (cells[pointer] == 0)
                              jump = 1;
                        break;
                  case ']':
                        if (cells[pointer] != 0)
                        jump = -1;
                        break;
                  default:
                        break;
                  }
            }

            // for (int i = 0; i < 10; i++)
            // {
            //       printf("%i ", cells[i]);
            // }
            // printf("\nexec: %i %c | ptr: %i | our cell: %i | jump: %i \n", execPtr, ch, pointer, cells[pointer], jump);
            
            if (jump == -1)
                  execPtr--;
            else
                  execPtr++;
      }

      free(sourceCode);
      free(cells);
}