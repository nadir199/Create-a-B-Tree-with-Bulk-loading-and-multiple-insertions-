#include "display.h"
void ClearChaine(char *chaine,int longu)
{
    int i=0;
    for(i=0;i<longu;i++)
        chaine[i]='\0';
}
void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}
void gotoxy (int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!SetConsoleCursorPosition(hConsole, pos))
    {
        printf("Erreur dans le GOTOXY CURSOR POS SET");
    }
}
void DrawBox(int x,int y,int sizeX,int sizeY)
{
    int i=x;
    gotoxy(x,y);printf("%c",201);
    for(i=x+1;i<x+sizeX;i++)  printf("%c",205);
    printf("%c",187);

    gotoxy(x,y+sizeY);printf("%c",200);
    for(i=x+1;i<x+sizeX;i++)  printf("%c",205);
    printf("%c",188);

    for(i=y+1;i<y+sizeY;i++)
    {
        gotoxy(x,i);
        printf("%c",186);
        gotoxy(x+sizeX,i);
        printf("%c",186);
    }
    gotoxy(x+1,y+1);printf("%c",177);
    gotoxy(x+1,y+sizeY-1);printf("%c",177);
    gotoxy(x+sizeX-1,y+1);printf("%c",177);
    gotoxy(x+sizeX-1,y+sizeY-1);printf("%c",177);
}
void EraseZone(int x,int y,int sizeX,int sizeY)
{
    int i=x,j=y;
    for(j=y;j<y+sizeY;j++)
    {
        i=x;
        gotoxy(i,j);
        for(i=x;i<x+sizeX;i++)
        {
            printf(" ");
        }
    }
}

/** AUTRE*/
int lireAlphaNum(char *result, int longueur,int x,int y)
{
    char chaine[256];
    char c;
    int i=0;

    do
    {
        c=getch(); //RECUPERE UN CARACTERE
        if (i<longueur && ( ( c>='0' && c<='9')||(c>='a' && c<='z')||(c>='A' && c<='Z') ) ) // SI ON A PAS DEPASSE LA LONGUEUR
        {
            result[i]=c; // ON STOCKE LA CHAINE
            printf("%c",result[i]); // ON AFFICHE LE CARACTERE POUR SIMULER L'ECRITURE
            if(result[0]=='\n') // TRAITER LE CAS DU PREMIER CARACTERE \n POUR EVITER LE SAUT
            {
                gotoxy(x,y);//SE BRANCHER A LA POSITION D'ECRITURE
                return 0;
            }
            i++; // ON INCREMENTE LA LONGUEUR
        }
        if(c==8) // POUR EFFACER (BACK SPACE)
        {
            printf("%c",8);
            result[i]='\0';
            EraseZone(x+i-1,y,1,1); // ON EFFACE LE CARACTeRE A EFFACER
            i--; // ON DECREMENTE LA LONGUEUR
            if(i<0) //TRAITER LE CAS CHAINE NULLE
                i=0;
            gotoxy(x+i,y);// ON SE BRANCHE
        }
        //}

    }while (( (c!= NULL && c!=13 )) || i==0 );
    result[i]='\0'; // FIN DE CHAINE

    return 1;
}

void scanAlphaNum(char *retour ,int x,int y,int sizeX)
{
    int n;
    do
    {
        //EraseZone(x,y,sizeX,1);
        if(x>0 && y>0);
        gotoxy(x,y);
        fflush (stdout);
        n = lireAlphaNum(retour,sizeX,x,y);//scanf ("%d", retour);
        //clean();
    }while (n != 1);
}

void printAnim(char *chaine,int delay)
{
    int i;
    for(i=0;i<strlen(chaine);i++)
    {
        printf("%c",chaine[i]);
        Sleep(delay);
        if( _kbhit())
            getch();
    }
}
int yesNoQuestion(char *ch,int xPlus)
{
    int reprendre=1;
    char reponse;
    DrawBox(25,10,100,10);
    gotoxy(30+xPlus,15);
    printAnim(ch,5);
    gotoxy(35+xPlus,25);
    do
    {

        gotoxy(73,17); if(reprendre) color(12,15);printf("OUI");color(0,15);
        gotoxy(73,18); if(!reprendre) color(12,15);printf("NON");color(0,15);
        reponse=getch();
        while ( (reponse) == -32 /* && *select < nombreMenu-1 */)
        {
            reponse=getch();
            if(reponse==80)
            {
                    reprendre++;
                    if(reprendre >1)
                    {
                        reprendre= 0;
                    }

            }

            if (reponse == 72/* && *select */)
            {
                   reprendre--;
                    if(reprendre <0)
                    {
                        reprendre= 1;
                    }
            }

        }
    }while(reponse!=13);
    color(15,0);
    EraseZone(0,0,200,30);
    return reprendre;
}
void InitChaine(char *chaine,int longu,const char *input)
{
    int i=0;
    if(longu>=strlen(input))
        for(i=0;i<strlen(input);i++)
            chaine[i]=input[i];
        chaine[i]='\0';
}
char* demandeNomFich()
{
     system("cls");
    char chemin[256]="Results/";
    //char nomFichier[256];
    char *nomFichier = malloc(256*sizeof(char));
    char nomExtension[256];
    char quest[256]="Nom choisi \"";
 // ON CREE LE REPERTOIRE QUI CONTIENDRA LES FICHIERS RESULTATS ET ON VERIEFIE QUE LA CREATION S'EST BIEN DEROULEE
    if(yesNoQuestion("Choisissez le nom de votre fichier",5) )
    {
        system("mkdir Results 2> nul");

        do
        {
            InitChaine(quest,256,"Nom choisi : \"");
            InitChaine(chemin,256,"Results/");
            system("cls");
            gotoxy(20,5);
            printf("Entrez le nom du fichier? (Alpha.Num sans Extension)");
            color(2,15);gotoxy(40,6);scanAlphaNum (nomFichier,45,6,20);color(0,15);

            gotoxy(20,9);
            printf("Donnez l'extension");
            color(2,15);gotoxy(40,10);scanAlphaNum (nomExtension,45,10,20);color(0,15);
            strcat(nomFichier,".");
            strcat(nomFichier,nomExtension);
            strcat(chemin,nomFichier);
            strcat(quest,nomFichier);
            strcat(quest,"\" (Valider?)");
        }while(!yesNoQuestion(quest,3));
    }
     //EraseZone(15,5,NB_CARAC_LIG_MAX+3,NB_LIG_PAGE_MAX);
     return nomFichier;
}

int lireNb(int *retour, int longueur,int x,int y)
{
    char chaine[256];
    char c;
    int i=0;
    *retour=0;

    do
    {
        c=getch(); //RECUPERE UN CARACTERE
        if (i<longueur && c>='0' && c<='9') // SI ON A PAS DEPASSE LA LONGUEUR
        {
            chaine[i]=c; // ON STOCKE LA CHAINE
            printf("%c",chaine[i]); // ON AFFICHE LE CARACTERE POUR SIMULER L'ECRITURE
            if(chaine[0]=='\n') // TRAITER LE CAS DU PREMIER CARACTERE \n POUR EVITER LE SAUT
            {
                gotoxy(x,y);//SE BRANCHER A LA POSITION D'ECRITURE
                return 0;
            }
           // if (!(chaine[i]>='0' && chaine[i]<='9'))// SI LE CARACT EST UN CHIFFRE ERREUR
               // return 0;
            *retour=(*retour)*10+chaine[i]-'0'; // ON COMPOSE DE GAUCHE A DROITE
            i++; // ON INCREMENTE LA LONGUEUR
        }
         if(c==8) // POUR EFFACER (BACK SPACE)
        {
            printf("%c",8);
            (*retour)=(*retour)/10;
            chaine[i]='\0';
            EraseZone(x+i-1,y,1,1); // ON EFFACE LE CARACTeRE A EFFACER
            i--; // ON DECREMENTE LA LONGUEUR
            if(i<0) //TRAITER LE CAS CHAINE NULLE
                i=0;
            gotoxy(x+i,y);// ON SE BRANCHE
        }
        //}

    }while (( (c!= NULL && c!=13 )) || i==0 );

    return 1;
}

void scanEntier(int *retour ,int x,int y,int sizeX)
{
    int n;
    do
    {
        //EraseZone(x,y,sizeX,1);
        if(x>0 && y>0);
        gotoxy(x,y);
        fflush (stdout);
        n = lireNb(retour,sizeX,x,y);//scanf ("%d", retour);
        //clean();
    }while (n != 1);
}
