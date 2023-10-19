#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#include "string.h"

#include "simple_one_pass_comp.c"

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

int start_compilation(char* str)
{
//
// PUT HERE THE COMPILATION ENDPOINT
//
compile(str);
//
//
//
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

      start_compilation(sourceCode);

      free(sourceCode);
}