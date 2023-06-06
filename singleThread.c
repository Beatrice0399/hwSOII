#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "format.h"
#include "fileIO.h"
/* Dati in input i parametri e dopo aver inizializzato le variabili necessarie, attraverso un ciclo for leggo ogni riga del testo che rappresenta 
un paragrafo. Per ogni riga del testo in input, calcolo la lunghezza della singola parola, se questa rientra nella lunghezza del numero di caratteri
per colonna, allora inserisco la parola nel bufferArray, aumento il contatore del numero di parole per riga e aumento la variabile
con spazi della lunghezza della parola +1 (spazio). Ripeto il ciclo finchè la nuova parola non eccede il numero di caratteri per colonna.
Viene invocata la funzione per giustifare le parole contenute all'interno del bufferArray e verranno copiate nell'array oputputText che verrà poi 
scritto nel file output.txt . Se siamo alla fine del paragrafo, allora le parole non verranno giustificate al margine destro e verrà aggiunta 
una riga vuota per dividere un paragrafo dall'altro. 
*/

int main(int argc, char const *argv[])
{
    //Settings
    char* pathText = malloc(sizeof(*pathText));     //nome del file in input
    int* nColumn = malloc(sizeof(int));             //numero di colonne 
    int* linesPerColumn = malloc(sizeof(int));      //numero di righe per colonna
    int* columnWidth = malloc(sizeof(int));         //numero di caratteri per colonna
    int* distanceColumn = malloc(sizeof(int));      //distanza tra una colonna e l'altra

    strcpy(pathText, argv[1]);
    *nColumn = atoi(argv[2]);
    *linesPerColumn = atoi(argv[3]);
    *columnWidth = atoi(argv[4]);
    *distanceColumn = atoi(argv[5]);

    int* pageLength = malloc(sizeof(int));
    *pageLength = lenArrayOutput(nColumn, columnWidth, distanceColumn);

    //array bidimensionale per inserire le parole che dovranno essere giustificate in una riga del testo 
    char** bufferArray = malloc(*columnWidth/2*sizeof(*bufferArray)); 
    inizializza(bufferArray, columnWidth, columnWidth);

    //array bidimensionale per inserire il testo formattato e giustificato che verrà scritto sul file in output
    char** outputText = malloc(*linesPerColumn*sizeof(*outputText));
    inizializza(outputText, linesPerColumn, pageLength);

    int* currentRow = malloc(sizeof(int));      //per scrivere sull'bufferArray di output
    int* nwords = malloc(sizeof(int));          //per contare il numero di parole che entrano su una riga di una colonna
    int* countRow = malloc(sizeof(int));        //per tenere traccia delle righe scritte in una colonna
    int* countColumn = malloc(sizeof(int));     //per tenere traccia del numero di colonne scritte
    int* startRow = malloc(sizeof(int));        //puntatore per tornare all'inizio della colonna successiva (quando non si è più nella prima pagina)
    int* nPage = malloc(sizeof(int));           //contatore per il numero di pagine scritte
    int* conspazi = malloc(sizeof(int));        //lughezza delle parole, compresi gli spazi, che possono essere scritte in una riga; utilzzata per la funzione che non giustifica il testo
    *nPage = 1;

    char* pathFile = getPath(pathText);
    FILE *file = fopen(pathFile, "r");

    char* buff_str = malloc(1024*sizeof(char*));
    //leggo ogni paragrafo del testo in input
    for(char* riga_attuale = fgets(buff_str, 1024, file); riga_attuale != NULL; riga_attuale = fgets(buff_str, 1024, file)){
        riga_attuale = strtok(riga_attuale, "\n\r");
        if(riga_attuale != NULL){
            //leggo ciascuna parola del paragrafo
            for(char* token = strtok(riga_attuale, " "); token != NULL; token = strtok(NULL, " "))
            {
                int lenWord = len(token);
                //se la parola (token) entra nella riga, allora verrà inserita nel bufferArray
                if ( (*conspazi+lenWord) <= *columnWidth) {    
                *conspazi = *conspazi + lenWord + 1;
                strcpy(bufferArray[*nwords], token);
                *nwords += 1;
                }
                //altrimenti scrivo le parole, contenute nel bufferArray, nell'array outputText
                else {
                    //se nella riga può essere scritta una sola parola, allora verrà invocata la funzione per scrivere il testo non giustificato
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
            //L'ultima riga di ogni paragrafo verrà scritta non giustificata
            outputText = noJustified(nwords, currentRow, bufferArray, outputText, columnWidth, conspazi, distanceColumn, 
                                                    nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
            //la riga vuota tra un paragrafo e l'altro non verrà aggiunta se la successiva riga da scrivere risulta essere la prima di una colonna
            if (*currentRow != *startRow) {
            outputText = emptyRow(currentRow, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, 
                                    linesPerColumn, startRow, pageLength, nPage);
            }
            //*conspazi = 0;
            //*nwords = 0;
        }
    }
    char* path = malloc(1024* sizeof(char));
    strcpy(path, "single_output.txt");
    writeText(path, outputText, linesPerColumn, nPage);

    return 0;
}
