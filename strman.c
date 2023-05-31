#include "strman.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void add_spaces(char* str, int spaces){
    for(int i = 0; i < spaces; i++){
        strcat(str, " ");
    }
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

int lstrlen(const char str[]){
    double counter = 0;
    int value = 0;
    for(int i = 0; str[i] != '\0'; i++){
        value = (int) str[i];
        
        if(value == -17 | value == -69 | value == -65){
        }
        else if(value == 92 | value < 0){
            counter += 0.5;
        }
        else{
            counter += 1;
        }
    }
    return (int) counter;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result; 
}