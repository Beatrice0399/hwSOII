#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "format.h"
#include "fileIO.h"

int lenPage(int* n_column, int* column_width, int* distance_column);

void inizializza(char** array, int* size, int* length);

void addSpace(char** outputText, int* currentRow, int spazi);

char* getPath(const char* txt_path);

int len(char* word);

char* concat(const char *s1, const char *s2);

char* readFile(char* path);

void writeText(char** outputText, int* linesPerColumn, int* nPage);

void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces);

char** newLine(char** outputText, int* currentRow, int* countRow, int* startRow, int* linesPerColumn,
     int* countColumn, int* nColumn, int* total_length, int* nPage);

char** justify(int *nwords, int* currentRow, char** array, char **outputText, int* countColumn, int* distanceColumn, int* nColumn, int* countRow,
    int* linesPerColumn, int* startRow, int* columnWidth, int* total_length, int* nPage);

char** noJustified(int *nwords, int* currentRow, char** array, char** outputText, int* columnWidt, int* conSpazi, int* distanceColumn, int* ncolumn, 
    int* countColumn, int* linesPerColumn, int* countRow, int* startRow, int* total_length, int* nPage);

char** emptyRow(int* currentRow, char** outputText, int* columnWidth, int* distanceColumn, int* ncolumn, int* countColumn, int* countRow,
     int*linesPerColumn, int* startRow, int* total_length, int* nPage);

char** newPage(char** outputText, int* currentRow, int*startRow, int* linesPerColumn, int* total_length, int*nPage); 

void printArray(char** a, int* linesPerColumn, int* nPage);

int main(int argc, char const *argv[])
{
    //Settings
    char* pathText = malloc(sizeof(*pathText));
    int* nColumn = malloc(sizeof(int));
    int* linesPerColumn = malloc(sizeof(int));    //6
    int* columnWidth = malloc(sizeof(int));      //12
    int* distanceColumn = malloc(sizeof(int)); 

    /*
    strcpy(pathText, argv[1]);
    *nColumn = atoi(argv[2]);
    *linesPerColumn = atoi(argv[3]);
    *columnWidth = atoi(argv[4]);
    *distanceColumn = atoi(argv[5]);
    */
    *nColumn = 5;
    *linesPerColumn = 33;
    *columnWidth = 20;
    *distanceColumn = 4;
 
    int* pageLength = malloc(sizeof(int));
    *pageLength = lenPage(nColumn, columnWidth, distanceColumn);

    //Initialize both bufferArray buffers
    char** bufferArray = malloc(*columnWidth/2*sizeof(*bufferArray)); 
    inizializza(bufferArray, columnWidth, columnWidth);

    char** outputText = malloc(*linesPerColumn*sizeof(*outputText));
    inizializza(outputText, linesPerColumn, pageLength);

    int* currentRow = malloc(sizeof(int));      //per scrivere sull'bufferArray di output
    int* nwords = malloc(sizeof(int));          //per contare il numero di parole che entrano su una riga di una colonna
    int* countRow = malloc(sizeof(int));        //per tenere traccia delle righe scritte in una colonna
    int* countColumn = malloc(sizeof(int));     //per tenere traccia del numero di colonne scritte
    int* startRow = malloc(sizeof(int));        //puntatore per tornare all'inizio della colonna successiva (quando non si è più nella prima pagina)
    int* nPage = malloc(sizeof(int));
    int* conspazi = malloc(sizeof(int));
    *nPage = 1;

    char* pathFile = getPath("Input.txt");
    FILE *file = fopen(pathFile, "r");

    char* buff_str = malloc(1024*sizeof(char*));
    for(char* riga_attuale = fgets(buff_str, 1024, file); riga_attuale != NULL; riga_attuale = fgets(buff_str, 1024, file)){
        riga_attuale = strtok(riga_attuale, "\n\r");
        if(riga_attuale != NULL){
            for(char* token = strtok(riga_attuale, " "); token != NULL; token = strtok(NULL, " "))
            {
                int lenWord = len(token);
                if ( (*conspazi+lenWord) <= *columnWidth) {    
                *conspazi = *conspazi + lenWord + 1;
                strcpy(bufferArray[*nwords], token);
                *nwords += 1;
                }
                else {
                    if (*nwords == 1) {
                        *conspazi -=1;
                        outputText = noJustified(nwords, currentRow, bufferArray, outputText, columnWidth, conspazi, distanceColumn, 
                                                    nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
                        *conspazi = lenWord + 1;         
                        strcpy(bufferArray[0], token);  
                        *nwords = 1;
                    }
                    else {
                        outputText = justify(nwords, currentRow, bufferArray, outputText, countColumn, distanceColumn, nColumn,
                                                countRow, linesPerColumn, startRow, columnWidth, pageLength, nPage); 
                        *conspazi = lenWord + 1;         
                        strcpy(bufferArray[0], token);  
                        *nwords = 1;
                    }
                } 
            }
            outputText = noJustified(nwords, currentRow, bufferArray, outputText, columnWidth, conspazi, distanceColumn, 
                                                    nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
            
            if (*currentRow != *startRow) {
                outputText = emptyRow(currentRow, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, 
                                    linesPerColumn, startRow, pageLength, nPage);
            }
            *conspazi = 0;
            *nwords = 0;
        }
    }
    writeText(outputText, linesPerColumn, nPage);
    return 0;
}


int lenPage(int* n_column, int* column_width, int* distance_column){
    int i = (*column_width*(*n_column))*2 + *distance_column*(*n_column-1) +1;
    return i;
}

void inizializza(char** array, int* size, int* length) {
    for (int i = 0; i < *size; i++) {
                //array[i] = malloc(*length*sizeof(*array[i]));    
                array[i] = (char*)malloc((*length+1)* sizeof(char));
            }
}

void addSpace(char** outputText, int* currentRow, int spazi){
    for(int i = 0; i < spazi; i++){
        strcat(outputText[*currentRow], " ");      
    }
}

char* getPath(const char* txt_path){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *result = malloc(strlen(cwd) + strlen(txt_path) + 6);
    strcpy(result, cwd);
    strcat(result, "/");
    strcat(result, txt_path);
    return result;
}


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

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result; 
}

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
    FILE *fp = fopen("singleText.txt", "w+"); 
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
        if (count < *nwords ) {
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
    *conSpazi = 0;
    *nwords = 0;
    return outputText;
}


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


char** newPage(char** outputText, int* currentRow, int*startRow, int* linesPerColumn, int* total_length, int* nPage){
    int lungh = (*linesPerColumn + 1)*(*nPage+1) - 1;
    outputText = realloc(outputText, sizeof(char*) * lungh);
    //outputText = realloc(outputText, lungh * sizeof(*outputText));
    int riga = *currentRow;
    for (int i = riga; i <= lungh; i++) {
        //outputText[i] = malloc(*total_length*sizeof(*outputText[i])); 
        outputText[i] = (char*)malloc(*total_length* sizeof(char));

    }
    *currentRow = riga;
    int spazi = *total_length;
    strcat(outputText[*currentRow], "%%%");
    *currentRow += 1;
    *startRow = *currentRow;
    *nPage +=1;
    return outputText;
} 


void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces){
    for(int i = 0; i < remaining_spaces; i++){
        array_spaces[i % *n_spaces] ++;
    }
}

void printArray(char** a, int* linesPerColumn, int* nPage) {
    int p = 0;
    if (*nPage == 1) {
        p = *linesPerColumn; 
    }
    else {
        p = (*linesPerColumn+1)**nPage-1; 
    }
    for(int i = 0; i < p; i++){
        printf("%s\n", a[i]);
        }
}