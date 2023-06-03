#ifndef FORMAT_H
#define FORMAT_H

int lenArrayOutput(int* n_column, int* column_width, int* distance_column);

void addSpace(char** outputText, int* currentRow, int spazi);

int len(char* word);

void inizializza(char** array, int* size, int* length);

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

#endif