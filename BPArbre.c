#include "BPArbre.h"

int ouvrir_BPArbre(BPArbre **f,char *path,char mode)
{
    *f=malloc(sizeof(BPArbre));
    char modeOuv[3];

    if((mode == 'A') || (mode == 'a'))
        sprintf(modeOuv,"rb+"); // LECT ECR BINAIRE
    else if ((mode == 'N') || (mode == 'n'))
        sprintf(modeOuv,"wb+"); // LECT ECR BINAIRE NEW FICHIER
    else
        return 0; // AUTRE VALEUR ERREUR

    /// Ouverture du fichier spécifié
    (*f)->fich = fopen(path, modeOuv);

    ///fail ouverture return 0
    if((*f)->fich == NULL)
        return 0;

    /// Si ancien on récupère l'entete
    if((mode == 'A') || (mode == 'a'))
    {
        fread(&((*f)->entete), sizeof(EnteteBP), 1, (*f)->fich);
    }
    else if ((mode == 'N') || (mode == 'n')) /// si nouveau on initialise
    {
        (*f)->entete.numLastBloc = 0;
        (*f)->entete.racine = NIL;
        (*f)->entete.prof = 0;
        CreerPile_TNoeuB(&(*f)->pile);
    }
    return 1;
}
void fermer_BPArbre(BPArbre *f)
{
    //Sauvegarde de l'entête
    rewind(f->fich);
    fwrite(&(f->entete), sizeof(EnteteBP), 1, f->fich);
    fclose(f->fich);
    free(f);
}
void lireDir_BPArbre(BPArbre *f,long numBloc,TNoeuB *buffer)
{
    if(numBloc<= (f->entete).numLastBloc)
    {
        fseek(f->fich,(long)sizeof(EnteteBP) + (numBloc-1)*(long)sizeof(TNoeuB) , SEEK_SET);
        fread(buffer,1,sizeof(TNoeuB),f->fich);
    }
}
void ecrireDir_BPArbre(BPArbre *f,long numBloc,TNoeuB *buffer)
{
    if(numBloc<= (f->entete).numLastBloc)
    {
        fseek(f->fich,(long)sizeof(EnteteBP) + (numBloc-1)*(long)sizeof(TNoeuB) , SEEK_SET);
        fwrite(buffer,1,sizeof(TNoeuB),f->fich);
    }
    //printf("\n Le : numBloc : %d  sizeBloc: %d  POSITION: %d  MAIS : %d",numBloc,sizeof(TNoeuB),sizeof(Entete) + (numBloc-1)*sizeof(TNoeuB),ftell(f->fich));

}
long entete_BPArbre(BPArbre *f,int i)
{
    if(i == 1) return (f->entete).racine;
    else if(i == 2) return (f->entete).numLastBloc;
    else if(i == 3) return (f->entete).prof;
    else return -1;
}
void aff_entete_BPArbre(BPArbre *f,int i,long val)
{
    if(i == 1) (f->entete).racine = val;
    else if(i == 2) (f->entete).numLastBloc = val;
    else if(i == 3) (f->entete).prof = val;
}
long allouerBloc_BPArbre(BPArbre *f)
{
    aff_entete_BPArbre(f,2,entete_BPArbre(f,2)+1);
    return entete_BPArbre(f,2);
}


/// Fonctions @Pile
void CreerPile_TNoeuB(TBPile *p)
{
    p->sommet = NIL;
}

int PileVide_TNoeuB(TBPile p)
{
    return p.sommet == NIL;
}
int PilePleine_TNoeuB(TBPile p)
{
    return p.sommet == PROF_MAX;
}
void Empiler_TNoeuB(TBPile *p,TNoeuB buf,long i,long j)
{
    if(!PilePleine_TNoeuB(*p))
    {
        (p->sommet)++;
        p->nbloc[p->sommet]=i;
        p->nBlocTqFils[p->sommet]=j;
        p->noeud[p->sommet]=buf;
    }
}
void Depiler_TNoeuB(TBPile *p,TNoeuB *buf,long *i,long *j)
{
    if (!PilePleine_TNoeuB(*p))
    {
        *buf=p->noeud[p->sommet];
        *i=p->nbloc[p->sommet];
        *j=p->nBlocTqFils[p->sommet];
        (p->sommet)--;
    }
}


/** FONCTIONS SUR LES B+ ARBRES */

///RECHERCHE
void Recherche_BPArbre(BPArbre *f,long cle,int *Trouv,int *i,int *j,int *prec)
{
    /// déclarations
    int profisoire=0;
    TNoeuB buffer;

    /// initialisations
    *i=entete_BPArbre(f,1);
    *j=0;
    *prec=-1;
    *Trouv=0;
    profisoire=0;
    if (*i==-1) /// SI l'arbre est vide on retourne avec trouv == faux , i =-1 ,prec = -1 et j=0
        return;
    /// DESCENDRE JUSQUA LA FEUILLE (ON LA PARCOURT PAS ON SORT AVEC @i correspondant à la feuille

    while(profisoire<entete_BPArbre(f,3))
    {
        lireDir_BPArbre(f,*i,&buffer);
        *j=0;
        while(*j<buffer.nbVals && buffer.Val[*j]<cle)
        {
            (*j)++;
        }
        Empiler_TNoeuB(&(f->pile),buffer,*i,*j);
        *prec=*i;
        *i=buffer.Fils[*j];
        profisoire++;
    }
    /// RECHERCHE SEQUENTIELLE DANS LA FEUILLE
    *j=0;
    lireDir_BPArbre(f,*i,&buffer);
    while(*j<buffer.nbVals && buffer.Val[*j]<cle && !*Trouv)
    {
        if (buffer.Val[*j]==cle)
            *Trouv=1;
        else
            (*j)++;

    }
}

/// INSERTION DANS UN BLOC
    //DECALAGES
void DecalVal(TNoeuB *buf,long posDeb,long valIns)
{
    int i=0;
    if(buf->nbVals<NB_VALS_MAX)
    {
        //printf("LAFONC %d %d",buf->nbVals,posDeb);
        for(i=buf->nbVals; i>posDeb;i--)
        {
            buf->Val[i]=buf->Val[i-1];
        }
        buf->Val[posDeb]=valIns;
    }
}

void DecalFils(TNoeuB *buf,long posDeb,long nouveaune)
{
    int i=0;
    if(buf->nbVals<NB_VALS_MAX)
    {
        for(i=buf->nbVals+1; i>posDeb;i--)
        {
            buf->Fils[i]=buf->Fils[i-1];
        }
        buf->Fils[posDeb]=nouveaune;
    }
}

void insere_in_bloc_BP(TNoeuB *buf,long cle,long fd)
{
    if(buf->nbVals <NB_VALS_MAX)
    {
        int i=0;
        while(i<buf->nbVals && buf->Val[i]<cle)
        {
            i++;
        }
        DecalVal(buf,i,cle);
        DecalFils(buf,i+1,fd);
        (buf->nbVals)++;
    }
}

/// Eclate un noeud en insérant la valeur @cle et renvoie la valeur qui monte @v
void SplitNoeuB(long cle,long fd,TNoeuB* buffer,long *autNum,int *v,BPArbre *f,long profact)
{
    int gauche=0,deb=0;
    long j=0,i=0;
    TNoeuB *Autre;
    *autNum=-1;
    /// pour equilibrer les noeuds comme il faut, on saura si on ajoute la cle au 1er ou 2e NoeuB
    if(buffer->Val[buffer->nbVals/2] > cle)
    {
        deb=buffer->nbVals/2;
        gauche=1;
    }
    else
        deb=buffer->nbVals/2+1;

    *autNum=allouerBloc_BPArbre(f);
    Autre=malloc(sizeof(TNoeuB));
    int prec=0;
    for(prec=0;prec<NB_VALS_MAX+1;prec++)
    {
        Autre->Fils[prec]=NIL;
    }

    for(i=deb;i<buffer->nbVals;i++)
    {
        Autre->Val[j]=buffer->Val[i];
        Autre->Fils[j]=buffer->Fils[i];
        j++;
    }
    Autre->Fils[j]=buffer->Fils[i];

    Autre->nbVals=j;
    buffer->nbVals-=j;
    buffer->Fils[buffer->nbVals]=NIL;

    if (gauche)
        insere_in_bloc_BP(buffer,cle,fd);
    else
        insere_in_bloc_BP(Autre,cle,fd);

    ecrireDir_BPArbre(f,*autNum,Autre);
    *v=buffer->Val[buffer->nbVals-1];
    if(profact!=entete_BPArbre(f,3))
    {
        (buffer->nbVals)--;
    }
    free(Autre);
}

void Insertion_BPArbre(BPArbre *f,long cle)
{
    int trouv,i,j,prec;
    TNoeuB buffer;
    for(prec=0;prec<NB_VALS_MAX+1;prec++)
    {
        buffer.Fils[prec]=NIL;
    }

    f->pile.sommet=-1;
    Recherche_BPArbre(f,cle,&trouv,&i,&j,&prec);
    //printf("\n SOMMET %d , POS %d %d",f->pile.sommet,i,j);
    if (!trouv && i!=NIL) // Donc si la valeur n'existe pas et l'arbre n'es pas vide
    {
        lireDir_BPArbre(f,i,&buffer);
        if(buffer.nbVals < NB_VALS_MAX) // si le buffer FEUILLE n'est pas plein
        {
            //printf("\nAJOUT\n");
            DecalVal(&buffer,j,cle);
            (buffer.nbVals)++;
            ecrireDir_BPArbre(f,i,&buffer);
        }
        else // Donc S'il est plein
        {

            long autNum=-1;
            long refoulee=0;
            TNoeuB Pere;
            long numPere;
            long noeudTransition=0;
            int stop=0;
            long profactu=entete_BPArbre(f,3);
            do
            {
                SplitNoeuB(cle,autNum,&buffer,&autNum,&refoulee,f,profactu);
                ecrireDir_BPArbre(f,i,&buffer);
                prec=i;
                if(!PileVide_TNoeuB(f->pile))
                    Depiler_TNoeuB(&(f->pile),&buffer,&i,&noeudTransition);
                else
                {

                    buffer.nbVals=1;
                    buffer.Val[0]=refoulee;
                    buffer.Fils[0]=i;
                    buffer.Fils[1]=autNum;
                    i=allouerBloc_BPArbre(f);
                    aff_entete_BPArbre(f,1,i);
                    ecrireDir_BPArbre(f,i,&buffer);
                    aff_entete_BPArbre(f,3,entete_BPArbre(f,3)+1);
                    return;
                }

                if(buffer.nbVals!=NB_VALS_MAX)
                {
                    //printf("AUUUUT %d",autNum);
                    insere_in_bloc_BP(&buffer,refoulee,autNum);
                    /*DecalVal(&buffer,noeudTransition,refoulee);
                    DecalFils(&buffer,noeudTransition+1,autNum);*/
                    ecrireDir_BPArbre(f,i,&buffer);
                    stop=1;
                }
                else if (PileVide_TNoeuB(f->pile))
                {
                    //printf("RAC PLEINE ");
                    SplitNoeuB(refoulee,autNum,&buffer,&autNum,&refoulee,f,profactu);
                    ecrireDir_BPArbre(f,i,&buffer);
                    //
                    buffer.nbVals=1;
                    buffer.Val[0]=refoulee;
                    buffer.Fils[0]=i;
                    buffer.Fils[1]=autNum;
                    i=allouerBloc_BPArbre(f);
                    aff_entete_BPArbre(f,1,i);
                    ecrireDir_BPArbre(f,i,&buffer);
                    aff_entete_BPArbre(f,3,entete_BPArbre(f,3)+1);
                }

                cle=refoulee;
                profactu--;
                //printf("COMPARAISON %d == %d",NB_VALS_MAX,buffer.nbVals);
            }while(!PileVide_TNoeuB(f->pile) && buffer.nbVals==NB_VALS_MAX && !stop );
        }
    }
    else if(i==-1) // la racine n'existe pas
    {
        int root=allouerBloc_BPArbre(f);
        buffer.nbVals=0;
        insere_in_bloc_BP(&buffer,cle,-1);
        aff_entete_BPArbre(f,1,root);
        ecrireDir_BPArbre(f,root,&buffer);
    }
}
/*void AffInord_BPArbre(BPArbre *arb,long rac)
{
    if(rac!=NIL && arb!=NULL)
    {
        int k=0;
        TNoeuB buf;
        lireDir_BPArbre(arb,rac,&buf);
        AffInord_BPArbre(arb,buf.Fils[0]);
        for(k=0;k<buf.nbVals;k++)
        {
            printf("%d|",buf.Val[k]);
            AffInord_BPArbre(arb,buf.Fils[k+1]);
        }
    }


}*/


void BulkLoad_BPArbre(char *nomArb,char *sourceTOF)
{
    /**TOF*/

    TOF *source=NULL;
    BPArbre *f=NULL;
    ouvrir_TOF(&source,sourceTOF,'a');
    ouvrir_BPArbre(&f,nomArb,'n');

    long parBufTOF=0,nBlocTOF=0;
    TBloc bufSource;
    int initBuffer=0;

    /**ARB*/
    long j=0,i=0;
    long currentNoeud=0;

    long refoulee=0;
    TNoeuB buffer,bufDeRemp;
    /// LA RACINE ET SON FILS GAUCHE
    i=allouerBloc_BPArbre(f);
    aff_entete_BPArbre(f,1,i);
    bufDeRemp.nbVals=1;

    buffer.nbVals=0;
    for(j=0;j<NB_VALS_MAX+1;j++)
    {
        buffer.Fils[j]=NIL;
    }
    j=0;
    currentNoeud=allouerBloc_BPArbre(f);
    bufDeRemp.Fils[0]=currentNoeud;
    bufDeRemp.Fils[1]=NIL;

    nBlocTOF=1;
    lireDir_TOF(source,nBlocTOF,&bufSource);
    parBufTOF=0;
    printf("NBLOC : %d \n",nBlocTOF);
    //printf("HELLO %d",NB_VALS_MAX,entete_TOF());
    while(parBufTOF<bufSource.nb )
    {
        buffer.Val[j]=bufSource.tab[parBufTOF];
        buffer.nbVals++;
        j++;
        parBufTOF++;
    }
    bufDeRemp.Val[0]=buffer.Val[buffer.nbVals-1];

    buffer.suiv=allouerBloc_BPArbre(f);
    bufDeRemp.Fils[1]=buffer.suiv;
    ecrireDir_BPArbre(f,i,&bufDeRemp);
    ecrireDir_BPArbre(f,currentNoeud,&buffer);
    Empiler_TNoeuB(&(f->pile),bufDeRemp,i,0);
    aff_entete_BPArbre(f,3,1);
    buffer.nbVals=0;
    currentNoeud=buffer.suiv;
    j=0;
    printf("LE SUIV %d",currentNoeud);
    int atraiter=0;

    /// LES AUTRES
    int pourcentage=0;
    long valpour=0,pas=entete_TOF(source,1)/100;
    valpour=pas;
    printf("Nombre de blocs TOF : %d\n",entete_TOF(source,1));
    for(nBlocTOF=2;nBlocTOF<=entete_TOF(source,1);nBlocTOF++)
    {
        lireDir_TOF(source,nBlocTOF,&bufSource);
        parBufTOF=0;
        int traite=0;
        if(nBlocTOF==valpour)
        {
            pourcentage++;
            valpour+=pas;
            gotoxy(4,5);
            color(12,0);
            printf("%d",pourcentage);
            color(15,0);
            printf("%%");
        }

        while(parBufTOF<bufSource.nb || !traite)
        {
            if(buffer.nbVals == NB_VALS_MAX || parBufTOF==bufSource.nb) // dès qu'il est plein => insertion
            {
                // FILS A NIL PEUT ETRE A LA FIN PR LIER
                j=0;
                refoulee=buffer.Val[buffer.nbVals-1];
                long autNum=allouerBloc_BPArbre(f);
                //chainage
                buffer.suiv=autNum;
                ecrireDir_BPArbre(f,currentNoeud,&buffer);
                currentNoeud=autNum;
                buffer.nbVals=0;
                bufDeRemp.nbVals=0;
                j=0;

                long noeudTransition=0;
                int stop=0;
                long profactu=entete_BPArbre(f,3)-1;
                for(initBuffer=0;initBuffer<NB_VALS_MAX+1;initBuffer++)
                {
                    buffer.Fils[initBuffer]=NIL;
                }
                for(initBuffer=0;initBuffer<NB_VALS_MAX+1;initBuffer++)
                {
                    bufDeRemp.Fils[initBuffer]=NIL;
                }
                do
                {
                    atraiter=0;
                    if(!PileVide_TNoeuB(f->pile))
                        Depiler_TNoeuB(&(f->pile),&bufDeRemp,&i,&noeudTransition);
                    else
                    {

                        bufDeRemp.nbVals=1;
                        bufDeRemp.Val[0]=refoulee;
                        bufDeRemp.Fils[0]=i;
                        bufDeRemp.Fils[1]=autNum;
                        i=allouerBloc_BPArbre(f);
                        aff_entete_BPArbre(f,1,i);
                        ecrireDir_BPArbre(f,i,&bufDeRemp);
                        aff_entete_BPArbre(f,3,entete_BPArbre(f,3)+1);
                        stop=1;
                    }

                    if(bufDeRemp.nbVals!=NB_VALS_MAX)
                    {
                        insere_in_bloc_BP(&bufDeRemp,refoulee,autNum);
                        ecrireDir_BPArbre(f,i,&bufDeRemp);
                        stop=1;
                    }
                    else if (PileVide_TNoeuB(f->pile))
                    {
                        SplitNoeuB(refoulee,autNum,&bufDeRemp,&autNum,&refoulee,f,profactu);
                        ecrireDir_BPArbre(f,i,&bufDeRemp);
                        bufDeRemp.nbVals=1;
                        bufDeRemp.Val[0]=refoulee;
                        bufDeRemp.Fils[0]=i;
                        bufDeRemp.Fils[1]=autNum;
                        i=allouerBloc_BPArbre(f);
                        aff_entete_BPArbre(f,1,i);
                        ecrireDir_BPArbre(f,i,&bufDeRemp);
                        aff_entete_BPArbre(f,3,entete_BPArbre(f,3)+1);
                        stop=1;
                    }
                    else
                    {
                        SplitNoeuB(refoulee,autNum,&bufDeRemp,&autNum,&refoulee,f,profactu);
                        ecrireDir_BPArbre(f,i,&bufDeRemp);
                        atraiter=1;
                    }
                    profactu--;
                }while( (!PileVide_TNoeuB(f->pile) && bufDeRemp.nbVals==NB_VALS_MAX && !stop) || atraiter);

                // redescendre à la nouvelle feuille la charger dans buffer ou l'enr avant
                long retour=entete_BPArbre(f,1);
                profactu=0;
                f->pile.sommet=NIL;
                while(profactu<entete_BPArbre(f,3))
                {
                    lireDir_BPArbre(f,retour,&bufDeRemp);
                    Empiler_TNoeuB(&(f->pile),bufDeRemp,retour,bufDeRemp.nbVals-1);

                    retour=bufDeRemp.Fils[bufDeRemp.nbVals];
                    profactu++;
                }
                /**printf("\n le sommet %d",f->pile.sommet);
                system("pause");*/

                traite=1;

                for(initBuffer=0;initBuffer<NB_VALS_MAX+1;initBuffer++)
                {
                    buffer.Fils[initBuffer]=NIL;
                }

            }
            // a la sortie buffer == le buffer où continuer l'insertion et son numéro == currentNoeud
            if(!traite)
            {
                buffer.nbVals++;
                buffer.Val[j]=bufSource.tab[parBufTOF];
                ///printf("(%d ,  %d)  ",j,buffer.Val[j]);
                j++;
                parBufTOF++;
            }
        }
    }
//    if(buffer.nbVals<NB_VALS_MAX)
//    {
//        buffer.suiv=NIL;
//        lireDir_TOF(source,entete_TOF(source,1),&bufSource);
//        for(parBufTOF=0;parBufTOF<bufSource.nb;parBufTOF++)
//        {
//            buffer.Val[buffer.nbVals++]=bufSource.tab[parBufTOF];
//        }
//        ecrireDir_BPArbre(f,currentNoeud,&buffer);
//    }
    fermer_TOF(source);
    fermer_BPArbre(f);
    system("cls");
}

///*
//void Inordre_btree(BPArbre *bt, long i, int avec_affichage )
//{
//   TNoeuB localBuf;
//   int j;
//   if (i != -1) {
//      LireBloc_btree(bt, &localBuf, i);
//      Inordre_btree(bt, localBuf.Fils[0], avec_affichage);
//      for (j=0; j<localBuf.nbVals; j++) {
//	if ( avec_affichage )
//      	   printf("%d ", localBuf.Val[j]);
//
//	/*if (val_prec > localBuf.Val[j])
//		Erreur = 1;		// pour vérification de l'ordre
//	nbvaleurs++;			// pour vérification du nombre total de valeurs insérées
//	val_prec = localBuf.Val[j];
//	Inordre_btree(bt, localBuf.Fils[j+1], avec_affichage);
//      }
//   }
//}*/
