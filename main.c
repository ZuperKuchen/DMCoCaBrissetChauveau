#include "test/all.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

#define GLUCOSE_EXE "glucose-syrup-4.1/simp/glucose"
#define ARG       0
#define OPEN_FAIL 1
#define FORK      2
#define EXEC      3
#define LIM_BUF   10


void usage(int err, char* str){
  switch(err){
  case ARG:
    printf("usage : ./nom_exe h avec h > 0 \n");
    break;
  case OPEN_FAIL:
    printf("probleme lors de l'ouverture de %s \n",str);
    break;
  case FORK:
    printf("probleme de fork() !\n");
    break;
  case EXEC:
    printf("execl n'a pas lancé %s\n",str);
    break;
  default:
    printf("Appel usage non reconnue \n");
    break;
  }
}

/*Calcule la matrice d'adjacence de l'arbre couvrant en fonction du tableau de variables t
*/
void SAT_to_HAC(int nbVer,int t[nbVer], int matrice_adj[nbVer][nbVer], int height){
  //Initialisation matrice d'adjacence
  for(int i=0 ; i < nbVer ; i++){
    for(int j=0 ; j < nbVer ; j++){
      matrice_adj[i][j] = 0;
    }
  }

  int tab_parents_pots[height+1][nbVer];
  //on initialise à -1
  for (int i = 0; i <= height ; i++){
    for (int j=0 ; j < nbVer; j++){
      tab_parents_pots[i][j] = -1;
    }
  }

  //On remplit les parents potentiels
  int cpt;
  for (int k = 0; k <= height ; k++){
    cpt = 0;
    for (int i=0 ; i < nbVer; i++){
      if (t[i] == k){ 
	tab_parents_pots[k][cpt++] = i;
      }
    }
  }

  //On remplit la matrice d'adjacence
  for (int k = 1; k <= height; k++){
    for (int i = 0; i < nbVer; i++){
      if (t[i] == k){
	for(int j = 0; j < nbVer; j++){
	  if(are_adjacent(tab_parents_pots[k][j],i)){
	    matrice_adj[tab_parents_pots[k][j]][i] = 1;
	    matrice_adj[i][tab_parents_pots[k][j]] = 1;
	    break;
	  }
	}
      }
    }
  }
}

/**Calcule le nombre de clauses de la reduction en foction de
 **nb = nombre de sommets du graphe
 **k = profondeur de l'arbre couvrant recherché
*/
int calculeNombreClause(int nb, int k){
  int res = 0;
  //1ere condition
  res += nb + nb * ((pow(k,2) + k) / 2);
  //2eme condition
  res += 1 + ((pow(nb-1,2) + (nb-1)) / 2);
  //3eme condition
  res ++;
  //4eme condition
  res += nb * k;
  //return
  return res;
}

void write_matrice(int nbVer, int matrice_adj[nbVer][nbVer], char* str){
  char *buffer = malloc(sizeof(char)*50); //Bien suffisant
  FILE *file = fopen(str, "w+");
  if(!file){
    usage(OPEN_FAIL,str);
    exit(EXIT_FAILURE);
  }
  sprintf(buffer,"Nombre de sommets:%d \n", nbVer);
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  sprintf(buffer,"Matrice d'adjacence: \n");
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  for (int i = 0; i < nbVer; i++){
    sprintf(buffer,"( ");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    for (int j = 0; j < nbVer ; j++){
      sprintf(buffer, "%d ", matrice_adj[i][j]);
      fwrite(buffer, sizeof(char), strlen(buffer), file);
    }
    sprintf(buffer,")\n");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
  }
  fclose(file);
  free(buffer);
}

int main(int argc, char **argv){ 
  if (argc != 2) { //On verifie si l'utilisateur à bien passé la hauteur en paramètre
    usage(ARG,NULL);
    return EXIT_FAILURE;
  }
  int const height = atoi(argv[1]); //On recupere la hauteur
  if (height <= 0){ //On verifie que k > 0 
    usage(ARG,NULL);
    return EXIT_FAILURE;
  }
  int nbVer = orderG(); //nombre de sommets n
  //int nbEdg = sizeG();  //nombre d'aretes m
  int nbClauses = 0;
  const int nbVar = nbVer * (height+1) ; //nombre de variables Xv,h
  int nbClausesCal = calculeNombreClause(nbVer, height);
  int nom_var = 1;

  //Matrice d'adjacence du graphe 
  int matrice_adj[nbVer][nbVer];
  for (int i = 0 ;i < nbVer; i++){
    for (int j = 0; j < nbVer; j++){
      if(are_adjacent(i,j)){
	matrice_adj[i][j] = 1;
	matrice_adj[j][i] = 1;
      }
      else{
	matrice_adj[i][j] = 0;
	matrice_adj[j][i] = 0;
      }
    }
  }
  write_matrice(nbVer,matrice_adj,"Probleme.txt");

 

  //INITIALISATION VAR//
  int matrice_var[nbVer][height+1];
  for(int i=0; i < nbVer; i++){
    for(int j=0; j <= height; j++){
      matrice_var[i][j] = nom_var++;
    }
  } 
  
  //ECRIRE LE FICHIER//
  char* file_name = "file.txt";
  FILE* file = fopen(file_name, "w+");
  if(!file){
    usage(OPEN_FAIL,file_name);
    return EXIT_FAILURE;
  }
  char *buffer = malloc(sizeof(char) * (nbVar*2+2)); //Taille maximal d'une clause: 2*nb variables (variables + espaces) + 2 ( 0 de fin de ligne et \n)
  
  //*1ere ligne*//
  sprintf(buffer,"p cnf %d %d\n",nbVar, nbClausesCal);
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  
  //*Condition 1*//
  //Pour tout v dans V Il existe au moins un h tel que Xvh vrai 
  for(int i=0; i < nbVer; i++){
    for(int j=0; j <= height; j++){
      sprintf(buffer, "%d ", matrice_var[i][j]);
      fwrite(buffer, sizeof(char), strlen(buffer), file);
    }
    sprintf(buffer,"0\n");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    nbClauses++;
  }
  //Il en existe au plus un 
  for(int i=0; i < nbVer; i++){
    for(int j=0; j < height; j++){
      for(int l=j+1; l <=height; l++){
	sprintf(buffer,"-%d -%d 0\n", matrice_var[i][j], matrice_var[i][l]);
	fwrite(buffer, sizeof(char), strlen(buffer), file);
	nbClauses++;
      }
    }
  }  
  //*Condition 2*//
  //Il existe au moins un xv0 vrai
  for (int i=0; i < nbVer; i++){
    sprintf(buffer,"%d ",matrice_var[i][0]);
    fwrite(buffer, sizeof(char), strlen(buffer), file);
  }
  sprintf(buffer,"0\n");
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  nbClauses++;
  //Il en existe au  plus un
  for (int i=0; i < nbVer - 1; i++){
    for (int j=i+1; j < nbVer; j++){
      sprintf(buffer,"-%d -%d 0\n",matrice_var[i][0], matrice_var[j][0]);
      fwrite(buffer, sizeof(char), strlen(buffer), file);
      nbClauses++;
    }
  }

  //*Condition 3*//
  //Il existe au moins un sommet v tel que Xvk vrai
  for(int i=0; i < nbVer; i++){
    sprintf(buffer,"%d ", matrice_var[i][height]);
    fwrite(buffer, sizeof(char), strlen(buffer), file);
  }
  sprintf(buffer,"0\n");
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  nbClauses++;

  //*Condition 4*//
  for(int i=0; i < nbVer; i++){
    for(int j=1; j <= height; j++){
      sprintf(buffer,"-%d ", matrice_var[i][j]);
      fwrite(buffer, sizeof(char), strlen(buffer), file);
      for (int l=0; l < nbVer ; l++){
	if (are_adjacent(i,l)){
	  sprintf(buffer,"%d ",matrice_var[l][j-1]);
	  fwrite(buffer, sizeof(char), strlen(buffer), file);
	}
      }
      sprintf(buffer,"0\n");
      fwrite(buffer, sizeof(char), strlen(buffer), file);
      nbClauses++;
    }
  }
 

  printf("Compteur : %d | %d : Calcul \n Il y en a %d qui saute\n",
	 nbClauses, nbClausesCal, nbClauses - nbClausesCal);
  fclose(file);
  ///// *****************************************///////////////////
  
  char *file_res_name = "res.txt";
    
  //
  //LE PASSER DANS GLUCOSE
  //Penser à gérer le cas ou glucose n'est pas compilé
  pid_t pid ;//= fork();
  /*if (pid == -1){
    usage(FORK,NULL);
    return EXIT_FAILURE;
  }
  //if (pid > 0){
    int status;
    wait(&status);
    printf("...\n%d\n",status);
    }*/
  pid = fork();
  if (pid == 0){
    execl(GLUCOSE_EXE, "./glucose", file_name, file_res_name, '\0');
    usage(EXEC,"glucose");
    fprintf(stderr,"execl ne s'est pas lancé\n");
  }

  wait(NULL);
  FILE* file_res = fopen(file_res_name, "r");
  if(!file_res){
    usage(OPEN_FAIL,file_res_name);
    return EXIT_FAILURE;
  }
  int tab_res[nbVer];
  int ind_tab = 0;
  int ibuf;
  char* cbuf_res= malloc(sizeof(char)*LIM_BUF);
  int cursor_res = 0;
  char cbuf;
  fread(&cbuf, sizeof(char), 1, file_res);
  while(cbuf != '\n'){
    if(cbuf == '-'){
      while(cbuf != ' '){
	fread(&cbuf, sizeof(char), 1, file_res);
      }
      fread(&cbuf, sizeof(char), 1, file_res);
    }
    else{
      while(cbuf != ' ' && cbuf != '\n'){
	cbuf_res[cursor_res] = cbuf;
	cursor_res++;
	fread(&cbuf, sizeof(char), 1, file_res);
      }
      cbuf_res[cursor_res] = '\0';
      cursor_res = 0;
      ibuf = atoi(cbuf_res);
      if(ibuf != 0){
	tab_res[ind_tab] = ibuf;
	ind_tab++;
      }
      fread(&cbuf, sizeof(char), 1, file_res);
    }
  }
  

  for(int i = 0; i < nbVer; i++){
    printf("%d ", tab_res[i]);
  }
  
  printf("\n");


  //AJOUT
  //On "clean" le tableau
  for(int i = 0; i < nbVer; i++){
    tab_res[i] = tab_res[i] - i*height - i -1;
  }
  //TMP
   for(int i = 0; i < nbVer; i++){
    printf("%d ", tab_res[i]);
  }
  
  printf("\n");
  //FIN TMP
  SAT_to_HAC( nbVer,tab_res, matrice_adj, height);
  write_matrice(nbVer, matrice_adj, "Solution.txt");
  fclose(file_res);
  free(buffer);
  free(cbuf_res);
  return 1;
  
}
