#include "test/all.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

#define GLUCOSE_EXE "./glucose-syrup-4.1/simp/glucose"
#define ARG       0
#define OPEN_FAIL 1
#define FORK      2


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
  default:
    printf("Appel usage non reconnue \n");
    break;
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

  //INITIALISATION VAR//
  int matrice_var[nbVer][height+1];
  for(int i=0; i < nbVer; i++){
    for(int j=0; j <= height; j++){
      matrice_var[i][j] = nom_var++;
    }
  } 
  
  //ECRIRE LE FICHIER//
  const char* file_name = "file.txt";
  FILE* file = fopen(file_name, "w+");
  char *buffer = malloc(sizeof(char) * (nbVar*2+2)); //Taille maximal d'une clause: 2*nb variables (variables + espaces) + 2 ( 0 de fin de ligne et \n)
  
  //*1ere ligne*//
  sprintf(buffer,"p cnf %d %d\n ",nbVar, nbClausesCal);
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
	if (i !=l || are_adjacent(i,l)){
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
  if ((pid = fork()) == 0){
    execl(GLUCOSE_EXE, GLUCOSE_EXE, file_name, file_res_name, (const char*) NULL);  
  }
  //
  FILE* file_res = fopen(file_res_name, "r");
  if(!file_res){
    usage(OPEN_FAIL,file_res_name);
    return EXIT_FAILURE;
  }
  char c = fgetc(file_res);
  while(c != EOF){
    if ((char)c == '-'){
      while( c != ' ' || c != EOF){
	fseek(file_res,1, SEEK_CUR);
	c = fgetc(file_res);
      }
    }
    if((char)c != ' '){
      printf("%d ",c);
    }
    c = fgetc(file_res);
  }
  printf("\n");
  
  
  
  fclose(file_res);
  
  printf("ta daronne\n");
  return 1;
  }
