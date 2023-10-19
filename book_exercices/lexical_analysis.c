#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

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

typedef enum { eKeyword = 0, eIdentifier = 1, eDelimiter = 2, eNumber = 3, eNullToken = 4 } tokenType;
typedef struct
{
      keywords keyword;
      char* identifier;
      char delimiter; // delimiters & operators
      int64_t number;
      tokenType type;
} token;

token* create_token_stream()
{
      token* stream = malloc(sizeof(token));
      stream->type = eNullToken;
      return stream;
}
void destroy_token_stream(token* tokenStream)
{
      if (tokenStream)
            free(tokenStream);
}

const int sizeMult = 2;
void push(token** tokenStream, token elem)
{
      int allocatedSize;
      int ptr = 0;
      while((*tokenStream)[ptr++].type != eNullToken);

      allocatedSize = (int) pow(2, ceil(log2(ptr)));

      if (ptr + 1 > allocatedSize)
      {
            // reallocate the array
            while (ptr + 1 > allocatedSize)
                  allocatedSize *= sizeMult;
            token* newStream = malloc(allocatedSize * sizeof(token));
            for (int i = 0; i < ptr - 1; i++)
                  newStream[i] = (*tokenStream)[i];
            if (tokenStream)
                  free(*tokenStream);
            *tokenStream = newStream;
      }

      (*tokenStream)[ptr - 1] = elem;
      (*tokenStream)[ptr].type = eNullToken;
}

int check_keywords(char* buffer, token* t)
{
      for (int i = 0; i < KEYWORD_COUNT; i++)
      {
            if (!strcmp(buffer, keywordNames[i]))
            {
                  t->type = eKeyword;
                  t->keyword = i;
                  return 1;
            }
      }

      return 0;
}
// checks for delimiters, this includes operators
int check_delimiters(char* buffer, token* t)
{
      assert(buffer[0] != '\0');

      int ptr = 0;
      char ch;

      // this requires the buffer to contain at least one character
      while (buffer[ptr + 1] != '\0') ptr++;
      ch = buffer[ptr];
      if (
            strchr(delimiters, ch) != NULL ||
            strchr(operators, ch) != NULL
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
      int size = 0;
      while (buffer[size++] != '\0');
      
      if (size > 0)
      {
            t->type = eIdentifier;
            t->identifier = malloc(size * sizeof(char));
            for (int i = 0; i < size; i++)
                  t->identifier[i] = buffer[i];
            
            printf("identifier found: \"%s\"\n", t->identifier);
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
      assert(buffer[0] != '\0');

      t->number = 0;
      int power = 1;
      char ch; int ptr = 0;
      // set the ptr to the end of the string
      while (buffer[ptr + 1] != '\0') ptr++;
      while (ptr >= 0)
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
                  return 0;
            }
            ptr--;
      }
      return 1;
}

void clear_buffer(int* end, char* buffer)
{
      for (int i = 0; i < *end; i++)
      {
            buffer[i] = '\0';
      }
      *end = 0;
}

// ERROR CODES
// Prefix for Lexical Analysis: LA
#define LA_BUFFER_SIZE_EXCEEDED 101
#define LA_NO_VALID_LITERAL 102

#define LEXICAL_ANALYSIS_BUFFER_SIZE 30
int lexical(const char* inputStream, token** tokenStream, char** errorString)
{
      char buffer[LEXICAL_ANALYSIS_BUFFER_SIZE];
      int bufferPtr = LEXICAL_ANALYSIS_BUFFER_SIZE;
      clear_buffer(&bufferPtr, buffer);
      char cur;
      int inputPtr = 0;

      int streamSize = 0;

      printf("lexical analysis start\n");
      printf("input stream: \"%s\"\n", inputStream);

      while ((cur = inputStream[inputPtr++]) != '\0')
      {
            token t;

            if (bufferPtr >= LEXICAL_ANALYSIS_BUFFER_SIZE)
            {
                  return LA_BUFFER_SIZE_EXCEEDED;
            }

            buffer[bufferPtr++] = cur;

            printf("next char: %c | complete buffer: %s | ptr: %i", cur, buffer, bufferPtr - 1);
            printf("\n");

            if (check_keywords(buffer, &t))
            {
                  push(tokenStream, t);
                  clear_buffer(&bufferPtr, buffer);

                  printf("keyword found: %s\n", keywordNames[t.keyword]);
            }
            else if (check_delimiters(buffer, &t))
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

                        push(tokenStream, t);
                  }
                  push(tokenStream, delToken);
                  clear_buffer(&bufferPtr, buffer);

                  printf("delimiter found: \'%c\'\n", delToken);
            }
            else if (check_numbers(buffer, &t))
            {
                  push(tokenStream, t);
                  clear_buffer(&bufferPtr, buffer);

                  printf("number found: %i\n", t.number);
            }
      }
}

int main(int argc, const char** argv)
{
      const char* input = "int a = 2;";
      token* tokenStream = create_token_stream();
      char* errorString;

      lexical(input, &tokenStream, &errorString);

      int ptr = -1;
      while (tokenStream[++ptr].type != eNullToken)
            printf("%i ", tokenStream[ptr].type);

      destroy_token_stream(tokenStream);
      
      return 0;
}