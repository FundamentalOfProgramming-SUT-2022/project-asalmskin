
                fprintf(targetfile, "\0");
            }
            if(flag) {
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