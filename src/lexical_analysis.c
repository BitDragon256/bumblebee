#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "symbols.h"
#include "tokenStream.h"

const char* numbers = "0123456789";

size_t identifierCount;
char** identifiers;

int check_keywords(char* buffer, Token t)
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
// // checks for delimiters, this includes operators
// int check_delimiters(char* buffer, Token t)
// {
//       assert(buffer[0] != '\0');

//       int ptr = 0;
//       char ch;

//       // this requires the buffer to contain at least one character
//       while (buffer[ptr + 1] != '\0') ptr++;
//       ch = buffer[ptr];
//       if (strchr(delNames, ch) != NULL)
//       {
//             t->delimiter = ch;
//             t->type = eDelimiter;
//             return 1;
//       }
//       else if (strchr(opNames, ch) != NULL)
//       {
//             t->delimiter = ch;
//             t->type = eOperator;
//             return 1;
//       }
//       return 0;
// }

#define LAZY_CHECK_NOTHING -1
int lazy_check(char* buffer, const char** symbolNames, const size_t nameLen)
{
      const int bufLen = strlen(buffer);
      int* dataIndex = malloc(nameLen * sizeof(int));
      memset(dataIndex, 0, nameLen * sizeof(int));

      int possibleCount;

      for (int bufI = 0; bufI < bufLen; bufI++)
      {
            for (size_t dataI = 0; dataI < nameLen; dataI++)
            {
                  if (dataIndex[dataI] == -1)
                        continue;
                  
                  if (buffer[bufI] == symbolNames[dataI][dataIndex[dataI]])
                        dataIndex[dataI]++;
                  else
                        dataIndex[dataI] = 0;
                        
                  if (dataIndex[dataI] >= (int) strlen(symbolNames[dataI]))
                  {
                        possibleCount++;
                        dataIndex[dataI] = -1;
                        continue;
                  }
            }
      }
      // return LAZY_CHECK_NOTHING if there are unfinished symbols
      for (size_t dataI = 0; dataI < nameLen; dataI++)
            if (dataIndex[dataI] > 0)
                  return LAZY_CHECK_NOTHING;
      
      if (possibleCount)
      {
            // get the longest symbol
            size_t longest = 0;
            int symbolData = -1;

            for (size_t dataI = 0; dataI < nameLen; dataI++)
            {
                  if (dataIndex[dataI] != -1)
                        continue;

                  const size_t len = strlen(symbolNames[dataI]);
                  if (len > longest)
                  {
                        symbolData = (int) dataI;
                        longest = len;
                  }
            }
            return symbolData;
      }
      
      // no delimiter found
      return LAZY_CHECK_NOTHING;
}

int check_delimiters(char* buffer, Token t)
{
      int result = lazy_check(buffer, delNames, DELIMITER_COUNT);
      if (result == -1)
            return 0;
      
      t->delimiter = result;
      t->type = eDelimiter;
      return 1;
}
int check_operators(char* buffer, Token t)
{
      int result = lazy_check(buffer, opNames, OPERATOR_COUNT);
      if (result == -1)
            return 0;

      t->operator = result;
      t->type = eOperator;
      return 1;
}
int check_identifiers(char* buffer, Token t)
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
int check_numbers(char* buffer, Token t)
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
int lexical(const char* inputStream, TokenStream tokenStream, char** errorString)
{
      char buffer[LEXICAL_ANALYSIS_BUFFER_SIZE];
      int bufferPtr = LEXICAL_ANALYSIS_BUFFER_SIZE;
      clear_buffer(&bufferPtr, buffer);
      char cur;
      int inputPtr = 0;

      printf("lexical analysis start\n");
      printf("input stream: \"%s\"\n", inputStream);

      // ERROR is no error
      int ERROR = 0;

      Token t = NULL;
      while ((cur = inputStream[inputPtr++]) != '\0')
      {
            if (!t)
                  t = create_token();

            if (bufferPtr >= LEXICAL_ANALYSIS_BUFFER_SIZE)
            {
                  ERROR = LA_BUFFER_SIZE_EXCEEDED;
                  break;
            }

            buffer[bufferPtr++] = cur;

            printf("next char: %c | complete buffer: %s | ptr: %i", cur, buffer, bufferPtr - 1);
            printf("\n");
            
            size_t delLen = 0;
            if (check_delimiters(buffer, t))
            {
                  delLen = strlen(delNames[t->delimiter]);

                  printf("delimiter found: \'%s\'\n", delNames[t->delimiter]);
            }
            else if (check_operators(buffer, t))
            {
                  delLen = strlen(opNames[t->operator]);

                  printf("operator found: \'%s\'\n", opNames[t->operator]);
            }

            if (delLen > 0)
            {
                  Token delToken = create_token();
                  assign_token(delToken, t);
                  if (bufferPtr > 1)
                  {
                        // there are unrecognized literals in front of the delimiter
                        buffer[bufferPtr - delLen] = '\0';
                        if (check_keywords(buffer, t));
                        else if (check_numbers(buffer, t));
                        else if (check_identifiers(buffer, t));
                        else
                        {
                              ERROR = LA_NO_VALID_LITERAL;
                              break;
                        }

                        push_token_stream(tokenStream, t);
                  }
                  push_token_stream(tokenStream, delToken);
                  clear_buffer(&bufferPtr, buffer);
                  
                  t = NULL;
            }
      }
      
      if (!t)
            t = create_token();
      if (check_identifiers(buffer, t))
            push_token_stream(tokenStream, t);
      clear_buffer(&bufferPtr, buffer);
      
      return ERROR;
}

const int sizeMult = 2;
void push_data(void** dataStream, void* elem, const size_t stride, size_t* size)
{
      size_t allocatedSize;

      if (size)
            allocatedSize = (size_t) pow(2, ceil(log2(*size)));
      else
            allocatedSize = 1;

      if (*size + 1 > allocatedSize || allocatedSize == 1)
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

int main(int argc, const char** argv)
{
      const char* input = "abc==def";
      TokenStream tokenStream = create_token_stream();
      char* errorString;

      lexical(input, tokenStream, &errorString);

      Token it = tokenStream->start;
      if (it)
            printf("%s ", tokenTypeNames[it->type]);
      while (next_token(&it))
            printf("%s ", tokenTypeNames[it->type]);

      destroy_token_stream(tokenStream);

      printf("\nPress Enter to exit ");
      getchar();
      
      return 0;
}