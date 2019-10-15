
#include "generator.h"
#include <ctype.h>
#include <errno.h>
#include "error_ifj.h"
#include "prefix_tool.h"

void generate(tIList *iList)
{
	static int firstrun = 0;
	char *call_bro = NULL;

    tIListItem *instruction = iList->head;
    htab_item_t *op1, *op2, *op3;

    // init file
    if (!firstrun) {
		firstrun = 1;
    	printf(".IFJcode17\n");
		printf("DEFVAR GF@guard\n");
		printf("MOVE GF@guard bool@true\n");
    	printf("JUMP _MAIN\n");
        printDefaultFunctions();
	}
    while (instruction != NULL) {
        // TODO zmena framu - kdy:
        // podle promenne, porad TF, pokud jsme
        // Kazda
        // TODO
        op1 = instruction->instr.op1;
        op2 = instruction->instr.op2;
        op3 = instruction->instr.op3;
        // process instruction
        switch(instruction->instr.oper) {

			case I_POP_GUARD:
				printf("POPS GF@guard");
				break;

            case I_MOVE:
                printf("MOVE ");
                add_param(op1);
                add_param(op2);
                break;
            case I_CREATEFRAME:
                printf("CREATEFRAME");
                break;
            case I_PUSHFRAME:
                printf("PUSHFRAME");
                break;
            case I_POPFRAME:
                printf("POPFRAME");
                break;
			case I_PURE_DEFVAR:
				printf("DEFVAR "); add_param(op1);
				break;
            case I_DEFVAR:
				printf("JUMPIFNEQ l_def%s%s GF@guard bool@true\n", op1->prefix, op1->name);
                printf("DEFVAR ");
                add_param(op1);
				printf("\nLABEL l_def%s%s", op1->prefix, op1->name);
                break;
            case I_CALL:
				call_bro = createUniqPrefix(),
				printf("JUMPIFNEQ def_%s%scall_bro GF@guard bool@true", call_bro, op1->name);
                printf("\nDEFVAR TF@call_bro_%s%s", call_bro, op1->name);
				printf("\nLABEL def_%s%scall_bro", call_bro, op1->name);
				printf("\nMOVE TF@call_bro_%s%s GF@guard", call_bro, op1->name);
				printf("\nPUSHFRAME"); 
				printf("\nMOVE GF@guard bool@true");
                printf("\nCALL %s", op1->name);
				printf("\nPOPFRAME");
				printf("\nMOVE GF@guard TF@call_bro_%s%s", call_bro, op1->name);
                break;
            case I_RETURN:
                printf("RETURN");
                break;
            // stack operations

            case I_PUSHS:
                printf("PUSHS ");
                add_param(op1);
                break;
            case I_POPS:
                printf("POPS ");
                add_param(op1);
                break;
            case I_CLEARS:
                printf("CLEARS");
                break;

                // arithmetic, relation, boolean and conversion instructions
            case I_ADD:
                printf("ADD "); add_param(op1); add_param(op2); add_param(op3);
                break;
            case I_SUB:
                printf("SUB "); add_param(op1);  add_param(op2); add_param(op3);
                break;
            case I_MUL:
                printf("MUL "); add_param(op1);  add_param(op2); add_param(op3);
                break;
            case I_DIV:
                printf("DIV "); add_param(op1);  add_param(op2); add_param(op3);
                break;
            case I_LT:
                printf("LT "); add_param(op1); add_param(op2); add_param(op3);
                break;
            case I_GT:
                printf("GT "); add_param(op1); add_param(op2); add_param(op3);
                break;
            case I_EQ:
                printf("EQ "); add_param(op1); add_param(op2); add_param(op3);
                break;
            case I_AND:
                printf("AND "); add_param(op1);  add_param(op2); add_param(op3);
                break;
            case I_OR:
                printf("OR "); add_param(op1); add_param(op2); add_param(op3);
                break;
            case I_NOT:
                printf("NOT "); add_param(op1);  add_param(op2);
                break;

            // stack arithmetic, relation, boolean and conversion instructions
            case I_ADDS:
                printf("ADDS");
                break;
            case I_SUBS:
                printf("SUBS");
                break;
            case I_MULS:
                printf("MULS");
                break;
            case I_DIVS:
                printf("DIVS");
                break;
            case I_LTS:
                printf("LTS");
                break;
            case I_GTS:
                printf("GTS");
                break;
            case I_EQS:
                printf("EQS");
                break;
            case I_ANDS:
                printf("ANDS");
                break;
            case I_ORS:
                printf("ORS");
                break;
            case I_NOTS:
                printf("NOTS");
                break;

            // conversions
            case I_INT2FLOAT:
                printf("INT2FLOAT "); add_param(op1); add_param(op2);
                break;
            case I_FLOAT2INT:
                printf("FLOAT2INT "); add_param(op1); add_param(op2);
                break;
            case I_FLOAT2R2EINT:
                printf("FLOAT2R2EINT "); add_param(op1); add_param(op2);
                break;
            case I_FLOAT2R2OINT:
                printf("FLOAT2R2OINT "); add_param(op1); add_param(op2);
                break;
            case I_FLOAT2CHAR:
                printf("FLOAT2CHAR "); add_param(op1); add_param(op2);
                break;
            case I_STRI2INT:
                printf("STRI2INT "); add_param(op1); add_param(op2);
                break;
            // stack conversions
            case I_INT2FLOATS:
                printf("INT2FLOATS");
                break;
            case I_FLOAT2INTS:
                printf("FLOAT2INTS");
                break;
            case I_FLOAT2R2EINTS:
                printf("FLOAT2R2EINTS");
                break;
            case I_FLOAT2R2OINTS:
                printf("FLOAT2R2OINTS");
                break;
            case I_FLOAT2CHARS:
                printf("FLOAT2CHARS");
                break;
            case I_STRI2INTS:
                printf("STRI2INTS");
                break;

            // input: output
            case I_READ:
				printf("WRITE string@?\\%03d\n", (int)' ');
                printf("READ "); add_param(op1);
                switch (op1->type) {
                    case KW_INTEGER:
                        printf("int");
                        break;
                    case KW_DOUBLE:
                        printf("float");
                        break;
                    case KW_STRING:
                        printf("string");
                        break;
                    case KW_BOOLEAN:
                        printf("bool");
                        break;
                    default:
                        break;
                }
                break;
            case I_WRITE:
                printf("WRITE ");
                add_param(op1);
                break;

                // string handling
            case I_CONCAT:
                printf("CONCAT ");
                add_param(op1);
                add_param(op2);
                add_param(op3);
                break;
            case I_STRLEN:
                printf("STRLEN ");
                add_param(op1);
                add_param(op2);
                break;
            case I_GETCHAR:
                printf("GETCHAR ");
                add_param(op1);
                add_param(op2);
                add_param(op3);
                break;
            case I_SETCHAR:
                printf("SETCHAR ");
                add_param(op1);
                add_param(op2);
                add_param(op3);
                break;

                // type handling
            case I_TYPE:
                printf("TYPE ");
                add_param(op1);
                add_param(op2);
                break;

                // program flow control
            case I_LABEL: // defining function
                printf("LABEL %s", op1->name);
				printf("\nCREATEFRAME");
                break;
            case I_JUMP: // calling function
                printf("JUMP %s", op1->name);
                break;

                // debug instructions
            case I_BREAK:
                printf("BREAK");
                break;
            case I_DPRINT:
                printf("DPRINT ");
                add_param(op1);
                break;

			case I_GUARD:
				printf("JUMPIFNEQ l_def%s%s GF@guard bool@true", op1->prefix, op1->name);
				break;
			case I_GUARD_END:
				printf("LABEL l_def%s%s", op1->prefix, op1->name);
				break;
            case I_GUARD_PROTECTION:
                printf("PUSHS GF@guard");
                break;

            case I_IF_ELSE_PROTECTION:
                printf("JUMPIFNEQ l_def_ifguard%s GF@guard bool@true", op1->prefix);
                printf("\nDEFVAR TF@%s_ifg", op1->prefix);
                printf("\nMOVE TF@%s_ifg bool@true", op1->prefix);
                printf("\nDEFVAR TF@%s_elseg", op1->prefix);
                printf("\nMOVE TF@%s_elseg bool@true", op1->prefix);
                printf("\nLABEL l_def_ifguard%s", op1->prefix);
                break;

            case I_IF:
                printf("PUSHS bool@false");
                printf("\nJUMPIFEQS %s_endif", op1->prefix);
                printf("\nMOVE GF@guard TF@%s_ifg", op1->prefix);
                printf("\nMOVE TF@%s_ifg bool@false", op1->prefix);
                break;

            case I_ENDIF:
                printf("JUMP %s_endelse", op2->prefix);
                printf("\nLABEL %s_endif", op1->prefix);
                printf("\nMOVE GF@guard TF@%s_elseg", op1->prefix);
                printf("\nMOVE TF@%s_elseg bool@false", op1->prefix);
                break;

            case I_ENDELSE:
                printf("LABEL %s_endelse", op1->prefix);
                printf("\nPOPS GF@guard");
                break;

            case I_WHILE: // label
				printf("PUSHS GF@guard\n");
                printf("LABEL while_%s", op1->prefix);
                break;
            // tu bude vyhodnoceni podminky a pushnuti na zasobnik
            case I_WHILE_COND:
                printf("PUSHS bool@false");
                printf("\nJUMPIFEQS endwhile_%s", op1->prefix); // endwhile
                break;
            // telo whilu
            case I_WHILE_END:
				printf("MOVE GF@guard bool@false\n");
                printf("JUMP while_%s", op1->prefix);
                printf("\nLABEL endwhile_%s", op1->prefix);
				printf("\nPOPS GF@guard");
                break;

            case I_WHILE_BREAK:
                printf("JUMP endwhile_%s", op1->prefix);
                break;

            case I_WHILE_CONTINUE:
                printf("JUMP while_%s", op1->prefix);
                break;
            /*
            case I_FOR:
                printf("\nDEFVAR "); add_param(op2); // definuju inkrementator
                printf("\nMOVE "); add_param(op2); printf("int@0");
                printf("\nLABEL for_%s", op1->prefix); // zacatek foru
                printf("\nPUSHS "); add_param(op3);    // pushnu constantu
                printf("\nPUSHS "); add_param(op2);        // pushnu inkrementator
                printf("\nJUMPIFEQS endfor_%s", op1->prefix); // porovnam inkrementator a constantu
                printf("\nCLEARS"); // pop inkremenatotru
                break;

            case I_FOR_END:
                printf("LABEL forcont_%s", op1->prefix);
                printf("\nADD "); add_param(op2); add_param(op2); printf("int@1"); // inkrementace jednicky
                printf("\nJUMP for_%s", op1->prefix); // skoc zas na zacatek
                printf("\nLABEL endfor_%s", op1->prefix);
                printf("\nLABEL breakfor_%s", op1->prefix);
                printf("\nCLEARS");
                break;

            case I_FOR_BREAK:
                printf("JUMP breakfor_%s", op1->prefix);
                break;

            case I_FOR_CONTINUE:
                printf("JUMP forcont_%s", op1->prefix);
                break;
             */

            case I_MAIN:
                printf("LABEL _MAIN");
                break;
            default:
                break;
        }
        printf("\n");
        // go to next instruction
        instruction = instruction->next;
    }
}


void add_param(htab_item_t *op)
{
    if (errno)
        return;
    char *frame = getFrame(op);
    if (op->const_value == NULL) { // if is not constant
        printf ("%s@%s%s ", frame, op->prefix, op->name);
    }
    else {
        char *tmp;
        switch (op->type) {
            case KW_INTEGER:
                printf("int@%s ", op->const_value);
                break;
            case KW_DOUBLE:
                printf("float@%s ", op->const_value);
                break;
            case KW_STRING:
                tmp = strToOutStr(op->const_value);
                if (tmp == NULL)
                    return;
                printf("string@%s ", tmp);
                free (tmp);
                break;
            default:// not gonna happen
                break;
        }
    }
}

char* strToOutStr(char *inStr) {
    int i = 0, j = 0;
    char *outStr = (char *) calloc(15, sizeof(char));
    if (!outStr) {
        errno = E_SYS;
        return NULL;
    }
    char buffer[10];
    int outLen = 15;
    while (inStr[i] != '\0') {
        // escape, white spaces, \ and #
        if ((inStr[i] >= 0 && inStr[i] < 32 ) || isspace(inStr[i]) || inStr[i] == '#' || inStr[i] == '\\') {
            snprintf(buffer, 10,"\\%03d", (int) inStr[i]);
            strcat(outStr, buffer);
            j += strlen(buffer);
        }
        else {
            outStr[j++] = inStr[i];
        }
        outStr[j] = '\0';

        if (outLen - strlen(outStr) < 10) { // if outstr is 10 chars before end of allocated mem
            outLen *= 2;
            outStr = (char *) realloc(outStr, (size_t) outLen);
            if (outStr == NULL) {
                errno = E_SYS;
                return NULL;
            }
        }
        i++;
    }

    return outStr;
}

char* getFrame(htab_item_t* op) {
    char *localFrame = "TF";
    char *globalFrame = "GF";

    if (op->is_global)
        return globalFrame;
    else
        return localFrame;
}

void printDefaultFunctions(){
    // length function
    printf(
               "\n"
               "LABEL length\n"
               "\tCREATEFRAME\n"
               "\tDEFVAR TF@mujstring\n"
               "\tPOPS TF@mujstring\n"
               "\tDEFVAR TF@len\n"
               "\tSTRLEN TF@len TF@mujstring\n"
               "\tPUSHS TF@len\n"
               "RETURN\n"
               "\n"
               "LABEL asc\n"
               "CREATEFRAME\n"
               "\tDEFVAR TF@ind\n"
               "\tDEFVAR TF@mojstring\n"
               "\tPOPS TF@ind\n"
               "\tPOPS TF@mojstring\n"
               "\tDEFVAR TF@jetook\n"
               "\tPUSHS TF@mojstring\t\n"
               "\tPUSHFRAME\n"
               "\tCALL length \n"
               "\tPOPFRAME\n"
               "\tDEFVAR TF@delka\t\n"
               "\tPOPS TF@delka\n"
               "\tLT TF@jetook TF@ind TF@delka\n"
               "\tDEFVAR TF@jetonadnulou\n"
               "\tLT TF@jetonadnulou int@0 TF@ind\n"
               "\tAND TF@jetook TF@jetonadnulou TF@jetook\n"
               "\tJUMPIFEQ INDNENIMIMOINDEXRETEZCEHH TF@jetook bool@true\n"
               "\tPUSHS int@0\n"
               "\tRETURN\n"
               "\tLABEL INDNENIMIMOINDEXRETEZCEHH\n"
               "\tDEFVAR TF@toret\n"
               "\tSUB TF@ind TF@ind int@1\n"
               "\tSTRI2INT TF@toret TF@mojstring TF@ind\n"
               "\tPUSHS TF@toret\n"
               "RETURN\n"
               "\n"
               "LABEL chr\n"
               "CREATEFRAME\n"
               "\tDEFVAR TF@in\n"
               "\tPOPS TF@in\n"
               "\tDEFVAR TF@vseok\t\n"
               "\tLT TF@vseok TF@in int@256\n"
               "\tJUMPIFEQ CISLOJEMENSIJAKDVESTEPADESATPET TF@vseok bool@true\n"
               "\tPUSHS string@42\n"
               "\tRETURN\n"
               "\tLABEL CISLOJEMENSIJAKDVESTEPADESATPET\n"
               "\tDEFVAR TF@ret\n"
               "\tINT2CHAR TF@ret TF@in\n"
               "\tPUSHS TF@ret\n"
               "RETURN\n"
               "\n"
               "\n"
               "LABEL substr\n"
               "CREATEFRAME\n"
               "\tDEFVAR TF@inString\n"
               "\tDEFVAR TF@startInt\n"
               "\tDEFVAR TF@charsNum\n"
               "\tPOPS TF@charsNum\n"
               "\tPOPS TF@startInt\n"
               "\tPOPS TF@inString\n"
               "\tSUB TF@startInt TF@startInt int@1 # diky tomuto davam i < 0\n"
               "\t# if s == \"\" or i <= 0: return \"\"\n"
               "\tDEFVAR TF@inStringLen\n"
               "\tPUSHS TF@inString\n"
			   "\tPUSHFRAME\n"
               "\tCALL length\n"
			   "\tPOPFRAME\n"
               "\tPOPS TF@inStringLen\n"
               "\tDEFVAR TF@inStringLenIsZero\n"
               "\tEQ TF@inStringLenIsZero int@0 TF@inStringLen\n"
               "\tDEFVAR TF@iLessThenZero\n"
               "\tLT TF@iLessThenZero TF@startInt int@0\n"
               "\tDEFVAR TF@all\n"
               "\tOR TF@all TF@iLessThenZero TF@inStringLenIsZero\n"
               "\tJUMPIFEQ STARTINTANDINSTRINGISJUSTFINE TF@all bool@false\n"
               "\t\tPUSHS string@\n"
               "\t\tJUMP KONECTEHLEZATRACENEFUNCESUBSTR\n"
               "\tLABEL STARTINTANDINSTRINGISJUSTFINE\n"
               "\t# if n < 0 or n > len(s) - i : return  string[i:]\n"
               "\tDEFVAR TF@nLessThanZero\n"
               "\tLT TF@nLessThanZero TF@charsNum int@0\n"
               "\tDEFVAR TF@lenMinusI\n"
               "\tDEFVAR TF@iminusOne\n"
               "\tSUB TF@lenMinusI TF@inStringLen TF@startInt\n"
               "\tDEFVAR TF@nBiggerThanLenMinusI\n"
               "\tGT TF@nBiggerThanLenMinusI TF@charsNum TF@lenMinusI\n"
               "\tDEFVAR TF@bothTwo\n"
               "\tOR TF@bothTwo TF@nBiggerThanLenMinusI TF@nLessThanZero\n"
               "\tJUMPIFEQ ALLISSNADOKKAMO TF@bothTwo bool@false\n"
               "\t\tDEFVAR TF@returnString\n"
               "\t\tMOVE TF@returnString string@\n"
               "\t\tDEFVAR TF@tmpChar\n"
               "\t\t# while cyklus\n"
               "\t\tDEFVAR TF@iIsLessThanStrLen\n"
               "\t\tLABEL WHILEONECYKLUSSUBSTR\n"
               "\t\t\tLT TF@iIsLessThanStrLen TF@startInt TF@inStringLen \n"
               "\t\t\tJUMPIFEQ WHILEONECYKLUSSUBSTRKONEC TF@iIsLessThanStrLen bool@false\t\n"
               "\t\t\t\tGETCHAR TF@tmpChar TF@inString TF@startInt\n"
               "\t\t\t\tCONCAT TF@returnString TF@returnString TF@tmpChar\n"
               "\t\t\t\tADD TF@startInt TF@startInt int@1\n"
               "\t\t\t\tJUMP WHILEONECYKLUSSUBSTR\n"
               "\t\tLABEL WHILEONECYKLUSSUBSTRKONEC\n"
               "\t\tPUSHS TF@returnString\t\n"
               "\t\tJUMP KONECTEHLEZATRACENEFUNCESUBSTR\n"
               "\tLABEL ALLISSNADOKKAMO\n"
               "\t# normalni postup\n"
               "\t\tDEFVAR TF@returnString\n"
               "\t\tMOVE TF@returnString string@\n"
               "\t\tDEFVAR TF@tmpChar\n"
               "\t\tLABEL WHILESECONDCYKLUSSUBSTR\n"
               "\t\t\tJUMPIFEQ WHILESECONDCYKLUSSUBSTRKONEC TF@charsNum int@0\n"
               "\t\t\tGETCHAR TF@tmpChar TF@inString TF@startInt \n"
               "\t\t\tCONCAT TF@returnString TF@returnString TF@tmpChar\n"
               "\t\t\tADD TF@startInt TF@startInt int@1\t\t\t\n"
               "\t\t\tSUB TF@charsNum TF@charsNum int@1\n"
               "\t\t\tJUMP WHILESECONDCYKLUSSUBSTR\t\t\n"
               "\t\tLABEL WHILESECONDCYKLUSSUBSTRKONEC\n"
               "\t\tPUSHS TF@returnString\n"
               "LABEL KONECTEHLEZATRACENEFUNCESUBSTR\n"
               "RETURN\n");
}

