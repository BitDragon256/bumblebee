#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <stdint.h>

#include "symbols.h"

typedef enum
{
      eKeyword = 0,
      eIdentifier = 1,
      eDelimiter = 2,
      eOperator = 3,
      eNumber = 4,
      eNullToken = 5
} TokenType;

// FOR DEBUGGING
extern const char* tokenTypeNames[6];

struct Token_T
{
      char* identifier;
      Keyword keyword;
      Delimiter delimiter;
      BaseOperator operator;
      int64_t number;
      TokenType type;
      struct Token_T* nextToken;
};
typedef struct Token_T* Token;

Token create_token();
void assign_token(Token dst, const Token src);
void destroy_token(Token token);

struct TokenStream_T
{
      Token start;
      Token end;
};
typedef struct TokenStream_T* TokenStream;

TokenStream create_token_stream();
void destroy_token_stream(TokenStream stream);

int push_token_stream(TokenStream stream, Token elem);
int next_token(Token* t);

#endif