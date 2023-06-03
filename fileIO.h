#ifndef FILEIO_H
#define FILEIO_H

char* getPath(const char* txt_path);

void writeText(char* path, char** outputText, int* linesPerColumn, int* nPage);

#endif