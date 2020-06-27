#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
//#include "common.h"

int main(){
    fork();
    int *a = malloc(sizeof(int));
    printf("%p",a);
    return 0;
}
