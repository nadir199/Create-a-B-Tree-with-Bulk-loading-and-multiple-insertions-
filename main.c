#include <stdio.h>
#include <stdlib.h>
#include "modeleTOF.h"
#include "BPArbre.h"
#include <time.h>
#include <windows.h>

#define MILLE 1000
#define MILLION 1000000
#define FACTEUR_CHAR_INIT 1

/** STATISTIQUES */
int nombreES=0;

/** NEED POUR ARBRES*/
int CREATED=0;
char nomTOF[256]="";
char nomArbIns[256]="";
char nomArbBulk[256]="";
char ** tableauDeTOF;
char ** tableauDeBulk;
char ** tableauDeIns;
int nBElemTab=4;
int nBElemBulk=0;
int nBElemIns=0;

int nbEnr=0;

TOF *fichTOF=NULL;
BPArbre *fichIns=NULL;
BPArbre *fichBulk=NULL;

void AfficheEnt(BPArbre *f)
{
    int i=1;
    while(i<4)
    {
        printf("\nPrametre %d : %d",i,entete_BPArbre(f,i));
        i++;
    }
}
void Affichage(BPArbre *f,long nbloc)
{
    TNoeuB buf;
    int i=0;
    lireDir_BPArbre(f,nbloc,&buf);
    //gotoxy(1,2);
    printf("|");
    for(i=0;i<buf.nbVals;i++)
    {
        printf("%d|",buf.Val[i]);
    }
    printf("\n");
    for(i=0;i<buf.nbVals+1;i++)
    {
        printf("%d|",buf.Fils[i]);
    }
}
int tavant,tapres;

int Menu();
#define posXMenu 25
#define posYMenu 2
void HandMenu(int *choix,int *sousChoix,int *sousMenu,char *mov,int *justOpenedSousMenu,int *quit);
void DispMenu(int *select,char *mov,int *sousMenu,int *selSous,int *justOpenedSousMenu);
int main()
{
/** INITIALISATIONS*/
    system("mode con LINES=40 COLS=150");
    system("mkdir TOF 2>null");
    system("mkdir \"BPTree Ins\" 2>null");
    system("mkdir \"BPTree Bulk\" 2>null");


    Menu();
    return 0;
}

#define COUL_MENU_FOND 15
#define COUL_MENU_CADRE 15
#define COUL_MENU_TEXT 0
#define FLECHE {color(12,COUL_MENU_FOND);printf("<----");color(3,COUL_MENU_CADRE);printf("%c\n",186);}else {color(0,COUL_MENU_FOND);printf("     ");color(3,COUL_MENU_CADRE);printf("%c\n",186);}
#define nombreMenu 8
#define HAND_COL(x) gotoxy(posXMenu+22,posYMenu+4+x);color(3,COUL_MENU_CADRE);printf("%c",186);color(COUL_MENU_TEXT,COUL_MENU_FOND);if(*select == x) color(12,COUL_MENU_FOND); else color(0,COUL_MENU_FOND);


int tailleSousMenus[nombreMenu]={0};

void DispMenu(int *select,char *mov,int *sousMenu,int *selSous,int *justOpenedSousMenu)
{

    /** PARTIE MENU PRINCIPAL */
    gotoxy(posXMenu+13,posYMenu);printf("              %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    gotoxy(posXMenu+13,posYMenu+1);printf("              %c                    MENU                     %c\n",186,186);
    gotoxy(posXMenu+13,posYMenu+2);printf("              %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n\n",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    color(3,COUL_MENU_CADRE);
    gotoxy(posXMenu+22,posYMenu+3);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    color(0,COUL_MENU_FOND);

    HAND_COL(0)printf("      1 . Creer un fichier TOF                    "); if (*select==0 && !*sousMenu) FLECHE
    HAND_COL(1)printf("      2 . Ouvrir un fichier TOF existant          "); if (*select==1 && !*sousMenu) FLECHE
    HAND_COL(2)printf("      3 . Creer un B+ Arbre par insertions succ   "); if (*select==2 && !*sousMenu) FLECHE
    HAND_COL(3)printf("      4 . Creer un B+ Arbre par Bulk Loading      "); if (*select==3 && !*sousMenu) FLECHE
    HAND_COL(4)printf("      5 . Affichage Bloc par Bloc                 "); if (*select==4 && !*sousMenu) FLECHE
    HAND_COL(5)printf("      6 . Affichage d'un Bloc                     "); if (*select==5 && !*sousMenu) FLECHE

    HAND_COL(6)printf("      7 . Affichage de l'Entete                   "); if (*select==6 && !*sousMenu) FLECHE
    /*HAND_COL(8)printf("      9 . ////////////////////////////////////////"); if (*select==nombreMenu-3 && !*sousMenu) FLECHE
    HAND_COL(9)printf("      10. ////////////////////////////////////////"); if (*select==nombreMenu-2 && !*sousMenu) FLECHE*/
    HAND_COL(nombreMenu-1)printf("      8 . Quitter                                 "); if (*select==nombreMenu-1 && !*sousMenu) FLECHE

    printf("\n");
    color(3,COUL_MENU_CADRE);
    gotoxy(posXMenu+22,posYMenu+4+nombreMenu);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    color(0,COUL_MENU_FOND);

    color(15,0);

    /** PARTIE SOUS MENU */
    if(*sousMenu)
    {
    }

}
char*List_fichier(int *tai,char *repert)
{

     FILE *f=NULL;
     int cpt=0,c;
     char cmd[256]="";
     strcpy(cmd,"dir /b ");
     strcat(cmd,repert);
     strcat(cmd,">wach.txt");
     system(cmd);
     f=fopen("wach.txt","r+");
     char ca[256]="";
     while(!feof(f))
     {
         fgets(ca,256,f);
         cpt++;
     }

     char** tab_fich=malloc(cpt*sizeof(char*));
     int i=0;
     for(i=0;i<cpt;i++)
     {
         tab_fich[i]=malloc(256*sizeof(char));
     }
     rewind(f);
     int cur=0;
     while(!feof(f)&& cur<cpt-1)
     {
         fgets(tab_fich[cur],256,f);
         tab_fich[cur][strlen(tab_fich[cur])-1]='\0';
         cur++;
     }
     fclose(f);
     *tai=cpt-1;
     return tab_fich;
}
void HandMenu(int *choix,int *sousChoix,int *sousMenu,char *mov,int *justOpenedSousMenu,int *quit)
{
        *mov=getch();
        while ( (*mov) == -32 /* && *choix < nombreMenu-1 */)
        {
            *mov=getch();
            if(*mov==80)
            {
                if(!*sousMenu)
                {

                    (*choix)++;
                    if(*choix >nombreMenu-1)
                    {
                        *choix= 0;
                    }
                }
                else
                {
                    (*sousChoix)++;
                    if(*sousChoix>tailleSousMenus[*choix]-1)
                    {
                        *sousChoix=0;
                    }
                }
            }

            if (*mov == 72/* && *choix */)
            {
                if(!*sousMenu)
                {
                    (*choix)--;
                    if(*choix <0)
                    {
                        *choix= nombreMenu-1;
                    }
                }
                else
                {
                    (*sousChoix)--;
                    if(*sousChoix<0)
                        *sousChoix = tailleSousMenus[*choix]-1;
                }
            }

        }

        if(*mov == 13 && tailleSousMenus[*choix])
        {

            if(!*sousMenu )
                *justOpenedSousMenu=1; // POUR SAVOIR QU'ON VIENT JUSTE D'OUVRIR UN SOUS MENU DONC LA PREMIERE OPERATION DU SOUS MENU N'EST PAS VALIDEE DIRECTEMENT
            *sousMenu=1;
            //*sousChoix=0;
        }
        if (toupper(*mov)=='Q'&& *sousMenu)
        {
            *sousChoix=0;
            *sousMenu=0;
            EraseZone(58,43,51,10);
        }
        else if (toupper(*mov) == 'Q')
        {
            *choix = nombreMenu-1;
            *sousMenu=0;
            *mov=13;
        }


        if (*mov==13)
        {
            switch(*choix)
            {
                case 0:
                    color(0,15);
                    char *nomFich=NULL;
                    nomFich=demandeNomFich();
                    strcpy(nomTOF,"TOF\\");
                    strcat(nomTOF,nomFich);
                    gotoxy(2,19);
                    printf("Combien de cles ? :");
                    scanEntier(&nbEnr,2,20,9);
                    ChargementInitial(nomTOF,nbEnr,FACTEUR_CHAR_INIT);
                    /// AJOUT DANS LA TABLE
                    return 0;
                    break;
                case 1:
                    gotoxy(2,20);
                    printf("Quel genre de fichier existant charger TOF(0) insertions(1) Bulk(2) (Sert que pour les petits arbres affichage limite):");
                    int repo=5;
                    while(repo!=1 && repo!=2 && repo!=0)
                    {
                        color(12,0);
                        scanEntier(&repo,2,21,1);
                        color(15,0);
                    }
                    long rac=-1;
                    //system("cls");
                    char **TABCOURANT;
                    int NBELEM=0;
                    if(repo==0)
                    {
                       if(tableauDeTOF!=NULL)
                            free(tableauDeTOF);
                        tableauDeTOF=List_fichier(&nBElemTab,"TOF");
                        TABCOURANT=tableauDeTOF;
                        NBELEM=nBElemTab;
                    }
                    else if(repo==1)
                    {
                        if(tableauDeIns!=NULL)
                            free(tableauDeIns);
                        tableauDeIns=List_fichier(&nBElemIns,"\"BPTree Ins\"");
                        TABCOURANT=tableauDeIns;
                        NBELEM=nBElemIns;
                    }
                    else
                    {
                        if(tableauDeBulk!=NULL)
                            free(tableauDeBulk);
                        tableauDeBulk=List_fichier(&nBElemBulk,"\"BPTree Bulk\"");
                        TABCOURANT=tableauDeBulk;
                        NBELEM=nBElemBulk;
                    }
                    /**FUITES DE MEMOIRE A REVEOIR*/


                    gotoxy(20,20);
                    int menTable=1;
                    int sel=0;
                    char pris=0;
                    int posXT=20;
                    int posYT=20;
                    while(menTable)
                    {
                        color(0,15);
                        int sa=0;
                        for(sa=0;sa<NBELEM;sa++)
                        {
                            gotoxy(posXT,posYT+sa);
                            if (sel==sa) color(0,12); printf("%d . %30s",sa+1,TABCOURANT[sa]); color(0,15);
                        }

                        pris=getch();

                        if(pris==13)
                        {
                            menTable=0;
                            //charger le nomfich.
                            if(repo==0)
                            {
                                strcpy(nomTOF,"TOF\\");
                                strcat(nomTOF,TABCOURANT[sel]);
                            }
                            else if (repo==1)
                            {
                                strcpy(nomArbIns,"BPTree Ins\\");
                                strcat(nomArbIns,TABCOURANT[sel]);
                            }
                            else
                            {
                                strcpy(nomArbBulk,"BPTree Bulk\\");
                                strcat(nomArbBulk,TABCOURANT[sel]);
                            }

                            system("pause");

                        }
                        while ( pris == -32 /* && *choix < nombreMenu-1 */)
                        {
                            pris=getch();
                            if(pris==80)
                            {
                                sel++;
                            }

                            if (pris==72)
                            {
                                sel--;
                            }

                            if(sel<0)
                                sel=NBELEM-1;
                            if(sel>NBELEM-1)
                                sel=0;
                        }
                        color(15,0);
                        EraseZone(posXT,posYT,70,NBELEM);
                    }
                    system("cls");
                    break;
                case 2:
                        ouvrir_TOF(&fichTOF,nomTOF,'a');
                        char *provi=demandeNomFich();
                        strcpy(nomArbIns,"BPTree Ins\\");
                        strcat(nomArbIns,provi);
                        printf("|%s|%s|",nomTOF,nomArbIns);
                        system("pause");
                        ouvrir_BPArbre(&fichIns,nomArbIns,'n');
                        tavant=clock();
                        long nombreBlocs=entete_TOF(fichTOF,1);

                        int pas=entete_TOF(fichTOF,2)/100;
                        int pourc=pas;
                        int p=0;
                        int compte=0;
                        int parcBlocs=1,parcEnr=0;
                        TBloc bufTOF;
                        for(parcBlocs=1;parcBlocs<=nombreBlocs;parcBlocs++)
                        {
                            lireDir_TOF(fichTOF,parcBlocs,&bufTOF);
                            parcEnr=0;
                            while(parcEnr<bufTOF.nb)
                            {
                                Insertion_BPArbre(fichIns,bufTOF.tab[parcEnr]);
                                parcEnr++;
                                compte++;
                                if(compte==pourc)
                                {
                                    pourc+=pas;
                                    gotoxy(4,5);
                                    p++;
                                    color(12,0);
                                    printf("%d",p);
                                    color(15,0);
                                    printf("%%");

                                }
                            }
                        }
                        tapres=clock();

                        printf("\nL'insertion a pris %d ms  et la profondeur %d ",tapres-tavant,entete_BPArbre(fichIns,3));
                        fermer_TOF(fichTOF);
                        fermer_BPArbre(fichIns);
                    break;
                case 3:
                        gotoxy(2,20);
                        char *pro=demandeNomFich();
                        strcpy(nomArbBulk,"BPTree Bulk\\");
                        strcat(nomArbBulk,pro);
                        int av=clock();
                        BulkLoad_BPArbre(nomArbBulk,nomTOF);
                        int ap=clock();
                        printf("Le Bulk Loading a pris  %d ms soit %d seconde(s)",ap-av,(ap-av)/1000);
                    break;
                case 4:
                    gotoxy(2,20);
                    printf("Quel arbre afficher insertions(1) Bulk(2):");
                    int rep=0;
                    long nb;
                    long google=0;
                    while(rep!=1 && rep!=2)
                    {
                        color(12,0);
                        scanEntier(&rep,2,21,1);
                        color(15,0);
                    }


                    system("cls");
                    if(rep==1)
                    {
                        ouvrir_BPArbre(&fichIns,nomArbIns,'a');
                        long nbBlocs=entete_BPArbre(fichIns,2);
                        long i=1;

                        for(i=1;i<nbBlocs-1;i++)
                        {
                            gotoxy(0,10);
                            Affichage(fichIns,i);
                            system("pause");
                            system("cls");
                        }
                        fermer_BPArbre(fichIns);
                    }
                    else
                    {
                        ouvrir_BPArbre(&fichBulk,nomArbBulk,'a');
                        nb =entete_BPArbre(fichBulk,2);
                        google=1;
                        system("pause");

                        while(google <= nb )
                        {
                            if(google==nb-1)
                                break;
                            gotoxy(0,10);
                            Affichage(fichBulk,google);
                            system("pause");
                            system("cls");


                            google++;
                        }

                        fermer_BPArbre(fichBulk);
                    }
                    system("pause>null");
                    system("cls");

                    break;
                /*case 5:
                    gotoxy(2,20);
                    printf("Quel arbre afficher insertions(1) Bulk(2) (Sert que pour les petits arbres affichage limite):");
                    int repo=0;
                    while(repo!=1 && repo!=2)
                    {
                        color(12,0);
                        scanEntier(&repo,2,21,1);
                        color(15,0);
                    }
                    long rac=-1;
                    system("cls");
                    if(repo==1)
                    {
                        ouvrir_BPArbre(&fichIns,nomArbIns,'a');
                        AffInord_BPArbre(fichIns,entete_BPArbre(fichIns,1));
                        fermer_BPArbre(fichIns);
                    }
                    else
                    {
                        ouvrir_BPArbre(&fichBulk,nomArbBulk,'a');
                        printf("%d\n",entete_BPArbre(fichBulk,1));
                        AfficheEnt(fichBulk);
                        system("pause");
                        rac=entete_BPArbre(fichBulk,1);
                        AffInord_BPArbre(fichBulk,rac);
                        fermer_BPArbre(fichBulk);
                    }


                    break;*/
                case 5:
                    printf("Quel arbre utiliser insertions(1) Bulk(2):");
                    int reponse=0;
                    while(reponse!=1 && reponse!=2)
                    {
                        color(12,0);
                        scanEntier(&reponse,2,21,1);
                        color(15,0);
                    }


                    system("cls");
                    if(reponse==1)
                    {
                        gotoxy(2,20);
                        printf("Quel bloc afficher (numero)");
                        int affi;
                        color(12,0);
                        scanEntier(&affi,3,21,20);
                        color(15,0);
                        gotoxy(0,25);
                        ouvrir_BPArbre(&fichIns,nomArbIns,'a');
                        Affichage(fichIns,affi);
                        fermer_BPArbre(fichIns);
                    }
                    else
                    {
                        gotoxy(2,20);
                        printf("Quel bloc afficher (numero)");
                        int affi;
                        color(12,0);
                        scanEntier(&affi,3,21,20);
                        color(15,0);
                        gotoxy(0,25);
                        ouvrir_BPArbre(&fichBulk,nomArbBulk,'a');
                        Affichage(fichBulk,affi);
                        fermer_BPArbre(fichBulk);
                    }

                        system("pause>null");
                        system("cls");


                    break;
                case 6:
                    printf("Quel arbre utiliser insertions(1) Bulk(2):");
                    int resp=0;
                    while(resp!=1 && resp!=2)
                    {
                        color(12,0);
                        scanEntier(&resp,2,21,1);
                        color(15,0);
                    }


                    system("cls");
                    if(resp==1)
                    {
                        gotoxy(20,25);
                        ouvrir_BPArbre(&fichIns,nomArbIns,'a');
                        AfficheEnt(fichIns);
                        fermer_BPArbre(fichIns);
                    }
                    else
                    {
                        gotoxy(20,25);
                        ouvrir_BPArbre(&fichBulk,nomArbBulk,'a');
                        AfficheEnt(fichBulk);
                        fermer_BPArbre(fichBulk);
                    }
                    system("pause>null");
                    system("cls");

                    break;
                case nombreMenu-1:
                    *quit=1;
                    system("cls");
                    break;
            }
        }
        color(15,0);
}

int Menu()
{
    //system("color F0");

    static int quit =0;
    static int select='\0';
    static char mov='\0';
    static int sousMenu=0;
    static int selSous=0;
    static int justOpenedSousMenu=0;
    while(!quit)
    {
        DispMenu(&select,&mov,&sousMenu,&selSous,&justOpenedSousMenu);
        HandMenu(&select,&selSous,&sousMenu,&mov,&justOpenedSousMenu,&quit);
    }

    color(15,0);

}
