#include <GL/glut.h>
#include <string.h>

#include "graphic.h"
#include "mathparser.h"

char* expr;

int main(int argc, char *argv[]) {
  expr = malloc(1024);
  memset(expr, 0, 1024);
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Functional mathematic graph");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutReshapeFunc(reshape);
  glutIdleFunc(glutPostRedisplay);
  glutMainLoop();
  return 0;
}
