#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "asm.h"

opcode_S opcodes[] = {{"and",AND},{"eor",EOR},{"sub",SUB},{"rsb",RSB},{"add",ADD},{"sbc",SBC},{"rsc",RSC},{"tst",TST},{"teq",TEQ},{"cmp",CMP},{"cmn",CMN},{"orr",ORR},{"mov",MOV},{"bic",BIC},{"mvn",MVN}};

char *registers[] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "sp", "lr", "pc"};

int main(int argc, char *argv[]){
    
    if(argc < 2){
        printf("Improper execution \n\t Please supply an object file to create\n");
    }

    char *source = argv[1];
    
    FILE *source_file = fopen(source, "r");
    
    char *object = strtok(source, ".");
    
    if(object != NULL){
        
        strcat(object, ".o");
        
        FILE *object_file = fopen(object, "w+"); 
    
    }

    int cow = 10;

    char *instruction = int2bin(cow);

    printf("%s\n", instruction);

    char *add = "add";

    opcode opadd = determine_opcode(add);

    printf("Opcode: %s has value %d\n", add, opadd);

    free(instruction);

    return 0;
}

condition determine_condition(char *token){

    for(unsigned int i = 0; i < ((sizeof(condition_set)/sizeof(condition_type))); i++){

        if(!strcasecmp(condition_set[i].key,token)){

            return condition_set[i].cond;

        }
        
    }
    return AL; //This should never be reached

}

opcode determine_opcode(char *opcode_s){

    for(unsigned int i = 0; i < (sizeof(opcodes)/sizeof(opcode_S)); i++){

        if(!strcasecmp(opcodes[i].key, opcode_s)){
            
            return opcodes[i].code;

        }

    }
    
    return ERR; //This shouldn't happen

}

int determine_register(char *rgstr){

    for(unsigned int i = 0; i < sizeof(registers)/sizeof(registers[0]); i++){

        if(!strcasecmp(registers[i], rgstr)){

            return i;

        }

    }
    return -1;

}

char *int2bin(arm_instruction instruct){
       
    if(instruct == 0){

        return NULL;

    }

    int i = 0;

    while(instruct >> i){
        
        i++;
    
    }
    
    int count = 2;
    
    int modulo = 0;

    char *bin_ins = malloc((i+1) * sizeof(char));

    for(int j = i-1; j >= 0; j--){
       
        modulo = instruct % 2;

        instruct /= count;

        bin_ins[j] = (char)('0' + modulo);
    }

    bin_ins[strlen(bin_ins)] = '\0';

    return bin_ins;

}

void handle_line(FILE *read, FILE *obj){

     

}

char *handle_data(char *op, char *condition_p, char *rn, char *rd){

    int register_s[2];

    arm_instruction instr = DATA;

    condition cond = determine_condition(condition_p);

    opcode opcode_in = determine_opcode(op);

    register_s[0] = determine_register(rn);

    register_s[1] = determine_register(rd);

    instr = DATA | (opcode_in << 21) | (cond << 28)| (register_s[0]<<12) | (register_s[1]<<16);
    
    return int2bin(instr);

}


