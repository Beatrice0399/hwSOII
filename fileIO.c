#include <stdio.h>
#include <stdlib.h>
#include "fileIO.h"
#include "path.h"

char* readFile(char* path) {
    FILE *fp = fopen(path, "r");
    char *a = "Error";
    if(fp == NULL){
        printf("Errore nell'apertura del file");
        return a;
    }
    const unsigned num = 1024;
    char string[num];
    char* text = "";
    while(fgets(string, num, fp)){
        text = concat(text, string);
    }
    return text;
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