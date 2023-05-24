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
void addSpace(char** str, int* row, int spazi);
int lenPage(int n_column, int column_width, int distance_column);
void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces);
void newLine(int riga, int colonna, char** matrix);
void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice);
void arrayTxt(int *nwords, int* row, char** array, char **outputText, int* countColumn, int* distanceColumn, int* nColumn, int* countRow,
    int* linesPerColumn, int* countPage, int* columnWidth, int* total_length);
void writeText(char** array);
void inizializza(char** array, int* size, int* length);
void noJustified(int *nwords, int* row, char** array, char** outputText, int* columnWidt, int conSpazi, int* distanceColumn, int* ncolumn, int* countColumn, int* linesPerColumn, int* countRow, int* countPage, int* total_length);
void printArray(char** a, int* linesPerColumn, int* countPage);
void emptyRow(int* row, char** outputText, int* columnWidth, int* distanceColumn, int* ncolumn, int* countColumn, int* countRow, int*linesPerColumn, int* countPage, int* total_length);
void newPage(char** outputText, int* row, int*countPage, int* linesPerColumn, int* total_length); 

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
    *nColumn = 2;
    *linesPerColumn = 3;
    *columnWidth = 20;
    *distanceColumn = 7;

    //Input text
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char* pathFile = getPath("es.txt");
    char* input_text = readFile(pathFile);
    //Total length
    int* pageLength = malloc(sizeof(int));
    *pageLength = lenPage(*nColumn, *columnWidth, *distanceColumn);
    //Initialize both array buffers
    char** array = malloc(*columnWidth/2*sizeof(*array));
    inizializza(array, columnWidth, columnWidth);

    char** outputText = malloc(*linesPerColumn*sizeof(*outputText));
    inizializza(outputText, linesPerColumn, pageLength);



    int* row = malloc(sizeof(int));
    int* nwords = malloc(sizeof(int));
    int* countRow = malloc(sizeof(int));
    int* countColumn = malloc(sizeof(int));
    int* countPage = malloc(sizeof(int));
    int conspazi = 0;

    for (char* token = strtok(input_text, " "); token != NULL; token = strtok(NULL, " ")) {
        int lenWord = len(token);
        char* occurrence = strstr(token, "\r\n\r\n");
        if (occurrence) {
            //NoJUSTIFY
            //printf("si");
            int lenToken1 = len(token) - len(occurrence);
            char* token1 = malloc(sizeof(token1));
            strncpy(token1, token, lenToken1);
            char* token2 = malloc(sizeof(token2));
            strncpy(token2, &occurrence[4], lenWord-1 );
            if ( (conspazi+lenToken1) <= *columnWidth) {    //<=
                conspazi = conspazi + lenToken1;
                //strcat(token, " ");
                strcpy(array[*nwords], token1);
                *nwords += 1;
                // NO JUSTIFY
                noJustified(nwords, row, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, countPage, pageLength);
                //*nwords = 1;
                //strcpy(array[0], token2);
                //conspazi = len(token2) +1;
                if (*row != *countPage) {
                    emptyRow(row, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, linesPerColumn, countPage, pageLength);
                }
                printArray(outputText, linesPerColumn, countPage);
    
            }
            else {
                
                arrayTxt(nwords, row, array, outputText, countColumn, distanceColumn, nColumn, countRow, 
                linesPerColumn, countPage, columnWidth, pageLength);
                *nwords = 0;
                strcpy(array[*nwords], token1);
                *nwords +=1;
                conspazi = lenToken1 +1;
                noJustified(nwords, row, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, countPage, pageLength);
                if (*row != *countPage) {
                    emptyRow(row, outputText, columnWidth, distanceColumn, nColumn, countColumn, countRow, linesPerColumn, countPage, pageLength);
                }
            }
            *nwords = 1;
            strcpy(array[0], token2);
            conspazi = len(token2) +1;
        }
       /*
        if (*countRow == (*linesPerColumn)) {
            if (countColumn == *nColumn) {
                *row +=1;
                *countRow = 0;
                *countPage += 1;  //nuova pagina 
                //INSERIRE FUNZIONE PER %%%
            }
            else {
                *row = (*linesPerColumn+3)**countPage;
            }
            //*row += 2;
        }
        */
        else if ( (conspazi+lenWord) <= *columnWidth) {    //<=
            conspazi = conspazi + lenWord + 1;
            //strcat(token, " ");
            strcpy(array[*nwords], token);
            *nwords += 1;
        }
        else {
            arrayTxt(nwords, row, array, outputText, countColumn, distanceColumn, nColumn,
                                    countRow, linesPerColumn, countPage, columnWidth, pageLength);
            printArray(outputText, linesPerColumn, countPage);
            //*countRow += 1;
            conspazi = lenWord + 1;         
            *nwords = 0;
            strcpy(array[*nwords], token);  
            *nwords += 1;
        }
    }

    noJustified(nwords, row, array, outputText, columnWidth, conspazi, distanceColumn, nColumn, countColumn, linesPerColumn, countRow, countPage, pageLength);
    //printArray(outputText, countRow);
    return 0;
}

void arrayTxt(int *nwords, int* row, char** array, char **outputText, int* countColumn, int* distanceColumn, int* nColumn, int* countRow,
    int* linesPerColumn, int* countPage, int* columnWidth, int* total_length) {
    int n_of_char = 0;
    for(int i = 0; i < *nwords; i++){
        n_of_char += len(array[i]);
    }
    int* n_spaces = malloc(sizeof(int));
    *n_spaces = *nwords - 1;
    int* array_spaces = malloc(*n_spaces*sizeof(*array_spaces));
    int remaining_spaces = * columnWidth - n_of_char;
    spaceWord(array_spaces, n_spaces, remaining_spaces);
    /*  
    if(countColumn == 0){
        outputText[*row] = malloc(sizeof(char*) * (*total_length));
    }
    */
    //strcpy(outputText[*row], array[0]);
    char* str = malloc(1000* sizeof(*str));                     //100
    strcpy(str, strcat(outputText[*row], array[0]));

    for(int i = 1; i< *nwords; i++){
        addSpace(outputText, row, array_spaces[i - 1]);
        strcat(outputText[*row], array[i]);
        //strcpy(outputText[*row], strcat(str, array[i]));

    }
    if (*countColumn < *nColumn-1) {
        addSpace(outputText, row, *distanceColumn);
    }
    *row +=1;
    *countRow += 1;
    //GESTIRE PAGINA COLONNA
    //outputText[*row+1] = malloc(sizeof(char*) * (*total_length));
    if (*countRow == *linesPerColumn -1) {
        if (*countColumn == *nColumn-1) {
            //strcat(outputText[*row], "\n");
            *row +=1;
            *countRow = 0;
            //*countPage = *row + 3;
            //REALLOC
            newPage(outputText, row, countPage, linesPerColumn, total_length);

            /*
            for (int i = 0; i < 3; i++) {
                outputText[*row+i] = malloc(sizeof(char*) * (*total_length));
            }
            *row += 2;
            */
            //*countPage += 1;  //nuova pagina 
            //outputText = realloc(outputText, (*linesPerColumn+3)**countPage); 
            //INSERIRE FUNZIONE PER %%%
        }
        else {
                //*row = (*linesPerColumn+3)**countPage;
                *row = *countPage;
                *countRow = 0;
                //*countPage = *row;
                *countColumn += 1;
            }
    }

}

void addRow(){

}

void noJustified(int *nwords, int* row, char** array, char** outputText, int* columnWidth, int conSpazi, int* distanceColumn, int* ncolumn, int* countColumn, int* linesPerColumn, int* countRow, int* countPage, int* total_length){
    int count = 0;
    int lenParole = 0;
    for (int i = 0; i < *nwords; i++) {
        char* str = malloc(100* sizeof(*str));
        lenParole += len(array[i]);
        strcpy(str, strcat(outputText[*row], array[i]));
        count ++;                
        if (count < *nwords) {
            strcpy(outputText[*row], strcat(str, " "));
        }
        if (*countColumn < *ncolumn -1 ) {
            int spazi = *columnWidth - conSpazi + *distanceColumn;
            for (int i = 0; i <= spazi; i++) {
                strcpy(outputText[*row], strcat(str, " "));
            }
        }
    }
    *row += 1;
    *countRow +=1;
    if (*countRow == *linesPerColumn-1) {
        if (*countColumn == *ncolumn-1) {
            //strcat(outputText[*row], "\n"); //segmentation fault
            *row +=1;
            *countRow = 0;
            *countPage = *row + 1;
            for (int i = 0; i < 3; i++) {
                outputText[*row+i] = malloc(sizeof(char*) * (*total_length));
            }
            *row += 2;
            //*countPage += 1;  //nuova pagina 
            outputText = realloc(outputText, (*linesPerColumn+1)*(*countPage+1)); 
            //INSERIRE FUNZIONE PER %%%
        }
        else {
                //*row = (*linesPerColumn+3)**countPage;
                *row = *countPage;
                *countRow = 0;
                *countColumn += 1;
            }
    }
}

void emptyRow(int* row, char** outputText, int* columnWidth, int* distanceColumn, int* ncolumn, int* countColumn, int* countRow, int*linesPerColumn, int* countPage, int* total_length){
    char* str = malloc(100* sizeof(*str));
    strcpy(str, strcat(outputText[*row], " "));
    if (*countColumn < *ncolumn - 1) {
        int totSpace = *columnWidth + *distanceColumn;
        for (int i = 0; i < totSpace - 1; i++) {
            strcpy(outputText[*row], strcat(str," "));
        }
    }
    else {
        for (int i = 0; i < *columnWidth; i++) {
            //strcpy(outputText[*row], " ");
            strcpy(outputText[*row], strcat(str," "));
            //strcpy(outputText[*row], strcat(outputText[*row]," "));
        }
    }
    *row +=1;
    if (*row == *linesPerColumn-1) {
        if (*countColumn == *ncolumn-1) {
            //strcat(outputText[*row], "\n"); //segmentation fault
            *row +=1;
            *countRow = 0;
            *countPage = *row + 3;
            for (int i = 0; i < 3; i++) {
                outputText[*row+i] = malloc(sizeof(char*) * (*total_length));
            }
            *row += 2;
            //*countPage += 1;  //nuova pagina 
            outputText = realloc(outputText, (*linesPerColumn+3)**countPage); 
            //INSERIRE FUNZIONE PER %%%
        }
        else {
                //*row = (*linesPerColumn+3)**countPage;
                *row = *countPage;
                countColumn += 1;
            }
    }
}

void printArray(char** a, int* linesPerColumn, int* countPage) {
    int p = 0;
    if (*countPage == 0) {
        p = *linesPerColumn; //+1
    }
    else {
        p = (*linesPerColumn); //* *countPage;
    }
    for(int i = 0; i < p; i++){
        printf("%s\n", a[i]);
        }
}

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
        /*
    int n = (int) lenW;
    n -= 1;
    char ch = word[n];
    char ch2 = word[n-1];
    if (ch == '\r') {
        lenW = lenW -1;
    }
    */
    return (int) lenW;
}

//caratteri totali per riga
int lenPage(int n_column, int column_width, int distance_column){
    int i = column_width*n_column + distance_column*(n_column-1) +1;
    return i;
}

//spazi tra le parole di ogni riga
void spaceWord(int* array_spaces, int* n_spaces, int remaining_spaces){
    for(int i = 0; i < remaining_spaces; i++){
        array_spaces[i % *n_spaces] ++;
    }
}

void newLine(int riga, int colonna, char** matrix) {
    char nextLine[2] = "\n";
    for (int i = 0; i < 2; i++) {
        matrix[riga][colonna] = nextLine[i];
        colonna ++;
    }
    riga ++;
}

void newPage(char** outputText, int* row, int*countPage, int* linesPerColumn, int* total_length){
    outputText = realloc(outputText, (*linesPerColumn+1)*(*countPage+1)); 
    for (int i = *countPage; i < (*linesPerColumn+1); i++) {
        outputText[*row+i] = malloc(*total_length*sizeof(*outputText[*row+i]));
    }
    strcat(outputText[*row], "\n%%%\n");

    //strcpy(outputText[*row], "\n%%%\n");
    *row += 1;
    *countPage = *row;
} 

void emptySpaceColumns(int spazio_colonne, int riga, int colonna, char** matrice){
    for (int i = 1; i <= spazio_colonne; i++) {
        matrice[riga][colonna + i] = ' ';
    }
}

void addSpace(char** outputTxt, int* row, int spazi){
    char* str1 = malloc(1000* sizeof(*str1));                     //100
    strcpy(str1, strcat(outputTxt[*row], " "));
    for(int i = 0; i < spazi-1; i++){
        //strcat(str[*row], " ");
        strcpy(outputTxt[*row], strcat(str1, " "));
    }
}