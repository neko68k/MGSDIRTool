// MGSDIRTool.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <stdio.h>
#include <direct.h>

typedef struct{
	char name[8];	
	DWORD offset;		// block aligned. mult by 0x800

}TOC;

typedef struct{
	DWORD address;	//hardcoded psx memory address??
	WORD unk1;
	WORD type;
}DIR;

typedef struct{
	WORD unk1;
	BYTE numFiles;
	BYTE unk2;
}DIRHDR;


int main(int argc, char* argv[])
{
	DWORD TOCsize=0;		// in bytes
	
	TOC current;
	TOC next;
	
	DWORD curTocOfs=0;
	DWORD nextTocOfs=0;
	DWORD curFileOfs=0;
	DWORD dirFileSize=0;

	FILE *inFile=NULL;
	FILE *outFile=NULL;

	BYTE *buf=NULL;
	DWORD fileSize=0;

	char name[9];
	memset(name, 0, 9);
	inFile = fopen("STAGE.DIR", "rb");
	_mkdir("STAGE");
	_chdir("STAGE");
	fseek(inFile, 0, SEEK_END);
	dirFileSize = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	fread(&TOCsize, 4, 1, inFile);
	fread(&current, sizeof(TOC), 1, inFile);
	
	while(curTocOfs!=TOCsize+0x10){
		memcpy(name, current.name, 8);
		printf("Name: %s\n", name);
		fread(&next, sizeof(TOC), 1, inFile);
		nextTocOfs = ftell(inFile);
		
		if(curTocOfs==TOCsize+4)
			fileSize = (dirFileSize)-(current.offset*0x800);
		else
			fileSize = (next.offset*0x800)-(current.offset*0x800);
		buf = (BYTE*)calloc(fileSize, 1);
		fseek(inFile, current.offset*0x800, SEEK_SET);
		fread(buf, 1, fileSize, inFile);
		
		
		outFile = fopen(name, "wb");
		fwrite(buf, fileSize, 1, outFile);
		fclose(outFile);
		fseek(inFile, nextTocOfs, SEEK_SET);
		curTocOfs = nextTocOfs;
		current = next;
		free(buf);
	}
	_chdir("..");
	return 0;
}

