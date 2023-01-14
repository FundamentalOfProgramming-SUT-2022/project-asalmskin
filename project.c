#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void makeDirectory(const char*);
void getFirstWord(char*, char*);
int getPath(char*, char*);
int getString(char*, char*);
int createFile(char*);
int getFileName(char*, char*, char*);
int mainFunction(char*);
void makeAllDirectories(char*);
void makeFile(char*);
int insertstr(char*);
void writeToFile(char*, char*,int, int);
int checkPath(char*);

int main() {
    //mkdir("D:/root");
    char input[1000];
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
    else if(strcmp(word, "insertstr") == 0) {
        if(insertstr(input) == 0) {
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

int createFile(char input[]) {
    char filename[1000], path[1000];
    int check = getFileName(input, filename, "_file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    if(checkPath(path) == 0) {
        puts("invalid path");
        return 1;
    }
    char path2[100] = "D:";
    strcat(path2, path);
    makeAllDirectories(path2);
    makeFile(path2);
    return 1;
}

int insertstr(char *input) {
    char filename[1000], path[1000],stringname[1000], pos[1000];
    int check = getFileName(input, filename, "_file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    if(checkPath(path) == 0) {
        puts("invalid path");
        return 1;
    }
    char path2[100] = "D:";
    strcat(path2, path);
    check = getFileName(input, stringname, "_str");
    if(check == 0) {
        return 0;
    }
    check = getString(input, stringname);
    if(check == 0) {
        return 0;
    }
    check = getFileName(input, pos, "__pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    writeToFile(path2, stringname, line, character);
    return 1;
}

void makeDirectory(const char *path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        mkdir(path);
    }
}

void getFirstWord(char input[], char word[]) {
    char first[1000];
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
        int i = 1, j = 0, counter = 1;
        while (input[i] != '\"') {
            if(input[i] == ' ') {
                counter++;
            }
            path[j] = input[i];
            i++;
            j++;
        }
        path[j] = 0;
        char word[100];
        for(int j = 0; j < counter; j++) {
            getFirstWord(input, word);
        }
    }
    else {
        int i = 0;
        while (input[i] != 0 && input[i] != ' ') {
            path[i] = input[i];
            i++;
        }
        path[i] = 0;
        char word[100];
        getFirstWord(input, word);
    }
    return 1;
}

int getString(char input[], char path[]) {
    if(input[0] == 0) {
        return 0;
    }
    else if(input[0] == '\"') {
        int i = 1, j = 0, counter = 1;
        while (input[i] != '\"') {
            if(input[i] == ' ') {
                counter++;
            }
            path[j] = input[i];
            if(input[i] == '\\' && input[i + 1] == '\"') {
                path[j] = '\"';
                i++;
            }
            else if(input[i] == '\\' && input[i + 1] == 'n' && input[i - 1] != '\\') {
                path[j] = '\n';
                i++;
            }
            else if(input[i] == '\\' && input[i + 1] == 'n' && input[i - 1] == '\\') {
                path[j] = 'n';
                i++;
            }
            i++;
            j++;
        }
        path[j] = 0;
        char word[100];
        for(int j = 0; j < counter; j++) {
            getFirstWord(input, word);
        }
    }
    else {
        int i = 0, j = 0;
        while (input[i] != 0 && input[i] != ' ') {
            path[j] = input[i];
            if(input[i] == '\\' && input[i + 1] == '\"') {
                path[j] = '\"';
                i++;
            }
            else if(input[i] == '\\' && input[i + 1] == 'n' && input[i - 1] != '\\') {
                path[j] = '\n';
                i++;
            }
            else if(input[i] == '\\' && input[i + 1] == 'n' && input[i - 1] == '\\') {
                path[j] = 'n';
                i++;
            }
            i++;
            j++;
        }
        path[j] = 0;
        char word[100];
        getFirstWord(input, word);
    }
    return 1;
}

int getFileName(char input[], char filename[], char str[]) {
    if(input[0] == 0) {
        return 0;
    }
    getFirstWord(input, filename);
    if(strcmp(filename, str) == 0)
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

void writeToFile(char* path, char* string, int line, int character) {
    char path2[1000];
    int ch, counter = 0, isedited = 0, check = 1;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, path);
    strcat(path2, "thisfilewillbedeletedsoon");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL || targetfile == NULL) {
        puts("something went wrong, try again...");
        return;
    }
    while((ch = fgetc(firstfile)) != EOF) {
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            if(counter > 0) {
                fprintf(targetfile, "\n");
            }
            for(int j = 0; j < character; j++) {
                if(check == 1) {
                    if(counter > 0) {
                        ch = fgetc(firstfile);
                    }
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                    if(check == 1)
                        fprintf(targetfile, "%c", ch);
                    if (counter == 0) {
                        ch = fgetc(firstfile);
                    }
                }
                else{
                    fprintf(targetfile, " ");
                }
            }
            fprintf(targetfile, "%s", string);
            isedited = 1;
            if(check == 1) {
                while((ch = fgetc(firstfile)) != EOF) {
                    fprintf(targetfile, "%c", ch);
                    if(ch == '\n') {
                        break;
                    }
                }
            }
            if(check == 0) {
                fprintf(targetfile, "\n");
            }
        }
        else{
            fprintf(targetfile, "%c", ch);
        }
    }
    if(isedited == 0) {
        while (counter < line - 1) {
        fprintf(targetfile, "\n");
        counter++;
        }
        for(int j = 0; j < character; j++) {
            fprintf(targetfile, " ");
        }
        fprintf(targetfile, "%s", string);
    }
    fclose(targetfile);
    fclose(firstfile);
    remove(path);
    rename(path2, path);
}

int checkPath(char path[]) {
    char path2[1000];
    int i = 1;
    path2[0] = path[0];
    while(path[i] != 0 && path[i] !='/') {
        path2[i] = path[i];
        i++;
    }
    path2[i] = 0;
    if(strcmp(path2, "/root") == 0) {
        return 1;
    }
    return 0;
}