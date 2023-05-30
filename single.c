#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "path.h"
#include "formatText.h"
#include "fileIO.h"

int main(int argc, char ** argv) {

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
    *nColumn = 3;
    *linesPerColumn = 50;
    *columnWidth = 27;
    *distanceColumn = 4;
   
    //Input text
    char* pathFile = getPath("Input2.txt");
    char* input_text = readFile(pathFile);
    //Total length
    int* pageLength = malloc(sizeof(int));
    *pageLength = lenPage(*nColumn, *columnWidth, *distanceColumn);
    //Initialize both array buffers
    char** array = malloc(*columnWidth/2*sizeof(*array));
    inizializza(array, columnWidth, columnWidth);

    char** outputText = malloc(*linesPerColumn*sizeof(*outputText));
    inizializza(outputText, linesPerColumn, pageLength);

    int* currentRow = malloc(sizeof(int));      //per scrivere sull'array di output
    int* nwords = malloc(sizeof(int));          //per contare il numero di parole che entrano su una riga di una colonna
    int* countRow = malloc(sizeof(int));        //per tenere traccia delle righe scritte in una colonna
    int* countColumn = malloc(sizeof(int));     //per tenere traccia del numero di colonne scritte
    int* startRow = malloc(sizeof(int));        //puntatore per tornare all'inizio della colonna successiva (quando non si è più nella prima pagina)
    int* nPage = malloc(sizeof(int));
    int* conspazi = malloc(sizeof(int));
    *nPage = 1;

    //char* tokenFinale = malloc(sizeof(*tokenFinale));
    for (char* token = strtok(input_text, " "); token != NULL; token = strtok(NULL, " ")) {
        int lenWord = len(token);
        char* occurrence = strstr(token, "\r\n\r\n");
        //printArray(outputText, linesPerColumn, nPage);
        if (occurrence) {
            outputText = newParagraph(outputText, array, token, occurrence, conspazi, columnWidth, nwords, currentRow, distanceColumn, nColumn, 
                            countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
        }
        else if ( (*conspazi+lenWord) < *columnWidth) {    
            *conspazi = *conspazi + lenWord + 1;
            strcpy(array[*nwords], token);
            *nwords += 1;
        }
        else {
            if (*nwords == 1) {
                *conspazi -=1;
                outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, 
                                            nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
                *conspazi = lenWord + 1;         
                strcpy(array[0], token);  
                *nwords = 1;
            }
            else {
                outputText = justify(nwords, currentRow, array, outputText, countColumn, distanceColumn, nColumn,
                                        countRow, linesPerColumn, startRow, columnWidth, pageLength, nPage); 
                *conspazi = lenWord + 1;         
                strcpy(array[0], token);  
                *nwords = 1;
            }
        }
    }
    char* token0 = array[*nwords-1];
    char* occurrence = strstr(token0, "\r\n");
    int lenToken1 = occurrence - token0;
    char* token1 = malloc(lenToken1 * sizeof(char));
    //memcpy(token1, token, lenToken1);
    strncpy(token1, token0, lenToken1);
    strcpy(array[*nwords-1], token1);  
    outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, 
                                countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
    //printArray(outputText, linesPerColumn, nPage);
    writeText(outputText, linesPerColumn, nPage);
    return 0;
}
