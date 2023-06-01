#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "path.h"
#include "formatText.h"
#include "fileIO.h"

int main(int argc, char const *argv[])
{
    char* input_path = malloc(1024*sizeof(*input_path));
    int* numero_colonne = malloc(sizeof(*numero_colonne));
    int* riga_attualee_per_colonna = malloc(sizeof(riga_attualee_per_colonna));
    int* caratteri_ogni_colonna = malloc(sizeof(*caratteri_ogni_colonna));
    int* spazio_tra_colonne = malloc(sizeof(*spazio_tra_colonne));

    strcpy(input_path, argv[1]);
    *numero_colonne = atoi(argv[2]);
    *riga_attualee_per_colonna = atoi(argv[3]);
    *caratteri_ogni_colonna = atoi(argv[4]);
    *spazio_tra_colonne = atoi(argv[5]);
    
    /*
    int* lunghezza_totale = malloc(sizeof(int));
    *lunghezza_totale = lenPage(*numero_colonne, *caratteri_ogni_colonna, *spazio_tra_colonne) + 1;

    char** bufferArray = malloc((*lunghezza_totale/2) * sizeof(*bufferArray));
    inizializza(bufferArray, 0, (*lunghezza_totale/2), *caratteri_ogni_colonna);
    int* lunghezza_output = malloc(sizeof(int));
    *lunghezza_output = *riga_attualee_per_colonna;
    char** outputText = malloc(*lunghezza_output * sizeof(*outputText));
    initiArr(outputText, 0, *riga_attualee_per_colonna, *lunghezza_totale);

    int* nuova_pagine = malloc(sizeof(int));
    int* contatore_parole = malloc(sizeof(int));
    int* contatore_caratteri = malloc(sizeof(int));
    int* prima_riga_della_pagina = malloc(sizeof(int));
    int* riga = malloc(sizeof(int));
    int* colonna = malloc(sizeof(int));
    */
    int* pageLength = malloc(sizeof(int));
    *pageLength = lenPage(*nColumn, *columnWidth, *distanceColumn);
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
    char* pathFile = getPath(input_path);
    FILE *file = fopen(pathFile, "r");

    char* buff_str = malloc(1024*sizeof(char*));
    for(char* riga_attuale = fgets(buff_str, 1024, file); riga_attuale != NULL; riga_attuale = fgets(buff_str, 1024, file)){
        riga_attuale = strtok(riga_attuale, "\n\r");
        if(riga_attuale != NULL){
            for(char* token = strtok(riga_attuale, " "); token != NULL; token = strtok(NULL, " "))
            {
                if ( (*conspazi+lenWord) < *columnWidth) {    
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
            outputText = new_paragraph(outputText, bufferArray, nuova_pagine, riga, colonna,
                                                lunghezza_output, riga_attualee_per_colonna, prima_riga_della_pagina, lunghezza_totale, contatore_parole, caratteri_ogni_colonna,
                                                contatore_caratteri, spazio_tra_colonne, numero_colonne);
        }
    }

    writeFile(getPath("single_thread_output.txt"), outputText, lunghezza_output, lunghezza_totale);
    return 0;
}