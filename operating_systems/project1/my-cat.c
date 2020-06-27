#include <stdio.h>
#include <stdlib.h>
#define LINE_SIZE 100

int main(int argc, char *argv[]){

	int i;
	FILE *fp[argc-1]; //file pointer
	char str_buf[LINE_SIZE]; 
	
	if (argc == 1){  //명령어만 있고 file명을 입력하지 않았을 때
		printf("There is no file\n");
        exit(1);
    }
    for(i =1; i < argc ; i++) //명령어 뒤에 있는 file을 읽어서 print하는 과정
    {
        fp[i] = fopen(argv[i],"r"); //file open

        if (fp[i] == NULL){ //파일을 못 열었을 때 경고 메세지
            printf("my-cat: cannot open file\n");
            exit(1);
        }
        else{ //열린 파일의 텍스트를 읽어서 print함
            while(1){ //한 줄 씩 str_buf에 저장하고 print함
                if(fgets(str_buf, LINE_SIZE,fp[i]) != NULL){ 
                    printf("%s", str_buf);
                }
                else
                    break;
            }
            fclose(fp[i]); //close file
        }
    }
    exit(0);
	
}
