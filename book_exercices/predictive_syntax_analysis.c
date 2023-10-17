#include "stdio.h"

typedef const char* token;

typedef struct
{
      token* tokens;
      token token;
      int index;
} token_iterator;

void advance(token_iterator* it)
{
      it->index++;
      it->token = it->tokens[it->index];
}

void match(token t, token_iterator* lookahead)
{
      if (lookahead->token == t)
            advance(lookahead);
      else
            printf("match error: %s\n", lookahead->token);
}

void simple(token_iterator* lookahead)
{
      if (lookahead->token == "integer")
            match("integer", lookahead);
      else if (lookahead->token == "char")
            match("char", lookahead);
      else if (lookahead->token == "num")
      {
            match("num", lookahead);
            match("..", lookahead);
            match("num", lookahead);
      }
}

void type(token_iterator* lookahead)
{
      if (lookahead->token == "integer" || lookahead->token == "char" || lookahead->token == "num")
      {
            simple(lookahead);
      }
      else if (lookahead->token == "^")
      {
            match("^", lookahead);
            match("id", lookahead);
      }
      else if (lookahead->token == "array")
      {
            match("array", lookahead);
            match("[", lookahead);
            simple(lookahead);
            match("]", lookahead);
            match("of", lookahead);
            type(lookahead);
      }
      else
      {
            printf("type error: %s\n", lookahead->token);
      }
}

int analyse()
{
      token tokenstream[8] = { "array", "[", "num", "..", "num", "]", "of", "integer" };
      token_iterator it;
      it.index = -1;
      it.tokens = tokenstream;
      advance(&it);

      type(&it);

}

int main(int argc, const char** argv)
{
      analyse();
      return 0;
}