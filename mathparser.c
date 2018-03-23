#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "mathparser.h"

enum error nerr;

MathTree tokenize(char* s)
{
  nerr = NO_ERROR;

  MathTree t = calloc(sizeof (struct StMathTree), strlen(s)+1);
  memset(t, 0, sizeof (struct StMathTree)*(strlen(s)+1));

  size_t i = 0;
  while(*s){
    if (strchr("0123456789.", *s) && *s){
      size_t j = 0;
      char * tmp = malloc(strlen(s+1));
      while (strchr("0123456789.", *s) && *s) {
        tmp[j] = *s;
        j++, s++;
      }
      t[i].type = CONST;
      sscanf(tmp, "%lf", &t[i].u.val);
    } else if (!strncmp(s, "int", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = INT;
    } else if (!strncmp(s, "sqrt", 4)){
      s += 4;
      t[i].type = FUNCTION;
      t[i].u.function = SQRT;
    } else if (!strncmp(s, "abs", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = ABS;
    } else if (!strncmp(s, "exp", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = EXP;
    } else if (!strncmp(s, "log", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = LOG;
    } else if (!strncmp(s, "sinh", 4)){
      s += 4;
      t[i].type = FUNCTION;
      t[i].u.function = SINH;
    } else if (!strncmp(s, "cosh", 4)){
      s += 4;
      t[i].type = FUNCTION;
      t[i].u.function = COSH;
    } else if (!strncmp(s, "tanh", 4)){
      s += 4;
      t[i].type = FUNCTION;
      t[i].u.function = TANH;
    } else if (!strncmp(s, "sin", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = SIN;
    } else if (!strncmp(s, "cos", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = COS;
    } else if (!strncmp(s, "tan", 3)){
      s += 3;
      t[i].type = FUNCTION;
      t[i].u.function = TAN;
    } else if (!strncmp(s, "pi", 2)){
      s += 2;
      t[i].type = CONST;
      t[i].u.function = 3.141592;
    } else {
      switch (*s) {
        case 'x':
          t[i].type = VARIABLE;
          break;
        case 'e':
          t[i].type = CONST;
          t[i].u.val = exp(1);
          break;
        case '+':
          t[i].type = OPERATION;
          t[i].u.operation = ADD;
          break;
        case '-':
          t[i].type = OPERATION;
          t[i].u.operation = SUB;
          break;
        case '*':
          t[i].type = OPERATION;
          t[i].u.operation = MULT;
          break;
        case '/':
          t[i].type = OPERATION;
          t[i].u.operation = DIV;
          break;
        case '%':
          t[i].type = OPERATION;
          t[i].u.operation = MOD;
          break;
        case '^':
          t[i].type = OPERATION;
          t[i].u.operation = POW;
          break;
        case '(':
          t[i].type = LBRACKET;
          break;
        case ')':
          t[i].type = RBRACKET;
          break;
        default:
          nerr = TOKEN_ERROR;
      }
      s++;
    }
    i++;
  }
  return t;
}

MathTree parse(MathTree l)
{
  MathTree t = malloc(sizeof (struct StMathTree));
  memset(t, 0, sizeof (struct StMathTree));

  if (l->type == NO_TOKEN){
    nerr = PARSER_ERROR;
    return NULL;
  }

  bool fullbracket = true;
  bool done = false;

  for (size_t prior = 5; prior > 0; prior--) {
    int depth = 0;
    for (size_t i = 0; l[i].type != NO_TOKEN; i++) {
      if (l[i].type == NO_TOKEN) {
        continue;
      } else if (l[i].type == LBRACKET) {
        depth++;
      } else if (l[i].type == RBRACKET) {
        depth--;
        if (depth < 0) continue;
      } else if (!depth) {
        fullbracket = false;
        if (prior == 5 && l[i].type == OPERATION && l[i].u.operation == ADD) {
          t->type = OPERATION;
          t->u.operation = ADD;
          l[i].type = NO_TOKEN;
          t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 5 && l[i].type == OPERATION && l[i].u.operation == SUB) {
          t->type = OPERATION;
          t->u.operation = SUB;
          l[i].type = NO_TOKEN;
          if(l->type == NO_TOKEN){
            t->c1 = malloc(sizeof (struct StMathTree));
            t->c1->type = CONST;
            t->c1->u.val = 0;
          } else t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 4 && l[i].type == OPERATION && l[i].u.operation == MULT) {
          t->type = OPERATION;
          t->u.operation = MULT;
          l[i].type = NO_TOKEN;
          t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 4 && l[i].type == OPERATION && l[i].u.operation == DIV) {
          t->type = OPERATION;
          t->u.operation = DIV;
          l[i].type = NO_TOKEN;
          t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 4 && l[i].type == OPERATION && l[i].u.operation == MOD) {
          t->type = OPERATION;
          t->u.operation = MOD;
          l[i].type = NO_TOKEN;
          t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 3 && l[i].type == OPERATION && l[i].u.operation == POW) {
          t->type = OPERATION;
          t->u.operation = POW;
          l[i].type = NO_TOKEN;
          t->c1 = parse(l);
          t->c2 = parse(&l[i+1]);
          done = true;
        } else if (prior == 2 && l[i].type == FUNCTION) {
          t->type = FUNCTION;
          t->u.function = l[i].u.function;
          t->c1 = parse(&l[i+1]);
          done = true;
        } else if (prior == 1 && l[i].type == CONST) {
          t->type = CONST;
          t->u.val = l[i].u.val;
          done = true;
        } else if (prior == 1 && l[i].type == VARIABLE) {
          t->type = VARIABLE;
          done = true;
        }
      }
    }
    if (done) break;
  }

  if (fullbracket) {
    t = parse(&l[1]);
  }

  return t;
}

double evaluate(MathTree t, double x)
{
  if (!t) return 0;
  switch (t->type) {
    case CONST:
      return t->u.val;
    case VARIABLE:
      return x;
    case FUNCTION:
      switch (t->u.function) {
        case INT:
          return (int)evaluate(t->c1, x);
        case SQRT:
          return sqrt(evaluate(t->c1, x));
        case ABS:
          return fabs(evaluate(t->c1, x));
        case EXP:
          return exp(evaluate(t->c1, x));
        case LOG:
          return log(evaluate(t->c1, x));
        case SIN:
          return sin(evaluate(t->c1, x));
        case COS:
          return cos(evaluate(t->c1, x));
        case TAN:
          return tan(evaluate(t->c1, x));
        case SINH:
          return sinh(evaluate(t->c1, x));
        case COSH:
          return cosh(evaluate(t->c1, x));
        case TANH:
          return tanh(evaluate(t->c1, x));
      }
    case OPERATION:
      switch (t->u.operation) {
        case ADD:
          return evaluate(t->c1, x) + evaluate(t->c2, x);
        case SUB:
          return evaluate(t->c1, x) - evaluate(t->c2, x);
        case MULT:
          return evaluate(t->c1, x) * evaluate(t->c2, x);
        case DIV:
          return evaluate(t->c1, x) / evaluate(t->c2, x);
        case MOD:
          return fmod(evaluate(t->c1, x), evaluate(t->c2, x));
        case POW:
          return pow(evaluate(t->c1, x), evaluate(t->c2, x));
      }
  }
}
