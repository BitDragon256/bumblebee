#include "tokenStream.h"

#include <stdlib.h>

// FOR DEBUGGING
const char* tokenTypeNames[6] = {
      "keyword", "identifier", "delimiter", "operator", "number", "nulltoken"
};

Token create_token()
{
      Token token = malloc(sizeof(struct Token_T));

      token->identifier = "";
      token->keyword = ekwNull;
      token->delimiter = edNull;
      token->operator = eopNull;
      token->number = 0;

      token->type = eNullToken;
      token->nextToken = NULL;

      return token;
}
void assign_token(Token dst, Token src)
{
      dst->identifier = src->identifier;
      dst->keyword = src->keyword;
      dst->delimiter = src->delimiter;
      dst->operator = src->operator;
      dst->number = src->number;

      dst->type = src->type;
      dst->nextToken = src->nextToken;
}
void destroy_token(Token token)
{
      if (token)
            free(token);
}

TokenStream create_token_stream()
{
      TokenStream stream = malloc(sizeof(TokenStream));
      stream->start = NULL;
      stream->end = NULL;
      return stream;
}
void destroy_token_stream(TokenStream TokenStream)
{
      if (!TokenStream)
            return;
      
      Token it = TokenStream->start;
      Token last;

      while (it && next_token(&it))
      {
            last = it;
            it = it->nextToken;
            destroy_token(last);
      }
      destroy_token(TokenStream->start);
      free(TokenStream);
}

int push_token_stream(TokenStream stream, Token token)
{
      if (!stream)
            return 0;
      
      if (stream->start == NULL)
      {
            stream->start = token;
            stream->end = token;
      }
      else
      {
            stream->end->nextToken = token;
            stream->end = token;
      }
      return 1;
}
int next_token(Token* t)
{
      if (!(*t))
            return 0;
      if ((*t)->nextToken)
      {
            *t = (*t)->nextToken;
            return 1;
      }
      return 0;
}
