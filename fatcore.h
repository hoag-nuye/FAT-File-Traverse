/*-------------Script------------*/
/*
FatOpen("file_name")
->SET_ROOT_DIRECTORY
FatEIF()
FatEID()
FatEOE()
FatEOD()
FatReadEntry()
FatReadFile()
FatJumpinEntry(idEntry)
FatBackEntry()
*/

/*----------Deployment----------*/
/*==========DEFINE VARIABLE==========*/
#define Address_t uint32_t
#define ENTRY_SIZE 0x20
#define Attribute_t uint8_t
#define TRUE 0x01
#define FALSE 0x00
#define HEX_INVALID 0xFF

/*==========DEFINE STRUCT==========*/
/*Struct for the object*/
typedef struct Entry_t{
	uint8_t Name[8];// 8bytes
	uint8_t Extension[3];//3bytes
	uint8_t Attributes;//1byte
	uint8_t Reserved[10];//10bytes
	uint16_t Time;//2bytes
	uint16_t Date;//2bytes
	uint16_t Cluster;//2bytes
	uint32_t Size;//4bytes	
}Entry_t;

typedef struct{
	uint8_t numOfFats; 
	uint16_t numOfBlocksFat;
	uint16_t numOfBytesBlock;
	uint16_t numOfRootDirectory;
	uint16_t blockRootDir;
	uint16_t blockFat;
	uint16_t blockDataArea;
}Boot_t;

/*Attribute in an entry*/
typedef enum{
	END_OF_DATA_12 = 0xFFF,
	_FILE = 0x00,
	READ_ONLY = 0x01,
	SUB_DIR = 0x10,
	LFN = 0x0F,	
}EntryAttribute_t;

/*Offset in a data field*/
typedef enum{
	OFFSET_NUM_FAT = 0x10, 
	OFFSET_NUM_BLOCK_FAT = 0x16,
	OFFSET_NUM_BYTES_BLOCK = 0x0b,
	OFFSET_NUM_ROOT_DIRECTORY = 0x11
}BootOffset_t;

typedef enum{
	OFFSET_ATTRIBUTE = 0xb,
}EntryOffset_t;

typedef struct{
	uint8_t fateid : 1;
	uint8_t fateoe : 1;
	uint8_t fateod : 1;
}FatReadingStatus;

typedef enum{
	READ,
	JUMP,
	BACK,
	EXIT,
	DONOTHING,	
}State_t;










