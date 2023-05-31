#ifndef JOURNAL_H
#define JOURNAL_H

char** new_page(char** output_text, int* lunghezza_output, int* lin_per_col, int* need_new_page,
                int* starter_row, int* row, int* char_per_col);

void addRow(char** output_text, int* row, int* starter_row, int* column,
                int* n_column, int* lin_per_col, int* need_new_page);

char** addtok(char* token, char** output_text, char** str_wrt, int* need_new_page, int* row, 
            int* column, int* size_of_output, int* lin_for_col, int* starter_row, int* total_length,
            int* n_of_word, int* char_for_col, int* char_in_tot, int* spc_btw_col, int* num_of_col);

void justify_string(char** strings, char** output_text, const int* nmb_of_word, int* char_for_col, 
                    int* row, int* column, int* spc_btw_col, int* num_col, int* total_length, int* n_of_word);

void not_justify_string(char** strings, char** output_text, const int* nmb_of_word, int* char_for_col, 
                    int* row, int* column, int* spc_btw_col, int* num_col, int* total_length, int* n_of_word);

char** new_paragraph(char** output_text, char** str_wrt, int* need_new_page, int* row,
                    int* column, int* size_of_output, int* lin_for_col, int* starter_row, int* total_length, int* n_of_word,
                    int* char_for_col, int* char_int_tot, int* spc_btw_col, int* num_of_col);

void initiArr(char** pointer, int start, int end, int length_id);

int get_total_char_lenght(const int num_of_col, const int char_for_col, const int spc_btw_col);

#endif                