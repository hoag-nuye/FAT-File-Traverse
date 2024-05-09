/*----------Include Library----------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmdio.h"
#include "fatcore.h"
/*----------Deployment----------*/
/*==========SUPPORT CORE==========*/
/*Line Layout*/
void CmdDisplayLineLayout(){
	printf("|%-*s|%-*s|%-*s|%-*s|%-*s|\n", 
	ID_FIELD,"-------",
	NAME_FIELD,"---------------",
	TIME_FILE,"-----------------------",
	TYPE_FIELD,"-------",
	SIZE_FIELD,"---------------");
}
/*Instruction Layout*/
static void instructionLayout(uint8_t select){
//	printf("\033[H\033[J"); // delete all consolog
//	printf("\033[25;0H");   // Move the cursor to the 10th line
	
	//\033[%dm : color 
	//%6d\033[m" : string
	if(select == 1){
		printf("\033[0m%s\033[m\033[%dm%7s\033[m ", "1,2,3,...", 42, "Jumpin");//Create
		printf("\033[0m%c\033[m\033[%dm%7s\033[m ", 'b', 42, "Back");//Show
		printf("\033[0m%c\033[m\033[%dm%7s\033[m ", 'e', 41, "Exit");//Exit
		printf("\n");
	}
	if(select == 0){
		printf("\033[0m%c\033[m\033[%dm%7s\033[m ", 'b', 42, "Back");//Show
		printf("\033[0m%c\033[m\033[%dm%7s\033[m ", 'e', 41, "Exit");//Exit
		printf("\n");
	}

}

/*str to hex*/
static uint8_t chtoHex(char ch){
	uint8_t result;
	
	if('0' <= ch && '9' >= ch){
		result = ch - '0';
	}else if('A' <= ch && 'F' >= ch){
		result = ch - 'A' + 0xA;
	}else {
		result = HEX_INVALID;
	}
	
	return result;
}

//strtoHex: Convert string to dec <=> stdlib.h :  int number = atoi(str);
static strtoDec(uint8_t *str, uint16_t *decimal){
	*decimal = 0;
//    int sign = 1;
    int i = 0;

    // ignore empty characters at the beginning of the string
    while (str[i] == ' ') {
        i++;
    }

//    // identify sign
//    if (str[i] == '-') {
//        sign = -1;
//        i++;
//    } else if (str[i] == '+') {
//        i++;
//    }

    // Convert to decimal
    while (str[i] >= '0' && str[i] <= '9') {
        *decimal = *decimal * 10 + (str[i] - '0');
        i++;
    }
}
/*==========CORE FUNCTION==========*/
/*CmdScanFileName()*/
/*CmdDisplayFile()*/
void CmdDisplayFile(const char *filename , Address_t adrData, uint16_t MAX_READING){
	uint8_t status = 1;
	FILE *fileptr = fopen(filename, "r");
	if(fileptr == NULL){
		status = 0;
	}else{
		fseek(fileptr ,adrData , SEEK_SET);
		uint16_t idx;
		for(idx = 0; idx < MAX_READING; idx++){
			printf("%c", fgetc(fileptr));
		}
		fclose(fileptr);
	}
}

/*CmdDisplayEntryFields*/
void CmdDisplayEntryFields(){
	CmdDisplayLineLayout();
	printf("| %-*s\t|%-*s\t|%-*s\t|%-*s\t|%-*s\t|\n", 
	ID_FIELD,"stt",
	NAME_FIELD,"Name",
	TIME_FILE,"Date modified",
	TYPE_FIELD,"Type",
	SIZE_FIELD,"Size");
	CmdDisplayLineLayout();
}

/*CmdDisplayEntry()*/
void CmdDisplayEntry(Entry_t entryInfo, uint16_t entryOrder){
	printf("| %-*u\t|%-*s\t|%*d/%*d/%*d %*d:%*d\t|%-*s\t|%-*d\t| \n",
    ID_FIELD,entryOrder,
    NAME_FIELD,entryInfo.Name,
    DATE_DAY_FIELD,(entryInfo.Date & 0x1F),        // Extracting day (1-31)
    DATE_MONTH_FIELD,((entryInfo.Date >> 5) & 0xF),  // Extracting month (1-12)
    DATE_YEARS_FIELD,1980 + ((entryInfo.Date >> 9) & 0x7F), // Extracting year (1980-2107)
    HOURS_FIELD,(entryInfo.Time >> 11),          // Extracting hour (0-23)
    MINUTES_FIELD,((entryInfo.Time >> 5) & 0x3F),  // Extracting minute (0-59)
    TYPE_FIELD,entryInfo.Extension,
    SIZE_FIELD,entryInfo.Size
	);
}

/*CmdScan()*/
void CmdScan(char *dest){
	printf("\n");
	instructionLayout(1);	
	printf("What do you want ?: ");
	scanf("%s", dest);
	fflush(stdin);
	
}
/*CmdScanAgain()*/
void CmdScanAgain(char *dest){
	printf("\n");
	printf("you are in a file !");
	instructionLayout(0);
	printf("What do you want ?: ");
	scanf("%s", dest);
	fflush(stdin);
}
/*Change State*/
State_t ChangeState(char *inputState, uint16_t *id){
	State_t state;
	if(inputState[0] == 'b'){
		state = BACK;
	}else if(inputState[0] == 'e'){
		state = EXIT;
 	}else {
 		strtoDec(inputState, id);
 		state = JUMP;
	 }
	 return state;
	 
}





