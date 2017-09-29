#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void ClearChaine(char *chaine,int longu);
void color(int t,int f);
void gotoxy (int x, int y);
void DrawBox(int x,int y,int sizeX,int sizeY);
void EraseZone(int x,int y,int sizeX,int sizeY);

/** AUTRE*/

int lireAlphaNum(char *result, int longueur,int x,int y);

void scanAlphaNum(char *retour ,int x,int y,int sizeX);

void printAnim(char *chaine,int delay);
int yesNoQuestion(char *ch,int xPlus);
void InitChaine(char *chaine,int longu,const char *input);
char* demandeNomFich();

int lireNb(int *retour, int longueur,int x,int y);
void scanEntier(int *retour ,int x,int y,int sizeX);
#endif // DISPLAY_H_INCLUDED
