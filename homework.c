#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* getPath(const char* txt_path);

char* readFile(char* path);

char* concat(const char *s1, const char *s2);

int len(char* word);

void addSpace(char** str, int* row, int spazi);

int lenPage(int n_column, int column_width, int distance_column);

void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces);

char** newLine(char** outputText, int* currentRow, int* countRow, int* startRow, int* linesPerColumn,
     int* countColumn, int* nColumn, int* total_length, int* nPage);

void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice);

char** justify(int *nwords, int* currentRow, char** array, char **outputText, int* countColumn, int* distanceColumn, int* nColumn, int* countRow,
    int* linesPerColumn, int* startRow, int* columnWidth, int* total_length, int* nPage);

char** newParagraph(char** outputText, char** array, char* token, char* occurrence, int* conspazi, int* columnWidth, int* nwords, 
    int* currentRow, int* distanceColumn, int*nColumn, int* countColumn, int* linesPerColumn, int* countRow, int* startRow, int* pageLength, int* nPage); 

void writeText(char** array);

void inizializza(char** array, int* size, int* length);

char** noJustified(int *nwords, int* currentRow, char** array, char** outputText, int* columnWidt, int* conSpazi, int* distanceColumn, int* ncolumn, 
    int* countColumn, int* linesPerColumn, int* countRow, int* startRow, int* total_length, int* nPage);

void printArray(char** a, int* linesPerColumn, int* startRow);

char** emptyRow(int* currentRow, char** outputText, int* columnWidth, int* distanceColumn, int* ncolumn, int* countColumn, int* countRow,
     int*linesPerColumn, int* startRow, int* total_length, int* nPage);

char** newPage(char** outputText, int* currentRow, int*startRow, int* linesPerColumn, int* total_length, int*nPage); 


int main(int argc, char ** argv) {
    char* nameFile = argv[0];
    char* pathText = argv[1];
    /*
    int nColumn = atoi(argv[2]);
    int linesPerColumn = atoi(argv[3]);
    int columnWidth = atoi(argv[4]);
    int distanceColumn = atoi(argv[5]);
    */
    //Settings
    int* nColumn = malloc(sizeof(int));
    int* linesPerColumn = malloc(sizeof(int));    //6
    int* columnWidth = malloc(sizeof(int));      //12
    int* distanceColumn = malloc(sizeof(int));   
    *nColumn = 3;
    *linesPerColumn = 40;
    *columnWidth = 25;
    *distanceColumn = 7;

    //Input text
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
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

    int* currentRow = malloc(sizeof(int));             //per scrivere sull'array di output
    int* nwords = malloc(sizeof(int));          //per contare il numero di parole che entrano su una riga di una colonna
    int* countRow = malloc(sizeof(int));        //per tenere traccia delle righe scritte in una colonna
    int* countColumn = malloc(sizeof(int));     //per tenere traccia del numero di colonne scritte
    int* startRow = malloc(sizeof(int));       //puntatore per tornare all'inizio della colonna successiva (quando non si è più nella prima pagina)
    int* nPage = malloc(sizeof(int));
    int* conspazi = malloc(sizeof(int));
    *nPage = 1;

    for (char* token = strtok(input_text, " "); token != NULL; token = strtok(NULL, " ")) {
        int lenWord = len(token);
        char* occurrence = strstr(token, "\r\n\r\n");
        //printArray(outputText, linesPerColumn, nPage);
        if (occurrence) {
            outputText = newParagraph(outputText, array, token, occurrence, conspazi, columnWidth, nwords, currentRow, distanceColumn, nColumn, 
                            countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
        }
        else if ( (*conspazi+lenWord) <= *columnWidth) {    
            *conspazi = *conspazi + lenWord + 1;
            strcpy(array[*nwords], token);
            *nwords += 1;
        }
        else {
            if (*nwords == 1) {
                *conspazi -=1;
                outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
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
    outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
    //printArray(outputText, countRow);
    printArray(outputText, linesPerColumn, nPage);

    return 0;
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
    return outputText;
}

//funzione per scrivere la riga non allineata al margine sinistro
char** noJustified(int *nwords, int* currentRow, char** array, char** outputText, int* columnWidth, int* conSpazi, int* distanceColumn, int* ncolumn, int* countColumn, 
            int* linesPerColumn, int* countRow, int* startRow, int* total_length, int* nPage){
    int count = 0;
    int lenParole = 0;
    for (int i = 0; i < *nwords; i++) {
        lenParole += len(array[i]);
        strcat(outputText[*currentRow], array[i]);
        count ++;                
        if (count < *nwords) {
            strcat(outputText[*currentRow], " ");
        }
    }
    if(*countColumn < *ncolumn -1 ) {
        int spazi = *columnWidth - *conSpazi + *distanceColumn;
        addSpace(outputText, currentRow, spazi);
    }
    else {
        int spazi = *columnWidth - *conSpazi;
        addSpace(outputText, currentRow, spazi);
    }
    outputText = newLine(outputText, currentRow, countRow, startRow, linesPerColumn, countColumn, ncolumn, total_length, nPage);
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

//funzione per scrivere una riga vuoto, riempita da spazi, usata per creare i nuovi paragrafi
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

//crea un nuovo paragrafo nel testo
char** newParagraph(char** outputText, char** array, char* token, char* occurrence, int* conspazi, int* columnWidth, int* nwords, 
    int* currentRow, int* distanceColumn, int*nColumn, int* countColumn, int* linesPerColumn, int* countRow, int* startRow, int* pageLength, int* nPage) 
{
    int lenToken1 = len(token) - len(occurrence);
    char* token1 = malloc(sizeof(token1));
    strncpy(token1, token, lenToken1);
    char* token2 = malloc(sizeof(token2));
    strncpy(token2, &occurrence[4], len(token)-1 );
    //se l'ultima parola del paragrafo entra nella riga, allora la riga viene scritta senza essere allineata al bordo sinistro
    if ( (*conspazi+lenToken1) <= *columnWidth) {   
        *conspazi = *conspazi + lenToken1;
        strcpy(array[*nwords], token1);
        *nwords += 1;
        outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
        if (*currentRow != *startRow) {
            outputText = emptyRow(currentRow, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, linesPerColumn, startRow, pageLength, nPage);
        }
        //printArray(outputText, linesPerColumn, startRow);
    }
    //altrimenti l'ultima parola del paragrafo viene scritta nella riga successiva e non viene allineata a sinistra
    else {   
        if (*nwords == 1) {
            noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi+1, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
        }  
        else {
            outputText = justify(nwords, currentRow, array, outputText, countColumn, distanceColumn, nColumn, countRow, 
            linesPerColumn, startRow, columnWidth, pageLength, nPage);
            *nwords = 0;
            strcpy(array[*nwords], token1);
            *nwords +=1;
            *conspazi = lenToken1; 
            outputText = noJustified(nwords, currentRow, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, startRow, pageLength, nPage);
            if (*currentRow != *startRow) {
                outputText = emptyRow(currentRow, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, linesPerColumn, startRow, pageLength, nPage);
            }
        }
    }
    *nwords = 1;
    strcpy(array[0], token2);       //Prima parola del nuovo paragrafo
    *conspazi = len(token2) +1;
    return outputText;
}

//funzione per creare la nuova pagina
char** newPage(char** outputText, int* currentRow, int*startRow, int* linesPerColumn, int* total_length, int* nPage){
    int lungh = (*linesPerColumn + 1)*(*nPage+1) - 1;
    *outputText = realloc(*outputText, sizeof(char*) * lungh);
    int riga = *currentRow;
    for (int i = riga; i < lungh; i++) {
        outputText[i] = malloc(*total_length*sizeof(*outputText[i])); 
    }
    *currentRow = riga;
    int spazi = *total_length;
    strcat(outputText[*currentRow], "\n%%%\n");
    *currentRow += 1;
    *startRow = *currentRow;
    *nPage +=1;
    return outputText;
} 

void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice){
    for (int i = 1; i <= spazio_colonne; i++) {
        matrice[riga][colonna + i] = ' ';
    }
}

//spazi tra le parole di ogni riga
void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces){
    for(int i = 0; i < remaining_spaces; i++){
        array_spaces[i % *n_spaces] ++;
    }
}

void addSpace(char** outputText, int* currentRow, int spazi){
    for(int i = 0; i < spazi; i++){
        strcat(outputText[*currentRow], " ");      
    }
}

//funzione per inizializzare gli array
void inizializza(char** array, int* size, int* length) {
    for (int i = 0; i < *size; i++) {
                array[i] = malloc(*length*sizeof(*array[i]));    
            }
}

void writeText(char** outputText) {
    FILE *fp = fopen("output.txt", "w+"); 
    if (fp == NULL){ 
        printf("file non trovato");
        exit(-1); 
    }
    for (int i = 0; i <15; i++) {
        //fprintf(fp, outputText[i]);
    }
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

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result; 
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

//caratteri totali per riga
int lenPage(int n_column, int column_width, int distance_column){
    int i = column_width*n_column + distance_column*(n_column-1) +1;
    return i;
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