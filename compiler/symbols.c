#define KEYWORD_COUNT 4
#define DELIMITER_COUNT 11

typedef enum {
      ekwIf,
      ekwWhile,
      ekwFor,
      ekwIn
} keywords;

const char* kwNames[KEYWORD_COUNT] = {
      "if",
      "while",
      "for",
      "in"
};

typedef enum {
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
} delimiters;

const char* delNames[DELIMITER_COUNT] = {
      ";",
      ",",
      ".",
      " ",
      "[",
      "]",
      "(",
      ")",
      "{",
      "}",
      "\""
};

typedef enum {
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
} baseOperators;