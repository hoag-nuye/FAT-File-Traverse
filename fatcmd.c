/*----------Include Library----------*/
#include <stdio.h>
#include <stdint.h>
#include "cmdio.h"
#include "fatcore.h"
#include "fatcmd.h"

#define FILE_NAME "./floppy.img"
/*----------Deployment----------*/
/*==========SUPPORT CORE==========*/
/*1.1 ReadCmdAllEntry()*/
static void ReadCmdAllEntry(){
	Entry_t entryInfo;
	uint16_t idEntry = 1;
	CmdDisplayEntryFields();
	FatRST();
	while(!FatEOE()){
		FatReadEntry(&entryInfo);
		if(!FatEOE()){
		CmdDisplayEntry(entryInfo, idEntry);
		idEntry++;
		}	
	}
	CmdDisplayLineLayout();
}
/*1.2 ReadCmdFileEntry()*/
static void ReadCmdFileEntry(){
	Address_t adrData;
	uint16_t MAX_READING;
	while(!FatEOD()){
		FatReadFile(&adrData, &MAX_READING);
		CmdDisplayFile(FILE_NAME, adrData, MAX_READING);	
	}
}


/*==========CORE FUNCTION==========*/
/*0. FatCmdOpen()*/
void FatCmdOpen(){
	FatOpen(FILE_NAME);
}
/*1. ReadCmdJumpinEntry()*/
void ReadCmdJumpinEntry(){
	if(FatEID()){
		ReadCmdAllEntry();
	}
	else if(FatEIF()){
		ReadCmdFileEntry();
	}
}

/*2. JumpinCmdEntry()*/
uint8_t JumpinCmdEntry(uint16_t id){
	//2.1 CmdScanJumpinEntry()
	//2.2 FatJumpinEntry(idEntry)
	return FatJumpinEntry(id);
}
/*3. BackCmdEntry()*/
void BackCmdEntry(){
	FatBackEntry();
}

/*FATcmdRUN*/
void FatCmdRun(){
	State_t state = READ;
	char input[5];
	uint16_t idJump;
	while (state != EXIT){
		switch(state){
			case READ:
				ReadCmdJumpinEntry();
				CmdScan(input);
				state = ChangeState(input,&idJump);
				break;
			case JUMP:
				while(FatEIF()){
					CmdScanAgain(input);
					state = ChangeState(input,&idJump);
					if(state == BACK || state == EXIT){
						break;
					}
				}
				if(FatEIF()){
					break;
				}
				system("cls");
				JumpinCmdEntry(idJump);
				state = READ;
				break;
			case BACK:
				system("cls");
				BackCmdEntry();
				state = READ;
				break;
			case EXIT: 
				break;
		}
	}
}





