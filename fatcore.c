/*----------Include Library----------*/
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "fatcore.h"

/*----------Deployment----------*/
/*==========STATIC VARIABLE==========*/
static FILE *fileptr = NULL;
static Address_t adrRootDirectory;
static Address_t adrBackFile;
static uint16_t clusterBack;
static Boot_t bootInfo = {0};
static Entry_t entryInfo = {0};
static uint16_t fatmaxe = 0;// save max entry during read root directory
static uint8_t fatbtell = 0;// save the number of times the block value is taken in the FAT field
static uint16_t fatetell = 0;// save reading entry
static uint16_t fatftell = 0;// save reading the block in fat field
static uint8_t fateid = 0;// fat entry is directory
static uint8_t fateoe = 0;// fat end of entry
static uint8_t fateod = 0;// fat end of data



/*==========SUPPORT CORE==========*/
/*Read n byte at an address*/
static void readHexByteAddress(Address_t address, uint8_t sizeDist, void *dist){
	fseek(fileptr ,address, SEEK_SET);
	fread(dist, sizeDist, 1, fileptr);
}
/*Read 12bit data at an cluster_th*/
static void read_12bit_Fat(uint16_t cluster_th, uint32_t *dist){
	uint16_t temp;
	if(cluster_th %2 == 0){
		temp = cluster_th/2 +cluster_th ;
		temp+= bootInfo.blockFat*bootInfo.numOfBytesBlock;
		fseek(fileptr ,temp , SEEK_SET);
		fread(dist, 3, 1, fileptr);
		*dist = *dist & (0xFFF);
	}
	else{
		temp = (cluster_th-1)/2 +cluster_th - 1 ;
		temp+= bootInfo.blockFat*bootInfo.numOfBytesBlock;
		fseek(fileptr ,temp , SEEK_SET);
		fread(dist, 3, 1, fileptr);
		*dist = *dist >> 12;
	}
}
/*Find the address of data for a file*/
static Address_t addressDataOfBlock(uint16_t numOfBlock){
	return (bootInfo.blockDataArea - 2 + numOfBlock)*bootInfo.numOfBytesBlock;
}
/*Read 32byte data of entry*/
static void readEntry(Address_t adrCurrent){
	readHexByteAddress(adrCurrent,sizeof(entryInfo),&entryInfo);
}
/*Copy data*/
static void copyStr(void *src, void *dest, uint8_t sizeofData){
	uint8_t *pDest = (uint8_t *)dest;
    const uint8_t *pSrc = (const uint8_t *)src;
	uint8_t idx;
	for(idx = 0; idx < sizeofData; idx++){
		 pDest[idx] = pSrc[idx];
	}
}
static void copyEntry(Entry_t *src, Entry_t *dest){
	copyStr(&src->Name, &dest->Name, sizeof(entryInfo.Name));
	copyStr(&src->Extension, &dest->Extension, sizeof(entryInfo.Extension));
	dest->Attributes = src->Attributes;
	copyStr(&src->Reserved, &dest->Reserved, sizeof(entryInfo.Reserved));
	dest->Time = src->Time;
	dest->Date = src->Date;
	dest->Cluster = src->Cluster;
	dest->Size = src->Size;
}
/*Read Boot Sector*/
static void Read_BootSector(){
	//Number of fat
	readHexByteAddress(OFFSET_NUM_FAT,sizeof(bootInfo.numOfFats),&bootInfo.numOfFats);
	//Number of block fat
	readHexByteAddress(OFFSET_NUM_BLOCK_FAT,sizeof(bootInfo.numOfBlocksFat),&bootInfo.numOfBlocksFat);
	//Number of bytes per block
	readHexByteAddress(OFFSET_NUM_BYTES_BLOCK,sizeof(bootInfo.numOfBytesBlock),&bootInfo.numOfBytesBlock);
	//Caculate : starting block Root directory
	bootInfo.blockRootDir = (bootInfo.numOfFats*bootInfo.numOfBlocksFat + 1);
	//Number of root directory entries
	readHexByteAddress(OFFSET_NUM_ROOT_DIRECTORY,sizeof(bootInfo.numOfRootDirectory),&bootInfo.numOfRootDirectory);
	//Caculate : starting address Data area
	bootInfo.blockDataArea = bootInfo.blockRootDir + bootInfo.numOfRootDirectory*ENTRY_SIZE/bootInfo.numOfBytesBlock;
	//Caculate : Stating block FAT
	bootInfo.blockFat =  1;
}
/*Reset variable when working with new root directory*/
static void RESET(){
	fateoe = 0;
	fateod = 0;
	fatetell = 0;
	fatmaxe = 0;
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
void strtoDec(uint8_t *str, uint16_t *decimal){
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
/*==========DATASTREAM==========*/
static void SET_ADDRESS_ROOT_DIRECTORY(Address_t _adrRootDirectory){
	adrRootDirectory = _adrRootDirectory;
	RESET();
}
/*==========CORE FUNCTION==========*/
/*Check reading status*/
uint8_t FatEID(){
	return fateid;
}
uint8_t FatEIF(){
	return !FatEID();
}
uint8_t FatEOE(){
	return fateoe;
}
uint8_t FatEOD(){
	return fateod;
}
/*Open and close FILE*/
void FatOpen(const char *filename){
	uint8_t status = 1;
	fileptr = fopen(filename, "r");
	if(fileptr == NULL){
		status = 0;
	}else{
		Read_BootSector();
		SET_ADDRESS_ROOT_DIRECTORY(bootInfo.blockRootDir*bootInfo.numOfBytesBlock);
		fateid = TRUE;
	}
}

void FATClose(){
	if(fileptr != NULL){
		fclose(fileptr);
	}
}
/*Read each entry*/
void FatReadEntry(Entry_t *entryDist){
	Address_t adrCurrent;
	uint32_t nextDataBlock;
	if(adrRootDirectory == bootInfo.blockRootDir*bootInfo.numOfBytesBlock){
		nextDataBlock = END_OF_DATA_12;
	}else{
		read_12bit_Fat(fatftell, &nextDataBlock);
	}
	
	readEntry(adrRootDirectory + ENTRY_SIZE*fatetell);
	//Check Root directory is folder or not
	if(FatEOE() || !FatEID()){
		*entryDist = (Entry_t){0};
		return;
	}
	else if(entryInfo.Name[0] == 0x00){
		fateoe = TRUE;
		*entryDist = (Entry_t){0};
		return;
	}
	else if((fatetell == bootInfo.numOfBytesBlock/ENTRY_SIZE || fatetell == bootInfo.numOfRootDirectory) && nextDataBlock == END_OF_DATA_12){
		fateoe = TRUE;
		*entryDist = (Entry_t){0};
		return;
	}
	else if(fatetell == bootInfo.numOfBytesBlock/ENTRY_SIZE && nextDataBlock != END_OF_DATA_12){
		SET_ADDRESS_ROOT_DIRECTORY(addressDataOfBlock(nextDataBlock));
		adrCurrent = adrRootDirectory + ENTRY_SIZE*fatetell;
		fatftell++;
	}
	else{
		adrCurrent = adrRootDirectory + ENTRY_SIZE*fatetell;
		readEntry(adrCurrent);
	}
	Attribute_t attr;
	readHexByteAddress(adrCurrent + OFFSET_ATTRIBUTE, sizeof(entryInfo.Attributes), &attr);
	switch(attr){
		case LFN://0x0b => 0x0F -> LFN
			readEntry(adrCurrent + ENTRY_SIZE);
			fatetell ++;
			break;
		case SUB_DIR:
			readEntry(adrCurrent);
			clusterBack = entryInfo.Cluster;//save cluster to back when have jumped into a file
			break;
		case _FILE:
			readEntry(adrCurrent);
			break;
		case READ_ONLY:
			readEntry(adrCurrent);
			break;
		default :
			break;
	}
	copyEntry(&entryInfo, entryDist);
	fatetell ++;
	fatmaxe++;	
}
/*Read a file entry*/
void FatReadFile(Address_t *adrData, uint16_t *MAX_READING){
	if(FatEOD()){
		adrData = NULL;
		return;
	}
	uint32_t nextDataBlock;
	read_12bit_Fat(fatftell, &nextDataBlock);
	*MAX_READING = bootInfo.numOfBytesBlock;
	if(nextDataBlock == END_OF_DATA_12 && fatbtell == 0){
		*adrData = addressDataOfBlock(fatftell);
		fateod = TRUE;
	}else if(nextDataBlock == END_OF_DATA_12 && fatbtell != 0)
	{
		fateod = TRUE;
	}
	else{
		*adrData = addressDataOfBlock(nextDataBlock);
		fatftell ++;
	}
}
/*Jump in an entry*/
uint8_t FatJumpinEntry(uint16_t idEntry){
	uint8_t status = TRUE;
	if(idEntry > fatmaxe || idEntry == 0 ||  FatEIF()){
		status = FALSE;
	}else{	
		
		Entry_t findEntry;
		uint8_t idx;
		RESET();
		for(idx = 1; idx <=idEntry; idx++){
			FatReadEntry(&findEntry);
		}
		
		if(findEntry.Attributes == SUB_DIR){
			fateid = TRUE;
		}else{
			adrBackFile = adrRootDirectory;
			fateid = FALSE;//<=> FatEIF() = TRUE
		}
		
		if(findEntry.Cluster == 0x00){
			SET_ADDRESS_ROOT_DIRECTORY(bootInfo.blockRootDir*bootInfo.numOfBytesBlock);
		}else{
			SET_ADDRESS_ROOT_DIRECTORY(addressDataOfBlock(findEntry.Cluster));
			fatftell = findEntry.Cluster;
		}
	}
	return status;
}
/*Back an entry*/
uint8_t FatBackEntry(){
	uint8_t status = 1;
	Entry_t findEntry;
	if(adrRootDirectory == bootInfo.blockRootDir*bootInfo.numOfBytesBlock){
		SET_ADDRESS_ROOT_DIRECTORY(bootInfo.blockRootDir*bootInfo.numOfBytesBlock);
		status = 0;
	}
	else if(FatEIF()){
		SET_ADDRESS_ROOT_DIRECTORY(adrBackFile);
		fatftell = clusterBack;
	}else{	
		RESET();
		do{
			FatReadEntry(&findEntry);
		}while(!(findEntry.Name[0] == '.' && findEntry.Name[1] == '.'));
		if(findEntry.Cluster == 0x00){
			SET_ADDRESS_ROOT_DIRECTORY(bootInfo.blockRootDir*bootInfo.numOfBytesBlock);
		}
		else{
			SET_ADDRESS_ROOT_DIRECTORY(addressDataOfBlock(findEntry.Cluster));
			fatftell = findEntry.Cluster;
		}
	}
	fateid = TRUE;
	return status;
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
/*Reset when read new directory*/
void FatRST(){
	RESET();
}










