#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fileIO.h"

/** 
* \brief La funzione calcola e restituisce il path relativo del file dato in input
* \param txt_path 
* \return \c char* 
* */
char* getPath(const char* txt_path){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *result = malloc(strlen(cwd) + strlen(txt_path) + 6);
    strcpy(result, cwd);
    strcat(result, "/");
    strcat(result, txt_path);
    return result;
}

/** 
 * \brief La funzione, dati i parametri in input apre o crea, se non esiste, un file txt in cui verrà scritto il testo giustificato e incolonnato
 * \param path 
 * \param outputText un puntatore ad un array di array di caratteri in cui è il testo da scrivere sul file
 * \param linesPerColumn
 * \param nPage
 * \return \c void 
 * 
 */ 
void writeText(char* path, char** outputText, int* linesPerColumn, int* nPage) {
    FILE *fp = fopen(path, "w+"); 
    if (fp == NULL){ 
        printf("file non trovato");
        exit(-1); 
    }
    int p = 0;
    if (*nPage == 1) {
        p = *linesPerColumn; 
    }
    else {
        p = (*linesPerColumn+1)**nPage-1; 
    }
    for (int i = 0; i < p; i++) {
        fprintf(fp, "%s", outputText[i]);
        fprintf(fp, "\n");
    }
}