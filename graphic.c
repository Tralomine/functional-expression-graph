#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "mathparser.h"
#include "graphic.h"

static double width = 800, height = 600;
double zoom = 1;
double offsetx = 0, offsety = 0;
bool menu = true;

MathTree expression = NULL;
extern char* expr;
extern enum error nerr;

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double h = height*zoom/50, w = width*zoom/50;
  glOrtho(0, w, 0, h, 0, 1);
  glTranslated((w)/2-offsetx, (h)/2-offsety, 0);

  double scale;
  if(zoom > 1)
    scale = pow(10, (int)log10(3*zoom));
  else
    scale = pow(10, (int)log10(zoom/3));

  glColor3d(0.6, 0, 0);
  glBegin(GL_LINES);
    for (double i = (int)((-w/2+offsetx-1)/scale)*scale; i < (int)((w/2+offsetx+1)/scale)*scale; i += scale) {
      glVertex2d(i, -h+offsety);
      glVertex2d(i, h+offsety);
    }
    for (double i = (int)((-h/2+offsety-1)/scale)*scale; i < (int)((h/2+offsety+1)/scale)*scale; i += scale) {
      glVertex2d(-w+offsetx, i);
      glVertex2d(w+offsetx, i);
    }
    glColor3d(0.8, 0.8, 0.8);
    glVertex2d(-w+offsetx, 0);
    glVertex2d(w+offsetx, 0);
    glVertex2d(0, -h+offsety);
    glVertex2d(0, h+offsety);
  glEnd();

  glColor3d(0.6, 0.6, 0.6);
  char* tmp = malloc(10);
  for (double i = (int)((-w/2+offsetx-1)/scale)*scale; i < (int)((w/2+offsetx+1)/scale)*scale; i += scale) {
    glRasterPos2f(i, 0);
    if(scale >= 1)
      sprintf(tmp, "%d", (int)i);
    else
      sprintf(tmp, "%lf", i);
    char* s = tmp;
    while(*s!='\0') {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*s++);
    }
  }
  for (double i = (int)((-h/2+offsety-1)/scale)*scale; i < (int)((h/2+offsety+1)/scale)*scale; i += scale) {
    glRasterPos2f(0, i);
    if(scale >= 1)
      sprintf(tmp, "%d", (int)i);
    else
      sprintf(tmp, "%lf", i);
    char* s = tmp;
    while(*s!='\0') {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*s++);
    }
  }

  glColor3d(1, 1, 1);
  glBegin(GL_LINE_STRIP);
    for (double i = (int)((-w/2+offsetx-1)/scale)*scale; i < (int)((w/2+offsetx+1)/scale)*scale; i += 0.01*zoom) {
    // for (double i = -w+offsetx; i < w+offsetx; i+=0.01*zoom){
      glVertex2d(i, evaluate(expression, i));
    }
  glEnd();

  glLoadIdentity();
  char s[strlen(expr)+6];
  *s = 0;
  strcat(s, "f(x)=");
  strcat(s, expr);
  if(nerr)
    glColor3d(1, 0, 0);
  if(!menu)
    glColor3d(0, 1, 1);
  glRasterPos2f(-(width-20)/width, (height-40)/height);
  for (size_t i = 0; s[i] != '\0'; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
  }
  char errormsg[32];
  switch (nerr) {
    case TOKEN_ERROR:
      strcpy(errormsg, "unexpected char");
    case PARSER_ERROR:
      strcpy(errormsg, "invalid expression");
  }
  glRasterPos2f(-(width-20)/width, (height-80)/height);
  for (size_t i = 0; errormsg[i] != '\0'; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, errormsg[i]);
  }

  glFlush();
}

void keyboard(unsigned char c, int x, int y)
{
  switch(c){
    case '+':
      if (!menu) zoom /= 1.2;
      break;
    case '-':
      if (!menu) zoom *= 1.2;
      break;
    case 13:  //enter
      if (!nerr) menu = !menu;
      break;
    case 8:
      if (menu) expr[strlen(expr)-1] = 0;
      break;
    case 4:
    case 27:
      exit(0);
      break;
  }
  if( menu && c != 13 && c != 8) {
    expr[strlen(expr)] = c;
  }

  MathTree tmp = tokenize(expr);
  if(!nerr)
    expression = parse(tmp);

}

void special(int k, int x, int y)
{
  if(!menu){
    switch (k) {
      case GLUT_KEY_DOWN:
      offsety -= zoom;
      break;
      case GLUT_KEY_UP:
      offsety += zoom;
      break;
      case GLUT_KEY_LEFT:
      offsetx -= zoom;
      break;
      case GLUT_KEY_RIGHT:
      offsetx += zoom;
      break;
    }
  }
}

void reshape(int w, int h)
{
  width = w, height = h;
  glViewport(0, 0, width, height);
}
