#include<stdio.h>
#include<string.h>
#include "asm.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Improper execution \n\t Please supply an object file to create");
    }
    char *source = argv[1];
    FILE *source_file = fopen(source, "r");
    char *object = strtok(source, ".");
    if(object != NULL){
        strcat(object, ".o");
        FILE *object_file = fopen(object, "w+"); 
    }
}
