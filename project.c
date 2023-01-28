#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void makeDirectory(const char*);
int doesDirectoryExist(char*);
void getFirstWord(char*, char*);
int getPath(char*, char*);
int getString(char*, char*, int*);
int createFile(char*);
int getFileName(char*, char*, char*);
int mainFunction(char*);
void makeAllDirectories(char*);
void makeFile(char*);
int insertstr(char*);
void writeToFile(char*, char*,int, int);
int checkPath(char*);
int cat(char*);
void readFile(char*);
int removestr(char*);
void removeForward(char*, int, int, int);
void removeBackward(char*, int, int, int);
int copystr(char*);
void copyForward(char*, int, int, int);
void copyBackward(char*, int, int, int);
int cutstr(char*);
int pastestr(char*);
int find(char*);
int findInFile(char*, char*, int, int, int, int, int*);
void findAll(char*, char*, int, int);
int replace(char*);
void replaceAll(char*, char*, char*, int);

char clipboard[100000];

int main() {
    makeDirectory("./root");
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
    char word[1000];
    getFirstWord(input, word);
    if(strcmp(word, "createfile") == 0 && (createFile(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "insertstr") == 0 && (insertstr(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "cat") == 0 && (cat(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "removestr") == 0 && (removestr(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "copystr") == 0 && (copystr(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "cutstr") == 0 && (cutstr(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "pastestr") == 0 && (pastestr(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "find") == 0 && (find(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "replace") == 0 && (replace(input) != 0)) {
        return 1;
    }
    else {
        return 0;
    }
}

int createFile(char input[]) {
    char filename[100], path[1000];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[100] = ".";
    strcat(path2, path);
    makeAllDirectories(path2);
    makeFile(path2);
    return 1;
}

int insertstr(char *input) {
    char filename[100], path[1000],stringname[1000], pos[100];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    check = getFileName(input, stringname, "--str");
    if(check == 0) {
        return 0;
    }
    int flag;
    check = getString(input, stringname, &flag);
    if(check == 0) {
        return 0;
    }
    check = getFileName(input, pos, "--pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    writeToFile(path2, stringname, line, character);
    return 1;
}

int cat(char input[]) {
    char filename[100], path[1000];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    readFile(path2);
    return 1;
}

int removestr(char input[]) {
    char filename[100], path[1000], pos[100];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    check = getFileName(input, pos, "--pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    check = getFileName(input, pos, "-size");
    if(check == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    if(strcmp(pos, "-b") == 0) {
        removeBackward(path2, line, character, size);
        return 1;
    }
    if(strcmp(pos, "-f") == 0) {
        removeForward(path2, line, character, size);
        return 1;
    }
    return 0;
}

int copystr(char input[]) {
    char filename[100], path[1000], pos[100];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    check = getFileName(input, pos, "--pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    check = getFileName(input, pos, "-size");
    if(check == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    if(strcmp(pos, "-b") == 0) {
        copyBackward(path2, line, character - 1, size);
        return 1;
    }
    if(strcmp(pos, "-f") == 0) {
        copyForward(path2, line, character, size);
        return 1;
    }
    return 0;
}

int cutstr(char input[]) {
    char filename[100], path[1000], pos[100];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    check = getFileName(input, pos, "--pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    check = getFileName(input, pos, "-size");
    if(check == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    if(strcmp(pos, "-b") == 0) {
        copyBackward(path2, line, character - 1, size);
        removeBackward(path2, line, character, size);
        return 1;
    }
    if(strcmp(pos, "-f") == 0) {
        copyForward(path2, line, character, size);
        removeForward(path2, line, character, size);
        return 1;
    }
    return 0;
}

int pastestr(char input[]) {
    char filename[100], path[1000], pos[100];
    int check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    check = getFileName(input, pos, "--pos");
    if(check == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    char stringname[1000];
    strcpy(stringname, clipboard);
    writeToFile(path2, stringname, line, character);
    return 1;
    
}

int find(char input[]) {
    char filename[100], path[1000],stringname[1000], pos[100];
    int size;
    int check = getFileName(input, stringname, "--str");
    if(check == 0) {
        return 0;
    }
    int flag = -1;
    check = getString(input, stringname, &flag);
    if(check == 0) {
        return 0;
    }
    check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    getFirstWord(input, pos);
    int num;
    if(strcmp(pos, "-at") == 0) {
        int at;
        sscanf(input, "%d%s", &at, pos);
        if(strcmp(pos, "-byword") == 0)
            num = findInFile(path2, stringname, flag, at, 0, 1, &size);
        else
            num = findInFile(path2, stringname, flag, at, 0, 0, &size);
    }
    else if(strcmp(pos, "-count") == 0) {
        num = findInFile(path2, stringname, flag, 1, 1, 0, &size);
    }
    else if(strcmp(pos, "-byword") == 0) {
        int at;
        sscanf(input, "%s %d", pos, &at);
        if(strcmp(pos, "-at") == 0)
            num = findInFile(path2, stringname, flag, at, 0, 1, &size);
        else if(strcmp(pos, "-all") == 0) {
            findAll(path2, stringname, flag, 1);
            return 1;
        }
        else
            num = findInFile(path2, stringname, flag, 1, 0, 1, &size);
    }
    else if(strcmp(pos, "-all") == 0) {
        sscanf(input, "%s", pos);
        if(strcmp(pos, "-byword") == 0) {
            findAll(path2, stringname, flag, 1);
            return 1;
        }
        else {
            findAll(path2, stringname, flag, 0);
            return 1;
        }
    }
    else {
        num = findInFile(path2, stringname, flag, 1, 0, 0, &size);
    }
    if(num >= -1) {
        printf("%d\n", num);
    }
    return 1;
}

int replace(char input[]) {
    char filename[100], path[1000], stringname[1000], stringname2[1000], pos[100];
    int check = getFileName(input, stringname, "--str1");
    if(check == 0) {
        return 0;
    }
    int flag = -1;
    check = getString(input, stringname, &flag);
    if(check == 0) {
        return 0;
    }
    check = getFileName(input, pos, "--str2");
    if(check == 0) {
        return 0;
    }
    int flag2 = -1;
    check = getString(input, stringname2, &flag2);
    if(check == 0 || flag2 != -1) {
        return 0;
    }
    check = getFileName(input, filename, "--file");
    if(check == 0) {
        return 0;
    }
    check = getPath(input, path);
    if(check == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    int size = 0;
    getFirstWord(input, pos);
    int num;
    if(strcmp(pos, "-at") == 0) {
        int at;
        sscanf(input, "%d%s", &at, pos);
        num = findInFile(path2, stringname, flag, at, 0, 0, &size);
    }
    else if(strcmp(pos, "-all") == 0) {
        replaceAll(path2, stringname, stringname2, flag);
        return 1;
    }
    else {
        num = findInFile(path2, stringname, flag, 1, 0, 0, &size);
    }
    if(num >= 0) {
        removeForward(path2, 1, num, size);
        writeToFile(path2, stringname2, 1, num);
        puts("replaced successfully");
    }
    else{
        puts("couldn't find the string in your file");
    }
    return 1;
}

void makeDirectory(const char *path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        mkdir(path);
    }
}

int doesDirectoryExist(char* path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        return 0;
    }
    return 1;
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

int getString(char input[], char path[], int *flagptr) {
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
            else if(input[i] == '\\' && input[i + 1] == '*') {
                path[j] = '*';
                i++;
            }
            else if(input[i] == '*') {
                *flagptr = j;
                j--;
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
            else if(input[i] == '\\' && input[i + 1] == '*') {
                path[j] = '*';
                i++;
            }
            else if(input[i] == '*') {
                *flagptr = j;
                j--;
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
    int i = 0;
    while(path[i + 1] != 0) {
        new_path[i] = path[i];
        new_path[i + 1] = 0;
        if(path[i + 1] == '/' && i > 1) {
            makeDirectory(new_path);
        }
        i++;
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
    int ch, ch2, counter = 0, isedited = 0, check = 1, flag = 0;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, path);
    strcat(path2, "thisfilewillbedeletedsoon");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL || targetfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    if(character == 0 && line == 1) {
        isedited = 1;
        fprintf(targetfile, "%s", string);
    }
    while((ch = fgetc(firstfile)) != EOF) {
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            if(counter > 0) {
                fprintf(targetfile, "\n");
            }
            else if(character > 0 && line == 1) {
                fprintf(targetfile, "%c", ch);
                character--;
            }
            for(int j = 0; j < character; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                    if(check == 1)
                        fprintf(targetfile, "%c", ch);
                }
                else{
                    puts("the line is too short");
                    fclose(firstfile);
                    fclose(targetfile);
                    remove(path2);
                    return;
                }
            }
            fprintf(targetfile, "%s", string);
            if(flag == 1) {
                fprintf(targetfile, "%c", ch2);
            }
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
        fclose(targetfile);
        fclose(firstfile);
        remove(path2);
        puts("your file is too short");
        return;
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
    while(path[i] != 0) {
        path2[i] = path[i];
        if(path[i + 1] == '/' && i > 1) {
            path2[i + 1] = 0;
            if(doesDirectoryExist(path2) == 0)  {
                return 0;
            }
        }
        i++;
    }
    return 1;
}

void readFile(char path[]) {
    FILE *myfile = fopen(path, "r");
    if(myfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    char ch;
    while((ch = fgetc(myfile)) != EOF) {
        printf("%c", ch);
    }
    printf("\n");
    fclose(myfile);
}

void removeForward(char path[], int line, int character, int size) {
    char path2[1000];
    int ch, counter = 0, isedited = 0, check = 1, ch2, flag, flag2 = 0;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, path);
    strcat(path2, "thisfilewillbedeletedsoon");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL || targetfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    while((ch = fgetc(firstfile)) != EOF) {
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            if(line == 1 && character == 0) {
                flag2 = 1;
            }
            if(counter > 0) {
                fprintf(targetfile, "\n");
            }
            else if(character > 0) {
                fprintf(targetfile, "%c", ch);
            }
            else {
                ch2 = ch;
                flag = 1;
            }
            
            for(int j = 0 ; j < character - 1; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                    if(check == 1)
                        fprintf(targetfile, "%c", ch);
                }
                else{
                    fclose(firstfile);
                    fclose(targetfile);
                    remove(path2);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            int i = 0;
            if(flag2 == 1)
                i = 1;
            for( ; i < size; i++) {
                ch = fgetc(firstfile);
            }
        
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
    fclose(targetfile);
    fclose(firstfile);
    remove(path);
    rename(path2, path);
}


void removeBackward(char path[], int line, int character, int size) {
    char path2[1000];
    int ch, counter = 0, isedited = 0, check = 1, ch2, flag;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, path);
    strcat(path2, "thisfilewillbedeletedsoon");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL || targetfile == NULL) {
        puts("the file doesn't exist");
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
            else if(character > 0) {
                fprintf(targetfile, "%c", ch);
            }
            else {
                ch2 = ch;
                flag = 1;
            }
            for(int j = 0; j < character - 1; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                    if(check == 1)
                        fprintf(targetfile, "%c", ch);
                }
                else{
                    fclose(firstfile);
                    fclose(targetfile);
                    remove(path2);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            fseek(targetfile, -size, SEEK_CUR);
            ch = fgetc(firstfile);
            for(int i = 0; i < size; i++) {
                fprintf(targetfile, "\0");
            }
            if(flag == 1) {
                fprintf(targetfile, "%c", ch2);
            }
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
    fclose(targetfile);
    fclose(firstfile);
    remove(path);
    rename(path2, path);
}

void copyForward(char path[], int line, int character, int size) {
    int ch, counter = 0, isedited = 0, check = 1, flag = 0;
    FILE *firstfile = fopen(path, "r");
    if(firstfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    ch = fgetc(firstfile);
    if(line == 1 && character == 0) {
        clipboard[0] = ch;
        flag = 1;
    }
    while(ch != EOF) {
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            for(int j = 1; j < character; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                }
                else{
                    fclose(firstfile);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            int i = 0;
            if(flag == 1)
                i++;
            for( ; i < size; i++) {
                ch = fgetc(firstfile);
                if(ch == EOF) {
                    fclose(firstfile);
                    puts("your file is too short!");
                    return;
                }
                clipboard[i] = ch;
            }
            clipboard[i + 1] = 0;
            isedited = 1;
        }
        ch = fgetc(firstfile);
    }
    fclose(firstfile);
}

void copyBackward(char path[], int line, int character, int size) {
    int ch, counter = 0, isedited = 0, check = 1;
    FILE *firstfile = fopen(path, "r");
    if(firstfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    while((ch = fgetc(firstfile)) != EOF) {
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            for(int j = 0; j < character; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                }
                else{
                    fclose(firstfile);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            int i;
            fseek(firstfile, -size, SEEK_CUR);
            ch = fgetc(firstfile);
            for(i = 0; i < size; i++) {
                ch = fgetc(firstfile);
                if(ch == EOF) {
                    fclose(firstfile);
                    puts("your file is too short!");
                    return;
                }
                clipboard[i] = ch;
            }
            clipboard[i + 1] = 0;
            isedited = 1;
        }
    }
    fclose(firstfile);
}

int findInFile(char path[], char string[], int f, int at, int count, int byword, int *sizeptr) {
    int ch, counter = 0, flag = 0, i = 0,ans = -1, flag3 = 0, flag4 = 0;
    int l = strlen(string), match = 0, lastword = 0, lastans = -1;
    int something = 0, finish = 0;
    FILE *myfile = fopen(path, "r");
    ch = fgetc(myfile);
    while(i < l) {
        counter++;
        if(ch == ' ') {
            lastword = counter;
        }
        if(f == i && string[i] != ' ' && string[i] != 0 && something == 0) {
            int flag2 = 0;
            int k = i;
            if(i == 0) {
                ans = lastword;
            }
            while(ch != ' ' && ch != EOF) {
                if(string[k] == 0 || string[k] == ' ') {
                    if(flag2 == 1)
                        flag2 = 2;
                    break;
                }
                if(string[k] == ch) {
                    flag2 = 1;
                    k++;
                }
                else{
                    k = i;
                    flag2 = 0;
                }
                ch = fgetc(myfile);
                counter++;
                (*sizeptr)++;
            }
            if(flag2 == 2) {
                while(ch != ' ' && ch != EOF) {
                    ch = fgetc(myfile);
                    (*sizeptr)++;
                    counter++;
                }
            }
            if(ch == ' ' || ch == EOF) {
                lastword = counter;
            }
            if(flag2 != 0) {
                i = k;
                flag = 1;
                something = 1;
                if(string[i] == ' ') {
                    counter--;
                    fseek(myfile, -1, SEEK_CUR);
                }
            }
            else {
                flag = 0;
                i = 0;
                (*sizeptr) = 0;
            }
        }
        
        else if(string[i] == ch) {
            if(i == 0) {
                if(f != 0)
                    ans = counter - 1;
                else
                    ans = lastword;
                if(ans != counter - 1) {
                    (*sizeptr) += counter - lastword - 1;
                }
            }
            flag = 1;
            i++;
            (*sizeptr)++;
        }
        else if(i != f){
            flag = 0;
            if(f >= 0 && (string[f] != 0 && string[f] != ' ')) {
                fseek(myfile, -i, SEEK_CUR);
                counter -= i;
            }
            else {
                fseek(myfile, -i, SEEK_CUR);
                counter -= i;
            }
            i = 0;
            (*sizeptr) = 0;
            something = 0;
        }
        if(flag == 1 && f == i && (string[i] == ' ' || string[i] == 0) && flag4 == 0) {
            flag4 = 1;
            while(ch != ' ' && ch != EOF) {
                counter++;
                ch = fgetc(myfile);
                (*sizeptr)++;
            }
            if(ch == ' ') {
                lastword = counter;
            }
            fseek(myfile, -1, SEEK_CUR);
            (*sizeptr)--;
            counter--;
        }
        if(i >= l && flag == 1) {
            flag = 0;
            i = 0;
            flag4 = 0;
            if(lastans == ans) {
                continue;
            }
            something = 1;
            match++;
            lastans = ans;
            if(match == at && count == 0) {
                fclose(myfile);
                if(byword == 0)
                    return ans;
                else {
                    flag3 = 1;
                    break;
                }
            }
            else {
                (*sizeptr) = 0;
            }
        }
        if(ch == EOF)
            break;
        ch = fgetc(myfile);
    }
    if(count == 1) {
        fclose(myfile);
        return match;
    }
    if(byword == 1 && flag3 == 1) {
        FILE *myfile = fopen(path, "r");
        int counter2 = 0;
        flag = 0;
        for(int j = 0; j < ans; j++) {
            ch = fgetc(myfile);
            if(ch == ' '&& flag == 0) {
                flag = 1;
                counter2++;
            }
            else if (ch != ' '){
                flag = 0;
            }
        }
        fclose(myfile);
        return counter2;
    }
    fclose(myfile);
    return -1;
}

void findAll(char path[], char string[], int f, int byword) {
    int i = 1;
    int size;
    int num = findInFile(path, string, f, i, 0, byword, &size);
    if(num == -1) {
        puts("couldn't find any");
    }
    else {
        while (num != -1) {
            printf("%d", num);
            i++;
            num = findInFile(path, string, f, i, 0, byword, &size);
            if(num != -1)
                printf(", ");
        }
        printf("\n");
    }
}

void replaceAll(char* path, char* string, char* string2, int f) {
    int size = 0;
    int num = findInFile(path, string, f, 1, 1, 0, &size);
    if(num <= 0) {
        puts("could't find any");
    }
    while(num >= 0) {
        removeForward(path, 1, num, size);
        writeToFile(path, string2, 1, num);
        num = findInFile(path, string, f, 1, 0, 0, &size);
    }
    puts("replaced succesfully");
}