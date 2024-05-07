/*-------------Script------------*/
/*
CmdScanFileName()
CmdDisplayFile()
CmdDisplayEntry()
CmdScanJumpinEntry()
*/

/*----------Deployment----------*/
/*==========DEFINE VARIABLE==========*/
#define ID_FIELD 3
#define NAME_FIELD 12
#define DATE_DAY_FIELD 2
#define DATE_MONTH_FIELD 2
#define DATE_YEARS_FIELD 4
#define HOURS_FIELD 2
#define MINUTES_FIELD 2
#define TIME_FILE DATE_DAY_FIELD + DATE_MONTH_FIELD + DATE_YEARS_FIELD + HOURS_FIELD + MINUTES_FIELD + 4
#define TYPE_FIELD 5
#define SIZE_FIELD 12
