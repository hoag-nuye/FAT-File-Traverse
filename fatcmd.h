/*-------------Script------------*/
/*
0. FatCmdOpen()
	0.1 CmdScanFileName()
	0.2 FatOpen("file_name")
	
1. ReadCmdJumpinEntry()
	1.1 FatEIF() == 1
		ReadCmdAllEntry()
		When: FatEOD() != 1 :
			- FatReadFile()
			- CmdDisplayFile()
	1.2 FatEID()
		ReadCmdFileEntry()
		When: FatEOE() != 1 :
			- FatReadEntry() -> 
			- CmdDisplayEntry()

2. JumpinCmdEntry()
	2.1 CmdScanJumpinEntry()
	2.2 FatJumpinEntry(idEntry)
3. BackCmdEntry()
	3.1 FatBackEntry()
	
1., 2., 3. are States
0. can become a State if wanting to open more than a fat file
*/
/*----------Deployment----------*/
/*==========DEFINE VARIABLE==========*/


/*==========DEFINE STRUCT==========*/


