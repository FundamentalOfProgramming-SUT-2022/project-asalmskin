#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <ctype.h>
#include <dirent.h>

int getInput(char*, WINDOW*, WINDOW*, WINDOW*);
int openFile(char*, WINDOW*, WINDOW*, WINDOW*);
int saveWithOldName(char*, WINDOW*, WINDOW*, WINDOW*);
int saveWithNewName(char*, WINDOW*, WINDOW*, WINDOW*);
void cutOrCoppy(int, int, int, int, WINDOW*, int);
void paste(int, int, WINDOW*);
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
void showLines(char*, int, int, WINDOW*);

int file_line = 0, saved = 1, this_line = 1, number_of_chars[50];
char file_name[1000];
char *clipboard;
int arman = 0;
int arman_output = 0;
char arman_string[100000];
char grep_string[100000];

int main() {
    makeDirectory("./root");
    char status[10] = "NORMAL";
    char input[1000];
    initscr();
    start_color();
    noecho();
    cbreak();
    clear();
    refresh();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    WINDOW *code = newwin(23, 98, 0, 2);
    wbkgd(code, COLOR_PAIR(0));
    WINDOW *fileName= newwin(2 , 75, 24, 25);
    wbkgd(fileName, COLOR_PAIR(3));
    wattron(fileName, COLOR_PAIR(3));
    mvwprintw(fileName, 0, 1, " ");
    wattroff(fileName, COLOR_PAIR(3));
    wrefresh(fileName);
    WINDOW *win = newwin(2, 25, 24, 0);
    wattron(win, COLOR_PAIR(1));
    wbkgd(win, COLOR_PAIR(1));
    //box(win, 0, 0);
    mvwprintw(win, 0, 8, status);
    //wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
    WINDOW *mainwin = newwin(23, 100, 0, 0);
    WINDOW *command = newwin(3, 100, 27, 0);
    wattron(mainwin, COLOR_PAIR(2));
    for(int i = file_line; i < 23; i++) {
        wmove(mainwin, i, 0);
        wprintw(mainwin, "~");
    }
    wattroff(mainwin, COLOR_PAIR(2));
    wrefresh(mainwin);
    int y = 0, x = 2;
    int y1 = 0, x1 = 2;
    int x2, y2, flag = 0;
    int ch;
    while(1) {
        wattron(mainwin, COLOR_PAIR(0));
        for(int i = 0; i < file_line; i++) {
            wmove(mainwin, i, 0);
            wprintw(mainwin, "%d ", i+1);
        }
        wattroff(mainwin, COLOR_PAIR(0));
        wrefresh(mainwin);
        if(saved == 0) {
            mvwprintw(fileName, 0, 50, "+");
        }
        else {
            mvwprintw(fileName, 0, 50, " ");
        }
        wrefresh(fileName);
        wresize(code, file_line, 98);
        if(strcmp(status, "VISUAL") == 0) {
            noecho();
            ch = getch();
            if(ch == (int)'k' && y > 0) {
                this_line--;
                if(number_of_chars[this_line] + 2< x) {
                    x = number_of_chars[this_line];
                }
                move(--y, x);
            }
            else if(ch == (int)'j' && y < file_line - 1) {
                this_line++;
                if(number_of_chars[this_line] + 2 < x) {
                    x = number_of_chars[this_line] + 2;
                }
                move(++y, x);
            }
            else if(ch == (int)'l' && x < number_of_chars[this_line] + 2){
                move(y, ++x);
            }
            else if(ch == (int)'h' && x > 2) {
                move(y, --x);
            }
            else if(ch == 1) {
                x2 = x;
                y2 = y;
                flag = 1;
            }
            else if(ch == 'd' && flag == 1) {
                cutOrCoppy(x1 - 2, y1, x2 - 2, y2, code, 1);
                wrefresh(code);
                x = 2;
                y = 0;
                ch = 27;
                flag = 0;
            }
            else if(ch == 'y' && flag == 1) {
                cutOrCoppy(x1 - 2, y1, x2 - 2, y2, code, 0);
                ch = 27;
                flag = 0;
            }
            if(ch == 27){
                strcpy(status, "NORMAL");
                mvwprintw(win, 0, 8, status);
                wrefresh(win);
                move(29, 0);
            }
            else if(ch == 'i') {
                strcpy(status, "INSERT");
                if(file_line == 0) {
                    file_line = 1;
                    wresize(code, 1, 98);
                    x = 2;
                    y = 0;
                    move(y, x);
                    wrefresh(code);
                }
                mvwprintw(win, 0, 8, status);
                wrefresh(win);
                move(y, x);
            }
        }
        else if(strcmp(status, "NORMAL") == 0) {
            noecho();
            ch = getch();
            move(y, x);
            if(ch == (int)'k' && y > 0) {
                this_line--;
                if(number_of_chars[this_line] + 2 < x) {
                    x = number_of_chars[this_line] + 2;
                }
                move(--y, x);
            }
            else if(ch == (int)'j' && y < file_line - 1) {
                this_line++;
                if(number_of_chars[this_line] + 2 < x) {
                    x = number_of_chars[this_line] + 2;
                }
                move(++y, x);
            }
            else if(ch == (int)'l' && x < number_of_chars[this_line] + 2){
                move(y, ++x);
            }
            else if(ch == (int)'h' && x > 2) {
                move(y, --x);
            }
            else if(ch == 'p') {
                paste(y, x - 2, code);
                wresize(code, file_line, 98);
                wrefresh(code);
            }
            else if(ch == '/' || ch == ':') {
                move(29, 0);
                echo();
                wprintw(command, ":");
                wgetstr(command, input);
                wclear(command);
                if(getInput(input, command, code, fileName) != 0) {
                    y = 0;
                    x = 2;
                }
                wclear(command);
                wrefresh(command);
                if(strcmp(input, "q") == 0) {
                    break;
                }
                for(int i = file_line; i < 23; i++) {
                    wmove(mainwin, i, 0);
                    wprintw(mainwin, "~");
                }
                wrefresh(mainwin);
            }
            else if(ch == 'v') {
                wclear(command);
                wrefresh(command);
                strcpy(status, "VISUAL");
                mvwprintw(win, 0, 8, status);
                wrefresh(win);
                move(y, x);
                y1 = y;
                x1 = x;
            }
            else if(ch == 'i') {
                wclear(command);
                wrefresh(command);
                strcpy(status, "INSERT");
                if(file_line == 0) {
                    file_line = 1;
                    x = 2;
                    y = 0;
                    wresize(code, 1, 98);
                    move(y, x);
                    wrefresh(code);
                }
                mvwprintw(win, 0, 8, status);
                wrefresh(win);
                move(y, x);
            }
        }
        else if(strcmp(status, "INSERT") == 0) {
            noecho();
            ch = getch();
            if(ch == 27){
                strcpy(status, "NORMAL");
                mvwprintw(win, 0, 8, status);
                wrefresh(win);
                move(y, x);
            }
            else if(ch == '\n') {
                if(this_line == file_line) {
                    file_line++;
                }
                this_line++;
                saved = 0;
                wresize(code, file_line, 100);
                wprintw(code, "\n");
                x = 2;
                move(++y, x);
                wrefresh(code);
            }
            else if(ch == 8) {
                if(x > 2) {
                    saved = 0;
                    if(x == number_of_chars[this_line] + 2) {
                        number_of_chars[this_line]--;
                    }
                    x --;
                    mvwprintw(code, y, x - 2, " ");
                    move(y, x);
                    wrefresh(code);
                }
                else {
                    if(this_line != 1) {
                        saved = 0;
                        this_line--;
                        y--;
                        x = number_of_chars[this_line] + 2;
                        move(y, x);
                        wrefresh(code);
                    }
                }
            }
            else {
                saved = 0;
                if(x == number_of_chars[this_line] + 2) {
                    number_of_chars[this_line]++;
                }
                mvwprintw(code, y, x - 2, "%c", ch);
                x++;
                move(y, x);
                wrefresh(code);
            }
        }
        wattron(mainwin, COLOR_PAIR(0));
        for(int i = 0; i < file_line; i++) {
            wmove(mainwin, i, 0);
            wprintw(mainwin, "%d ", i+1);
        }
        wattroff(mainwin, COLOR_PAIR(0));
        wattron(mainwin, COLOR_PAIR(2));
        for(int i = file_line; i < 23; i++) {
            wmove(mainwin, i, 0);
            wprintw(mainwin, "~");
        }
        wattroff(mainwin, COLOR_PAIR(2));
        wrefresh(mainwin);
        wrefresh(fileName);
    }
    wattroff(mainwin, COLOR_PAIR(2));
    refresh();
    //getch();
    endwin();
    return 0;
}

int getInput(char input[], WINDOW *command, WINDOW *code, WINDOW *fileName) {
    char word[1000];
    getFirstWord(input, word);
    if(strcmp(word, "open") == 0 && openFile(input, command, code, fileName) != 0) {
        return 1;
    }
    else if(strcmp(word, "save") == 0 && saveWithOldName(input, command, code, fileName) != 0) {
        return 1;
    }
    else if(strcmp(word, "saveas") == 0 && saveWithNewName(input, command, code, fileName) != 0) {
        return 1;
    }
    else {
        return 0;
    }
}


int openFile(char input[], WINDOW* command, WINDOW* code, WINDOW *fileName) {
    if(file_line > 0 && saved == 0) {
        saveWithOldName("\0", command, code, fileName);
    }
    char path[1000];
    if(getPath(input, path) == 0) {
        return 0;
    }
    char path2[1000] = ".";
    strcat(path2, path);
    if(checkPath(path2) == 0) {
        move(28, 0);
        wprintw(command, "invalid path(press enter)");
        int ch = wgetch(command);
        wrefresh(command);
        return 1;
    }
    FILE *file = fopen(path2, "r");
    if(file == NULL) {
        move(28, 0);
        wprintw(command, "the file doesn't exist(press enter)");
        int ch = wgetch(command);
        wrefresh(command);
    }
    fclose(file);
    file_line = lineNumber(path2);
    wclear(code);
    wrefresh(code);
    wresize(code, file_line, 100);
    if(file_line <= 23) {
        showLines(path2, 1, file_line, code);
    }
    else if(file_line > 23) {
        showLines(path2, 1, 23, code);
    }
    wclear(fileName);
    mvwprintw(fileName, 0, 1, path);
    wrefresh(fileName);
    strcpy(file_name, path);
    saved = 1;
    return 1;
}

int saveWithOldName(char input[], WINDOW* command, WINDOW* code, WINDOW *fileName) {
    char path[1000];
    if(strlen(file_name) == 0) {
        move(28, 0);
        wprintw(command, "the file doesn' have name, please enter a name(press enter)");
        int ch = wgetch(command);
        wgetstr(command, input);
        if(getPath(input, path) == 0) {
           return 0;
        }
        strcpy(file_name, path);
        wattron(fileName, COLOR_PAIR(3));
        mvwprintw(fileName, 0, 1, file_name);
        wattroff(fileName, COLOR_PAIR(3));
        wrefresh(fileName);
        wclear(command);
        wrefresh(command);
    }
    if(saved == 0){
        char path2[1000] = ".";
        strcat(path2, file_name);
        if(checkPath(path2) == 0) {
            move(28, 0);
            wprintw(command, "invalid path(press enter)");
            int ch = wgetch(command);
            wrefresh(command);
            return 1;
        }
        char temp[100];
        strcpy(temp, "./root/temp.txt");
        FILE *file = fopen(temp, "w");
        fclose(file);
        move(0,2);
        char the_line[100];
        for(int i = 0; i < file_line; i++) {
            for(int j = 0; j < number_of_chars[i + 1]; j++) {
                char ch;
                ch = mvwinch(code, i, j);
                if(ch == '\0') {
                    break;
                }
                else if(ch != '\n') {
                    the_line[j] = ch;
                    the_line[j + 1] = '\n';
                    the_line[j + 2] = 0;
                }
                else {
                    break;
                }
            }
            writeToFile(temp, the_line, i+1, 0);
        }
        remove(path2);
        rename(temp, path2);
        saved = 1;
    }
    return 1;
}

int saveWithNewName(char input[], WINDOW* command, WINDOW* code, WINDOW *fileName) {
    char path[1000];
    if(getPath(input, path) == 0) {
        return 0;
    }
    strcpy(file_name, path);
    if(saved == 0){
        char path2[1000] = ".";
        strcat(path2, file_name);
        if(checkPath(path2) == 0) {
            move(28, 0);
            wprintw(command, "invalid path(press enter)");
            int ch = wgetch(command);
            wrefresh(command);
            return 1;
        }
        char temp[100];
        strcpy(temp, "./root/temp.txt");
        FILE *file = fopen(temp, "w");
        fclose(file);
        move(0,2);
        char the_line[100];
        for(int i = 0; i < file_line; i++) {
            for(int j = 0; j < number_of_chars[i + 1]; j++) {
                char ch;
                ch = mvwinch(code, i, j + 2);
                if(ch == '\0') {
                    break;
                }
                else if(ch != '\n') {
                    the_line[j] = ch;
                    the_line[j + 1] = '\n';
                    the_line[j + 2] = 0;
                }
                else {
                    break;
                }
            }
            writeToFile(temp, the_line, i+1, 0);
        }
        remove(path2);
        rename(temp, path2);
        saved = 1;
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

void makeDirectory(const char *path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        mkdir(path);
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

int doesDirectoryExist(char* path) {
    struct stat st = {0};
    if(stat(path, &st) == -1) {
        return 0;
    }
    return 1;
}

void showLines(char path[], int first, int last, WINDOW* code) {
    wclear(code);
    FILE *file = fopen(path, "r");
    char current_line[1000];
    int counter = 0;
    resize_window(code, last - first + 1, 100);
    wclear(code);
    move(0, 2);
    while(fgets(current_line, sizeof(current_line), file) != NULL) {
        counter++;
        if(counter >= first && counter <= last) {
            move(counter - 1, 2);
            wprintw(code, "%s", current_line);
            number_of_chars[counter] = strlen(current_line);
            if(counter != last) {
            number_of_chars[counter]--;
            }
        }
    }
    fclose(file);
    wrefresh(code);
}

void writeToFile(char* path, char* string, int line, int character) {

    char path2[1000];
    int ch, ch2, counter = 0, isedited = 0, check = 1, flag = 0;
    FILE *firstfile = fopen(path, "r");
    strcpy(path2, "./root/.temp.txt");
    FILE *targetfile = fopen(path2, "w");
    if(firstfile == NULL || targetfile == NULL) {
        //puts("the file doesn't exist");
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
                    //puts("the line is too short");
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
        //puts("your file is too short");
        return;
    }
    else {
        fclose(targetfile);
        fclose(firstfile);
        remove(path);
        rename(path2, path);
    }
}

void cutOrCoppy(int x1, int y1, int x2, int y2, WINDOW *code, int flag) {
    if(y1 > y2) {
        int temp = y1;
        y1 = y2;
        y2 = temp;
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    else if(y1 == y2 && x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    char temp[100];
    strcpy(temp, "./root/temp.txt");
    FILE *file = fopen(temp, "w");
    fclose(file);
    move(0,2);
    char the_line[100];
    for(int i = 0; i < file_line; i++) {
        for(int j = 0; j < number_of_chars[i + 1]; j++) {
            char ch;
            ch = mvwinch(code, i, j);
            if(ch == '\0') {
                break;
            }
            else if(ch != '\n') {
                the_line[j] = ch;
                the_line[j + 1] = '\n';
                the_line[j + 2] = 0;
            }
            else {
                break;
            }
        }
        writeToFile(temp, the_line, i+1, 0);
    }
    int size = 0;
    if(y1 == y2) {
        size = x2 - x1 + 1;
    }
    else {
        size += number_of_chars[y1 + 1] - x1 + 2 + x2;
        for(int i = y1 + 1; i < y2; i++) {
            size += 1 + number_of_chars[i + 1];
        }
    }
    free(clipboard);
    clipboard = (char*)malloc(size);
    copyForward(temp, y1 + 1, x1, size);
    if(flag == 1) {
    removeForward(temp, y1 + 1, x1, size);
    file_line = lineNumber(temp);
    wclear(code);
    wrefresh(code);
    showLines(temp, 1, file_line, code);
    }
    remove(temp);
}

void copyForward(char path[], int line, int character, int size) {
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
        if(counter2 < 0) {
            puts("your file is too short!");
            return;
        }
        else{
            FILE *file = fopen(path, "r");
            ch = fgetc(file);
            int counter3 = 0;
            while(counter3 < counter2) {
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

void removeForward(char path[], int line, int character, int size) {
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
    else if(counter2 < 0)  {
        puts("your file is too short!");
        return;
    }
    else {
        if(line == 1) {
            counter2--;
        }
        int counter3 = 0;
        char path2[100];
        strcpy(path2, "./root/.temp.txt");
        FILE *file = fopen(path, "r");
        FILE *targetfile = fopen(path2, "w");
        while(counter3 < counter2) {
            ch = fgetc(file);
            fprintf(targetfile, "%c", ch);
            counter3++;
        }
        for(int i = 0; i < size; i++) {
            ch = fgetc(file);
        }
        while((ch = fgetc(file)) != EOF) {
            fprintf(targetfile, "%c", ch);
        }
        fclose(file);
        fclose(targetfile);
        remove(path);
        rename(path2, path);
    }
}

void paste(int y, int x, WINDOW* code) {
    char temp[100];
    strcpy(temp, "./root/temp.txt");
    FILE *file = fopen(temp, "w");
    fclose(file);
    move(0,2);
    char the_line[100];
    for(int i = 0; i < file_line; i++) {
        for(int j = 0; j < number_of_chars[i + 1]; j++) {
            char ch;
            ch = mvwinch(code, i, j);
            if(ch == '\0') {
                break;
            }
            else if(ch != '\n') {
                the_line[j] = ch;
                the_line[j + 1] = '\n';
                the_line[j + 2] = 0;
            }
            else {
                break;
            }
        }
        writeToFile(temp, the_line, i+1, 0);
    }
    writeToFile(temp, clipboard, y + 1, x);
    file_line = lineNumber(temp);
    wclear(code);
    wrefresh(code);
    showLines(temp, 1, file_line, code);
    remove(temp);
}