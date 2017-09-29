#include "modeleTOF.h"

int ouvrir_TOF(TOF **f,char *path,char mode)
{
    *f=malloc(sizeof(TOF));
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
        fread(&((*f)->entete), sizeof(Entete), 1, (*f)->fich);
    }
    else if ((mode == 'N') || (mode == 'n')) /// si nouveau on initialise
    {
        (*f)->entete.numLastBloc = 0;
        (*f)->entete.nbEnreg = 0;
        (*f)->entete.provisoire = 0;
    }
    return 1;
}
void fermer_TOF(TOF *f)
{
    //Sauvegarde de l'entête
    rewind(f->fich);
    fwrite(&(f->entete), sizeof(Entete), 1, f->fich);
    fclose(f->fich);
    free(f);
}
void lireDir_TOF(TOF *f,long numBloc,TBloc *buffer)
{
    if(numBloc<= (f->entete).numLastBloc)
    {
        fseek(f->fich,(long)sizeof(Entete) + (numBloc-1)*(long)sizeof(TBloc) , SEEK_SET);
        fread(buffer,1,sizeof(TBloc),f->fich);
    }
}
void ecrireDir_TOF(TOF *f,long numBloc,TBloc *buffer)
{
    if(numBloc<= (f->entete).numLastBloc)
    {
        fseek(f->fich,(long)sizeof(Entete) + (numBloc-1)*(long)sizeof(TBloc) , SEEK_SET);
        fwrite(buffer,1,sizeof(TBloc),f->fich);
    }
    //printf("\n Le : numBloc : %d  sizeBloc: %d  POSITION: %d  MAIS : %d",numBloc,sizeof(TBloc),sizeof(Entete) + (numBloc-1)*sizeof(TBloc),ftell(f->fich));

}
long entete_TOF(TOF *f,int i)
{
    if(i == 1) return (f->entete).numLastBloc;
    else if(i == 2) return (f->entete).nbEnreg;
    else if(i == 3) return (f->entete).provisoire;
    else return -1;
}
void aff_entete_TOF(TOF *f,int i,long val)
{
    if(i == 1) (f->entete).numLastBloc = val;
    else if(i == 2) (f->entete).nbEnreg = val;
    else if(i == 3) (f->entete).provisoire = val;
}
long allouerBloc_TOF(TOF *f)
{
    aff_entete_TOF(f,1,entete_TOF(f,1)+1);
    return entete_TOF(f,1);
}

/***********************************************************/
/**                     FONCTIONS                         **/
/***********************************************************/

/// CHARGEMENT INITIAL D UN FICHIER DE NOM @nomFich AVEC @n ENREGISTREMENTS AVEC UN FACTEUR @u
void ChargementInitial(char *nomFich,long n,double u)
{
    long i,j,k,cle;
    TOF *f;
    TBloc *buffer=malloc(sizeof(TBloc));

    long geneCle=1;
    //
    ouvrir_TOF(&f,nomFich,'n');
    i=1;
    j=0;
        //sprintf(e.nom,"Test");
        //sprintf(e.prenom,"Essai");
    for(k=0;k<n;k++)
    {
        cle=geneCle;
        geneCle++;
        //printf("%d\n",k);
        if(j< TAI_TAB_BLOC)
        {
            buffer->tab[j]=cle;
            j++;
            //printf("%d\n",k);
        }
        else
        {
            //printf("\n------------- i = %d \n",i);
            buffer->nb=j;
            allouerBloc_TOF(f);
            ecrireDir_TOF(f,i,buffer);
            buffer->tab[0]=cle;
            i++;
            j=1;
            //printf("----------%d\n",k);²
        }
        //printf("%d\n",ftell(f->fich));
        //printf("%d\n",buffer->tab[j].cle);
    }
    buffer->nb=j;
    allouerBloc_TOF(f);
    ecrireDir_TOF(f,i,buffer);
    aff_entete_TOF(f,1,i);
    aff_entete_TOF(f,2,n);
    free(buffer);
    fermer_TOF(f);
}
