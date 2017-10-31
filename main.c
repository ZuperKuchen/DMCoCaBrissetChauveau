#include "test/all.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GLUCOSE_EXE "./glucose-syrup-4.1/simp/glucose"
#define ARG 0
#define FIN_LIGNE "\n"

void usage(int err){
  switch(err){
  case ARG:
    printf("usage : ./nom_exe h avec h > 0 \n");
    break;
  default:
    printf("Appel usage non reconnue \n");
    break;
  }
}

int main(int argc, char **argv){

  //TO REMOVE
  printf("argc: %d \n" ,argc);
  for (int i=0; i<argc; i++){
    printf("argv[%d]:%d \n" ,i,atoi(argv[i]));
  }
  //

  
  if (argc != 2) { //On verifie si l'utilisateur à bien passé la hauteur en paramètre
    usage(ARG);
    return EXIT_FAILURE;
  }
  int height = atoi(argv[1]); //On recupere la hauteur
  if (height <= 0){ //On verifie que h > 0 
    usage(ARG);
    return EXIT_FAILURE;
  }
  int nbVer = orderG(); //nombre de sommets n
  //int nbEdg = sizeG();  //nombre d'aretes m
  int nbClauses = 0;
  int nbVar = nbVer * height ; //nombre de variables Xv,h
  int nom_var = 1;

  //INITIALISATION VAR//
  int matrice_var[nbVer][height+1];
  for(int i=0; i < nbVer; i++){
    for(int j=0; j <= height; j++){
      matrice_var[i][j] = nom_var++;
    }
  } 
  //
  //ECRIRE LE FICHIER//
  const char* file_name = "file.txt";
  FILE* file = fopen(file_name, "w");
  char *buffer = malloc(sizeof(char) * (nbVar*2+2)); //Taille maximal d'une clause: 2*nb variables (variables + espaces) + 2 ( 0 de fin de ligne et \n)
  sprintf(buffer,"p cnf ");
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  sprintf(buffer,"%d",nbVar);
  fwrite(buffer, sizeof(char), strlen(buffer), file);
  //  fwrite(&nbVar, sizeof(char), 1,file);
  sprintf(buffer,"\n");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
  //*Condition 1*//
  for(int i=0; i < nbVer; i++){
    for(int j=0; j <= height; j++){
      sprintf(buffer, "%d ", matrice_var[i][j]);
      fwrite(buffer, sizeof(char), strlen(buffer), file);
    }
    sprintf(buffer,"0\n");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    nbClauses++;
  }






  
  fclose(file);
  //
  //
  /*
  char *file_name_out = "res.txt";
  FILE* file_res = fopen(file_name_out, "w");
  fclose(file_res);
  */

  char *file_name_test = "test.txt";
  
  //
  //LE PASSER DANS GLUCOSE
  //Penser à gérer le cas ou glucose n'est pas compilé
  //printf("test 1\n");
  //execl(GLUCOSE_EXE, GLUCOSE_EXE, file_name_test, (const char*) NULL);
  //printf("test 2\n");
  //
  //
  /*
  file_res = fopen(file_name_out, "r");
  fseek(file_res, SEEK_END, 
  */
  
  
  return 1;
}
