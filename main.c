/*-------------Scrip------------*/
/*
0.

Scanf State 2. or 3.

State : 1.
State : 2. -> set State is 1.
State : 3. -> set State is 1.

0. , 1., 2., 3. are States
*/

void main(){
	FatCmdOpen();
	ReadCmdJumpinEntry();
	JumpinCmdEntry(6);
	ReadCmdJumpinEntry();
	JumpinCmdEntry(4);
	ReadCmdJumpinEntry();
	BackCmdEntry();
	ReadCmdJumpinEntry();
	BackCmdEntry();
	ReadCmdJumpinEntry();
	BackCmdEntry();
	ReadCmdJumpinEntry();
	BackCmdEntry();
	ReadCmdJumpinEntry();
}
