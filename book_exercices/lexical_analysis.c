#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define KEYWORD_COUNT 3

typedef enum { eIf = 0, eInt = 1, eArray = 2 } keywords;
const char* keywordNames[KEYWORD_COUNT] = {
      "if",
      "int",
      "array"
};

const char* delimiters = "[];,. ";
const char* numbers = "0123456789";
const char* operators = "+-*/&%!^|~";

typedef enum { eKeyword, eIdentifier, eDelimiter, eNumber } tokenType;
typedef struct
{
      keywords keyword;
      char* identifier;
      char delimiter; // delimiters & operators
      int64_t number;
      tokenType type;
} token;

const int sizeMult = 2;
void push(int* streamSize, token** tokenStream, token elem)
{
      int allocatedSize;
      if (*streamSize == 0)
            allocatedSize = 1;
      else
            allocatedSize = (int) pow(2, ceil(log2(*streamSize)));

      (*streamSize)++;

      if (*streamSize > allocatedSize || allocatedSize == 1)
      {
            // reallocate the array
            allocatedSize *= 2;
            token* newStream = malloc(allocatedSize * sizeof(token));
            for (int i = 0; i < *streamSize - 1; i++)
                  newStream[i] = (*tokenStream)[i];
            if (tokenStream)
                  free(*tokenStream);
            *tokenStream = newStream;
      }

      (*tokenStream)[*streamSize] = elem;
}

bool check_keywords(char* buffer, token* t)
{
      for (int i = 0; i < KEYWORD_COUNT; i++)
      {

      }

      return 1;
}

void clear_buffer(int size, char* buffer)
{

      for (int i = 0; i < size; i++)
      {
            buffer[i] = '\0';
      }
}

#define LEXICAL_ANALYSIS_BUFFER_SIZE 30
void lexical(const char* inputStream, token** tokenStream, char** errorString)
{
      char buffer[LEXICAL_ANALYSIS_BUFFER_SIZE];
      clear_buffer(LEXICAL_ANALYSIS_BUFFER_SIZE, buffer);
      int bufferPtr;
      char cur = inputStream[0];
      int inputPtr = 0;

      int streamSize = 0;

      while (cur != '\0')
      {
            cur = inputStream[inputPtr];
            token t;

            buffer[bufferPtr] = cur;
            bufferPtr++;

            if (check_keywords(buffer, &t))
            {
                  t.type = eKeyword;
            }
            else continue;

            clear_buffer(bufferPtr, buffer);
            push(&streamSize, tokenStream, t);
      }
}

int main(int argc, const char** argv)
{
      const char* input = "int a = 2;";
      token* tokens;
      char* errorString;

      //lexical(input, &tokens, &errorString);
      
      return 0;
}