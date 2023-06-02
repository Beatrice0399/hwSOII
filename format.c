#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "format.h"

int lenPage(int* n_column, int* column_width, int* distance_column){
    int i = (*column_width*(*n_column))*2 + *distance_column*(*n_column-1) +1;
    return i;
}

/*data in input un array e un puntatore 
vengono aggiunti all'array il numero di spazi dati in input
*/
void addSpace(char** outputText, int* currentRow, int spazi){
    for(int i = 0; i < spazi; i++){
        strcat(outputText[*currentRow], " ");      
    }
}

// per calcolare la lunghezza della singola parola
int len(char* word) {
    double lenW = 0;
    int value = 0;
        for (int i = 0; word[i] != '\0'; i ++)
        {
            value = (int) word[i];
            
            if(value == -17 | value == -69 | value == -65){
            }
            else if(value == 92){
                lenW += 0.5;
            }
            else if(value < 0){
                lenW += 0.5;
            }
            else{
                lenW += 1;
            }
        }
    return (int) lenW;
}

void inizializza(char** array, int* size, int* length) {
    for (int i = 0; i < *size; i++) {
        array[i] = malloc(*length*sizeof(*array[i]));    
    }
}

//funzione per scrivere le righe allineate ad entramni i margini
char** justify(int *nwords, int* currentRow, char** array, char **outputText, int* countColumn, int* distanceColumn, int* nColumn, int* countRow,
    int* linesPerColumn, int* startRow, int* columnWidth, int* total_length, int* nPage) {
    int n_of_char = 0;
    for(int i = 0; i < *nwords; i++){
        n_of_char += len(array[i]);
    }
    int* n_spaces = malloc(sizeof(int));
    *n_spaces = *nwords - 1;
    int* array_spaces = malloc(*n_spaces*sizeof(*array_spaces));
    int remaining_spaces = * columnWidth - n_of_char;
    spaceWord(array_spaces, n_spaces, remaining_spaces);
    strcat(outputText[*currentRow], array[0]);
    for(int i = 1; i< *nwords; i++){
        addSpace(outputText, currentRow, array_spaces[i - 1]);
        strcat(outputText[*currentRow], array[i]);
    }
    if (*countColumn < *nColumn-1) {
        addSpace(outputText, currentRow, *distanceColumn);
    }
    outputText = newLine(outputText, currentRow, countRow, startRow, linesPerColumn, countColumn, nColumn, total_length, nPage);
    *nwords = 0;

    return outputText;
}

//funzione per scrivere la riga non allineata al margine sinistro
char** noJustified(int *nwords, int* currentRow, char** array, char** outputText, int* columnWidth, int* conSpazi, int* distanceColumn, int* ncolumn, int* countColumn, 
            int* linesPerColumn, int* countRow, int* startRow, int* total_length, int* nPage){
    int count = 0;
    int lenParole = 0;
    for (int i = 0; i < *nwords; i++) {
        lenParole += len(array[i]);
        //char* prova = malloc(sizeof(*prova));
        //strcpy(prova, array[i]);
        strcat(outputText[*currentRow], array[i]);
        count ++;                
        if (count < *nwords) {
            strcat(outputText[*currentRow], " ");
            lenParole +=1;
        }
    }
    if(*countColumn < *ncolumn -1 ) {
        int spazi = *columnWidth - lenParole + *distanceColumn ;   //QUI  
        addSpace(outputText, currentRow, spazi);
    }
    else {
        int spazi = *columnWidth - lenParole;
        addSpace(outputText, currentRow, spazi);
    }
    outputText = newLine(outputText, currentRow, countRow, startRow, linesPerColumn, countColumn, ncolumn, total_length, nPage);
    *nwords = 0;
    return outputText;
}

/*
    funzione per calcolare se la pagina è completa. Se sono state scritte il massimo di righe e di colonne possibili date in input, 
    verrà creata la nuova pagina
*/
char** newLine(char** outputText, int* currentRow, int* countRow, int* startRow, int* linesPerColumn, int* countColumn, int* ncolumn, int* total_length, int* nPage) {
    *currentRow += 1;
    *countRow +=1;
    if (*countRow == *linesPerColumn) {
        if (*countColumn == *ncolumn-1) {
            outputText = newPage(outputText, currentRow, startRow, linesPerColumn, total_length, nPage);
            *countRow = 0;
            *countColumn = 0;
        }
        else {
            *currentRow = *startRow;
            *countRow = 0;
            *countColumn += 1;
            }
    }
    return outputText;
}

//funzione per scrivere una riga vuota, riempita da spazi, usata per creare i nuovi paragrafi
char** emptyRow(int* currentRow, char** outputText, int* columnWidth, int* distanceColumn, int* ncolumn, int* countColumn, int* countRow, int*linesPerColumn, int* startRow, int* total_length, int* nPage){
    if (*countColumn < *ncolumn - 1) {
        int totSpace = *columnWidth + *distanceColumn;
        for (int i = 0; i < totSpace ; i++) {
            strcat(outputText[*currentRow], " ");
        }
    }
    else {
        for (int i = 0; i < *columnWidth; i++) {
            strcat(outputText[*currentRow], " ");
        }
    }
    outputText = newLine(outputText, currentRow, countRow, startRow, linesPerColumn, countColumn, ncolumn, total_length, nPage);
    return outputText;
}

//funzione per creare la nuova pagina
char** newPage(char** outputText, int* currentRow, int*startRow, int* linesPerColumn, int* total_length, int* nPage){
    int lungh = (*linesPerColumn + 1)*(*nPage+1) - 1;
    outputText = realloc(outputText, sizeof(char*) * lungh);
    int riga = *currentRow;
    for (int i = riga; i < lungh; i++) {
        outputText[i] = malloc(*total_length*sizeof(*outputText[i])); 
    }
    *currentRow = riga;
    int spazi = *total_length;
    strcat(outputText[*currentRow], "%%%");
    *currentRow += 1;
    *startRow = *currentRow;
    *nPage +=1;
    return outputText;
} 

//spazi tra le parole di ogni riga
void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces){
    for(int i = 0; i < remaining_spaces; i++){
        array_spaces[i % *n_spaces] ++;
    }
}