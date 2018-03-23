COMP := gcc
GL := -lGL -lGLU -lglut

all:
	${COMP} -c -Fpic mathparser.c
	${COMP} -c -Fpic graphic.c
	${COMP} -c -Fpic main.c
	${COMP} ${GL} -lm main.o graphic.o mathparser.o -o gef.out

clean:
	rm *.o
