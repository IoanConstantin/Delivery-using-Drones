#include <stdio.h>
#include <stdlib.h>
#define BUFFER 100
#define INFINIT 10000

/* Functie recursiva de afisare a nodurilor drumului minim cerut pentru TASK-ul 1, 
   respectiv de retinere in vectorul afis a nodurilor de la TASK-ul 2 */

void afisarecale(FILE *fp2,int *N,int drum[100][100],int x,int y,int *num,int *C,int ferma,int *ID,int *curent,int **afis,int spatiu[100][100])
{
       int t,scrie=1;// flag care verifica daca afiseaza nodul

       for(t=0;t<*N;t++)// se ia fiecare nod al grafului
       {
              if((drum[x][t]+drum[t][y]<spatiu[x][y]||spatiu[x][y]==0)&&drum[x][t]!=0&&scrie==1&&drum[x][t]+drum[t][y]==drum[x][y]&&drum[t][y]!=0)/* se verifica daca
              drumul mai lung ca numar de noduri are un cost strict mai mic decat 
              drumul mai scurt, daca exista drum intre nodul t si nodul sursa, respectiv
              intre t si destinatie si daca nodul t este continut intre nodurile din  
              descompunerea drumului dintre sursa si destinatie(dintre x si y)*/
              {

                        /* afiseaza descompunerea drumului dintre sursa si t, respectiv
                        dintre t si destinatie */
                        scrie=0;
                        afisarecale(fp2,N,drum,x,t,num,C,ferma,ID,curent,afis,spatiu);
                        afisarecale(fp2,N,drum,t,y,num,C,ferma,ID,curent,afis,spatiu);
              }
       }

       if(ferma==y&&scrie==1) (*num)++;/* num -> contorizeaza de cate ori a fost 
       printata ferma unica de la TASK-ul 1 - 1*/
       if(scrie==1) 
       {

              if(*ID==1)// TASK 1
              { 
                        /* daca num este egal cu numarul de comenzi/clienti,
                        inseamna ca y este ultimul nod printat si nu mai exista 
                        spatiu dupa el, altfel afiseaza nodul y si spatiu */
                        if(*C==(*num)) fprintf(fp2,"%d",y);
                                  else fprintf(fp2,"%d ",y);                        
              }

              if(*ID==2||*ID==3)//TASK 2 sau 3
              { 
                        /* se retine nodul y pe pozitia curenta a vectorului afis
                        si se incrementeaza pozitia curenta */
                        *afis[(*curent)]=y;
                        (*curent)++;
              }
       }  
}

int main(int argc, char *argv[])
{
     int i,j,x,y,z;

     /* Se deschid fisierele, primul de citire,iar al doilea de scriere */
     FILE *fp1=fopen(argv[1],"r");
     FILE *fp2=fopen(argv[2],"w");

     /* Citire numar TASK */
     int *ID=(int *)malloc(BUFFER*sizeof(int));
     fscanf(fp1,"%d",ID);

     if(*ID==1)
     {    
          /* Citire numar noduri graf */
          int *N=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",N);

          /* Citire tipul fiecarui nod al grafului */
          int *tip=(int *)malloc(BUFFER*sizeof(int));
          for(i=0;i<*N;i++)
          fscanf(fp1,"%d ",&tip[i]);

          /* Citire numar muchii */
          int *M=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",M);

          /* spatiu -> matricea de adiacenta a grafului */
          int spatiu[100][100];
          for(i=0;i<*N;i++)
               for(j=0;j<*N;j++)
                     spatiu[i][j]=0;

          int *sursa=(int *)malloc(BUFFER*sizeof(int));
          int *dest=(int *)malloc(BUFFER*sizeof(int));
          int *cost=(int *)malloc(BUFFER*sizeof(int));

          /* Se completeaza matricea de adiacenta cu costul 
          muchiei dintre nodul sursa si cel destinatie */
          for(i=0;i<*M;i++)
          {
               fscanf(fp1,"%d %d %d\n",sursa,dest,cost);
               spatiu[(*sursa)][(*dest)]=*cost;
          }

          /* Citire numar de comenzi */
          int *C=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",C);

          int *comsursa=(int *)malloc(BUFFER*sizeof(int));
          int *comdest=(int *)malloc(BUFFER*sizeof(int));
          
          /* drum -> matricea drumurilor minime */
          int drum[100][100];
          for(i=0;i<*N;i++)
               for(j=0;j<*N;j++)
                     drum[i][j]=0;

          int ferma=0,s=0,r=0;
          int *num=(int *)malloc(BUFFER*sizeof(int));

          int *curent=(int *)malloc(BUFFER*sizeof(int));
          *curent=0;

          int **afis=(int **)malloc(BUFFER*sizeof(int*));
          for(i=0;i<BUFFER;i++)
          afis[i]=(int *)calloc(BUFFER,sizeof(int));

          /* Se iau comenzile in ordine */
          for(i=0;i<*C;i++)
          {
               /* Citire tip ferma si destinatie comanda */
               fscanf(fp1,"%d %d\n",comsursa,comdest);

               /* Stabilire ferma */
               for(j=0;j<*N;j++)
                    if(tip[j]==*comsursa) ferma=j;
               
               /* Prima afisare a fermei */
               if(i==0) fprintf(fp2,"%d ",ferma);

               /* Initializare matrice drumuri minime */
               for(x=0;x<*N;x++)
               {
                     for(y=0;y<*N;y++)
                     { 
                           /* Daca exista/nu exista muchie */
                           if(spatiu[x][y]==0) drum[x][y]=INFINIT;
                           if(x==y) drum[x][y]=0;
                           if(spatiu[x][y]!=0) drum[x][y]=spatiu[x][y];
                     }
               }

               /* Completare matrice drumuri minime */
               for(z=0;z<*N;z++)
               {
                   for(x=0;x<*N;x++)
                   {
                       for(y=0;y<*N;y++)
                       {
                           /* Daca prin trecerea printr-un nod intermediar z
                           costul total este mai mic decat costul drumului x-y, 
                           se actualizeaza costul drumului x-y */
                           if((drum[x][z]+drum[z][y])<drum[x][y])
                           {
                                drum[x][y]=drum[x][z]+drum[z][y];                 
                           }
                       }
                   }
               }

               /* s -> suma totala a drumurilor parcurse 
                  Se adauga lui s costul drumului minim dintre ferma si destinatia comenzii
                  si costul drumului minim dintre destinatia comenzii si ferma */ 
               s=drum[ferma][(*comdest)]+drum[(*comdest)][ferma]+s;
               
               /* Se afiseaza nodurile drumului minim dintre ferma si destinatie, 
               respectiv dintre destinatie si ferma */
               afisarecale(fp2,N,drum,ferma,(*comdest),num,C,ferma,ID,curent,afis,spatiu); 
               afisarecale(fp2,N,drum,(*comdest),ferma,num,C,ferma,ID,curent,afis,spatiu);
          }

          /* Afisare s */
          fprintf(fp2,"\n%d\n",s);

          /* Eliberare memorie */
          free(ID);
          free(N);
          free(tip);
          free(M);
          free(sursa);
          free(dest);
          free(cost);
          free(C);
          free(comsursa);
          free(comdest);
          free(num);
          free(curent);
          for(r=0;r<BUFFER;r++)
          {
               int *fr=afis[r];
               free(fr);
          }
          free(afis);
     }

     if(*ID==2||*ID==3)
     {
          int *N=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",N);

          int *tip=(int *)malloc(BUFFER*sizeof(int));
          for(i=0;i<*N;i++)
          fscanf(fp1,"%d ",&tip[i]);

          int *M=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",M);

          int spatiu[100][100];
          for(i=0;i<*N;i++)
               for(j=0;j<*N;j++)
                     spatiu[i][j]=0;

          int *sursa=(int *)malloc(BUFFER*sizeof(int));
          int *dest=(int *)malloc(BUFFER*sizeof(int));
          int *cost=(int *)malloc(BUFFER*sizeof(int));

          for(i=0;i<*M;i++)
          {
               fscanf(fp1,"%d %d %d\n",sursa,dest,cost);
               spatiu[(*sursa)][(*dest)]=*cost;
          }

          int *C=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d",C);

          int *comsursa=(int *)malloc(BUFFER*sizeof(int));
          int *comdest=(int *)malloc(BUFFER*sizeof(int));

          int drum[100][100];
          for(i=0;i<*N;i++)
               for(j=0;j<*N;j++)
                     drum[i][j]=0;

          int ferma=0,s=0,k=0,w=0,r=0,vsursa[1000],vdest[1000],vcomsursa[1000];
          int *num=(int *)malloc(BUFFER*sizeof(int));

          int *curent=(int *)malloc(BUFFER*sizeof(int));
          *curent=0;

          int **afis=(int **)malloc(BUFFER*sizeof(int*));
          for(i=0;i<BUFFER;i++)
          afis[i]=(int *)calloc(BUFFER,sizeof(int));

          for(i=0;i<*C;i++)
          {
               fscanf(fp1,"%d %d\n",comsursa,comdest);

               for(j=0;j<*N;j++)
                    if(tip[j]==*comsursa) ferma=j;

               /* vcomsursa -> retine tipul fermei 
                  vsursa -> retine ferma 
                  vdest -> retine destinatia comenzii */
               vcomsursa[k]=*comsursa;
               vsursa[k]=ferma;
               vdest[k]=*comdest;
               k++;

               for(x=0;x<*N;x++)
               {
                     for(y=0;y<*N;y++)
                     {
                           if(spatiu[x][y]==0) drum[x][y]=INFINIT;
                           if(x==y) drum[x][y]=0;
                           if(spatiu[x][y]!=0) drum[x][y]=spatiu[x][y];
                     }
               }

               for(z=0;z<*N;z++)
               {
                   for(x=0;x<*N;x++)
                   {
                       for(y=0;y<*N;y++)
                       {
                           if((drum[x][z]+drum[z][y])<drum[x][y])
                           {
                                drum[x][y]=drum[x][z]+drum[z][y];                 
                           }
                       }
                   }
               }
          }

          /* citire si afisare ferma de start */
          int *fermastart=(int *)malloc(BUFFER*sizeof(int));
          fscanf(fp1,"%d\n",fermastart);
          fprintf(fp2,"%d ",*fermastart);
          vsursa[0]=*fermastart;

          /* Se ia fiecare comanda */
          for(i=0;i<k;i++)
          {  
              if(i==0) // Prima comanda
              {
                /* Se adauga lui s costul drumului ferma de start - prima destinatie
                   Se afiseaza nodurile drumului ferma de start - prima destinatie */
                s=s+drum[(*fermastart)][vdest[0]];
                afisarecale(fp2,N,drum,*fermastart,vdest[0],num,C,ferma,ID,curent,afis,spatiu);
              }

              if(i!=0)
              {
                 int *minj=(int *)malloc(BUFFER*sizeof(int));
                 int min=INFINIT;

                 for(w=0;w<k;w++)
                 {
                      for(j=0;j<*N;j++)
                      {                            
                           if(tip[j]==vcomsursa[w])// daca j este ferma de tip cerut
                           {
                                /* min -> minimul sumelor dintre costul  drumului 
                                destinatie precedenta - j si j - destinatie curenta 
                                si minj -> ferma prin care trec drumurile de 
                                suma minima */ 
                                if((drum[vdest[i-1]][j]+drum[j][vdest[i]])<min) 
                                {
                                     min=drum[vdest[i-1]][j]+drum[j][vdest[i]];
                                     *minj=j;
                                }
                           }
                      }
                 }

                 /* Adauga min la suma totala a drumurilor parcurse */
                 s=s+min;
                 
                 /* Afiseaza nodurile drumului destinatie precedenta - minj si 
                 minj - destinatie curenta */
                 afisarecale(fp2,N,drum,vdest[i-1],*minj,num,C,ferma,ID,curent,afis,spatiu);
                 afisarecale(fp2,N,drum,*minj,vdest[i],num,C,ferma,ID,curent,afis,spatiu);

                 free(minj);
              }

              int *minj1=(int *)malloc(BUFFER*sizeof(int));
              int min1=INFINIT;

              if(i==(k-1))// ultima comanda 
              {
                 for(w=0;w<k;w++)
                 {
                      for(j=0;j<*N;j++)
                      {
                           if(tip[j]==vcomsursa[w])
                           {
                                /* min1 -> minimul costurilor drumurilor ultima destinatie - j
                                   minj1 -> ultima ferma aferenta costului minim min1*/
                                if(drum[vdest[i]][j]<min1) 
                                {
                                     min1=drum[vdest[i]][j];
                                     *minj1=j;
                                }
                           }
                      }
                 }
                  
                  /* Adauga min1 la s */
                  s=s+min1;
                  
                  /* Afiseaza nodurile drumului ultima destinatie - minj1 */
                  afisarecale(fp2,N,drum,vdest[i],*minj1,num,C,ferma,ID,curent,afis,spatiu);
              }

              free(minj1);
          }

          /* Printarea fiecarui nod prin care trece drumul cerut,
          cu spatii intre ele si fara spatiu dupa ultimul nod */
          for(i=0;i<((*curent)-1);i++)
          fprintf(fp2,"%d ",*afis[i]);
          fprintf(fp2,"%d",*afis[(*curent)-1]);

          /* Printarea costului drumului parcurs */
          fprintf(fp2,"\n%d\n",s);
          
          /* Eliberare memorie */
          free(ID);
          free(N);
          free(tip);
          free(M);
          free(sursa);
          free(dest);
          free(cost);
          free(C);
          free(comsursa);
          free(comdest);
          free(num);
          free(fermastart);
          for(r=0;r<BUFFER;r++)
          {
              int *fr=afis[r];
              free(fr);
          }
          free(afis);
     }

     /* Inchidere fisiere */
     fclose(fp1);
     fclose(fp2);
     return 0;
}

