/*----------Include Library----------*/
#include <stdio.h>
#include <stdint.h>
#include "cmdio.h"
#include "fatcore.h"
/*----------Deployment----------*/
/*==========CORE FUNCTION==========*/
/*CmdScanFileName()*/
/*CmdDisplayFile()*/
void CmdDisplayFile(const char *filename , Address_t adrData, uint16_t MAX_READING){
	printf("__%s , %x, %d__\n", filename, adrData, MAX_READING);
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
    TYPE_FIELD,(entryInfo.Extension),
    SIZE_FIELD,entryInfo.Size
	);
}

/*CmdScanJumpinEntry()*/
