#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "strman.h"
#include "journal.h"
#include "fileio.h"

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
    
    int* lunghezza_totale = malloc(sizeof(int));
    *lunghezza_totale = get_total_char_lenght(*numero_colonne, *caratteri_ogni_colonna, *spazio_tra_colonne) + 1;

    char** buffer_array = malloc((*lunghezza_totale/2) * sizeof(*buffer_array));
    initiArr(buffer_array, 0, (*lunghezza_totale/2), *caratteri_ogni_colonna);
    int* lunghezza_output = malloc(sizeof(int));
    *lunghezza_output = *riga_attualee_per_colonna;
    char** output_text = malloc(*lunghezza_output * sizeof(*output_text));
    initiArr(output_text, 0, *riga_attualee_per_colonna, *lunghezza_totale);

    int* nuova_pagine = malloc(sizeof(int));
    int* contatore_parole = malloc(sizeof(int));
    int* contatore_caratteri = malloc(sizeof(int));
    int* prima_riga_della_pagina = malloc(sizeof(int));
    int* riga = malloc(sizeof(int));
    int* colonna = malloc(sizeof(int));

    char* pathFile = getPath(input_path);
    FILE *file = fopen(pathFile, "r");

    char* buff_str = malloc(1024*sizeof(char*));
    for(char* riga_attuale = fgets(buff_str, 1024, file); riga_attuale != NULL; riga_attuale = fgets(buff_str, 1024, file)){
        riga_attuale = strtok(riga_attuale, "\n\r");
        if(riga_attuale != NULL){
            for(char* token = strtok(riga_attuale, " "); token != NULL; token = strtok(NULL, " "))
            {
                output_text = addtok(token, output_text, buffer_array, nuova_pagine, riga,
                                        colonna, lunghezza_output, riga_attualee_per_colonna, prima_riga_della_pagina, lunghezza_totale,
                                        contatore_parole, caratteri_ogni_colonna, contatore_caratteri, spazio_tra_colonne, numero_colonne);  
            }
            output_text = new_paragraph(output_text, buffer_array, nuova_pagine, riga, colonna,
                                                lunghezza_output, riga_attualee_per_colonna, prima_riga_della_pagina, lunghezza_totale, contatore_parole, caratteri_ogni_colonna,
                                                contatore_caratteri, spazio_tra_colonne, numero_colonne);
        }
    }

    writeFile(getPath("single_thread_output.txt"), output_text, lunghezza_output, lunghezza_totale);
    return 0;
}