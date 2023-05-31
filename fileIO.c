#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#include "strman.h"

void writeFile(char* path, char** text, int* size, int* total_length){
    FILE *fp;
    fp = fopen(path, "w");
    fprintf(fp, "%s", text[0]);
    for(int i = 1; i < *size; i++){
        fprintf(fp, "\n");
        fprintf(fp, "%s", text[i]);
    }
}