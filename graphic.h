#pragma once

/*
 * @File graphic.h
 *
 *
 *
 * contient les définitions des fonctions implémentés dans graphic.c
 */


void display(void);
void keyboard(unsigned char c, int x, int y);
void special(int k, int x, int y);
void reshape(int w, int h);
