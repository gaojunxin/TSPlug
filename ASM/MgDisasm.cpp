// MgDisasm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MgAsmCom.h"
#include <cstdlib>

CMgDisasmBase		g_Disasm;
CMgAsmBase			g_Asm;

int main(int argc, char *argv[])
{
    int i, j, n;
    ulong l;
    char s[TEXTLEN], errtext[TEXTLEN];
    char *pasm;

    CMgDisasmBase::t_disasm da;
    CMgAsmBase::t_asmmodel  am;

    // Demonstration of Disassembler.
    printf("Disassembler:\n");

    //// Quickly determine size of command.
    //l=g_Disasm.Disasm("\xB8\x50\x11\x40\x00\xFF\xD0",
    //	10,0x400000,&da,DISASM_SIZE);
    //printf("Size of command = %i bytes\n",l);

	//ADD [475AE0],1 MASM mode, lowercase, don't show default segment
		g_Disasm.m_nIDEAL=0; g_Disasm.m_nLowercase=1; g_Disasm.m_nPutDefSeg=0;
	l=g_Disasm.Disasm("\xE8\x4B\x11\x00\x00",
		10,0x400000,&da,DISASM_CODE);
	printf("%3i  %-24s  %-24s   (MASM)\n",l,da.dump,da.result);


    // ADD [475AE0],1 IDEAL mode, uppercase, show default segment
    //g_Disasm.m_nIDEAL=1; g_Disasm.m_nLowercase=0; g_Disasm.m_nPutDefSeg=1;
    //l=g_Disasm.Disasm("\xE8\x4B\x11\x00\x00",
    //	10,0x400000,&da,DISASM_CODE);
    //printf("%3i  %-24s  %-24s   (IDEAL)\n",l,da.dump,da.result);

    //// CALL 45187C
    //l=g_Disasm.Disasm("\xE8\x1F\x14\x00\x00",
    //	5,0x450458,&da,DISASM_CODE);
    //printf("%3i  %-24s  %-24s   jmpconst=%08X\n",l,da.dump,da.result,da.jmpconst);
    //
    //// JNZ 450517
    //l=g_Disasm.Disasm("\xE8\x1F\x14\x00\x00",
    //	2,0x4504A3,&da,DISASM_CODE);
    //printf("%3i  %-24s  %-24s   jmpconst=%08X\n",l,da.dump,da.result,da.jmpconst);


    //g_Disasm.m_nIDEAL=0; g_Disasm.m_nLowercase=1; g_Disasm.m_nPutDefSeg=0;
    //l=g_Disasm.Disasm("\x0F\x9E\xC0",
    //	10,0x400000,&da,DISASM_CODE);
    //printf("%3i  %-24s  %-24s   (MASM)\n",l,da.dump,da.result);


    // Demonstration of Assembler.
    printf("\nAssembler:\n");

    // Assemble one of the commands above. First try form with 32-bit immediate.//32λ
    pasm = "mov eax,dword ptr [40E000]";
    printf("%s:\n", pasm);
    j = g_Asm.Assemble(pasm, 0x400000, &am, 0, 0, errtext);
    n = sprintf(s, "%3i  ", j);
    for (i = 0; i < j; i++) 
		n += sprintf(s + n, "%02X ", am.code[i]);
    if (j <= 0) 
		sprintf(s + n, "  error=\"%s\"", errtext);
    printf("%s\n", s);

    // Then variant with 8-bit immediate constant.//8λ
    //j=g_Asm.Assemble(pasm,0x400000,&am,0,2,errtext);
    //n=sprintf(s,"%3i  ",j);
    //for (i=0; i<j; i++) n+=sprintf(s+n,"%02X ",am.code[i]);
    //if (j<=0) sprintf(s+n,"  error=\"%s\"",errtext);
    //printf("%s\n",s);

    //// Error, unable to determine size of operands.
	//pasm="MOV [475AE0],1";
	//printf("%s:\n",pasm);
	//j=g_Asm.Assemble(pasm,0x400000,&am,0,0,errtext);
	//n=sprintf(s,"%3i  ",j);
	//for (i=0; i<j; i++) n+=sprintf(s+n,"%02X ",am.code[i]);
	//if (j<=0) sprintf(s+n,"  error=\"%s\"",errtext);
	//printf("%s\n",s);
 //   //

    //pasm="setle al";
    //printf("%s:\n",pasm);
    //j=g_Asm.Assemble(pasm,0x400000,&am,0,0,errtext);
    //n=sprintf(s,"%3i  ",j);
    //for (i=0; i<j; i++) n+=sprintf(s+n,"%02X ",am.code[i]);
    //if (j<=0) sprintf(s+n,"  error=\"%s\"",errtext);
    //printf("%s\n",s);


    // Show results.
    system("pause");


    return 0;
}

