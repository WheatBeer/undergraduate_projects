#include <stdio.h>
#include <stdlib.h>
#include <string.h> //string 비교를 위해 추가
#define LINE_SIZE 100 

int main(int argc, char *argv[]){

	FILE *fp; //file pointer(한 개의 파일만 읽음)
	char str_buf[LINE_SIZE];
	
    if(argc == 2){ //파일 없이 찾으려는 단어만 입력했을 때
        fp = stdin; //standard input을 file로 받음
        goto jump; //jump로 이동
    }

	if (argc != 3){ //grep word file 형식이 아닐 경우 오류 메세지
		printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    fp = fopen(argv[2],"r"); //입력한 파일을 open

    jump: //standard input일 때 argv[2]가 없으므로 skip

    if (fp == NULL){ //파일을 못 열었을 때 경고 메세지
        printf("my-grep: cannot open file\n");
        exit(1);
    }
        
    else{ //열린 파일의 텍스트를 읽어서 print함
        while(1){ //한 줄 씩 str_buf에 저장
            if(fgets(str_buf, LINE_SIZE,fp) != NULL){
                //찾으려는 단어가 포함되어 있는 줄만 print
                if(strstr(str_buf, argv[1]) != NULL){ 
                    //strstr: 두 개의 string을 비교해서 일치하면 1 아니면 0을 반환
                    printf("%s", str_buf); 
                }
            }
            else
                break;
        }
        fclose(fp);
        exit(0);
    }
}