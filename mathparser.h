#pragma once

typedef int bool;
#define true 1
#define false 0


struct StMathTree;
typedef struct StMathTree * MathTree;
struct StMathTree
{
  enum type {
    NO_TOKEN,
    CONST,
    VARIABLE,
    OPERATION,
    FUNCTION,
    LBRACKET,
    RBRACKET,
  } type;
  union u {
    enum operation {
      ADD,
      SUB,
      MULT,
      DIV,
      MOD,
      POW,
    } operation;
    enum function {
      INT,
      SQRT,
      ABS,
      EXP,
      LOG,
      SIN,
      COS,
      TAN,
      SINH,
      COSH,
      TANH,
    } function;
    double val;
  } u;
  MathTree c1, c2;
};

enum error {
  NO_ERROR,
  TOKEN_ERROR,
  PARSER_ERROR,
  UNBALANCED_BRACKETS,
};

MathTree tokenize(char* s);
MathTree parse(MathTree l);
double evaluate(MathTree t, double x);
