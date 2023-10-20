#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "symbols.h"
#include "TokenStream.h"

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

int check_delimiters(char* buffer, Token t)
{
      const int bufLen = strlen(buffer);
      int delIndex[DELIMITER_COUNT];
      memset(delIndex, 0, DELIMITER_COUNT * sizeof(int));

      int delimiterPossibleCount = 0;

      for (int bufI = 0; bufI < bufLen; bufI++)
      {
            for (int delI = 0; delI < DELIMITER_COUNT; delI++)
            {
                  if (delIndex[delI] == -1)
                        continue;
                  
                  if (buffer[bufI] == delNames[delI][delIndex[delI]])
                        delIndex[delI]++;
                  else
                        delIndex[delI] = 0;
                        
                  if (delIndex[delI] >= strlen(delNames[delI]))
                  {
                        delimiterPossibleCount++;
                        delIndex[delI] = -1;
                        continue;
                  }
            }
      }
      // return 0 if there are unfinished delimiters
      for (int delI = 0; delI < DELIMITER_COUNT; delI++)
            if (delIndex[delI] > 0)
                  return 0;
      
      t->type = eDelimiter;
      if (delimiterPossibleCount)
      {
            // get the longest delimiter
            int longest = -1;

            for (int delI = 0; delI < DELIMITER_COUNT; delI++)
            {
                  if (delIndex[delI] != -1)
                        continue;

                  const int len = strlen(delNames[delI]);
                  if (len > longest)
                  {
                        t->delimiter = delI;
                        longest = len;
                  }
            }
            return 1;
      }
      
      // no delimiter found
      return 0;
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
int lexical(const char* inputStream, TokenStream TokenStream, char** errorString)
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
            Token t = create_token();

            if (bufferPtr >= LEXICAL_ANALYSIS_BUFFER_SIZE)
            {
                  return LA_BUFFER_SIZE_EXCEEDED;
            }

            buffer[bufferPtr++] = cur;

            printf("next char: %c | complete buffer: %s | ptr: %i", cur, buffer, bufferPtr - 1);
            printf("\n");
            

            if (check_delimiters(buffer, t))
            {
                  Token delToken = create_token();
                  assign_token(delToken, t);
                  if (bufferPtr > 1)
                  {
                        // there are unrecognized literals in front of the delimiter
                        buffer[bufferPtr - strlen(delNames[delToken->delimiter])] = '\0';
                        if (check_keywords(buffer, t));
                        else if (check_numbers(buffer, t));
                        else if (check_identifiers(buffer, t));
                        else return LA_NO_VALID_LITERAL;

                        push_token_stream(TokenStream, t);
                  }
                  push_token_stream(TokenStream, delToken);
                  clear_buffer(&bufferPtr, buffer);

                  printf("delimiter found: \'%s\'\n", delNames[delToken->delimiter]);
            }
      }
}

const int sizeMult = 2;
void push_data(void** dataStream, void* elem, const size_t stride, size_t* size)
{
      int allocatedSize;
      int ptr = 0;

      if (size)
            allocatedSize = (int) pow(2, ceil(log2(*size)));
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
      const char* input = "abc;;def";
      TokenStream TokenStream = create_token_stream();
      char* errorString;

      lexical(input, TokenStream, &errorString);

      Token it = TokenStream->start;
      if (it)
            printf("%s ", tokenTypeNames[it->type]);
      while (next_token(&it))
            printf("%s ", tokenTypeNames[it->type]);

      destroy_token_stream(TokenStream);
      
      return 0;
}