#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fileIO.h"

//funzione per ottenere il path locale del file in input
char* getPath(const char* txt_path){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *result = malloc(strlen(cwd) + strlen(txt_path) + 6);
    strcpy(result, cwd);
    strcat(result, "/");
    strcat(result, txt_path);
    return result;
}

void writeText(char** outputText, int* linesPerColumn, int* nPage) {
    FILE *fp = fopen("output.txt", "w+"); 
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