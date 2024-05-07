/*----------Include Library----------*/
#include <stdio.h>
#include <stdint.h>
#include "cmdio.h"
#include "fatcore.h"

#define FILE_NAME "./floppy.img"
/*----------Deployment----------*/
/*==========SUPPORT CORE==========*/
/*1.1 ReadCmdAllEntry()*/
static void ReadCmdAllEntry(){
	Entry_t entryInfo;
	uint16_t idEntry = 1;
	while(!FatEOE()){
		FatReadEntry(&entryInfo);
		if(!FatEOE()){
		CmdDisplayEntry(entryInfo, idEntry);
		idEntry++;
		}	
	}
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
void JumpinCmdEntry(uint8_t id){
	//2.1 CmdScanJumpinEntry()
	//2.2 FatJumpinEntry(idEntry)
	FatJumpinEntry(id);
}
/*3. BackCmdEntry()*/
void BackCmdEntry(){
	FatBackEntry();
}




