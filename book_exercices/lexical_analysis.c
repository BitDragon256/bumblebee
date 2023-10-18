#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define KEYWORD_COUNT 3

typedef enum { eIf = 0, eInt = 1, eArray = 2 } keywords;
const char* keywordNames[KEYWORD_COUNT] = {
      "if",
      "int",
      "array"
};

const char* delimiters = "[];,. \"";
const char* operators = "+-*/&%!^|~";
const char* numbers = "0123456789";

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

int check_keywords(char* buffer, token* t)
{
      for (int i = 0; i < KEYWORD_COUNT; i++)
      {

      }

      return 1;
}
// checks for delimiters, this includes operators
int check_delimiters(char* buffer, token* t)
{
      int ptr = 0;
      char ch;
      while ((ch = buffer[(ptr++) + 1]) != '\0');
      if (
            strchr(delimiters, ch) != '\0' ||
            strchr(operators, ch) != '\0'
      )
      {
            t->delimiter = ch;
            t->type = eDelimiter;
            return 1;
      }
      return 0;
}
int check_identifiers(char* buffer, token* t)
{
      int size = -1;
      while (buffer[++size] != '\0');
      
      if (size > 0)
      {
            t->type = eIdentifier;
            t->identifier = malloc(size * sizeof(char));
            for (int i = 0; i < size; i++)
                  t->identifier[i] = buffer[i];
            return 1;
      }
      return 0;
}

int char_to_digit(const char ch)
{
      for (int i = 0; i < 10; i++)
            if (ch == numbers[i])
                  return i;
      return -1;
}

// only supports integers
int check_numbers(char* buffer, token* t)
{
      t->number = 0;
      int power = 1;
      char ch; int ptr = -1;
      // set the ptr to the end of the string
      while ((ch = buffer[++ptr]) != '\0');
      while (ptr-- > 0)
      {
            ch = buffer[ptr];

            int d = char_to_digit(ch);
            if (d != -1)
            {
                  t->number += power * d;
                  power *= 10;
            }
            else
            {
                  
            }
      }
}

void clear_buffer(int size, char* buffer)
{
      for (int i = 0; i < size; i++)
      {
            buffer[i] = '\0';
      }
}

// ERROR CODES
// Prefix for Lexical Analysis: LA
#define LA_BUFFER_SIZE_EXCEEDED 101
#define LA_NO_VALID_LITERAL 102

#define LEXICAL_ANALYSIS_BUFFER_SIZE 30
int lexical(const char* inputStream, token** tokenStream, char** errorString)
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

            if (bufferPtr >= LEXICAL_ANALYSIS_BUFFER_SIZE)
            {
                  return LA_BUFFER_SIZE_EXCEEDED;
            }

            buffer[bufferPtr] = cur;
            bufferPtr++;

            if (check_keywords(buffer, &t))
            {
                  push(&streamSize, tokenStream, t);
                  clear_buffer(bufferPtr, buffer);
            }
            if (check_delimiters(buffer, &t))
            {
                  token delToken = t;
                  if (bufferPtr > 1)
                  {
                        // there are unrecognized literals in front of the delimiter
                        buffer[bufferPtr - 1] = '\0';
                        if (check_keywords(buffer, &t));
                        else if (check_numbers(buffer, &t));
                        else if (check_identifiers(buffer, &t));
                        else return LA_NO_VALID_LITERAL;

                        push(&streamSize, tokenStream, t);
                  }
                  push(&streamSize, tokenStream, delToken);
                  clear_buffer(bufferPtr, buffer);
            }
            else if (check_numbers(buffer, &t))
            {
                  push(&streamSize, tokenStream, t);
                  clear_buffer(bufferPtr, buffer);
            }
            else continue;
      }
}

int main(int argc, const char** argv)
{
      const char* input = "int a = 2;";
      token* tokens;
      char* errorString;

      lexical(input, &tokens, &errorString);
      
      return 0;
}