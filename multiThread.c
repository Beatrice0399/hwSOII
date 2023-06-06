#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "format.h"
#include "fileIO.h"

int* nPage;
int* nColumn;
int* columnWidth;
int* linesPerColumn;
int* distanceColumn;

char** outputText;

char* riga_attuale;
int* pageLength;

int* thread_attivo;
int* thread_one_running;
int* thread_two_running;
int* thread_three_running;

void *thread_one(char* path);
void *thread_two(char* delimitatore);
void *thread_three(char* path);

int main(int argc, char const *argv[])
{
    //Settings
    char* pathText = malloc(sizeof(*pathText));
    char* outputPath = malloc(sizeof(char) * 1024);
    strcpy(outputPath, "multi_output.txt");
    nColumn = malloc(sizeof(int));
    linesPerColumn = malloc(sizeof(int));    //6
    columnWidth = malloc(sizeof(int));      //12
    distanceColumn = malloc(sizeof(int)); 

    strcpy(pathText, argv[1]);
    *nColumn = atoi(argv[2]);
    *linesPerColumn = atoi(argv[3]);
    *columnWidth = atoi(argv[4]);
    *distanceColumn = atoi(argv[5]);

    pageLength = malloc(sizeof(int));
    *pageLength = lenPage(nColumn, columnWidth, distanceColumn);

    riga_attuale = malloc(sizeof(char)* 2048);

    //Initialize both bufferArray buffers
    outputText = malloc(*linesPerColumn*sizeof(*outputText));
    inizializza(outputText, linesPerColumn, pageLength);

    char* delimitatore = malloc(sizeof(char) * 4);
    strcpy(delimitatore, " ");

    nPage = malloc(sizeof(int)); 
    *nPage = 1;

    //inizializzo le variabili per gestire i thread
    thread_attivo = malloc(sizeof(int));
    thread_one_running = malloc(sizeof(int));
    thread_two_running = malloc(sizeof(int));
    thread_three_running = malloc(sizeof(int));

    *thread_attivo = 1;
    *thread_one_running = 1;
    *thread_two_running = 1;
    *thread_three_running = 1;

    pthread_t tid[3];
    int ft, st, tt;
    void* t;


    //Creo i thread
    ft = pthread_create(&tid[0], NULL, &thread_one, pathText);
    st = pthread_create(&tid[1], NULL, &thread_two, delimitatore);
    tt = pthread_create(&tid[2], NULL, &thread_three, outputPath);

    //Aspetto che tutti i thread abbiano terminato 
    ft = pthread_join(tid[0], &t);
    st = pthread_join(tid[1], &t);
    tt = pthread_join(tid[2], &t);

    return 0;
}
/**
 * \brief la funzione legge il file in input, riga per riga
 * \param path
 * \return void
 * */
void *thread_one(char* path){
    char* pathFile = getPath(path);
    FILE *file = fopen(pathFile, "r");
    char* buff_str = malloc(1024*sizeof(char*));
    while(*thread_one_running){
        if(*thread_attivo == 1){
            char* riga = fgets(buff_str, 2048, file);
            if(riga){
                riga = strtok(riga, "\n\r");
                //Se la riga esiste, viene passata al secondo thread per essere formattata
                if(riga){
                    strcpy(riga_attuale, riga);
                    *thread_attivo = 2;
                }
            }
            //quando il file in input è terminato, viene attivato il thread per scrivere il testo formattato sul file output
            else{
                *thread_one_running = 0;
                *thread_two_running = 0;
                *thread_attivo = 3;

            }
        }
    }
    pthread_exit(0);
}

/** \brief La funzione richiama al suo interno le funzioni necessarie per la formattazione del testo 
*   \param delimitatore
*   \return void
*/
void *thread_two(char* delimitatore){
    int* currentRow = malloc(sizeof(int));      //per scrivere sull'bufferArray di output
    int* nwords = malloc(sizeof(int));          //per contare il numero di parole che entrano su una riga di una colonna
    int* countRow = malloc(sizeof(int));        //per tenere traccia delle righe scritte in una colonna
    int* countColumn = malloc(sizeof(int));     //per tenere traccia del numero di colonne scritte
    int* startRow = malloc(sizeof(int));        //puntatore per tornare all'inizio della colonna successiva (quando non si è più nella prima pagina)
    int* conspazi = malloc(sizeof(int));

    char** bufferArray = malloc(*columnWidth/2*sizeof(*bufferArray)); 
    inizializza(bufferArray, columnWidth, columnWidth);

    while(*thread_two_running){
        if(*thread_attivo == 2){
            for(char* token = strtok(riga_attuale, delimitatore); token != NULL; token = strtok(NULL, delimitatore))
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
            *thread_attivo = 1;

        }
    }
    pthread_exit(0);
}

/** \brief la funzione richiama la funzione per scrivere il testo sul file output
 *  \param path
 *  \return void
 * */
void *thread_three(char* path){
    char* pathFile = getPath(path);
    while(*thread_three_running){
        
        if(*thread_attivo == 3){
            writeText(pathFile, outputText, linesPerColumn, nPage);
            *thread_three_running = 0;
        }
    }
    pthread_exit(0);
}