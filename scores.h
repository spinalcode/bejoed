char playerName[10][10];
int playerScore[10];

int isdigit(int c){
	return (c >= '0' && c <= '9');
}

// extracts the last positive integral value of string s
// returns...
// on success, the int value scanned
// -1, if the string is null, empty, or not terminated by a number
int extractLastIntegral(const char* s) {
    int value = -1;
    if (s && *s) {  // don't parse null and empty strings
        const char *scanstr = s + strlen(s) - 1;
        while (scanstr > s && isdigit(*(scanstr-1))) {
            scanstr--;
        }
        sscanf(scanstr,"%d", &value);
    }
    printf("Found:%d\n",value);
    return value;
}

void saveScores(){
#ifndef POK_SIM    
    File f;
    
    if(f.openRW("data/bejoed/scores.dat",1,0)){
        char tempText[40];
        for(int t=0; t<10; t++){
            sprintf(tempText,"name%d = %s\nscore%d = %d\n",t, playerName[t],t, (int)playerScore[t]);
            f.write(tempText, strlen(tempText));
        }

        f.close();
    }
#endif
}

void loadScores(){
#ifndef POK_SIM
    File f;
    if(f.openRO("data/bejoed/scores.dat")){
        char txtLine[64];
        char tempChar;
        int index = 0;
        int numScore = 0;
        
        while(f.read(&tempChar, 1)){

            if (tempChar == EOF) {
                txtLine[index] = '\0';
                break;
            }
    
            if (tempChar == '\n' || tempChar == '\r') {
                txtLine[index] = '\0';
                index=0;
                if(strncmp(txtLine, "name", strlen("name"))==0){
                    char *p = strrchr(txtLine, '=');
                    if (p && *(p + 1))
                        sprintf(playerName[numScore], "%9s\0", p + 1);
                    // terminate the string again, just incase
                    //playerName[numScore][10]=0;
                    // remove any spaces from start of station name
                    while(playerName[numScore][0]==' '){
                        memmove(playerName[numScore], playerName[numScore]+1, strlen(playerName[numScore]));
                    }
                }
                if(strncmp(txtLine, "score", strlen("score"))==0){
                    playerScore[numScore] = extractLastIntegral(txtLine);
                    if(numScore < 10)numScore++;
                }

                continue;
            }else{
                txtLine[index] = (char)tempChar;
            }
            index++;
        }



    }else{
        if(f.openRW("data/bejoed/scores.dat",1,0)){
            char tempText[40];
            for(int t=0; t<10; t++){
                sprintf(tempText,"name%d = NoName\nscore%d = 0\n",t,t);
                f.write(tempText, strlen(tempText));
                sprintf(playerName[t],"NoName");
                playerScore[t] = 0;
            }
    
            f.close();
        }
    }

#endif
}
