#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include"asm.h"


char *conditions[] = {"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al"};
int lc;

/*Function to populate a spot in array with either 
 * a new linked symbol list, or a new node in an
 * already created linked list*/
void addLabel(char* name, int loc){

    symbol* new_symbol = (symbol*)malloc(sizeof(symbol));

    new_symbol->loc = loc;
    
    char *n_name= malloc(sizeof(name));
    new_symbol->name = n_name;
    
    strncpy(new_symbol->name, name,strlen(name)-1);
    new_symbol->name[strlen(name)-1] = '\0';
    
    int place;
    
    if(islower(name[0])){

        place = name[0] - 'a';

    }else{

        place = name[0] - 'A';

    }
    if(symT[place]==NULL){

        symT[place] =  new_symbol;
    }else{

        append(symT[place], new_symbol);

    }
}

/*Helper function to append symbols to a place in the list*/
void append(symbol* symb, symbol* new_symb){
    
    symbol* cur = symb;
    
    while(cur){
        
        cur = cur->next;
    
    }

    cur = new_symb;

}

/*A helper function to determine if a string is a label*/
bool isLabel(char *line){
    
    line[strlen(line)] = '\0';
    
    const char *delim = " ";
    
    char *token;
    
    token = strtok(line, delim);
    
    char *ticker = token;
    
    bool returned = isPresent(token, ':');
    
    while(returned && *ticker != ':'){
        
        if(!isalnum(*ticker)){
            
            returned = false;
        
        }
        
        ticker++;
    }
    
    return returned;
}

bool isCondition(char *condition){    
    
    if(!strcmp(condition, "")){
        
        return true;
    
    }
    
    for(unsigned int i = 0; i < (sizeof(conditions)/sizeof(*conditions)); i++){
        
        if(!strcmp(conditions[i], condition)){
           
            return true;
        
        }
    
    }
    
    return false;
 
}

/*The function used to detect which condition 
 * to attribute to the instruction*/
char *detectCondition(char *token){
    
    char *condition;

    for(unsigned int i = 0; i < (unsigned int)((sizeof(instructions)/sizeof(instructions[i]))); i++){

        if(!strncmp(instructions[i].key, token, strlen(instructions[i].key))){

            condition = calloc(strlen(instructions[i].key), strlen(instructions[i].key));
            
            strncpy(condition, (token + strlen(instructions[i].key)), strlen(token)-strlen(instructions[i].key));
            
            if(isCondition(condition)){

                if(!strcmp("", condition)){
                    free(condition);
                    return "AL";
                }
                
                for(unsigned int i =0; i <(sizeof(conditions)/sizeof(*conditions));i++){
                    
                    if(!strcmp(conditions[i], condition)){
                        free(condition); 
                        return conditions[i];
                    }

                }
                free(condition);
            }
            
            free(condition);
            
            return "ERROR";
        
        }

    }
    
    return "ERROR";

}

/* Helper function that detects the instruction
 * and removes the condition*/
char *detectInstruction(char *token){

    for(unsigned int i = 0; i < sizeof(instructions)/sizeof(keyword); i++){
        
        if(!strncmp(token, instructions[i].key, strlen(instructions[i].key))){
            
            return instructions[i].key;
        
        }
    
    }
    
    return ""; //Should never occur

}

/* The function that generates the token to placed into 
 * the file for the second pass*/
void handleInstruction(char *line, FILE *file){
  
    char *token;
    char *c_token;
    
    token = strtok(line, " ,");
    
    c_token = detectInstruction(token);

    char *condition = detectCondition(token);
    
    if(token != NULL){
       
        fprintf(file, "<%s,%s,%d,",c_token,condition,lc);
        
        token = strtok(NULL, " ,");
    
    }
    
    while(token != NULL){
        
        size_t len = strlen(token)-1;
        
        if(token[len] == '\n'){
            
            token[len] = '\0';
            
            fputs(token, file);
        
        } 

        else{
            
            fputs(token, file);
            
            fputc(',', file);
        
        }
        
        token = strtok(NULL, " ,-");
    
    }
    
    fputc('>', file);
    
    fputc('\n', file);

    lc+=2;

}

/* Helper function to determine if a string is an instruction*/
bool isInstruction(char *line){
    
    char *token;
    
    token = strtok(line, " ");
    
    for(unsigned int i = 0; i < (unsigned int)(sizeof(instructions)/sizeof(keyword)); i++){
        
        if(!strncmp(instructions[i].key, token, strlen(instructions[i].key))){
            
            return true;
        
        }
    
    }
    
    return false;

}

/*Memory cleanup*/
void free_table(){
    
    for(int i = 0; i < ALPHABET; i++){
        
        symbol *entry = symT[i];
        
        while(entry){
            
            symbol *place = entry->next;
            
            free(entry->name);

            free(entry);
            
            entry=place;
        
        }
    
    }

}

/* Function to check for a certain character in a line*/
bool isPresent(char* line, char delimiter){
    
    while(*line++){
        
        if(*line == delimiter){
            
            return true;
        
        }
    
    }
    
    return false;
}

/*Helper function to determine if a line is empty*/
int isEmpty(const char *line){
    
    while(isspace(*line++)){}
    
    return *line == '\0'? 1: 0;

}

/* TODO: Recognize the "START" macro and properly 
 * accomodate it */
void findStart(FILE *file){
    
    char *line = malloc(sizeof(char)*255);
    
    int count = 0;

    while(fgets(line, 255, file)){
        
        if(!strcmp(line, "START")){
            lc = 0; 
        }
    
    }

}

void printSymTable(){

    symbol *marker;

    for(int i = 0; i < ALPHABET; i++){
        
        if(symT[i]){
            printf("%d:\n", i+1);
            
            marker = symT[i];

            while(marker){

                printf("Name: %s Location: %d\n", marker->name, marker->loc);
                marker= marker->next;
            
            }
        
        }
    
    }

}

/*The function that removes the ":" from the label name
 * and pass it into the add label function*/
void handleLabel(char *label){
    
    char *token;
    
    token = strtok(label, ":");
    
    addLabel(token,lc);

}

/*Main function*/
int main(int argc, char* argv[]){
    
    for(int i = 0; i < ALPHABET; i++){
        symT[i] = NULL;
    
    }
    
    FILE *file, *obj;
    
    if(argc < 1){
        
        printf("Incorrect input, please provide a filename\n");
        
        return 0;
    
    }
    
    char line[255]; 

    if((file = fopen(argv[1], "r")) == NULL){
        
        printf("The file you requested either doesn't exist or you don't have proper permissions to access it\n");
    
    }

    obj = fopen("first.tok", "w+");
   
    lc = 0;
    while(fgets(line, 255,file)){
        
        if(isEmpty(line)){
            
            continue;
        
        }else{
            
            char *token = malloc(sizeof(line));
            
            strncpy(token, line, strlen(line));
            
            token[strlen(line)-1] = '\0';
            
            if(isLabel(token)){
                
                fputs("Label\n", obj);
                
                addLabel(token, lc);
            
            }else if(isInstruction(token)){
               
                handleInstruction(line,obj); 
            
            }else{
                
                fputs("other\n", obj);
            
            }
            
            free(token);
        
        }
    
    }
    
    printSymTable();
    
    fclose(file);
    
    fclose(obj);
    
    return 0;

}
