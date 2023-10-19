#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "symbols.c"

const char* numbers = "0123456789";

size_t identifierCount;
char** identifiers;

typedef enum { eKeyword = 0, eIdentifier = 1, eDelimiter = 2, eOperator = 3, eNumber = 4, eNullToken = 5 } tokenType;
// FOR DEBUGGING
const char* tokenTypeNames[6] = {
      "keyword", "identifier", "delimiter", "operator", "number", "nulltoken"
};

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
            if (!strcmp(buffer, kwNames[i]))
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
      if (strchr(delimiters, ch) != NULL)
      {
            t->delimiter = ch;
            t->type = eDelimiter;
            return 1;
      }
      else if (strchr(operators, ch) != NULL)
      {
            t->delimiter = ch;
            t->type = eOperator;
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

      t->type = eNumber;
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

                  printf("delimiter found: \'%c\'\n", delToken.delimiter);
            }
            else if (check_numbers(buffer, &t))
            {
                  push(tokenStream, t);
                  clear_buffer(&bufferPtr, buffer);

                  printf("number found: %i\n", t.number);
            }
      }
}

void push_data(void** dataStream, void* elem, const size_t stride, size_t* size)
{
      int allocatedSize;
      int ptr = 0;

      if (size)
            allocatedSize = (int) pow(2, ceil(log2(*size)));
      else
            allocatedSize = 1;

      if (size + 1 > allocatedSize || allocatedSize == 1)
      {
            // reallocate the array
            allocatedSize *= sizeMult;
            void* newStream = malloc(allocatedSize * stride);
            memcpy(newStream, dataStream, stride * *size);
            if (*dataStream)
                  free(*dataStream);
            *dataStream = newStream;
      }

      memcpy((void*)(*dataStream + *size), elem, stride);
      (*size)++;
}

void load_symbols()
{
      const char* filename = "lexan_symbols";
      FILE* file = fopen(filename, 'r');
      if (!file)
      {
            printf("file could not be opened");
            return;
      }
      char ch;
      char buffer[LEXICAL_ANALYSIS_BUFFER_SIZE];
      int bufferPtr = 0;
      tokenType type = eNullToken;
      while ((ch = fgetc(file)) != EOF)
      {
            if (ch == '\n')
            {
                  switch (type)
                  {
                        case eDelimiter:
                        push_data(&delimiters, &buffer, sizeof(char*), &delimiterCount);
                        break;
                        
                        case eOperator:
                        push_data(&operators, &buffer, sizeof(char*), &operatorCount);
                        break;

                        case eIdentifier:
                        push_data(&identifiers, &buffer, sizeof(char*), &identifierCount);
                        break;

                        case eKeyword:
                        push_data(&keywords, &buffer, sizeof(char*), &keywordCount);
                  }
                  bufferPtr = 0;
                  buffer[]
            }
            buffer[bufferPtr++] = ch;
            if (strcmp(buffer, "keywords"))
      }
      fclose(file);
}

int main(int argc, const char** argv)
{
      const char* input = "int a = 2+3;";
      token* tokenStream = create_token_stream();
      char* errorString;

      lexical(input, &tokenStream, &errorString);

      int ptr = -1;
      while (tokenStream[++ptr].type != eNullToken)
            printf("%s ", tokenTypeNames[tokenStream[ptr].type]);

      destroy_token_stream(tokenStream);
      
      return 0;
}