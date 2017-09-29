#ifndef MODELETOF_H_INCLUDED
#define MODELETOF_H_INCLUDED
    #include "stdio.h"
    #define TAI_BLOC 1024
    #define TAI_TAB_BLOC 126


    /** 1024 div  = 126 => donc taille du tableau == 126*/
    typedef struct TBloc
    {
        long tab[TAI_TAB_BLOC];
        int nb;
    } TBloc;

    typedef struct Entete
    {
        long numLastBloc;
        long nbEnreg;
        long provisoire;
    } Entete;

    typedef struct TOF
    {
        FILE *fich;
        Entete entete;
    } TOF;
    /**Protos---------------------------------------------*/
    int ouvrir_TOF(TOF **f,char *path,char mode);
    void fermer_TOF(TOF *f);
    void lireDir_TOF(TOF *f,long numBloc,TBloc *buffer);
    void ecrireDir_TOF(TOF *f,long numBloc,TBloc *buffer);
    long entete_TOF(TOF *f,int i);
    void aff_entete_TOF(TOF *f,int i,long val);
    long allouerBloc_TOF(TOF *f);
    /**---------------------------------------------------*/

#endif // MODELETOF_H_INCLUDED
