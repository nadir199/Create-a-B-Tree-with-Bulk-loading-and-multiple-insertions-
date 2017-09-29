#ifndef BPARBRE_H_INCLUDED
#define BPARBRE_H_INCLUDED
#include <stdio.h>
#include "modeleTOF.h"

    #define NB_VALS_MAX 126
    #define PROF_MAX 1000
    #define NIL -1
    typedef struct TNoeuB
    {
        int Val[NB_VALS_MAX]; /// contient les clés seulement peut être mettre @Tenr
        long Fils[NB_VALS_MAX+1];
        long nbVals;
        long suiv;
    } TNoeuB;

    typedef struct EnteteBP
    {
        long racine;
        long numLastBloc;
        long prof;
    } EnteteBP;

    typedef struct pile
    {
        long sommet;
        long nbloc[PROF_MAX];
        long nBlocTqFils[PROF_MAX];
        TNoeuB noeud[PROF_MAX];
    } TBPile;

    typedef struct BPArbre
    {
        FILE *fich;
        EnteteBP entete;
        TBPile pile;
    } BPArbre;

    /// Fonctions @BPArbre
    int ouvrir_BPArbre(BPArbre **f,char *path,char mode);
    void fermer_BPArbre(BPArbre *f);
    void lireDir_BPArbre(BPArbre *f,long numBloc,TNoeuB *buffer);
    void ecrireDir_BPArbre(BPArbre *f,long numBloc,TNoeuB *buffer);
    long entete_BPArbre(BPArbre *f,int i);
    void aff_entete_BPArbre(BPArbre *f,int i,long val);
    long allouerBloc_BPArbre(BPArbre *f);

    /// Fonctions @Pile
    void CreerPile_TNoeuB(TBPile *p);
    int PileVide_TNoeuB(TBPile p);
    int PilePleine_TNoeuB(TBPile p);
    void Empiler_TNoeuB(TBPile *p,TNoeuB buf,long i,long j);
    void Depiler_TNoeuB(TBPile *p,TNoeuB *buf,long *i,long *j);
/**********************************************************************/
/**                     PARTIE ALGORITHMES                           **/
/**********************************************************************/

    void Recherche_BPArbre(BPArbre *f,long cle,int *Trouv,int *i,int *j,int *prec);
    /**DECALAGES*/
    void DecalVal(TNoeuB *buf,long posDeb,long valIns);
    void DecalFils(TNoeuB *buf,long posDeb,long nouveaune);
    /**Insertion dans un bloc*/
    void insere_in_bloc_BP(TNoeuB *buf,long cle,long fd);
    void SplitNoeuB(long cle,long fd,TNoeuB* buffer,long *autNum,int *v,BPArbre *f,long profact);
    void Insertion_BPArbre(BPArbre *f,long cle);

    void AffInord_BPArbre(BPArbre *arb,long rac);

#endif // BPARBRE_H_INCLUDED
