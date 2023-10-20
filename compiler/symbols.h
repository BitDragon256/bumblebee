#ifndef SYMBOLS_H
#define SYMBOLS_H

#define KEYWORD_COUNT 4
#define DELIMITER_COUNT 11
#define OPERATOR_COUNT 13

typedef enum {
      ekwNull,
      ekwIf,
      ekwWhile,
      ekwFor,
      ekwIn
} Keyword;

extern const char* kwNames[KEYWORD_COUNT];

typedef enum {
      edNull,
      edSemicolon,
      edComma,
      edDot,
      edSpace,
      edOpenSquareBrace,
      edCloseSquareBrace,
      edOpenParentheses,
      edCloseParentheses,
      edOpenCurlyBrace,
      edCloseBrace,
      edDoubleQuotes
} Delimiter;

extern const char* delNames[DELIMITER_COUNT];

typedef enum {
      eopNull,
      eopAdd,
      eopSubtract,
      eopMultiply,
      eopDivide,
      eopModulo,
      eopAssign,
      eopBitAnd,
      eopBitOr,
      eopBitXor,
      eopAnd,
      eopOr,
      eopXor,
      eopEquals
} BaseOperator;

extern const char* opNames[OPERATOR_COUNT];

#endif