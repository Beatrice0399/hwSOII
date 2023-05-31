#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "strman.h"
#include "journal.h"

char** new_paragraph(char** output_text, char** str_wrt, int* need_new_page, int* row,
                    int* column, int* size_of_output, int* lin_for_col, int* starter_row, int* total_length, int* n_of_word,
                    int* char_for_col, int* char_in_tot, int* spc_btw_col, int* num_of_col){
    
    not_justify_string(str_wrt, output_text, n_of_word, char_for_col, row, column, spc_btw_col, num_of_col, total_length, n_of_word);
    addRow(output_text, row, starter_row, column, num_of_col, lin_for_col, need_new_page);
    if(*need_new_page == 1){
        output_text = new_page(output_text, size_of_output, lin_for_col, need_new_page, starter_row, row, total_length);
    }
    not_justify_string(str_wrt, output_text, n_of_word, char_for_col, row, column, spc_btw_col, num_of_col, total_length, n_of_word);
    if(*need_new_page == 1){
        output_text = new_page(output_text, size_of_output, lin_for_col, need_new_page, starter_row, row, total_length);
    }
    *char_in_tot = 0;
    addRow(output_text, row, starter_row, column, num_of_col, lin_for_col, need_new_page);
    return output_text;
}

char** addtok(char* token, char** output_text, char** str_wrt, int* need_new_page, int* row, 
            int* column, int* size_of_output, int* lin_for_col, int* starter_row, int* total_length,
            int* n_of_word, int* char_for_col, int* char_in_tot, int* spc_btw_col, int* num_of_col){
    if(*need_new_page == 1){
        output_text = new_page(output_text, size_of_output, lin_for_col, need_new_page, starter_row, row, total_length);
    }

    //Getting string length and updating total row length
    int* token_length = malloc(sizeof(int));
    *token_length = lstrlen(token);
    *char_in_tot += *token_length;

    //Or justify the string if the length is greater than the one needed
    if(*char_in_tot > *char_for_col){
        
        //Justifying the strings
        if(*n_of_word == 1){
            not_justify_string(str_wrt, output_text, n_of_word, char_for_col, row, column, spc_btw_col, num_of_col, total_length, n_of_word);
        }
        else{
            justify_string(str_wrt, output_text, n_of_word, char_for_col, row, column, spc_btw_col, num_of_col, total_length, n_of_word);
        }

        //Adding a row and resetting the counters
        addRow(output_text, row, starter_row, column, num_of_col, lin_for_col, need_new_page);
        *char_in_tot = *token_length;
    }
    *char_in_tot += 1;
    strcpy(str_wrt[*n_of_word], token);
    *n_of_word += 1;
    return output_text;
}

void justify_string(char** strings, char** output_text, const int* nmb_of_word, int* char_for_col, 
                    int* row, int* column, int* spc_btw_col, int* num_col, int* total_length,int* n_of_word)
{
    // Getting length of all words in the string
    int len_str = 0;
    for(int i = 0; i < *nmb_of_word; i++){
        len_str += lstrlen(strings[i]);
    }
    //Creating the spaces between each word
    const int n_spaces = *nmb_of_word - 1;
    int spaces[n_spaces];
    for (int i = 0; i < n_spaces; i++){
        spaces[i] = 0;
    }
    int remaining_spaces = *char_for_col - len_str;
    for(int i = 0; i < remaining_spaces; i++){
        spaces[i % n_spaces] ++;
    }

    //Creating a buffer string and justifing the string
    char* bufferstr = (char*) malloc(*char_for_col + *spc_btw_col + 2);
    strcpy(bufferstr, strings[0]);
    for(int j = 1; j < *nmb_of_word; j++){
        add_spaces(bufferstr, spaces[j - 1]);
        strcat(bufferstr, strings[j]);
    }
    if(*column != *num_col - 1){
        add_spaces(bufferstr, *spc_btw_col);
    }

    //Copying the buffer string to the output text
    if(*column == 0){
        output_text[*row] = malloc(*total_length * sizeof(char*));
        strcpy(output_text[*row], bufferstr);
    }
    else{
        strcat(output_text[*row], bufferstr);
    }
    *n_of_word = 0;
    return;
}

void not_justify_string(char** strings, char** output_text, const int* nmb_of_word, int* char_for_col, 
                    int* row, int* column, int* spc_btw_col, int* num_col, int* total_length, int* n_of_word)
{
    //Getting the length of the string and creating it
    int sizeofstr = *char_for_col + *spc_btw_col;
    char* bufferstr = (char*)malloc(sizeofstr);
    for(int i = 0; i < *nmb_of_word; i++){
        strcat(bufferstr, strings[i]);
        strcat(bufferstr, " ");
    }

    //Calculating the remaining spaces and adding them to the end
    int remaining_spaces = sizeofstr - lstrlen(bufferstr);
    if(*column != *num_col - 1){
        add_spaces(bufferstr, remaining_spaces);
    }

    //Copying the buffer string to the output text
    if(*column == 0){
        output_text[*row] = malloc(*total_length * sizeof(char*));
        strcpy(output_text[*row], bufferstr);
    }
    else{
        strcat(output_text[*row], bufferstr);
    }
    *n_of_word = 0;
}


void addRow(char** output_text, int* row, int* starter_row, int* column,
            int* n_column, int* lin_per_col, int* need_new_page){
    *row += 1;
    if(*row == *lin_per_col + *starter_row){
        *row = *starter_row;
        *column += 1;
        if(*column == *n_column){
            *column = 0;
            *need_new_page = 1;
        }
    }
}

char** new_page(char** output_text, int* lunghezza_output, int* lin_per_col, int* need_new_page,
                int* starter_row, int* row, int* total_length){
    int new_size = *lunghezza_output + *lin_per_col + 1;
    output_text = realloc(output_text, new_size * sizeof(*output_text));
    initiArr(output_text, *lunghezza_output, new_size, *total_length);
    strcpy(output_text[*lunghezza_output], "%%%");
    *starter_row = *lunghezza_output + 1;
    *lunghezza_output = new_size;
    *row = *starter_row;
    *need_new_page = 0;
    return output_text;
}

void initiArr(char** pointer, int start, int end, int length_id){
   for(int i = start; i <= end; i++){
      pointer[i] = (char*)malloc(length_id * sizeof(char));
   }
}

int get_total_char_lenght(const int num_of_col, const int char_for_col, const int spc_btw_col){
    int lenght = 0;
    lenght += char_for_col * num_of_col;
    lenght += (num_of_col - 1) * spc_btw_col;
    return lenght;
}
