
            }
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
        }
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
            if(ch != ' ' && flag == 0) {
                flag = 1;
                counter2++;
            }
            else {
                flag = 0;
            }
        }
        fclose(myfile);
        return counter2;
    }
    fclose(myfile);
    return -1;
}