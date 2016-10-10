#include<stdio.h>
#include<stdbool.h>

#ifndef ASM_H
#define ASM_H

#define ALPHABET 26

/*Our binary/hex values that represent conditions
 * @EQ if equal
 * @NE not equal
 * @CS unsigned more than or equal to
 * @CC unsigned less than
 * @MI if negative flag set
 * @PL if positive or zero
 * @VS if the overflow flag is set
 * @VC if the overflow flag is clear
 * @HI unsigned more than
 * @LS unsigned less than or equal to
 * @GE signed greater than or equal to
 * @LT signed less than
 * @GT signed greater than
 * @LE signed less than or equal to
 * @AL always (ignored)*/
typedef enum {
    EQ,
    NE,
    CS,
    CC,
    MI,
    PL,
    VS,
    VC,
    HI,
    LS,
    GE,
    LT,
    GT,
    LE,
    AL
}condition;

/*The codes for various instructions
 * @DATA The DATA PROCESSING instruction
 * @BX The BRANCH AND EXCHANGE instruction
 * @BL The BRANCH WITH LINK instruction
 * @MUL The MULTIPLY instruction
 * @MULA The MULTIPLY-ACCUMULATE instruction*/
typedef enum {
    DATA,
    BX = 0x97FF88,
    BL = 0xA000000,
    MUL = 0x90,
    MULA = 0x200090,
    LdrStr = 2000000
}instruction;

/*The opcode values for data processing instructions
 * @AND The LOGICAL AND operation code
 * @EOR The EXCLUSIVE OR operation code 
 * @SUB The SUBTRACTION operation code
 * @RSB The REVERSE SUBTRACTION operation code
 * @ADD The ADDITION operation code
 * @ADC The ADDITION WITH CARRY operation code
 * @SBC The SUBTRACTION WITH CARRY operation code
 * @RSC The REVERSE SUBTRACTION WITH CARRY operation code
 * @TST The TEST operation code
 * @TEQ The TEST BITWISE EQUALITY operation code
 * @CMP The COMPARE operation code
 * @CMN The COMPARE NEGATIVE operation code
 * @ORR The LOGICAL ORR operation code
 * @MOV The MOVE operation code
 * @BIC The BIT CLEAR operation code
 * @MVN The MOVE NEGATIVE REGISTER operation code
 * */
typedef enum{
    AND,
    EOR,
    SUB,
    RSB,
    ADD,
    ADC,
    SBC,
    RSC,
    TST,
    TEQ,
    CMP,
    CMN,
    ORR,
    MOV,
    BIC,
    MVN
}opcode;

/*Basic structure for the symbol table*/
typedef struct Symbol_S{
	char *name;
        int loc;
	struct Symbol_S* next;
}symbol;

symbol *symT[ALPHABET];

/*Condition structure*/
typedef struct condition_S{
    char *key;
    condition cond;
}condition_type;

condition_type condition_set[] = {{"eq",EQ},{"ne",NE},{"cs",CS},{"cc",CC},{"mi",MI},{"pl",PL},{"vs",VS},{"vc",VC},{"hi",HI},{"ls",LS},{"ge",GE},{"lt",LT},{"gt",GT},{"le",LE},{"al",AL}};

typedef struct keyword_S{
    char *key;
    instruction instruct;
}keyword;

keyword instructions[] = {{"mov",DATA},{"add",DATA},{"ldr",LdrStr},{"str",LdrStr},{"bx",BX},{"bl",BL},{"mul",MUL},{"mula",MULA}};

/*Symbol holding array*/
extern symbol *symT[ALPHABET];

/*Helper functions*/
void addLabel();

void append(symbol *symb, symbol *new_symb);

bool isabel(char *line);

bool isPresent(char *line, char delimiter);

int isEmpty(const char *line);

bool isInstruction(char *line);
#endif
