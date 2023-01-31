#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <ctype.h>
#include <dirent.h>

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
int findInFile(char*, char*, int, int, int, int, int*, int*);
void findAll(char*, char*, int, int);
int replace(char*);
void replaceAll(char*, char*, char*, int);
int grep(char*);
int simplegrep(char*, char*, int, int, char*);
int makeHiddenFile(char*);
void makeHiddenPath(char*, char*);
int undo(char*);
int autoIndent(char*);
void indent(char*);
int compare(char*);
void compareFiles(char*, char*);
int lineNumber(char*);
void printLines(char*, int);
int tree(char*);
static void listDir(const char*, int, int);
int armanFunction(char*);
void printLine(int, char*, char*);
int bywordCounter(char*, int, int);

char *clipboard;
int arman = 0;
int arman_output = 0;
char arman_string[100000];
char grep_string[100000];

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
    arman = 0;
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
    else if(strcmp(word, "grep") == 0 && (grep(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "undo") == 0 && (undo(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "auto-indent") == 0 && (autoIndent(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "compare") == 0 && (compare(input) != 0)) {
        return 1;
    }
    else if(strcmp(word, "tree") == 0 && (tree(input) != 0)) {
        return 1;
    }
    else {
        return 0;
    }
}

int createFile(char input[]) {
    char filename[100], path[1000];
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    makeAllDirectories(path2);
    makeFile(path2);
    makeHiddenFile(path2);
    return 1;
}

int insertstr(char *input) {
    char filename[100], path[1000],stringname[1000], pos[100];
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    if(arman_output == 0) {
        if(getFileName(input, stringname, "--str") == 0) {
            return 0;
        }
        int flag;
        if(getString(input, stringname, &flag) == 0) {
            return 0;
        }
    }
    if(getFileName(input, pos, "--pos") == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    makeHiddenFile(path2);
    if(arman_output == 0)
    writeToFile(path2, stringname, line, character);
    else
    writeToFile(path2, arman_string, line, character);
    return 1;
}

int cat(char input[]) {
    char filename[100], path[1000];
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    getFirstWord(input, path);
    if(strcmp(path, "=D") == 0) {
        arman = 1;
        arman_string[0] = 0;
    }
    readFile(path2);
    if(arman == 1 && armanFunction(input) == 0)
        return 0;
    return 1;
}

int removestr(char input[]) {
    char filename[100], path[1000], pos[100];
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    if(getFileName(input, pos, "--pos") == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    if(getFileName(input, pos, "-size") == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    makeHiddenFile(path2);
    if(strcmp(pos, "-b") == 0) {
        if(size <= character) {
            removeForward(path2, line, character - size, size);
        }
        else
            removeBackward(path2, line, character, size - 1);
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
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    if(getFileName(input, pos, "--pos") == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    if(getFileName(input, pos, "-size") == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    free(clipboard);
    clipboard = (char*)malloc(size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    if(strcmp(pos, "-b") == 0) {
        copyBackward(path2, line, character, size);
        clipboard[size] = 0;
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
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    if(getFileName(input, pos, "--pos") == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    getFirstWord(input, pos);
    if(getFileName(input, pos, "-size") == 0) {
        return 0;
    }
    int size;
    sscanf(input, "%d", &size);
    free(clipboard);
    clipboard = (char*)malloc(size);
    getFirstWord(input, pos);
    sscanf(input, "%s", pos);
    makeHiddenFile(path2);
    if(strcmp(pos, "-b") == 0) {
        copyBackward(path2, line, character, size);
        if(size <= character)
            removeForward(path2, line, character - size, size);
        else
            removeBackward(path2, line, character, size - 1);
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
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    if(getFileName(input, pos, "--pos") == 0) {
        return 0;
    }
    int line, character;
    sscanf(input, "%d%*[^0123456789]%d", &line, &character);
    makeHiddenFile(path2);
    writeToFile(path2, clipboard, line, character);
    return 1;
    
}

int find(char input[]) {
    char filename[100], path[1000],stringname[1000], pos[100];
    int size, flag = -1, byword = 0;
    if(arman_output == 0) {
        if(getFileName(input, stringname, "--str") == 0) {
            return 0;
        }
        if(getString(input, stringname, &flag) == 0) {
            return 0;
        }
    }
    else {
        strcpy(stringname, arman_string);
    }
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    makeHiddenFile(path2);
    getFirstWord(input, pos);
    int num, line = 1;
    if(strcmp(pos, "-at") == 0) {
        int at;
        char arman_check[1000];
        sscanf(input, "%d%s%s", &at, pos, arman_check);
        if(strcmp(pos, "-byword") == 0) {
            byword = 1;
            if(strcmp(arman_check, "=D") == 0) {
                arman = 1;
                arman_string[0] = 0;
                for(int i = 0; i < 3; i++)
                    getFirstWord(input, path);
            }
            else if(strcmp(arman_check, "-count") == 0) {
                puts("invalid options");
                return 1;
            }
            else if(strcmp(arman_check, "-all") == 0) {
                puts("invalid options");
                return 1;
            }
            num = findInFile(path2, stringname, flag, at, 0, 1, &size, &line);
        }
        else if(strcmp(pos, "-count") == 0) {
            puts("invalid options");
            return 1;
        }
        else if(strcmp(pos, "-all") == 0) {
            puts("invalid options");
            return 1;
        }
        else {
            if(strcmp(pos, "=D") == 0) {
                arman = 1;
                arman_string[0] = 0;
                for(int i = 0; i < 2; i++)
                getFirstWord(input, pos);
            }
            num = findInFile(path2, stringname, flag, at, 0, 0, &size, &line);
        }
    }
    else if(strcmp(pos, "-count") == 0) {
        getFirstWord(input, pos);
        if(strcmp(pos, "at") == 0 || strcmp(pos, "-all") == 0 || strcmp(pos, "-byword") == 0) {
            puts("invalid options");
            return 1;
        }
        else if(strcmp(pos, "=D") == 0) {
            arman = 1;
            arman_string[0] = 0;
            getFirstWord(input, path);
        }
        num = findInFile(path2, stringname, flag, 1, 1, 0, &size, &line);
    }
    else if(strcmp(pos, "-byword") == 0) {
        byword = 1;
        int at;
        char arman_check[1000];
        sscanf(input, "%s", pos);
        if(strcmp(pos, "-count") == 0) {
            puts("invalid options");
            return 1;
        }
        else if(strcmp(pos, "-at") == 0) {
            sscanf(input, "%s %d%s", pos, &at, arman_check);
            if(strcmp(arman_check, "-all") == 0 || strcmp(arman_check, "-count") == 0) {
                puts("invalid options");
                return 1;
            }
            else if(strcmp(arman_check, "=D") == 0) {
                for(int i = 0; i < 3; i++)
                    getFirstWord(input, path);
                arman = 1;
                arman_string[0] = 0;
            }
            num = findInFile(path2, stringname, flag, at, 0, 1, &size, &line);
        }
        else if(strcmp(pos, "-all") == 0) {
            sscanf(input, "%s %d %s", pos, &at, arman_check);
            if(strcmp(arman_check, "=D") == 0) {
                for(int i = 0; i < 2; i++)
                    getFirstWord(input, path);
                arman = 1;
                arman_string[0] = 0;
            }
            findAll(path2, stringname, flag, 1);
            if(arman == 1 && armanFunction(input) == 0)
                return 0;
            return 1;
        }
        else {
            if(strcmp(pos, "=D") == 0) {
                getFirstWord(input, path);
                arman = 1;
                arman_string[0] = 0;
            }
            num = findInFile(path2, stringname, flag, 1, 0, 1, &size, &line);
        }
    }
    else if(strcmp(pos, "-all") == 0) {
        char arman_check[1000];
        sscanf(input, "%s %s", pos, arman_check);
        if(strcmp(pos, "-count") == 0 || strcmp(pos, "-at") == 0) {
            puts("invalid options");
            return 1;
        }
        else if(strcmp(pos, "-byword") == 0) {
            byword = 1;
            if(strcmp(arman_check, "-at") == 0 || strcmp(arman_check, "-count") == 0) {
                puts("invalid options");
                return 1;
            }
            else if(strcmp(arman_check, "=D") == 0) {
                for(int i = 0; i < 2; i++)
                    getFirstWord(input, path);
                arman = 1;
                arman_string[0] = 0;
            }
            findAll(path2, stringname, flag, 1);
            if(arman == 1 && armanFunction(input) == 0)
                return 0;
            return 1;
        }
        else {
            if(strcmp(pos, "=D") == 0) {
                getFirstWord(input, path);
                arman = 1;
                arman_string[0] = 0;
            }
            findAll(path2, stringname, flag, 0);
            if(arman == 1 && armanFunction(input) == 0)
                return 0;
            return 1;
        }
    }
    else {
        if(strcmp(pos, "=D") == 0) {
            arman = 1;
            arman_string[0] = 0;
        }
        num = findInFile(path2, stringname, flag, 1, 0, 0, &size, &line);
    }
    if(num >= -1) {
        if(byword == 1) {
            num = bywordCounter(path2, num, line);
        }
        if(arman == 0) {
            printf("%d:%d\n", line, num);
        }
        else
            sprintf(arman_string, "%d:%d\n", line, num);
        if(arman == 1 && armanFunction(input) == 0)
            return 0;
    }
    return 1;
}

int replace(char input[]) {
    char filename[100], path[1000], stringname[1000], stringname2[1000], pos[100];
    int flag = -1;
    if(arman_output == 0) {
        if(getFileName(input, stringname, "--str1") == 0) {
            return 0;
        }
        if(getString(input, stringname, &flag) == 0) {
            return 0;
        }
    }
    if(getFileName(input, pos, "--str2") == 0) {
        return 0;
    }
    int flag2 = -1;
    if(getString(input, stringname2, &flag2) == 0 || flag2 != -1) {
        return 0;
    }
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
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
    int num, line = 1;
    if(strcmp(pos, "-at") == 0) {
        int at;
        sscanf(input, "%d%s", &at, pos);
        if(strcmp(pos, "-all") == 0) {
            puts("invalid options");
            return 1;
        }
        if(arman_output == 0)
        num = findInFile(path2, stringname, flag, at, 0, 0, &size, &line);
        else
        num = findInFile(path2, arman_string, flag, at, 0, 0, &size, &line);
    }
    else if(strcmp(pos, "-all") == 0) {
        getFirstWord(input, pos);
        if(strcmp(pos, "-at") == 0) {
            puts("invalid options");
            return 1;
        }
        if(arman_output == 0)
        replaceAll(path2, stringname, stringname2, flag);
        else
        replaceAll(path2, arman_string, stringname2, flag);
        return 1;
    }
    else {
        if(arman_output == 0)
        num = findInFile(path2, stringname, flag, 1, 0, 0, &size, &line);
        else 
        num = findInFile(path2, arman_string, flag, 1, 0, 0, &size, &line);
    }
    if(num >= 0) {
        removeForward(path2, 1, num, size);
        writeToFile(path2, stringname2, 1, num);
    }
    else{
        puts("couldn't find the string in your file");
    }
    return 1;
}

int grep(char input[]) {
    char pos[1000], stringname[1000], path[1000], path2[1000], temp[1000];
    int option = 0;
    getFirstWord(input, pos);
    if(strcmp(pos, "-c") == 0) {
        option = 1;
        if(arman_output == 0)
        getFirstWord(input, pos);
    }
    else if(strcmp(pos, "-l") == 0) {
        option = 2;
        if(arman_output == 0)
        getFirstWord(input, pos);
    }
    int flag = -1;
    if(arman_output == 0) {
        if(strcmp(pos, "--str") != 0) {
            return 0;
        }
        if(getString(input, stringname, &flag) == 0) {
            return 0;
        }
    }
    else {
        strcpy(stringname, arman_string);
    }
    if(getFileName(input, pos, "--files") == 0) {
        return 0;
    }
    int i = 0;
    int l = strlen(input), counter = 0;
    input[l] = 0;
    while(1) {
        if(getPath(input, path) == 0) {
            break;
        }
        if(strcmp(path, "=D") == 0) {
            arman = 1;
            arman_string[0] = 0;
            strcat(arman_string, grep_string);
            grep_string[0] = 0;
            break;
        }
        path2[0] = '.';
        path2[1] = 0;
        strcat(path2,(const char*)path);
        if(checkPath(path2) == 0) {
            puts("invalid path");
            return 1;
        }
        i++;
        int num = simplegrep(path2, stringname, flag, option, path);
        counter += num;
        if(option == 2 && num > 0) {
            sprintf(temp, "%s\n", path);
            if(arman == 0)
            strcat(grep_string, temp);
            else
            strcat(arman_string, temp);
        }
        if(num == -1)
            return 1;
    }
    if(counter == 0) {
        if(arman == 0)
            puts("couldn't find any");
        else
            sprintf(arman_string, "couldn't find any\n");
    }
    else if(option == 1) {
        if(arman == 0)
            printf("%d\n", counter);
        else
            sprintf(arman_string, "%d\n", counter);
    }
    else if (arman == 0) {
        printf("%s", grep_string);
    }
    if(arman == 1 && armanFunction(input) == 0)
        return 0;
    return 1;
}

int undo(char input[]) {
    char filename[100], path[1000];
    if(getFileName(input, filename, "--file") == 0) {
        return 0;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    FILE *file = fopen(path2, "r");
    if(file == NULL) {
        puts("the file doesn't exist");
        return 1;
    }
    fclose(file);
    makeHiddenPath(path2, path);
    remove(path2);
    rename(path, path2);
    DWORD attributes = GetFileAttributes(path2);
    SetFileAttributes(path2, attributes - FILE_ATTRIBUTE_HIDDEN);
    makeHiddenFile(path2);
    return 1;
}

int autoIndent(char input[]) {
    char path[1000];
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    makeHiddenFile(path2);
    indent(path2);
    return 1;
}

int compare(char input[]) {
    char path[1000];
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path1[1000] = ".";
    strcat(path1, path);
    if(checkPath(path1) == 0) {
        puts("invalid path");
        return 1;
    }
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        puts("invalid path");
        return 1;
    }
    getFirstWord(input, path);
    if(strcmp(path, "=D") == 0) {
        arman = 1;
        arman_string[0] = 0;
    }
    compareFiles(path1, path2);
    if(arman == 1 && armanFunction(input) == 0) {
        return 0;
    }
    return 1;
}

int tree(char input[]) {
    int depth;
    char pos[1000];
    sscanf(input, "%d %s", &depth, pos);
    if(strcmp(pos, "=D") == 0) {
        arman = 1;
        arman_string[0] = 0;
        getFirstWord(input, pos);
        getFirstWord(input, pos);
    }
    if(depth < -1) {
        puts("invalid depth");
        return 1;
    }
    if(arman == 0)
    puts("root:");
    else
    sprintf(arman_string, "root:\n");
    listDir("./root", depth, 1);
    if(arman == 1 && armanFunction(input) == 0) {
        return 0;
    }
    return 1;
}

int armanFunction(char input[]) {
    char word[1000];
    arman = 0;
    arman_output = 1;
    getFirstWord(input, word);
    if(strcmp(word, "insertstr") == 0 && insertstr(input) != 0) {
        arman_output = 0;
        return 1;
    }
    else if(strcmp(word, "find") == 0 && find(input) != 0) {
        arman_output = 0;
        return 1;
    }
    else if(strcmp(word, "replace") == 0 && replace(input) != 0) {
        arman_output = 0;
        return 1;
    }
    else if(strcmp(word, "grep") == 0 && grep(input) != 0) {
        arman_output = 0;
        return 1;
    }
    arman_output = 0;
    return 0;
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
    strcpy(path2, "./root/.temp.txt");
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
    else {
        fclose(targetfile);
        fclose(firstfile);
        remove(path);
        rename(path2, path);
    }
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
    int counter = 0;
    while((ch = fgetc(myfile)) != EOF) {
        if(arman == 0)
        printf("%c", ch);
        else
        arman_string[counter] = ch;
        counter++;
    }
    if(arman == 0)
    printf("\n");
    else
    strcat(arman_string, "\n\0");
    fclose(myfile);
}

void removeForward(char path[], int line, int character, int size) {
    char path2[1000];
    int ch, counter = 0, isedited = 0, check = 1, ch2, flag, flag2 = 0;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, "./root/.temp.txt");
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
    strcpy(path2, "./root/.temp.txt");
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
            if(ch == '\n') {
                ch = fgetc(firstfile);
            }
            fseek(firstfile, -1, SEEK_CUR);
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
                    fclose(firstfile);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            fseek(targetfile, -size - 1, SEEK_CUR);
            fprintf(targetfile, "\0");
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
                    clipboard[0] = 0;
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
                    clipboard[0] = 0;
                    puts("your file is too short!");
                    return;
                }
                clipboard[i] = ch;
            }
            clipboard[i] = 0;
            isedited = 1;
        }
        ch = fgetc(firstfile);
    }
    fclose(firstfile);
}

void copyBackward(char path[], int line, int character, int size) {
    int ch, counter = 0, isedited = 0, check = 1, flag = 0, counter2 = 0;
    FILE *firstfile = fopen(path, "r");
    if(firstfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    ch = fgetc(firstfile);
    while(ch != EOF) {
        counter2++;
        if(ch == '\n') {
            counter++;
        }
        if(counter == line - 1 && isedited == 0) {
            for(int j = 0; j < character; j++) {
                if(check == 1) {
                    ch = fgetc(firstfile);
                    counter2++;
                    if(ch == '\n' || ch == EOF) {
                        check = 0;
                    }
                }
                if(check == 0){
                    fclose(firstfile);
                    puts("there is not enough characters in your chosen line");
                    return;
                }
            }
            isedited = 1;
            fclose(firstfile);
            break;
        }
        ch = fgetc(firstfile);
    }
    if(isedited == 0) {
        puts("your file is too short!");
        fclose(firstfile);
    }
    else {
        if(counter2 - size < 0) {
            puts("your file is too short!");
            return;
        }
        else{
            FILE *file = fopen(path, "r");
            ch = fgetc(file);
            int counter3 = 0;
            while(counter3 < counter2 - size) {
                counter3++;
                ch = fgetc(file);
            }
            int i;
            for(i = 0; i < size; i++) {
                clipboard[i] = ch;
                ch = fgetc(file);
            }
            clipboard[i] = 0;
            fclose(file);
        }
    }
}

int findInFile(char path[], char string[], int f,
               int at, int count, int byword, int *sizeptr, int *lineptr) {
    int counter = 0, flag = 0, i = 0,ans = -1, flag3 = 0, flag4 = 0;
    int l = strlen(string), match = 0, lastword = 0, lastans = -1;
    char ch;
    int something = 0, line = 1;
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
                *lineptr = line;
                fclose(myfile);
                return ans;
            }
            else {
                (*sizeptr) = 0;
            }
        }
        if(ch == EOF)
            break;
        if(ch == '\n') {
            line++;
            counter = 0, flag = 0, i = 0;
            ans = -1, flag3 = 0, flag4 = 0;
            lastword = 0, lastans = -1;
            (*sizeptr) = 0;
        }
        ch = fgetc(myfile);
    }
    if(count == 1) {
        fclose(myfile);
        return match;
    }
    int counter2;
    if(byword == 1 && flag3 == 1) {
        return bywordCounter(path, ans, *lineptr);
    }
    fclose(myfile);
    return -1;
}

int bywordCounter(char path[], int ans, int line) {
    FILE *myfile = fopen(path, "r");
    if(myfile == NULL) {
        puts("the file doesn't exist");
        return -1;
    }
    int ch;
    char counter2 = 0;
    int flag = 0;
    int linecounter = 1;
    while(1) {
        ch = fgetc(myfile);
        if(linecounter == line) {
            for(int j = 0; j < ans; j++) {
                if(ch == ' '&& flag == 0) {
                    flag = 1;
                    counter2++;
                }
                else if (ch != ' '){
                    flag = 0;
                }
                ch = fgetc(myfile);
            }
            break;
        } 
        if(ch == '\n') {
            linecounter++; 
        }
    }
    fclose(myfile);
    return counter2;
}

void findAll(char path[], char string[], int f, int byword) {
    char temp[1000];
    int i = 1;
    int size, line = 1;
    int num = findInFile(path, string, f, i, 0, byword, &size, &line);
    if(num == -1) {
        if(arman == 0)
        puts("couldn't find any");
        else 
        sprintf(arman_string, "couldn't find any\n");
    }
    else {
        while (num != -1) {
            if(byword == 1) {
            num = bywordCounter(path, num, line);
            }
            if(arman == 0)
                printf("%d:%d", line, num);
            else {
                sprintf(temp, "%d:%d", line, num);
                strcat(arman_string, temp);
            }
            i++;
            num = findInFile(path, string, f, i, 0, byword, &size, &line);
            if(num != -1) {
                if(arman == 0)
                    printf(", ");
                else {
                    sprintf(temp, ", ");
                    strcat(arman_string, temp);
                }
            }
        }
        if(arman == 0)
            printf("\n");
        else {
            sprintf(temp, "\n");
            strcat(arman_string, temp);
        }
    }
}

void replaceAll(char* path, char* string, char* string2, int f) {
    int size = 0, line = 1;
    int num = findInFile(path, string, f, 1, 1, 0, &size, &line);
    if(num <= 0) {
        puts("could't find any");
    }
    while(num >= 0) {
        removeForward(path, 1, num, size);
        writeToFile(path, string2, 1, num);
        num = findInFile(path, string, f, 1, 0, 0, &size, &line);
    }
}

int simplegrep(char path[], char string[], int flag, int option, char path2[]) {
    int counter = 0, size = 0, line = -1, line2, ch, at = 1;
    int num = findInFile(path, string, flag, at, 0, 0, &size, &line);
    line2 = -1;
    while(num >= 0) {
        if(line != line2) {
            counter++;
            if(option == 0) {
                printLine(line, path, path2);
            }
            if(option == 2) {
                return 1;
            }
        }
        at++;
        line2 = line;
        num = findInFile(path, string, flag, at, 0, 0, &size, &line);
    }
    return counter;
}

void makeHiddenPath(char path[], char path2[]) {
    int l = strlen(path), counter = 0;
    for(int i = 0; i < l; i++) {
        if(path[i] == '/')
            counter = i;
    }
    int i;
    for(i = 0; i <= counter; i++) {
        path2[i] = path[i];
    }
    path2[i] = '.';
    for(i = counter + 1; i < l; i++) {
        path2[i + 1] = path[i];
    }
    path2[i + 1] = 0;
}

int makeHiddenFile(char path[]) {
    char path2[1000], ch;
    makeHiddenPath(path, path2);
    remove(path2);
    FILE *firstfile = fopen(path, "r");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL)
        return 0;
    while((ch = fgetc(firstfile)) != EOF)
        fprintf(targetfile, "%c", ch);
    fclose(firstfile);
    fclose(targetfile);
    DWORD attributes = GetFileAttributes(path2);
    SetFileAttributes(path2, attributes + FILE_ATTRIBUTE_HIDDEN);
    return 1;
}

void indent(char path[]) {
    char path2[100] = "./root/temp.txt\0", ch;
    int spacecounter = 0, tabcounter = 0, flag = 0, flag2 = 0, flag3 = 0;
    FILE *firstfile = fopen(path, "r"); 
    if(firstfile == NULL) {
        puts("the file doesn't exist");
        return;
    }
    FILE *targetfile = fopen(path2, "w");
    ch = fgetc(firstfile);
    while(ch != EOF) {
        if(isspace(ch))
            spacecounter++;
        if(ch == '{') {
            fseek(targetfile, -spacecounter, SEEK_CUR);
            spacecounter = 0;
            fprintf(targetfile, "\0");
            if(flag2 == 1) 
                fprintf(targetfile, " ");
            else {
                for(int i = 0; i < tabcounter; i++) {
                    fprintf(targetfile, "    ");
                }
            }
            flag2 = 0;
            fprintf(targetfile, "%c\n", ch);
            ch = fgetc(firstfile);
            while(isspace(ch)) {
                ch = fgetc(firstfile);
            }
            flag = 1;
            flag3 = 1;
            tabcounter++;
        }
        if(ch == '}') {
            tabcounter--;
            if(flag3 == 1)
                spacecounter += 2;
            fseek(targetfile, -spacecounter, SEEK_CUR);
            fprintf(targetfile, "\0");
            spacecounter = 0;
            fprintf(targetfile,"\n");
            for(int i = 0; i < tabcounter; i++) {
                fprintf(targetfile,"    ");
            }
            fprintf(targetfile,"}");
            while(isspace(ch)) {
                ch = fgetc(firstfile);
            }
            flag2 = 0;
            flag3 = 0;
        }
        if(ch == '}') {
            ch = fgetc(firstfile);
            continue;
        }
        else if(ch == '{')
            continue;
        if(flag == 1) {
            for(int i = 0; i < tabcounter; i++) {
                fprintf(targetfile,"    ");
            }
            flag = 0;
        }
        fprintf(targetfile, "%c", ch);
        if(isspace(ch) == 0) {
            spacecounter = 0;
        }
        if(isspace(ch) == 0 && ch != '{' && ch != '}') {
            flag2 = 1;
        }
        if(ch != '{' && isspace(ch) == 0)
            flag3 = 0;
        if(ch == '\n') {
            flag = 1;
        }
        ch = fgetc(firstfile);
    }
    fclose(firstfile);
    fclose(targetfile);
    remove(path);
    rename(path2, path);
}

void compareFiles(char path1[], char path2[]) {
    char temp[1000];
    FILE *file1 = fopen(path1, "r");
    FILE *file2 = fopen(path2, "r");
    if(file1 == NULL || file2 == NULL) {
        fclose(file1);
        fclose(file2);
        puts("the file doesn't exist");
        return;
    }
    int num1 = lineNumber(path1);
    int num2 = lineNumber(path2);
    char line1[1000], line2[1000];
    int linecounter = 0, anscounter = 0;
    while(1) {
        if(fgets(line1, sizeof(line1), file1) == NULL || fgets(line2, sizeof(line2), file2) == NULL) {
            break;
        }
        linecounter++;
        if(strlen(line1) == 0 || strlen(line2) == 0) {
            break;
        }
        if(strcmp(line1, line2) != 0) {
            anscounter++;
            if(arman == 0) {
                printf("============ #%d ============\n", linecounter);
                printf("%s",line1);
                if(linecounter == num1)
                printf("\n");
                printf("%s",line2);
                if(linecounter == num2)
                printf("\n");
            }
            else {
                sprintf(temp, "============ #%d ============\n%s", linecounter, line1);
                strcat(arman_string, temp);
                if(linecounter == num1) {
                    sprintf(temp, "\n");
                    strcat(arman_string, temp);
                }
                sprintf(temp, "%s",line2);
                strcat(arman_string, temp);
                if(linecounter == num2) {
                    sprintf(temp, "\n");
                    strcat(arman_string, temp);
                }
            }
        }
    }
    fclose(file1);
    fclose(file2);
    if(num1 > num2) {
        if(arman == 0)
            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", num2 + 1, num1);
        else {
            sprintf(temp, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", num2 + 1, num1);
            strcat(arman_string, temp);
        }
        printLines(path1, num2 + 1);
    }
    else if(num2 > num1) {
        if(arman == 0)
            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", num1 + 1, num2);
        else {
            sprintf(temp, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", num1 + 1, num2);
            strcat(arman_string, temp);
        }
        printLines(path2, num1 + 1);
    }
    else if(anscounter == 0) {
        if(arman == 0)
        puts("files are the same");
        else
        printf(arman_string, "files are the same\n");
    }
}

int lineNumber(char path[]) {
    FILE *file = fopen(path, "r");
    int counter = 0;
    char line[1000];
    while(fgets(line, sizeof(line), file) != NULL) {
        if(strlen(line) == 0)
        break;
        counter++;
    }
    fclose(file);
    return counter;
}

void printLines(char path[], int first) {
    FILE *file = fopen(path, "r");
    int counter = 0;
    char line[1000], temp[1000];
    while(fgets(line, sizeof(line), file) != NULL) {
        counter++;
        if(counter >= first) {
            if(arman == 0)
                printf("%s", line);
            else {
                sprintf(temp, "%s", line);
                strcat(arman_string, temp);
            }
        }
    }
    if(arman == 0)
    printf("\n");
    else
    strcat(arman_string, "\n");
    fclose(file);
}

void printLine(int line, char path[], char path2[]) {
    FILE *file = fopen(path, "r");
    if(file == NULL) {
        puts("file doesn't exist");
        arman = 0;
        return;
    }
    int counter = 0;
    char line_string[1000], temp[1000];
    while(fgets(line_string, sizeof(line_string), file) != NULL) {
        counter++;
        if(counter == line) {
            sprintf(temp, "%s: %s", path2, line_string);
            if(arman == 0)
                strcat(grep_string, temp);
            else
                strcat(arman_string, temp);
            if(line == lineNumber(path)) {
                sprintf(temp, "\n");
                if(arman == 0)
                    strcat(grep_string, temp);
                else
                    strcat(arman_string, temp);
            }
            fclose(file);
            return;
        }
    }
}

static void listDir(const char *path, int depth, int level) {
    char temp[1000];
    if(depth != -1 && level > depth)
        return;
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    while((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.') {
            for(int i = 1; i < level; i++) {
                if(arman == 0)
                printf("| ");
                else
                strcat(arman_string, "| ");
            }
            if(arman == 0)
            printf("|---- %s",entry->d_name);
            else {
            sprintf(temp, "|---- %s",entry->d_name);
            strcat(arman_string, temp);
            }
            if(entry->d_type == DT_DIR) {
                if(arman == 0)
                printf(":\n");
                else
                strcat(arman_string, ":\n");
                char path2[1000];
                strcpy(path2, path);
                strcat(path2, "/");
                strcat(path2, entry->d_name);
                listDir(path2, depth, level + 1);
            }
            else{
                if(arman == 0)
                printf("\n");
                else
                strcat(arman_string, "\n");
            }
        }
    }
    closedir(dir);
}