#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "format.h"
#include "fileIO.h"

int main(int argc, char const *argv[])
{
    //Settings
    char* pathText = malloc(sizeof(*pathText));
    int* nColumn = malloc(sizeof(int));
    int* linesPerColumn = malloc(sizeof(int));    //6
    int* columnWidth = malloc(sizeof(int));      //12
    int* distanceColumn = malloc(sizeof(int)); 

    strcpy(pathText, argv[1]);
    *nColumn = atoi(argv[2]);
    *linesPerColumn = atoi(argv[3]);
    *columnWidth = atoi(argv[4]);
    *distanceColumn = atoi(argv[5]);

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

    char* pathFile = getPath(pathText);
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
