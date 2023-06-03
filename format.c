#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "format.h"

/**
* \brief La funzione calcola la lunghezza necessaria per inizializzare l'array bidimensionale che conterrà il testo formattato.
* \param n_column numero di colonne nel testo
* \param column_width caratteri per colonna
* \param distance_column puntatore alla variabile che rappresenta la distanza tra una colonna e l'altra
* \return \c int un intero che rappresenta la lunghezza delle righe nell'array bidimensionale
* */
int lenArrayOutput(int* n_column, int* column_width, int* distance_column){
    int i = (*column_width*(*n_column))*2 + *distance_column*(*n_column-1) +1;
    return i;
}

/**
* \brief La funzione aggiunge tanti spazi quanti sono quelli dati in input
* \param outputText
* \param currentRow
* \param spazi intero, numero di spazi da inserire nell'outputText
* \return \c void
* */
void addSpace(char** outputText, int* currentRow, int spazi){
    for(int i = 0; i < spazi; i++){
        strcat(outputText[*currentRow], " ");      
    }
}

/**
* \brief data un puntatore ad un array di caratteri (stringa) viene calcolata la lunghezza della parola, considerando il valore dei singoli 
*       \b char. I caratteri speciali non possono essere rappresentati con un byte perciò quando si incontreranno due valori negativi, 
*       il contatore della lunghezza della parola verrà aumentato di 1.
* \param word \c char* parola di cui si vuole calcolare la lunghezza
* \return \c int 
* */
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

/**
* \brief La funzione inizializza l'array bidimensionale.
* \param array
* \param size
* \param length
* \return \c void
* */
void inizializza(char** array, int* size, int* length) {
    for (int i = 0; i < *size; i++) {
        array[i] = malloc(*length*sizeof(*array[i]));    
    }
}

/**
* \brief Dati i seguenti parametri in input, la funzione giustifica il testo in un una colonna al margine destro e sinistro. 
*       Se non sono state scritte il numero massino di colone \b nColumn, allora verranno aggiunti tanti spazi quanta la distanza 
*       tra una colonna e l'altra, in modo che le parole della colonna successiva siano già allineate al margine sinistro. Dopo aver giustificato
*       il testo verrà invocata la funzione \b newLine per aumentare i contatori della righe scritte.
* \param nwords
* \param currentRow
* \param array
* \param outputText
* \param countColumn
* \param distanceColumn
* \param nColumn
* \param countRow
* \param linesPerColumn
* \param startRow
* \param columnWidth
* \param total_length
* \param nPage
* \return \c char** outputText
* */
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

/**
* \brief La funzione scrive la riga senza giustificare il testo al margine destro, quindi verrà aggiunto solo uno spazio tra una parola e 
*       l'altra e saranno aggiunti spazi vuoti per raggiungere la lunghezza della riga. Se non sono state scritte il numero massino di colone 
*       \b nColumn, allora verranno aggiunti tanti spazi quanta la distanza tra una colonna e l'altra, in modo che le parole della colonna 
*       successiva siano già allineate al margine sinistro. Dopo aver giustificato il testo verrà invocata la funzione \b newLine per aumentare 
*       i contatori della righe scritte.
* \param nwords
* \param currentRow
* \param array
* \param outputText
* \param columnWidth
* \param conspazi
* \param distanceColumn
* \param nColumn
* \param countColumn
* \param linesPerColumn
* \param countRow
* \param startRow
* \param total_length
* \param nPage
* \return \c char** outputText
* */
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
/**
* \brief La funzione è utilizzata per aumentare i contatori relativi alle righe scritte nella pagina. Se il numero delle righe scritte è uguale
*       al numero \b linesPerColumn e se non sono state scritte il massimo numero di colonne \b nColumn, allora al puntatore della riga verrà
*       assegnato il valore \b startRow; se è stato raggiunto anche il numero massimo di colonne, allora verrà invocata la funzione per la 
*       creazione della nuova pagina.
* \param outputText
* \param currentRow
* \param countRow
* \param startRow
* \param linesPerColumn
* \param countColumn
* \param nColumn
* \param total_length
* \param nPage
* \return \c char** outputText
* */
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
/**
* \brief La funzione aggiunge una riga vuota rappresenta da tanti spazi quanti sono i caratteri per colonna. Se riga non viene scritta 
*        nell'ultima colonna, allora verranno aggiunti anche gli spazi di distanza tra una colonna e l'altra. Dopo aver aggiunto la riga vuota
*        nel testo, verrà invocata la funzione \b newLine per aumentare i contatori della righe scritte.
* \param currentRow
* \param outputText
* \param columnWidth
* \param distanceColumn
* \param nColumn
* \param countColumn
* \param countRow
* \param linesPerColumn
* \param startRow
* \param total_length
* \param nPage
* \return \c char** outputText
* */
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

/**
* \brief La funzione crea una nuova pagina riallocando l'array bidimensionale contenente il testo giustificato, con la funzione \b realloc.
*        Alla variabile \b startRow sarà assegnato il valore corrispondente all'indice che punta alla riga che rappresenta la prima riga
*        della nuova pagina e verrà aumentato il contatore \b nPage del numero di pagine. 
* \param outputText
* \param currentRow
* \param startRow
* \param linesPerColumn
* \param total_length
* \param nPage
* \return \c char** outputText con la nuova dimensione
* */
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
/**
* \brief 
* \param array_spaces
* \param n_spaces
* \param remaining_spaces
* \return \c void
* */
void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces){
    for(int i = 0; i < remaining_spaces; i++){
        array_spaces[i % *n_spaces] ++;
    }
}