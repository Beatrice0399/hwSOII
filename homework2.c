#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*
int main(int argc, char ** argv) {
    char stringa[10];
    stringa[0] = 'H';
    //printf("%c", stringa[0]);
    char* str = "Stringa";
    for (int i = 0; i <= strlen(str); i++ ) {
        stringa[i] = str[i];
    }
    printf("%s", stringa);
    return 0;
} 
*/
char* getPath(const char* txt_path);
char* readFile(char* path);
char* concat(const char *s1, const char *s2);
int len(char* word);
int lenPage(int n_column, int column_width, int distance_column);
int spaceWord(int len_column, int len_words, int n_words);
void newLine(int riga, int colonna, char** matrix);
void newPage(int riga, int colonna, char** matrix); 
void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice);
int* arrayTxt(int space, int *nwords, int* puntatore, char** array, char** outputText, int countColumn, int distanceColumn, int nColumn, int nrighe);
void writeText(char** array);
void inizializza(char** array, int n);
int* noJustified(int *nwords, int* puntatore, char** array, char** outputText, int columnWidt, int conSpazi, int distanceColumn, int ncolumn, int countColumn);
void printArray(char** a, int righe);
int* emptyRow(int* puntatore, char** outputText, int columnWidth, int distanceColumn, int ncolumn, int countColumn);

int main(int argc, char ** argv) {
    char* nameFile = argv[0];
    char* pathText = argv[1];
    /*
    int nColumn = atoi(argv[2]);
    int linesPerColumn = atoi(argv[3]);
    int columnWidth = atoi(argv[4]);
    int distanceColumn = atoi(argv[5]);
    */
    int nColumn = 3;
    int linesPerColumn = 4;    //6
    int columnWidth = 25;      //12
    int distanceColumn = 7;   

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char* pathFile = getPath("Input2.txt");
    char* input_text = readFile(pathFile);
    //char* token = strtok(text, " ");
    int* nwords = malloc(sizeof(int));
    //*nwords = columnWidth/2;
    //int** n_of_word = malloc(sizeof(int));
    char** array = malloc(columnWidth/2*sizeof(*array));
    for (int i = 0; i < columnWidth/2; i++) {
        array[i] = malloc(50*sizeof(*array[i]));    
    }
    int* puntatoreTxt = malloc(sizeof(int));
    char** outputText = malloc(linesPerColumn*sizeof(*outputText));
    for (int i = 0; i < columnWidth; i++) {
        outputText[i] = malloc(1000*sizeof(*outputText[i]));     //50
    }
    int* countRow = malloc(sizeof(int));
    int countColumn = 0;
    int countPage = 0;
    int c = 0;
    int conspazi = 0;
    for (char* token = strtok(input_text, " "); token != NULL; token = strtok(NULL, " \n")) {
        int lenWord = len(token);
        if (*puntatoreTxt == linesPerColumn) {
            if (countColumn == nColumn) {
                *puntatoreTxt +=1;
                countRow = 0;
                countPage += 1;  //nuova pagina 
                //INSERIRE FUNZIONE PER %%%
            }
            *puntatoreTxt = (linesPerColumn+3)*countPage;
        }
        if (!strcmp(token, "\r")) {
            puntatoreTxt = noJustified(nwords, puntatoreTxt, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn);
            //RIGAVUOTA CON SPAZI
            puntatoreTxt = emptyRow(puntatoreTxt, outputText, columnWidth, distanceColumn, nColumn, countColumn);
            countRow += 2;
            //strcpy(outputText[*puntatoreTxt], "\n");
            //*puntatoreTxt += 1;
            inizializza(array, columnWidth);
        }
        else if ( (conspazi+lenWord) <= columnWidth) {    //<=
            c = c + lenWord;
            conspazi = conspazi + lenWord + 1;
            //strcat(token, " ");
            strcpy(array[*nwords], token);
            *nwords += 1;
        }
        else {
            int space = spaceWord(columnWidth, c, *nwords);
            puntatoreTxt = arrayTxt(space, nwords, puntatoreTxt, array, outputText, countColumn, distanceColumn, nColumn, linesPerColumn);
            countRow +=1;
            printArray(outputText, countRow);
            //printf("\n\n\n");
            c = len(token);
            conspazi = lenWord + 1;         
            *nwords = 0;
            inizializza(array, columnWidth);
            strcpy(array[*nwords], token);  
            *nwords += 1;
        }
    }

    puntatoreTxt = noJustified(nwords, puntatoreTxt, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn);
    printArray(outputText, countRow);
    return 0;
}

int* arrayTxt(int space, int *nwords, int* puntatore, char** array, char **outputText, int countColumn, int distanceColumn, int nColumn, int nrighe) {
    int count = 0;
    for (int i = 0; i < *nwords; i++) {
        char* str = malloc(1000* sizeof(*str));                     //100
        strcpy(str, strcat(outputText[*puntatore], array[i]));
        count ++;                
        if (count < *nwords) {
            for (int j = 0; j <= space; j++) {
                strcpy(outputText[*puntatore], strcat(str, " "));
            }
        }
        else if (countColumn < nColumn) {
            for (int i = 0; i <= distanceColumn; i++) {
                strcpy(outputText[*puntatore], strcat(str, " "));
            }
        }
    }
    if (*puntatore == nrighe && countColumn < nColumn) {
        //countRow = 0;
        *puntatore = 0;
    }
    else {
        *puntatore += 1;
    }
    return puntatore;
}

int* noJustified(int *nwords, int* puntatore, char** array, char** outputText, int columnWidth, int conSpazi, int distanceColumn, int ncolumn, int countColumn){
    int count = 0;
    int lenParole = 0;
    for (int i = 0; i < *nwords; i++) {
        char* str = malloc(100* sizeof(*str));
        lenParole += len(array[i]);
        strcpy(str, strcat(outputText[*puntatore], array[i]));
        count ++;                
        if (count < *nwords) {
            strcpy(outputText[*puntatore], strcat(str, " "));
        }
        if (countColumn < ncolumn) {
            int spazi = columnWidth - conSpazi + distanceColumn;
            for (int i = 0; i <= spazi; i++) {
                strcpy(outputText[*puntatore], strcat(str, " "));
            }
        }
    }
    *puntatore += 1;
    return puntatore;
}

int* emptyRow(int* puntatore, char** outputText, int columnWidth, int distanceColumn, int ncolumn, int countColumn){
    char* str = malloc(100* sizeof(*str));
    strcpy(str, strcat(outputText[*puntatore], " "));
    if (countColumn < ncolumn) {
        int totSpace = columnWidth + distanceColumn;
        for (int i = 0; i < totSpace; i++) {
            strcpy(outputText[*puntatore], strcat(str," "));
        }
    }
    else {
        for (int i = 0; i < columnWidth; i++) {
            //strcpy(outputText[*puntatore], " ");
            strcpy(outputText[*puntatore], strcat(str," "));
            //strcpy(outputText[*puntatore], strcat(outputText[*puntatore]," "));
        }
    }
    *puntatore +=1;
    return puntatore;
}

void printArray(char** a, int righe) {
    for(int i = 0; i < righe; i++){
        printf("%s\n", a[i]);
        }
}

void inizializza(char** array, int n) {
    for (int i = 0; i < n/2; i++) {
                array[i] = malloc(50*sizeof(*array[i]));    
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

char* readFile(char* path){
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

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
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
        for (int i = 0; i < strlen(word); i ++)
        {
            char ch = word[i];
            int val = word[i];
            if (val >= 0)
            {
                lenW ++;
            }
            else { lenW = lenW + 0.5; }
        }
    int n = (int) lenW;
    n -= 1;
    char ch = word[n];
    char ch2 = word[n-1];
    if (ch == '\r') {
        lenW = lenW -1;
    }
    return (int) lenW;
}

//caratteri totali per riga
int lenPage(int n_column, int column_width, int distance_column){
    int i = column_width*n_column + distance_column*(n_column-1);
    return i;
}

//spazi tra le parole di ogni riga
int spaceWord(int len_column, int len_words, int n_words){
    int emptySpaces = len_column - len_words;
    emptySpaces = emptySpaces/(n_words-1);
    return emptySpaces;
}

void newLine(int riga, int colonna, char** matrix) {
    char nextLine[2] = "\n";
    for (int i = 0; i < 2; i++) {
        matrix[riga][colonna] = nextLine[i];
        colonna ++;
    }
    riga ++;
}

void newPage(int riga, int colonna, char** matrix) {
    char nextPage[7] = "\n%%%\n";
    for (int i = 0; i < 7; i++) {
        matrix[riga][colonna] = nextPage[i];
        colonna ++;
    }
    riga ++;
} 

void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice){
    for (int i = 1; i <= spazio_colonne; i++) {
        matrice[riga][colonna + i] = ' ';
    }
}