#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void makeDirectory(const char *path);
void getFirstWord(char*, char*);
int getPath(char*, char*);
int createFile(char*);
int getFileName(char*, char*);
int mainFunction(char*);
void makeAllDirectories(char*);
void makeFile(char*);

int main() {
    char input[300];
    gets(input);
    while (1) {
        if(mainFunction(input) == 0) {
            puts("invalid input");
        }
        gets(input);
    }
    return 0;
}

int mainFunction(char* input) {
    char word[100];
    getFirstWord(input, word);
    if(strcmp(word, "createfile") == 0) {
        if(createFile(input) == 0){
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 0;
    }
}

void makeDirectory(const char *path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        mkdir(path);
    }
}

void getFirstWord(char input[], char word[]) {
    char first[100];
    strcpy(first, input);
    input = strtok(input, (const char *)"   ");
    if(input == NULL) {
        strcpy(first, input);
    }
    strcpy(word, input);
    int l = strlen(word), i, j;
    for(i = l + 1, j = 0; first[i] != 0; i++, j++) {
        input[j] = first[i];
    }
    input[j] = 0;
}

int getPath(char input[], char path[]) {
    if(input[0] == 0) {
        return 0;
    }
    else if(input[0] == '\"') {
        int i = 1;
        while (input[i] != '\"') {
            path[i - 1] = input[i];
            i++;
        }
        path[i - 1] = 0;
    }
    else {
        int i = 0;
        while (input[i] != 0 && input[i] != ' ') {
            path[i] = input[i];
            i++;
        }
        path[i] = 0;
    }
    return 1;
}

int createFile(char input[]) {
    char filename[100], path[200];
    int check = getFileName(input, filename);
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[100] = "D:";
    strcat(path2, path);
    makeAllDirectories(path2);
    makeFile(path2);
    return 1;
}

int getFileName(char input[], char filename[]) {
    if(input[0] == 0) {
        return 0;
    }
    getFirstWord(input, filename);
    if(strcmp(filename,"__file") == 0)
        return 1;
    return 0;
}

void makeAllDirectories(char* path) {
    char new_path[200];
    int l = strlen(path);
    for(int i = 0; i < l; i++){
        new_path[i] = path[i];
        if(path[i + 1] == '/' && i > 1) {
            makeDirectory(new_path);
        }
    }
}

void makeFile(char *path) {
    FILE *file;
    if(file = fopen(path, "r")) {
        puts("the file exists");
    }
    else {
        FILE *myfile = fopen(path, "w");
        fclose(myfile);
    }
}