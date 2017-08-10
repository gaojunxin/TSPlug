// MgDisasmBase.cpp: implementation of the CMgDisasmBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MgAsmCom.h"
#include "MgAsmComDef.h"



//-------------------------------------------------------------------------------------------------------------------------
//全局变量声明:


// Bit combinations that can be potentially dangerous when executed:
t_cmddata		g_Dangerous[] =
{
    { 0x00FFFF, 0x00DCF7, 0, 0, 0, 0, 0, C_DANGER95, "Win95/98 may crash when NEG ESP is executed" },
    { 0x00FFFF, 0x00D4F7, 0, 0, 0, 0, 0, C_DANGER95, "Win95/98 may crash when NOT ESP is executed" },
    { 0x00FFFF, 0x0020CD, 0, 0, 0, 0, 0, C_DANGER95, "Win95/98 may crash when VxD call is executed in user mode" },
    { 0xF8FFFF, 0xC8C70F, 0, 0, 0, 0, 1, C_DANGERLOCK, "LOCK CMPXCHG8B may crash some processors when executed" },
    { 0x000000, 0x000000, 0, 0, 0, 0, 0, 0, "" }
};

//-------------------------------------------------------------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMgDisasmBase::CMgDisasmBase()
{
    //
    m_pDisasm	= NULL ;

    //
    m_pCmd		= NULL ;            // Pointer to binary data
    m_pPFixup	= NULL ;            // Pointer to possible fixups or NULL
    m_ulSize	= 0 ;               // Remaining size of the command buffer
    m_nMode		= 0 ;               // Disassembly mode (DISASM_xxx)

    //
    m_ulDataSize	= 0 ;			// Size of data (1,2,4 bytes)
    m_ulAddrSize	= 0 ;			// Size of address (2 or 4 bytes)

    //
    m_nSegPrefix	= 0 ;			// Segment override prefix or SEG_UNDEF
    m_nHasRM		= 0 ;			// Command has ModR/M byte
    m_nHasSIB		= 0 ;			// Command has SIB byte
    m_nDispSize		= 0 ;			// Size of displacement (if any)
    m_nImmSize		= 0 ;			// Size of immediate data (if any)
    m_nSoftError	= 0 ;			// Noncritical disassembler error
    m_nDump			= 0 ;           // Current length of command dump
    m_nResult		= 0 ;           // Current length of disassembly
    m_nAddComment	= 0 ;			// Comment value of operand


    //
    m_nIDEAL			= 0 ;         // Force IDEAL decoding mode
    m_nLowercase		= 0 ;         // Force lowercase display
    m_nTabArguments		= 1 ;         // Tab between mnemonic and arguments
    m_nExtraSpace		= 0 ;         // Extra space between arguments
    m_nPutDefSeg		= 0 ;         // Display default segments in listing
    m_nShowMemSize		= 0 ;         // Always show memory size
    m_nShowNEAR			= 0 ;         // Show NEAR modifiers
    m_nShortStringCmds	= 0 ;         // Use short form of string commands
    m_nSizeSens			= 0 ;         // How to decode size-sensitive mnemonics
    m_nSymbolic			= 0 ;         // Show symbolic addresses in disasm
    m_nFarCalls			= 0 ;         // Accept far calls, returns & addresses
    m_nDecodeVxd		= 0 ;         // Decode VxD calls (Win95/98)
    m_nPrivileged		= 0 ;         // Accept privileged commands
    m_nIOCommand		= 0 ;         // Accept I/O commands
    m_nBadShift			= 0 ;         // Accept shift out of range 1..31
    m_nExtraPrefix		= 0 ;         // Accept superfluous prefixes
    m_nLockedbus		= 0 ;         // Accept LOCK prefixes
    m_nStackAlign		= 0 ;         // Accept unaligned stack operations
    m_nIsWindowsNT		= 0 ;         // When checking for dangers, assume NT

}

CMgDisasmBase::~CMgDisasmBase()
{

}

//----------------------------------------------------------------------------
// Disassemble name of 1, 2 or 4-byte general-purpose integer register and, if
// requested and available, dump its contents. Parameter type changes decoding
// of contents for some operand types.
//----------------------------------------------------------------------------
void			CMgDisasmBase::DecodeRG(int index, int datasize, int type)
{
    int sizeindex;
    char name[9];
    if ( m_nMode < DISASM_DATA)
        {
            return;    // No need to decode
        }

    index &= 0x07;

    if( m_ulDataSize == 1 )
        {
            sizeindex = 0;
        }
    else if(m_ulDataSize == 2)
        {
            sizeindex = 1;
        }
    else if(m_ulDataSize == 4)
        {
            sizeindex = 2;
        }
    else
        {
            m_pDisasm->error = DAE_INTERN;
            return;
        }

    if( m_nMode >= DISASM_FILE )
        {
            strcpy(name, g_szRegName[sizeindex][index]);
            if(m_nLowercase)
                {
                    strlwr(name);
                }
            if(type < PSEUDOOP)               // Not a pseudooperand
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s"), name);
                }
        }
}

//----------------------------------------------------------------------------
// Disassemble name of 80-bit floating-point register and, if available, dump
// its contents.
//----------------------------------------------------------------------------
void			CMgDisasmBase::DecodeST(int index, int pseudoop)
{
    int i;
    char s[32];
    if (m_nMode < DISASM_FILE)
        {
            return;    // No need to decode
        }

    index &= 0x07;

    i = sprintf(s, ("%s(%i)"), (m_nLowercase ? ("st") : ("ST")), index);
    if (pseudoop == 0)
        {
            strcpy(m_pDisasm->result + m_nResult, s);
            m_nResult += i;
        };
}


//----------------------------------------------------------------------------
// Disassemble name of 64-bit MMX register.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeMX(int index)
{
    char *pr;
    if(m_nMode < DISASM_FILE)
        {
            return;    // No need to decode
        }
    index &= 0x07;
    pr = m_pDisasm->result + m_nResult;
    m_nResult += sprintf(pr, ("%s%i"), (m_nLowercase ? ("mm") : ("MM")), index);
}

//----------------------------------------------------------------------------
// Disassemble name of 64-bit 3DNow! register and, if available, dump its
// contents.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeNR(int index)
{
    char *pr;
    if(m_nMode < DISASM_FILE)
        {
            return;    // No need to decode
        }
    index &= 0x07;
    pr = m_pDisasm->result + m_nResult;
    m_nResult += sprintf(pr, ("%s%i"), (m_nLowercase ? ("mm") : ("MM")), index);
}


//----------------------------------------------------------------------------
// Disassemble memory/register from the ModRM/SIB bytes and, if available, dump
// address and contents of memory.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeMR(int type)
{
    int j, memonly, inmemory, seg;
    int c, sib;
    ulong dsize, regsize, addr;
    char s[TEXTLEN];

    //
    if( m_ulSize < 2 )
        {
            m_pDisasm->error = DAE_CROSS;    // ModR/M byte outside the memory block
            return;
        }

    m_nHasRM = 1;
    dsize = regsize = m_ulDataSize;              // Default size of addressed reg/memory
    memonly = 0;                           // Register in ModM field is allowed

    // Size and kind of addressed memory or register in ModM has no influence on
    // the command size, and exact calculations are omitted if only command size
    // is requested. If register is used, optype will be incorrect and we need
    // to correct it later.
    c = m_pCmd[1] & 0xC7;                     // Leave only Mod and M fields

    if( m_nMode >= DISASM_DATA )
        {
            // Register operand
            if ((c & 0xC0) == 0xC0)
                {
                    inmemory = 0;
                }
            else
                {
                    inmemory = 1;    // Memory operand
                }

            switch(type)
                {
                case MRG:                        // Memory/register in ModRM byte
                {
                    if (inmemory)
                        {
                            if(m_ulDataSize == 1)
                                {
                                    m_pDisasm->memtype = DEC_BYTE;
                                }
                            else if(m_ulDataSize == 2)
                                {
                                    m_pDisasm->memtype = DEC_WORD;
                                }
                            else
                                {
                                    m_pDisasm->memtype = DEC_DWORD;
                                }
                        }
                    break;
                }
                case MRJ:                        // Memory/reg in ModRM as JUMP target
                {
                    if( m_ulDataSize != 2 && inmemory )
                        {
                            m_pDisasm->memtype = DEC_DWORD;
                        }
                    if( m_nMode >= DISASM_FILE && m_nShowNEAR != 0 )
                        {
                            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s "), ( m_nLowercase ? ("near") : ("NEAR") ) );
                        }
                    break;
                }
                case MR1:                        // 1-byte memory/register in ModRM byte
                {
                    dsize = regsize = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_BYTE;
                        }
                    break;
                }
                case MR2:                        // 2-byte memory/register in ModRM byte
                {
                    dsize = regsize = 2;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_WORD;
                        }
                    break;
                }
                case MR4:                        // 4-byte memory/register in ModRM byte
                case RR4:                        // 4-byte memory/register (register only)
                {
                    dsize = regsize = 4;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_DWORD;
                        }
                    break;
                }
                case MR8:                        // 8-byte memory/MMX register in ModRM
                case RR8:                        // 8-byte MMX register only in ModRM
                {
                    dsize = 8;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_QWORD;
                        }
                    break;
                }
                case MRD:                        // 8-byte memory/3DNow! register in ModRM
                case RRD:                        // 8-byte memory/3DNow! (register only)
                {
                    dsize = 8;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_3DNOW;
                        }
                    break;
                }
                case MMA:                        // Memory address in ModRM byte for LEA
                {
                    memonly = 1;
                    break;
                }
                case MML:                        // Memory in ModRM byte (for LES)
                {
                    dsize = m_ulDataSize + 2;
                    memonly = 1;
                    if(m_ulDataSize == 4 && inmemory)
                        {
                            m_pDisasm->memtype = DEC_FWORD;
                        }
                    m_pDisasm->warnings |= DAW_SEGMENT;
                    break;
                }
                case MMS:                        // Memory in ModRM byte (as SEG:OFFS)
                {
                    dsize = m_ulDataSize + 2;
                    memonly = 1;
                    if(m_ulDataSize == 4 && inmemory)
                        {
                            m_pDisasm->memtype = DEC_FWORD;
                        }
                    if(m_nMode >= DISASM_FILE)
                        {
                            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s "), (m_nLowercase ? ("far") : ("FAR")) );
                        }
                    break;
                }
                case MM6:                        // Memory in ModRM (6-byte descriptor)
                {
                    dsize = 6;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_FWORD;
                        }
                    break;
                }
                case MMB:                        // Two adjacent memory locations (BOUND)
                {
                    dsize = (m_nIDEAL ? m_ulDataSize : m_ulDataSize * 2);
                    memonly = 1;
                    break;
                }
                case MD2:                        // Memory in ModRM byte (16-bit integer)
                case MB2:                        // Memory in ModRM byte (16-bit binary)
                {
                    dsize = 2;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_WORD;
                        }
                    break;
                }
                case MD4:                        // Memory in ModRM byte (32-bit integer)
                {
                    dsize = 4;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_DWORD;
                        }
                    break;
                }
                case MD8:                        // Memory in ModRM byte (64-bit integer)
                {
                    dsize = 8;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_QWORD;
                        }
                    break;
                }
                case MDA:                        // Memory in ModRM byte (80-bit BCD)
                {
                    dsize = 10;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_TBYTE;
                        }
                    break;
                }
                case MF4:                        // Memory in ModRM byte (32-bit float)
                {
                    dsize = 4;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_FLOAT4;
                        }
                    break;
                }
                case MF8:                        // Memory in ModRM byte (64-bit float)
                {
                    dsize = 8;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_FLOAT8;
                        }
                    break;
                }
                case MFA:                        // Memory in ModRM byte (80-bit float)
                {
                    dsize = 10;
                    memonly = 1;
                    if(inmemory)
                        {
                            m_pDisasm->memtype = DEC_FLOAT10;
                        }
                    break;
                }
                case MFE:                        // Memory in ModRM byte (FPU environment)
                {
                    dsize = 28;
                    memonly = 1;
                    break;
                }
                case MFS:                        // Memory in ModRM byte (FPU state)
                {
                    dsize = 108;
                    memonly = 1;
                    break;
                }
                case MFX:                        // Memory in ModRM byte (ext. FPU state)
                {
                    dsize = 512;
                    memonly = 1;
                    break;
                }
                default:                         // Operand is not in ModM!
                {
                    m_pDisasm->error = DAE_INTERN;
                    break;
                }
                }
        }

    addr = 0;

    // There are many possibilities to decode ModM/SIB address. The first
    // possibility is register in ModM - general-purpose, MMX or 3DNow!
    if((c & 0xC0) == 0xC0)            // Decode register operand
        {
            if(type == MR8 || type == RR8)
                {
                    DecodeMX(c);    // MMX register
                }
            else if(type == MRD || type == RRD)
                {
                    DecodeNR(c);    // 3DNow! register
                }
            else
                {
                    DecodeRG(c, regsize, type);    // General-purpose register
                }

            //
            if(memonly != 0)
                {
                    m_nSoftError = DAE_MEMORY;     // Register where only memory allowed
                }
            return;
        }


    // Next possibility: 16-bit addressing mode, very seldom in 32-bit flat model
    // but still supported by processor. SIB byte is never used here.
    if(m_ulAddrSize == 2)
        {
            if(c == 0x06)                    // Special case of immediate address
                {
                    m_nDispSize = 2;
                    if(m_ulSize < 4)
                        {
                            m_pDisasm->error = DAE_CROSS;    // Disp16 outside the memory block
                        }
                    else if(m_nMode >= DISASM_DATA)
                        {
                            m_pDisasm->adrconst = addr = *(ushort *)(m_pCmd + 2);
                            if (addr == 0)
                                {
                                    m_pDisasm->zeroconst = 1;
                                }
                            seg = SEG_DS;
                            Memadr(seg, (""), addr, dsize);
                        }
                }
            else
                {
                    m_pDisasm->indexed = 1;
                    if((c & 0xC0) == 0x40)       // 8-bit signed displacement
                        {
                            if(m_ulSize < 3)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    addr = (signed char)m_pCmd[2] & 0xFFFF;
                                }
                            m_nDispSize = 1;
                        }
                    else if((c & 0xC0) == 0x80)     // 16-bit unsigned displacement
                        {
                            if(m_ulSize < 4)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    addr = *(ushort *)(m_pCmd + 2);
                                }
                            m_nDispSize = 2;
                        }

                    if(m_nMode >= DISASM_DATA && m_pDisasm->error == DAE_NOERR)
                        {
                            m_pDisasm->adrconst = addr;
                            if(addr == 0)
                                {
                                    m_pDisasm->zeroconst = 1;
                                }
                            seg = g_addr16[c & 0x07].defseg;
                            Memadr(seg, g_addr16[c & 0x07].descr, addr, dsize);
                        }
                }
        }

    // Next possibility: immediate 32-bit address.
    else if(c == 0x05)                  // Special case of immediate address
        {
            m_nDispSize = 4;
            if(m_ulSize < 6)
                {
                    m_pDisasm->error = DAE_CROSS;    // Disp32 outside the memory block
                }
            else if(m_nMode >= DISASM_DATA)
                {
                    m_pDisasm->adrconst = addr = *(ulong *)(m_pCmd + 2);
                    if(m_pPFixup == NULL)
                        {
                            m_pPFixup = m_pCmd + 2;
                        }
                    m_pDisasm->fixupsize += 4;
                    if(addr == 0)
                        {
                            m_pDisasm->zeroconst = 1;
                        }
                    seg = SEG_DS;
                    Memadr(seg, (""), addr, dsize);
                }
        }

    // Next possibility: 32-bit address with SIB byte.
    else if ((c & 0x07) == 0x04)         // SIB addresation
        {
            sib = m_pCmd[2];
            m_nHasSIB = 1;
            *s = '\0';
            if(c == 0x04 && (sib & 0x07) == 0x05)
                {
                    m_nDispSize = 4;                    // Immediate address without base
                    if(m_ulSize < 7)
                        {
                            m_pDisasm->error = DAE_CROSS;          // Disp32 outside the memory block
                        }
                    else
                        {
                            m_pDisasm->adrconst = addr = *(ulong *)(m_pCmd + 3);
                            if(m_pPFixup == NULL)
                                {
                                    m_pPFixup = m_pCmd + 3;
                                }
                            m_pDisasm->fixupsize += 4;
                            if(addr == 0)
                                {
                                    m_pDisasm->zeroconst = 1;
                                }
                            if ((sib & 0x38) != 0x20)     // Index register present
                                {
                                    m_pDisasm->indexed = 1;
                                    if (type == MRJ)
                                        {
                                            m_pDisasm->jmptable = addr;
                                        }
                                }
                            seg = SEG_DS;
                        }
                }
            else                                // Base and, eventually, displacement
                {
                    if ((c & 0xC0) == 0x40)         // 8-bit displacement
                        {
                            m_nDispSize = 1;
                            if(m_ulSize < 4)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    m_pDisasm->adrconst = addr = (signed char)m_pCmd[3];
                                    if(addr == 0)
                                        {
                                            m_pDisasm->zeroconst = 1;
                                        }
                                }
                        }
                    else if ((c & 0xC0) == 0x80)      // 32-bit displacement
                        {
                            m_nDispSize = 4;
                            if (m_ulSize < 7)
                                {
                                    m_pDisasm->error = DAE_CROSS;    // Disp32 outside the memory block
                                }
                            else
                                {
                                    m_pDisasm->adrconst = addr = *(ulong *)(m_pCmd + 3);
                                    if (m_pPFixup == NULL)
                                        {
                                            m_pPFixup = m_pCmd + 3;
                                        }
                                    m_pDisasm->fixupsize += 4;
                                    if (addr == 0)
                                        {
                                            m_pDisasm->zeroconst = 1;
                                        }

                                    // Most compilers use address of type [index*4+displacement] to
                                    // address jump table (switch). But, for completeness, I allow all
                                    // cases which include index with scale 1 or 4, base or both.
                                    if (type == MRJ)
                                        {
                                            m_pDisasm->jmptable = addr;
                                        }
                                }
                        }
                    m_pDisasm->indexed = 1;
                    j = sib & 0x07;
                    if (m_nMode >= DISASM_FILE)
                        {
                            strcpy(s, g_szRegName[2][j]);
                            seg = g_addr32[j].defseg;
                        }
                }
            if ((sib & 0x38) != 0x20)          // Scaled index present
                {
                    if ((sib & 0xC0) == 0x40)
                        {
                            m_pDisasm->indexed = 2;
                        }
                    else if ((sib & 0xC0) == 0x80)
                        {
                            m_pDisasm->indexed = 4;
                        }
                    else if ((sib & 0xC0) == 0xC0)
                        {
                            m_pDisasm->indexed = 8;
                        }
                    else
                        {
                            m_pDisasm->indexed = 1;
                        }
                }
            if(m_nMode >= DISASM_FILE && m_pDisasm->error == DAE_NOERR)
                {
                    if ((sib & 0x38) != 0x20)        // Scaled index present
                        {
                            if (*s != '\0')
                                {
                                    strcat(s, ("+"));
                                }
                            strcat(s, g_addr32[(sib >> 3) & 0x07].descr);
                            if ((sib & 0xC0) == 0x40)
                                {
                                    m_pDisasm->jmptable = 0;            // Hardly a switch!
                                    strcat(s, ("*2"));
                                }
                            else if ((sib & 0xC0) == 0x80)
                                {
                                    strcat(s, ("*4"));
                                }
                            else if ((sib & 0xC0) == 0xC0)
                                {
                                    m_pDisasm->jmptable = 0;            // Hardly a switch!
                                    strcat(s, ("*8"));
                                }
                        }
                    Memadr(seg, s, addr, dsize);
                }
        }

    // Last possibility: 32-bit address without SIB byte.
    else                                 // No SIB
        {
            if ((c & 0xC0) == 0x40)
                {
                    m_nDispSize = 1;
                    if (m_ulSize < 3)
                        {
                            m_pDisasm->error = DAE_CROSS;    // Disp8 outside the memory block
                        }
                    else
                        {
                            m_pDisasm->adrconst = addr = (signed char)m_pCmd[2];
                            if (addr == 0)
                                {
                                    m_pDisasm->zeroconst = 1;
                                }
                        }
                }
            else if ((c & 0xC0) == 0x80)
                {

                    m_nDispSize = 4;
                    if (m_ulSize < 6)
                        {
                            m_pDisasm->error = DAE_CROSS;    // Disp32 outside the memory block
                        }
                    else
                        {
                            m_pDisasm->adrconst = addr = *(ulong *)(m_pCmd + 2);
                            if (m_pPFixup == NULL)
                                {
                                    m_pPFixup = m_pCmd + 2;
                                }
                            m_pDisasm->fixupsize += 4;
                            if (addr == 0)
                                {
                                    m_pDisasm->zeroconst = 1;
                                }
                            if (type == MRJ)
                                {
                                    m_pDisasm->jmptable = addr;
                                }
                        }
                }
            m_pDisasm->indexed = 1;
            if (m_nMode >= DISASM_FILE && m_pDisasm->error == DAE_NOERR)
                {
                    seg = g_addr32[c & 0x07].defseg;
                    Memadr(seg, g_addr32[c & 0x07].descr, addr, dsize);
                }
        }

}


//----------------------------------------------------------------------------
// Disassemble implicit source of string operations and, if available, dump
// address and contents.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeSO(void)
{

    if (m_nMode < DISASM_FILE)
        {
            return;    // No need to decode
        }
    if (m_ulDataSize == 1)
        {
            m_pDisasm->memtype = DEC_BYTE;
        }
    else if (m_ulDataSize == 2)
        {
            m_pDisasm->memtype = DEC_WORD;
        }
    else if (m_ulDataSize == 4)
        {
            m_pDisasm->memtype = DEC_DWORD;
        }
    m_pDisasm->indexed = 1;
    Memadr(SEG_DS, g_szRegName[m_ulAddrSize == 2 ? 1 : 2][REG_ESI], 0L, m_ulDataSize);
}


//----------------------------------------------------------------------------
// Disassemble implicit destination of string operations and, if available,
// dump address and contents. Destination always uses segment ES, and this
// setting cannot be overridden.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeDE(void)
{
    int seg;
    if (m_nMode < DISASM_FILE)
        {
            return;     // No need to decode
        }
    if (m_ulDataSize == 1)
        {
            m_pDisasm->memtype = DEC_BYTE;
        }
    else if (m_ulDataSize == 2)
        {
            m_pDisasm->memtype = DEC_WORD;
        }
    else if (m_ulDataSize == 4)
        {
            m_pDisasm->memtype = DEC_DWORD;
        }
    m_pDisasm->indexed = 1;
    seg = m_nSegPrefix;
    m_nSegPrefix = SEG_ES;   // Fake Memadr by changing segment prefix
    Memadr(SEG_DS, g_szRegName[m_ulAddrSize == 2 ? 1 : 2][REG_EDI], 0L, m_ulDataSize);
    m_nSegPrefix = seg;                     // Restore segment prefix
}


//----------------------------------------------------------------------------
// Decode XLAT operand and, if available, dump address and contents.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeXL(void)
{
    if (m_nMode < DISASM_FILE)
        {
            return;    // No need to decode
        }
    m_pDisasm->memtype = DEC_BYTE;
    m_pDisasm->indexed = 1;
    Memadr(SEG_DS, (m_ulAddrSize == 2 ? ("BX+AL") : ("EBX+AL")), 0L, 1);
}

//----------------------------------------------------------------------------
// Decode immediate operand of size constsize. If sxt is non-zero, byte operand
// should be sign-extended to sxt bytes. If type of immediate constant assumes
// this, small negative operands may be displayed as signed negative numbers.
// Note that in most cases immediate operands are not shown in comment window.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeIM(int constsize, int sxt, int type)
{
    int i;
    signed long data;
    ulong l;
    char name[TEXTLEN] = {0}, comment[TEXTLEN] = {0};

    //
    m_nImmSize += constsize;                    // Allows several immediate operands
    if (m_nMode < DISASM_DATA)
        {
            return;
        }

    l = 1 + m_nHasRM + m_nHasSIB + m_nDispSize + (m_nImmSize - constsize);
    data = 0;

    //
    if (m_ulSize < l + constsize)
        {
            m_pDisasm->error = DAE_CROSS;
        }
    else if (constsize == 1)
        {
            if (sxt == 0)
                {
                    data = (uchar)m_pCmd[l];
                }
            else
                {
                    data = (signed char)m_pCmd[l];
                }
            if (type == IMS && ((data & 0xE0) != 0 || data == 0))
                {
                    m_pDisasm->warnings |= DAW_SHIFT;
                    m_pDisasm->cmdtype |= C_RARE;
                }
        }
    else if (constsize == 2)
        {
            if (sxt == 0)
                {
                    data = *(ushort *)(m_pCmd + l);
                }
            else
                {
                    data = *(short *)(m_pCmd + l);
                }
        }
    else
        {
            data = *(long *)(m_pCmd + l);
            if (m_pPFixup == NULL)
                {
                    m_pPFixup = m_pCmd + l;
                }
            m_pDisasm->fixupsize += 4;
        }
    if (sxt == 2)
        {
            data &= 0x0000FFFF;
        }
    if (data == 0 && m_pDisasm->error == 0)
        {
            m_pDisasm->zeroconst = 1;
        }
    // Command ENTER, as an exception from Intel's rules, has two immediate
    // constants. As the second constant is rarely used, I exclude it from
    // search if the first constant is non-zero (which is usually the case).
    if (m_pDisasm->immconst == 0)
        {
            m_pDisasm->immconst = data;
        }
    if (m_nMode >= DISASM_FILE && m_pDisasm->error == DAE_NOERR)
        {
            if (m_nMode >= DISASM_CODE && type != IMU)
                {
                    i = Decodeaddress(data, name, TEXTLEN - m_nResult - 24, comment);
                }
            else
                {
                    i = 0;
                    comment[0] = '\0';
                }
            if (i != 0 && m_nSymbolic != 0)
                {
                    strcpy(m_pDisasm->result + m_nResult, name);
                    m_nResult += i;
                }
            else if (type == IMU || type == IMS || type == IM2 || data >= 0 || data < NEGLIMIT)
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%lX"), data);
                }
            else
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("-%lX"), -data);
                }

            //
            if (m_nAddComment && comment[0] != '\0')
                {
                    strcpy(m_pDisasm->comment, comment);
                }
        }
}

//----------------------------------------------------------------------------
// Decode VxD service name (always 4-byte).
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeVX(void)
{
    ulong l, data;

    //
    m_nImmSize += 4;                        // Allows several immediate operands
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    l = 1 + m_nHasRM + m_nHasSIB + m_nDispSize + (m_nImmSize - 4);
    if (m_ulSize < l + 4)
        {
            m_pDisasm->error = DAE_CROSS;
            return;
        }
    data = *(long *)(m_pCmd + l);
    if (data == 0 && m_pDisasm->error == 0)
        {
            m_pDisasm->zeroconst = 1;
        }
    if (m_pDisasm->immconst == 0)
        {
            m_pDisasm->immconst = data;
        }
    if (m_nMode >= DISASM_FILE && m_pDisasm->error == DAE_NOERR)
        {
            if ((data & 0x00008000) != 0 && memicmp(("VxDCall"), m_pDisasm->result, 7) == 0)
                {
                    memcpy(m_pDisasm->result, m_nLowercase ? ("vxdjump") : ("VxDJump"), 7);
                }
            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%lX"), data);
        }
}

//----------------------------------------------------------------------------
// Decode implicit constant 1 (used in shift commands). This operand is so
// insignificant that it is never shown in comment window.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeC1(void)
{
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    m_pDisasm->immconst = 1;
    if (m_nMode >= DISASM_FILE)
        {
            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("1") );
        }
}

//----------------------------------------------------------------------------
// Decode immediate absolute data address. This operand is used in 8080-
// compatible commands which allow to move data from memory to accumulator and
// back. Note that bytes ModRM and SIB never appear in commands with IA operand.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeIA(void)
{
    ulong addr;
    if (m_ulSize < 1 + m_ulAddrSize)
        {
            m_pDisasm->error = DAE_CROSS;
            return;
        }
    m_nDispSize = m_ulAddrSize;
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    if (m_ulDataSize == 1)
        {
            m_pDisasm->memtype = DEC_BYTE;
        }
    else if (m_ulDataSize == 2)
        {
            m_pDisasm->memtype = DEC_WORD;
        }
    else if (m_ulDataSize == 4)
        {
            m_pDisasm->memtype = DEC_DWORD;
        }
    if (m_ulAddrSize == 2)
        {
            addr = *(ushort *)(m_pCmd + 1);
        }
    else
        {
            addr = *(ulong *)(m_pCmd + 1);
            if (m_pPFixup == NULL)
                {
                    m_pPFixup = m_pCmd + 1;
                }
            m_pDisasm->fixupsize += 4;
        }
    m_pDisasm->adrconst = addr;
    if (addr == 0)
        {
            m_pDisasm->zeroconst = 1;
        }
    if (m_nMode >= DISASM_FILE)
        {
            Memadr(SEG_DS, (""), addr, m_ulDataSize);
        }
}

//----------------------------------------------------------------------------
// Decodes jump relative to nextip of size offsize.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeRJ(ulong offsize, ulong nextip)
{
    int i;
    ulong addr;
    char s[TEXTLEN];

    //
    if (m_ulSize < offsize + 1)
        {
            m_pDisasm->error = DAE_CROSS;
            return;
        }

    m_nDispSize = offsize;                  // Interpret offset as displacement
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    if (offsize == 1)
        {
            addr = (signed char)m_pCmd[1] + nextip;
        }
    else if (offsize == 2)
        {
            addr = *(signed short *)(m_pCmd + 1) + nextip;
        }
    else
        {
            addr = *(ulong *)(m_pCmd + 1) + nextip;
        }

    if (m_ulDataSize == 2)
        {
            addr &= 0xFFFF;
        }
    m_pDisasm->jmpconst = addr;
    if (addr == 0)
        {
            m_pDisasm->zeroconst = 1;
        }
    if (m_nMode >= DISASM_FILE)
        {
            if (offsize == 1)
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s "), (m_nLowercase == 0 ? ("SHORT") : ("short")));
                }
            if (m_nMode >= DISASM_CODE)
                {
                    i = Decodeaddress(addr, s, TEXTLEN, m_pDisasm->comment);
                }
            else
                {
                    i = 0;
                }

            if (m_nSymbolic == 0 || i == 0)
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%08lX"), addr);
                }
            else
                {
                    m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%.*s"), TEXTLEN - m_nResult - 25, s);
                }
            if (m_nSymbolic == 0 && i != 0 && m_pDisasm->comment[0] == '\0')
                {
                    strcpy(m_pDisasm->comment, s);
                }
        }
}

//----------------------------------------------------------------------------
// Decode immediate absolute far jump address. In flat model, such addresses
// are not used (mostly because selector is specified directly in the command),
// so I neither decode as symbol nor comment it. To allow search for selector
// by value, I interprete it as an immediate constant.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeJF(void)
{
    ulong addr, seg;

    //
    if (m_ulSize < 1 + m_ulAddrSize + 2)
        {
            m_pDisasm->error = DAE_CROSS;
            return;
        }
    m_nDispSize = m_ulAddrSize;
    m_nImmSize = 2;      // Non-trivial but allowed interpretation
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    if (m_ulAddrSize == 2)
        {
            addr = *(ushort *)(m_pCmd + 1);
            seg = *(ushort *)(m_pCmd + 3);
        }
    else
        {
            addr = *(ulong *)(m_pCmd + 1);
            seg = *(ushort *)(m_pCmd + 5);
        }

    m_pDisasm->jmpconst = addr;
    m_pDisasm->immconst = seg;
    if (addr == 0 || seg == 0)
        {
            m_pDisasm->zeroconst = 1;
        }
    if (m_nMode >= DISASM_FILE)
        {
            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s %04X:%08X"), (m_nLowercase == 0 ? ("FAR") : ("far")), seg, addr);
        }
}

//----------------------------------------------------------------------------
// Decode segment register. In flat model, operands of this type are seldom.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeSG(int index)
{
    int i;
    if (m_nMode < DISASM_DATA)
        {
            return;
        }
    index &= 0x07;
    if (index >= 6)
        {
            m_nSoftError = DAE_BADSEG;    // Undefined segment register
        }
    if (m_nMode >= DISASM_FILE)
        {
            i = sprintf(m_pDisasm->result + m_nResult, ("%s"), g_szSegName[index]);
            if (m_nLowercase)
                {
                    strlwr(m_pDisasm->result + m_nResult);
                }
            m_nResult += i;
        }
}


//----------------------------------------------------------------------------
// Decode control register addressed in R part of ModRM byte. Operands of
// this type are extremely rare. Contents of control registers are accessible
// only from privilege level 0, so I cannot dump them here.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeCR(int index)
{
    m_nHasRM = 1;
    if (m_nMode >= DISASM_FILE)
        {
            index = (index >> 3) & 0x07;
            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s"), g_szCRName[index]);
            if (m_nLowercase)
                {
                    strlwr(m_pDisasm->result + m_nResult);
                }
        }
}


//----------------------------------------------------------------------------
// Decode debug register addressed in R part of ModRM byte. Operands of
// this type are extremely rare. I can dump only those debug registers
// available in CONTEXT structure.
//----------------------------------------------------------------------------
void		CMgDisasmBase::DecodeDR(int index)
{
    int i;
    m_nHasRM = 1;
    if (m_nMode >= DISASM_FILE)
        {
            index = (index >> 3) & 0x07;
            i = sprintf(m_pDisasm->result + m_nResult, ("%s"), g_szDRName[index]);
            if (m_nLowercase)
                {
                    strlwr(m_pDisasm->result + m_nResult);
                }
            m_nResult += i;
        }
}

//----------------------------------------------------------------------------
// Service function, adds valid memory adress in MASM or Ideal format to
// disassembled string. Parameters: defseg - default segment for given
// register combination, descr - fully decoded register part of address,
// offset - constant part of address, dsize - data size in bytes. If global
// flag 'symbolic' is set, function also tries to decode offset as name of
// some label.
//----------------------------------------------------------------------------
void		CMgDisasmBase::Memadr(int defseg, const char *descr, long offset, int dsize)
{
    int i, n, seg;
    char *pr;
    char s[TEXTLEN];
    if( m_nMode < DISASM_FILE || descr == NULL)
        {
            return;    // No need or possibility to decode
        }

    //
    pr = m_pDisasm->result + m_nResult;
    n = 0;

    //
    if (m_nSegPrefix != SEG_UNDEF)
        {
            seg = m_nSegPrefix;
        }
    else
        {
            seg = defseg;
        }

    //
    if (m_nIDEAL != 0)
        {
            pr[n++] = '[';
        }


    // In some cases Disassembler may omit size of memory operand. Namely, flag
    // showmemsize must be 0, type bit C_EXPL must be 0 (this bit namely means
    // that explicit operand size is necessary) and type of command must not be
    // C_MMX or C_NOW (because bit C_EXPL has in these cases different meaning).
    // Otherwise, exact size must be supplied.
    if ( m_nShowMemSize != 0 || (m_pDisasm->cmdtype & C_TYPEMASK) == C_MMX ||
            (m_pDisasm->cmdtype & C_TYPEMASK) == C_NOW || (m_pDisasm->cmdtype & C_EXPL) != 0
       )
        {
            if (dsize < sizeof(g_szSizeName) / sizeof(g_szSizeName[0]))
                {
                    n += sprintf(pr + n, ("%s %s"), g_szSizeName[dsize], (m_nIDEAL == 0 ? ("PTR ") : ("")));
                }
            else
                {
                    n += sprintf(pr + n, ("(%i-BYTE) %s"), dsize, (m_nIDEAL == 0 ? ("PTR ") : ("")));
                }
        }

    //
    if ((m_nPutDefSeg != 0 || seg != defseg) && seg != SEG_UNDEF)
        {
            n += sprintf(pr + n, ("%s:"), g_szSegName[seg]);
        }

    //
    if (m_nIDEAL == 0)
        {
            pr[n++] = '[';
        }

    //
    n += sprintf(pr + n, ("%s"), descr);
    if (m_nLowercase)
        {
            strlwr(pr);
        }

    //
    if (offset == 0L)
        {
            if (*descr == '\0')
                {
                    pr[n++] = '0';
                }
        }
    else
        {
            if (m_nSymbolic && m_nMode >= DISASM_CODE)
                {
                    i = Decodeaddress(offset, s, TEXTLEN - n - 24, NULL);
                }
            else
                {
                    i = 0;
                }

            //
            if (i > 0)
                {
                    // Offset decoded in symbolic form
                    if (*descr != '\0')
                        {
                            pr[n++] = '+';
                        }
                    strcpy(pr + n, s);
                    n += i;
                }
            else if (offset < 0 && offset > -16384 && *descr != '\0')
                {
                    n += sprintf(pr + n, ("-%lX"), -offset);
                }
            else
                {
                    if (*descr != '\0') pr[n++] = '+';
                    n += sprintf(pr + n, ("%lX"), offset);
                }
        }
    pr[n++] = ']';
    pr[n] = '\0';
    m_nResult += n;
}


//----------------------------------------------------------------------------
// Decodes address into symb (nsymb bytes long, including the terminating zero
// character) and comments its possible meaning. Returns number of bytes in
// symb not including terminating zero.
//----------------------------------------------------------------------------
int			CMgDisasmBase::Decodeaddress(ulong addr, char *symb, int nsymb, char *comment)
{
    // Environment-specific routine! Do it yourself!

    return 0;
}


//----------------------------------------------------------------------------
// Skips 3DNow! operands and extracts command suffix. Returns suffix or -1 if
// suffix lies outside the memory block. This subroutine assumes that cmd still
// points to the beginning of 3DNow! command (i.e. to the sequence of two bytes
// 0F, 0F).
//----------------------------------------------------------------------------
int			CMgDisasmBase::Get3dnowsuffix(void)
{
    int c, sib;
    ulong offset = 3;
    if( m_ulSize < 3 )
        {
            return -1;    // Suffix outside the memory block
        }

    //
    c = m_pCmd[2] & 0xC7;                     // Leave only Mod and M fields

    // Register in ModM - general-purpose, MMX or 3DNow!
    if ( (c & 0xC0) == 0xC0 ) {}

    // 16-bit addressing mode, SIB byte is never used here.
    else if( m_ulAddrSize == 2 )
        {
            if ( c == 0x06 )                     // Special case of immediate address
                {
                    offset += 2;
                }
            else if ( (c & 0xC0) == 0x40 )       // 8-bit signed displacement
                {
                    offset ++;
                }
            else if ( (c & 0xC0) == 0x80 )       // 16-bit unsigned displacement
                {
                    offset += 2;
                }
        }

    // Immediate 32-bit address.
    else if( c == 0x05 )                  // Special case of immediate address
        {
            offset += 4 ;
        }

    // 32-bit address with SIB byte.
    else if( (c & 0x07) == 0x04)       // SIB addresation
        {
            if( m_ulSize < 4 )
                {
                    return -1;    // Suffix outside the memory block
                }

            //
            sib = m_pCmd[3];
            offset++;

            //
            if( c == 0x04 && (sib & 0x07) == 0x05 )
                {
                    offset += 4;    // Immediate address without base
                }
            else if( (c & 0xC0) == 0x40 )         // 8-bit displacement
                {
                    offset += 1;
                }
            else if( (c & 0xC0) == 0x80 )         // 32-bit dislacement
                {
                    offset += 4;
                }
        }

    // 32-bit address without SIB byte
    else if( (c & 0xC0) == 0x40 )
        {
            offset += 1;
        }
    else if( (c & 0xC0) == 0x80 )
        {
            offset += 4;
        }

    //
    if( offset >= m_ulSize )
        {
            return -1;    // Suffix outside the memory block
        }
    return m_pCmd[offset];
}


//--------------------------------------------------------------------------------
// Function attempts to calculate address of assembler instruction which is n
// lines back in the listing. Maximal stepback is limited to 127. In general,
// this is rather non-trivial task. Proposed solution may cause problems which
// however are not critical here.
//--------------------------------------------------------------------------------
ulong		CMgDisasmBase::Disassembleback(char *block, ulong base, ulong size, ulong ip, int n)
{
	int i;
	ulong abuf[131], addr, back, cmdsize;
	char *pdata;
	t_disasm da;

	//
	if (block == NULL)
	{
		return 0;    // Error, no code!
	}
	if (n < 0)
	{
		n = 0;    // Try to correct obvious errors
	}
	else if (n > 127)
	{
		n = 127;
	}
	if (ip > base + size)
	{
		ip = base + size;
	}
	if (n == 0)
	{
		return ip;    // Obvious answers
	}
	if (ip <= base + n)
	{
		return base;
	}

	back = MAXCMDSIZE * (n + 3);         // Command length limited to MAXCMDSIZE

	if (ip < base + back)
	{
		back = ip - base;
	}

	addr = ip - back;
	pdata = block + (addr - base);

	for (i = 0; addr < ip; i++)
	{
		abuf[i % 128] = addr;
		cmdsize = Disasm(pdata, back, addr, &da, DISASM_SIZE);
		pdata += cmdsize;
		addr += cmdsize;
		back -= cmdsize;
	}
	if (i < n)
	{
		return abuf[0];
	}
	else
	{
		return abuf[(i - n + 128) % 128];
	}
}

//--------------------------------------------------------------------------------
// Function attempts to calculate address of assembler instruction which is n
// lines forward in the listing.
//--------------------------------------------------------------------------------
ulong		CMgDisasmBase::Disassembleforward(char *block, ulong base, ulong size, ulong ip, int n)
{
	int i;
	ulong cmdsize;
	char *pdata;
	t_disasm da;
	if (block == NULL)
	{
		return 0;    // Error, no code!
	}
	if (ip < base)
	{
		ip = base;    // Try to correct obvious errors
	}
	if (ip > base + size)
	{
		ip = base + size;
	}
	if (n <= 0)
	{
		return ip;
	}

	//
	pdata = block + (ip - base);
	size -= (ip - base);

	//
	for (i = 0; i < n && size > 0; i++)
	{
		cmdsize = Disasm(pdata, size, ip, &da, DISASM_SIZE);
		pdata += cmdsize;
		ip += cmdsize;
		size -= cmdsize;
	}
	return ip;
}

//----------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------
ulong		CMgDisasmBase::Disasm( char *src, ulong srcsize, ulong srcip, t_disasm *disasm, int disasmmode )
{
    //
    int		nIsPrefix = 0;
    int		nRepeated = 0;
    int		nLockPrefix = 0;                      // Non-zero if lock prefix present
    int		nRepPrefix  = 0;                      // REPxxx prefix or 0

    //
    ulong	u = 0 , code = 0 ;
    int i = 0, j = 0 , is3dnow = 0 ;
    int cxsize = 0;
    int mnemosize = 0;
    int operand = 0;
    int arg = 0;

    //
    t_cmddata *pd, *pdan;

    //
    char name[TEXTLEN], *pname;


    // 32-bit code and data segments only!
    m_ulDataSize	= 4;
    m_ulAddrSize	= 4;

    // Prepare disassembler variables and initialize structure disasm.
    m_nSegPrefix	= SEG_UNDEF;
    m_nHasRM		= 0;
    m_nHasSIB		= 0;
    m_nDispSize		= 0;
    m_nImmSize		= 0;
    m_nDump			= 0;
    m_nResult		= 0;
    m_nSoftError	= 0;

    //
    m_pCmd	 = src;
    m_ulSize = srcsize;
    m_pPFixup = NULL;

    //结构初始化:
    m_pDisasm = disasm;
    m_pDisasm->ip			= srcip;
    m_pDisasm->comment[0]	= '\0';
    m_pDisasm->cmdtype		= C_BAD;
    m_pDisasm->nprefix		= 0;
    m_pDisasm->memtype		= DEC_UNKNOWN;
    m_pDisasm->indexed		= 0;
    m_pDisasm->jmpconst		= 0;
    m_pDisasm->jmptable		= 0;
    m_pDisasm->adrconst		= 0;
    m_pDisasm->immconst		= 0;
    m_pDisasm->zeroconst	= 0;
    m_pDisasm->fixupoffset	= 0;
    m_pDisasm->fixupsize	= 0;
    m_pDisasm->warnings		= 0;
    m_pDisasm->error		= DAE_NOERR;

    // No need to use register contents
    m_nMode = disasmmode;


    // Correct 80x86 command may theoretically contain up to 4 prefixes belonging
    // to different prefix groups. This limits maximal possible size of the
    // command to MAXCMDSIZE=16 bytes. In order to maintain this limit, if
    // Disasm() detects second prefix from the same group, it flushes first
    // prefix in the sequence as a pseudocommand.

    //
    while( m_ulSize > 0 )
        {
            nIsPrefix = 1;

            switch( *m_pCmd )
                {
                case 0x26:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_ES;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x2E:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_CS;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x36:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_SS;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x3E:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_DS;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x64:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_FS;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x65:
                {
                    if(m_nSegPrefix == SEG_UNDEF)
                        {
                            m_nSegPrefix = SEG_GS;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x66:
                {
                    if(m_ulDataSize == 4)
                        {
                            m_ulDataSize = 2;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0x67:
                {
                    if(m_ulAddrSize == 4)
                        {
                            m_ulAddrSize = 2;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0xF0:
                {
                    if (nLockPrefix == 0)
                        {
                            nLockPrefix = 0xF0;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0xF2:
                {
                    if (nRepPrefix == 0)
                        {
                            nRepPrefix = 0xF2;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                case 0xF3:
                {
                    if (nRepPrefix == 0)
                        {
                            nRepPrefix = 0xF3;
                        }
                    else
                        {
                            nRepeated = 1;
                        }
                    break;
                }
                default:
                {
                    nIsPrefix = 0;
                    break;
                }
                }

            //
            if( nRepPrefix == 0 || nRepeated != 0 )
                {
                    break;
                }

            //
            if( m_nMode >= DISASM_FILE )
                {
                    m_nDump += sprintf( m_pDisasm->dump + m_nDump , ("%02X:"), *m_pCmd );
                }

            //
            m_pDisasm->nprefix ++;
            m_pCmd ++;
            srcip ++;
            m_ulSize --;
            u ++;
        }

    //
    if( nRepeated ) //nRepeated == 1
        {
            //
            if( m_nMode >= DISASM_FILE )
                {
                    m_pDisasm->dump[3] = '\0'; //end 00 // Leave only first dumped prefix
                    m_pDisasm->nprefix = 1;

                    switch( m_pCmd[-(long)u])
                        {
                        case 0x26:
                        {
                            pname = (char *)(g_szSegName[SEG_ES]);
                            break;
                        }
                        case 0x2E:
                        {
                            pname = (char *)(g_szSegName[SEG_CS]);
                            break;
                        }
                        case 0x36:
                        {
                            pname = (char *)(g_szSegName[SEG_SS]);
                            break;
                        }
                        case 0x3E:
                        {
                            pname = (char *)(g_szSegName[SEG_DS]);
                            break;
                        }
                        case 0x64:
                        {
                            pname = (char *)(g_szSegName[SEG_FS]);
                            break;
                        }
                        case 0x65:
                        {
                            pname = (char *)(g_szSegName[SEG_GS]);
                            break;
                        }
                        case 0x66:
                        {
                            pname = ("DATASIZE");
                            break;
                        }
                        case 0x67:
                        {
                            pname = ("ADDRSIZE");
                            break;
                        }
                        case 0xF0:
                        {
                            pname = ("LOCK");
                            break;
                        }
                        case 0xF2:
                        {
                            pname = ("REPNE");
                            break;
                        }
                        case 0xF3:
                        {
                            pname = ("REPE");
                            break;
                        }
                        default:
                        {
                            pname = "?";
                            break;
                        }
                        }
                    m_nResult += sprintf( m_pDisasm->result + m_nResult, ("PREFIX %s:"), pname );

                    if( m_nLowercase )
                        {
                            strlwr( m_pDisasm->result );
                        }
                    if( m_nExtraPrefix == 0 ) strcpy( m_pDisasm->comment, ("Superfluous prefix") );
                }
            m_pDisasm->warnings |= DAW_PREFIX;
            if (nLockPrefix)
                {
                    m_pDisasm->warnings |= DAW_LOCK;
                }
            m_pDisasm->cmdtype = C_RARE;
            return 1;
        }


    // If lock prefix available, display it and forget, because it has no
    // influence on decoding of rest of the command.
    if (nLockPrefix != 0)
        {
            if(m_nMode >= DISASM_FILE)
                {
                    m_nResult += sprintf( m_pDisasm->result + m_nResult, ("LOCK ") );
                }
            m_pDisasm->warnings |= DAW_LOCK;
        }

    // Fetch (if available) first 3 bytes of the command, add repeat prefix and
    // find command in the command table.
    code = 0;
    if (m_ulSize > 0)
        {
            *( ((char *)&code) + 0 ) = m_pCmd[0];
        }
    if (m_ulSize > 1)
        {
            *( ((char *)&code) + 1 ) = m_pCmd[1];
        }
    if (m_ulSize > 2)
        {
            *( ((char *)&code) + 2 ) = m_pCmd[2];
        }

    if (nRepPrefix != 0)                  // RER/REPE/REPNE is considered to be
        {
            code = (code << 8) | nRepPrefix;        // part of command.
        }

    if (m_nDecodeVxd && (code & 0xFFFF) == 0x20CD)
        {
            pd = &g_VxdCmd;                        // Decode VxD call (Win95/98)
        }
    else
        {
            for( pd = g_CmdData; pd->mask != 0; pd++ )
                {
                    if (((code ^ pd->code) & pd->mask) != 0)
                        {
                            continue;
                        }
                    if ( m_nMode >= DISASM_FILE && m_nShortStringCmds &&
                            (pd->arg1 == MSO || pd->arg1 == MDE || pd->arg2 == MSO || pd->arg2 == MDE) )
                        {
                            continue;                      // Search short form of string command
                        }
                    break;
                }
        }

    //
    if ((pd->type & C_TYPEMASK) == C_NOW)
        {
            // 3DNow! commands require additional search.
            is3dnow = 1;
            j = Get3dnowsuffix();
            if (j < 0)
                {
                    m_pDisasm->error = DAE_CROSS;
                }
            else
                {
                    for ( ; pd->mask != 0; pd++ )
                        {
                            if (((code ^ pd->code) & pd->mask) != 0)
                                {
                                    continue;
                                }
                            if (((uchar *) & (pd->code))[2] == j)
                                {
                                    break;
                                }
                        }
                }
        }

    // Command not found
    if ( pd->mask == 0)
        {
            m_pDisasm->cmdtype = C_BAD;
            if( m_ulSize < 2 )
                {
                    m_pDisasm->error = DAE_CROSS;
                }
            else
                {
                    m_pDisasm->error = DAE_BADCMD;
                }

        }
    else
        {
            m_pDisasm->cmdtype = pd->type;
            cxsize = m_ulDataSize;

            //
            if( m_nSegPrefix == SEG_FS || m_nSegPrefix == SEG_GS || nLockPrefix != 0 )
                {
                    m_pDisasm->cmdtype |= C_RARE;             // These prefixes are rare
                }

            //
            if(pd->bits == PR)
                {
                    m_pDisasm->warnings |= DAW_PRIV;      // Privileged command (ring 0)
                }
            else if(pd->bits == WP)
                {
                    m_pDisasm->warnings |= DAW_IO;        // I/O command
                }

            // Win32 programs usually try to keep stack dword-aligned, so INC ESP
            // (44) and DEC ESP (4C) usually don't appear in real code. Also check for
            // ADD ESP,imm and SUB ESP,imm (81,C4,imm32; 83,C4,imm8; 81,EC,imm32;
            // 83,EC,imm8).

            if(m_pCmd[0] == 0x44 || m_pCmd[0] == 0x4C ||
                    (m_ulSize >= 3 && (m_pCmd[0] == 0x81 || m_pCmd[0] == 0x83) &&
                     (m_pCmd[1] == 0xC4 || m_pCmd[1] == 0xEC) && (m_pCmd[2] & 0x03) != 0)
              )
                {
                    m_pDisasm->warnings |= DAW_STACK;
                    m_pDisasm->cmdtype |= C_RARE;
                };

            // Warn also on MOV SEG,... (8E...). Win32 works in flat mode.
            if(m_pCmd[0] == 0x8E)
                {
                    m_pDisasm->warnings |= DAW_SEGMENT;
                }

            // If opcode is 2-byte, adjust command.
            if(pd->len == 2)
                {
                    if(m_ulSize == 0)
                        {
                            m_pDisasm->error = DAE_CROSS;
                        }
                    else
                        {
                            if(m_nMode >= DISASM_FILE)
                                {
                                    m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd);
                                }
                            m_pCmd[0]++;
                            srcip++;
                            m_ulSize--;
                        }
                }
            if(m_ulSize == 0)
                {
                    m_pDisasm->error = DAE_CROSS;
                }

            // Some commands either feature non-standard data size or have bit which
            // allowes to select data size.
            if ((pd->bits & WW) != 0 && (*m_pCmd & WW) == 0)
                {
                    m_ulDataSize = 1;            // Bit W in command set to 0
                }
            else if ((pd->bits & W3) != 0 && (*m_pCmd & W3) == 0)
                {
                    m_ulDataSize = 1;            // Another position of bit W
                }
            else if ((pd->bits & FF) != 0)
                {
                    m_ulDataSize = 2;
                }

            //
            // Some commands either have mnemonics which depend on data size (8/16 bits
            // or 32 bits, like CWD/CDQ), or have several different mnemonics (like
            // JNZ/JNE). First case is marked by either '&' (mnemonic depends on
            // operand size) or '$' (depends on address size). In the second case,
            // there is no special marker and disassembler selects main mnemonic.
            if (m_nMode >= DISASM_FILE)
                {
                    //
                    if (pd->name[0] == '&')
                        {
                            mnemosize = m_ulDataSize;
                        }
                    else if (pd->name[0] == '$')
                        {
                            mnemosize = m_ulDataSize;
                        }
                    else
                        {
                            mnemosize = 0;
                        }

                    //
                    if(mnemosize != 0)
                        {
                            for(i = 0, j = 1; pd->name[j] != '\0'; j++)
                                {
                                    if (pd->name[j] == ':')      // Separator between 16/32 mnemonics
                                        {
                                            if(mnemosize == 4)
                                                {
                                                    i = 0;
                                                }
                                            else
                                                {
                                                    break;
                                                }

                                        }
                                    else if (pd->name[j] == '*')  // Substitute by 'W', 'D' or none
                                        {
                                            if (mnemosize == 4 && m_nSizeSens != 2)
                                                {
                                                    name[i++] = 'D';
                                                }
                                            else if (mnemosize != 4 && m_nSizeSens != 0)
                                                {
                                                    name[i++] = 'W';
                                                }

                                        }
                                    else
                                        {
                                            name[i++] = pd->name[j];
                                        }
                                }
                            name[i] = '\0';
                        }
                    else
                        {
                            strcpy(name, pd->name);
                            for (i = 0; name[i] != '\0'; i++)
                                {
                                    // Use main mnemonic
                                    if (name[i] == ',')
                                        {
                                            name[i] = '\0';
                                            break;
                                        }
                                }
                        }

                    //
                    if( nRepPrefix != 0 && m_nTabArguments )
                        {
                            for (i = 0; name[i] != '\0' && name[i] != ' '; i++)
                                {
                                    m_pDisasm->result[m_nResult++] = name[i];
                                }

                            if (name[i] == ' ')
                                {
                                    m_pDisasm->result[m_nResult++] = ' ';
                                    i++;
                                }

                            while(m_nResult < 8)
                                {
                                    m_pDisasm->result[m_nResult++] = ' ';
                                }

                            for ( ; name[i] != '\0'; i++)
                                {
                                    m_pDisasm->result[m_nResult++] = name[i];
                                }

                        }
                    else
                        {
                            m_nResult += sprintf(m_pDisasm->result + m_nResult, ("%s"), name);
                        }

                    if(m_nLowercase)
                        {
                            strlwr(m_pDisasm->result);
                        }
                }

            //
            // Decode operands (explicit - encoded in command, implicit - present in
            // mmemonic or assumed - used or modified by command). Assumed operands
            // must stay after all explicit and implicit operands. Up to 3 operands
            // are allowed.
            for(operand = 0; operand < 3; operand++)
                {
                    if (m_pDisasm->error)
                        {
                            break;    // Error - no sense to continue
                        }

                    // If command contains both source and destination, one usually must not
                    // decode destination to comment because it will be overwritten on the
                    // next step. Global addcomment takes care of this. Decoding routines,
                    // however, may ignore this flag.
                    if (operand == 0 && pd->arg2 != NNN && pd->arg2 < PSEUDOOP)
                        {
                            m_nAddComment = 0;
                        }
                    else
                        {
                            m_nAddComment = 1;
                        }

                    // Get type of next argument.
                    if (operand == 0)
                        {
                            arg = pd->arg1;
                        }
                    else if(operand == 1)
                        {
                            arg = pd->arg2;
                        }
                    else
                        {
                            arg = pd->arg3;
                        }

                    //
                    if (arg == NNN)
                        {
                            break;    // No more operands
                        }

                    // Arguments with arg>=PSEUDOOP are assumed operands and are not
                    // displayed in disassembled result, so they require no delimiter.
                    if ((m_nMode >= DISASM_FILE) && arg < PSEUDOOP)
                        {
                            if(operand == 0)
                                {
                                    m_pDisasm->result[m_nResult++] = ' ';
                                    if (m_nTabArguments)
                                        {
                                            while(m_nResult < 8)
                                                {
                                                    m_pDisasm->result[m_nResult++] = ' ';
                                                }
                                        }
                                }
                            else
                                {
                                    m_pDisasm->result[m_nResult++] = ',';
                                    if (m_nExtraSpace)
                                        {
                                            m_pDisasm->result[m_nResult++] = ' ';
                                        }
                                }
                        }

                    //
                    // Decode, analyse and comment next operand of the command.
                    switch(arg)
                        {
                        case REG:                      // Integer register in Reg field
                        {
                            if( m_ulSize < 2 )
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    DecodeRG(m_pCmd[1] >> 3, m_ulDataSize, REG);
                                }
                            m_nHasRM = 1;
                            break;
                        }
                        case RCM:                      // Integer register in command byte
                        {
                            DecodeRG(m_pCmd[0], m_ulDataSize, RCM);
                            break;
                        }
                        case RG4:                      // Integer 4-byte register in Reg field
                        {
                            if(m_ulSize < 2)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    DecodeRG(m_pCmd[1] >> 3, 4, RG4);
                                }
                            m_nHasRM = 1;
                            break;
                        }
                        case RAC:                      // Accumulator (AL/AX/EAX, implicit)
                        {
                            DecodeRG(REG_EAX, m_ulDataSize, RAC);
                            break;
                        }
                        case RAX:                      // AX (2-byte, implicit)
                        {
                            DecodeRG(REG_EAX, 2, RAX);
                            break;
                        }
                        case RDX:                      // DX (16-bit implicit port address)
                        {
                            DecodeRG(REG_EDX, 2, RDX);
                            break;
                        }
                        case RCL:                      // Implicit CL register (for shifts)
                        {
                            DecodeRG(REG_ECX, 1, RCL);
                            break;
                        }
                        case RS0:                      // Top of FPU stack (ST(0))
                        {
                            DecodeST(0, 0);
                            break;
                        }
                        case RST:                      // FPU register (ST(i)) in command byte
                        {
                            DecodeST(m_pCmd[0], 0);
                            break;
                        }
                        case RMX:                      // MMX register MMx
                        {
                            if (m_ulSize < 2)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    DecodeMX(m_pCmd[1] >> 3);
                                }
                            m_nHasRM = 1;
                            break;
                        }
                        case R3D:                      // 3DNow! register MMx
                        {
                            if(m_ulSize < 2)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            else
                                {
                                    DecodeNR(m_pCmd[1] >> 3);
                                }
                            m_nHasRM = 1;
                            break;
                        }
                        case MRG:                      // Memory/register in ModRM byte
                        case MRJ:                      // Memory/reg in ModRM as JUMP target
                        case MR1:                      // 1-byte memory/register in ModRM byte
                        case MR2:                      // 2-byte memory/register in ModRM byte
                        case MR4:                      // 4-byte memory/register in ModRM byte
                        case MR8:                      // 8-byte memory/MMX register in ModRM
                        case MRD:                      // 8-byte memory/3DNow! register in ModRM
                        case MMA:                      // Memory address in ModRM byte for LEA
                        case MML:                      // Memory in ModRM byte (for LES)
                        case MM6:                      // Memory in ModRm (6-byte descriptor)
                        case MMB:                      // Two adjacent memory locations (BOUND)
                        case MD2:                      // Memory in ModRM byte (16-bit integer)
                        case MB2:                      // Memory in ModRM byte (16-bit binary)
                        case MD4:                      // Memory in ModRM byte (32-bit integer)
                        case MD8:                      // Memory in ModRM byte (64-bit integer)
                        case MDA:                      // Memory in ModRM byte (80-bit BCD)
                        case MF4:                      // Memory in ModRM byte (32-bit float)
                        case MF8:                      // Memory in ModRM byte (64-bit float)
                        case MFA:                      // Memory in ModRM byte (80-bit float)
                        case MFE:                      // Memory in ModRM byte (FPU environment)
                        case MFS:                      // Memory in ModRM byte (FPU state)
                        case MFX:                      // Memory in ModRM byte (ext. FPU state)
                        {
                            DecodeMR(arg);
                            break;
                        }
                        case MMS:                      // Memory in ModRM byte (as SEG:OFFS)
                        {
                            DecodeMR(arg);
                            m_pDisasm->warnings |= DAW_FARADDR;
                            break;
                        }
                        case RR4:                      // 4-byte memory/register (register only)
                        case RR8:                      // 8-byte MMX register only in ModRM
                        case RRD:                      // 8-byte memory/3DNow! (register only)
                        {
                            if ((m_pCmd[1] & 0xC0) != 0xC0)
                                {
                                    m_nSoftError = DAE_REGISTER;
                                }
                            DecodeMR(arg);
                            break;
                        }
                        case MSO:                      // Source in string op's ([ESI])
                        {
                            DecodeSO();
                            break;
                        }
                        case MDE:                      // Destination in string op's ([EDI])
                        {
                            DecodeDE();
                            break;
                        }
                        case MXL:                      // XLAT operand ([EBX+AL])
                        {
                            DecodeXL();
                            break;
                        }
                        case IMM:                      // Immediate data (8 or 16/32)
                        case IMU:                      // Immediate unsigned data (8 or 16/32)
                        {
                            if ((pd->bits & SS) != 0 && (*m_pCmd & 0x02) != 0)
                                {
                                    DecodeIM(1, m_ulDataSize, arg);
                                }
                            else
                                {
                                    DecodeIM(m_ulDataSize, 0, arg);
                                }
                            break;
                        }
                        case VXD:                      // VxD service (32-bit only)
                        {
                            DecodeVX();
                            break;
                        }
                        case IMX:                      // Immediate sign-extendable byte
                        {
                            DecodeIM(1, m_ulDataSize, arg);
                            break;
                        }
                        case C01:                      // Implicit constant 1 (for shifts)
                        {
                            DecodeC1();
                            break;
                        }
                        case IMS:                      // Immediate byte (for shifts)
                        case IM1:                      // Immediate byte
                        {
                            DecodeIM(1, 0, arg);
                            break;
                        }
                        case IM2:                      // Immediate word (ENTER/RET)
                        {
                            DecodeIM(2, 0, arg);
                            if ((m_pDisasm->immconst & 0x03) != 0)
                                {
                                    m_pDisasm->warnings |= DAW_STACK;
                                }
                            break;
                        }
                        case IMA:                      // Immediate absolute near data address
                        {
                            DecodeIA();
                            break;
                        }
                        case JOB:                      // Immediate byte offset (for jumps)
                        {
                            DecodeRJ(1, srcip + 2);
                            break;
                        }
                        case JOW:                      // Immediate full offset (for jumps)
                        {
                            DecodeRJ(m_ulDataSize, srcip + m_ulDataSize + 1);
                            break;
                        }
                        case JMF:                      // Immediate absolute far jump/call addr
                        {
                            DecodeJF();
                            m_pDisasm->warnings |= DAW_FARADDR;
                            break;
                        }
                        case SGM:                      // Segment register in ModRM byte
                        {
                            if(m_ulSize < 2)
                                {
                                    m_pDisasm->error = DAE_CROSS;
                                }
                            DecodeSG(m_pCmd[1] >> 3);
                            m_nHasRM = 1;
                            break;
                        }
                        case SCM:                      // Segment register in command byte
                        {
                            DecodeSG(m_pCmd[0] >> 3);
                            if((m_pDisasm->cmdtype & C_TYPEMASK) == C_POP)
                                {
                                    m_pDisasm->warnings |= DAW_SEGMENT;
                                }
                            break;
                        }
                        case CRX:                      // Control register CRx
                        {
                            if ((m_pCmd[1] & 0xC0) != 0xC0)
                                {
                                    m_pDisasm->error = DAE_REGISTER;
                                }
                            DecodeCR(m_pCmd[1]);
                            break;
                        }
                        case DRX:                      // Debug register DRx
                        {
                            if ((m_pCmd[1] & 0xC0) != 0xC0)
                                {
                                    m_pDisasm->error = DAE_REGISTER;
                                }
                            DecodeDR(m_pCmd[1]);
                            break;
                        }
                        case PRN:                      // Near return address (pseudooperand)
                        {
                            break;
                        }
                        case PRF:                      // Far return address (pseudooperand)
                        {
                            m_pDisasm->warnings |= DAW_FARADDR;
                            break;
                        }
                        case PAC:                      // Accumulator (AL/AX/EAX, pseudooperand)
                        {
                            DecodeRG(REG_EAX, m_ulDataSize, PAC);
                            break;
                        }
                        case PAH:                      // AH (in LAHF/SAHF, pseudooperand)
                        case PFL:                      // Lower byte of flags (pseudooperand)
                        {
                            break;
                        }
                        case PS0:                      // Top of FPU stack (pseudooperand)
                        {
                            DecodeST(0, 1);
                            break;
                        }
                        case PS1:                      // ST(1) (pseudooperand)
                        {
                            DecodeST(1, 1);
                            break;
                        }
                        case PCX:                      // CX/ECX (pseudooperand)
                        {
                            DecodeRG(REG_ECX, cxsize, PCX);
                            break;
                        }
                        case PDI:                      // EDI (pseudooperand in MMX extentions)
                        {
                            DecodeRG(REG_EDI, 4, PDI);
                            break;
                        }
                        default:
                        {
                            m_pDisasm->error = DAE_INTERN;      // Unknown argument type
                            break;
                        }
                        }
                }

            // Check whether command may possibly contain fixups.
            if( m_pPFixup != NULL && m_pDisasm->fixupsize > 0)
                {
                    m_pDisasm->fixupoffset = m_pPFixup - src;
                }

            // Segment prefix and address size prefix are superfluous for command which
            // does not access memory. If this the case, mark command as rare to help
            // in analysis.
            if ( m_pDisasm->memtype == DEC_UNKNOWN && ( m_nSegPrefix != SEG_UNDEF || (m_ulAddrSize != 4 && pd->name[0] != '$') ) )
                {
                    m_pDisasm->warnings |= DAW_PREFIX;
                    m_pDisasm->cmdtype |= C_RARE;
                }

            // 16-bit addressing is rare in 32-bit programs. If this is the case,
            // mark command as rare to help in analysis.
            if(m_ulAddrSize != 4)
                {
                    m_pDisasm->cmdtype |= C_RARE;
                }
        }

    //
    // Suffix of 3DNow! command is accounted best by assuming it immediate byte
    // constant.
    if (is3dnow)
        {
            if (m_nImmSize != 0)
                {
                    m_pDisasm->error = DAE_BADCMD;
                }
            else m_nImmSize = 1;
        }

    // Right or wrong, command decoded. Now dump it.
    if (m_pDisasm->error != 0)
        {
            // Hard error in command detected
            if (m_nMode >= DISASM_FILE)
                {
                    m_nResult = sprintf(m_pDisasm->result, ("???"));
                }
            if (m_pDisasm->error == DAE_BADCMD && (*m_pCmd == 0x0F || *m_pCmd == 0xFF) && m_ulSize > 0)
                {
                    if (m_nMode >= DISASM_FILE)
                        {
                            m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd);
                        }
                    m_pCmd++;
                    m_ulSize--;
                }
            if (m_ulSize > 0)
                {
                    if (m_nMode >= DISASM_FILE)
                        {
                            m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd);
                        }
                    m_pCmd++;
                    m_ulSize--;
                }
        }
    else                                  // No hard error, dump command
        {
            if (m_nMode >= DISASM_FILE)
                {
                    m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd++);
                    if (m_nHasRM)
                        {
                            m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd++);
                        }
                    if (m_nHasSIB)
                        {
                            m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd++);
                        }
                    if (m_nDispSize != 0)
                        {
                            m_pDisasm->dump[m_nDump++] = ' ';
                            for (i = 0; i < m_nDispSize; i++)
                                {
                                    m_nDump += sprintf(m_pDisasm->dump + m_nDump, ("%02X"), *m_pCmd++);
                                }
                        }
                    if (m_nImmSize != 0)
                        {
                            m_pDisasm->dump[m_nDump++] = ' ';
                            for (i = 0; i < m_nImmSize; i++)
                                {
                                    m_nDump += sprintf(m_pDisasm->dump + m_nDump, "%02X", *m_pCmd++);
                                }
                        }
                }
            else
                {
                    m_pCmd += 1 + m_nHasRM + m_nHasSIB + m_nDispSize + m_nImmSize;
                }

            //
            m_ulSize -= 1 + m_nHasRM + m_nHasSIB + m_nDispSize + m_nImmSize;
        }

    // Check that command is not a dangerous one.
    if (m_nMode >= DISASM_DATA)
        {
            for(pdan = g_Dangerous; pdan->mask != 0; pdan++)
                {
                    if (((code ^ pdan->code) & pdan->mask) != 0)
                        {
                            continue;
                        }
                    if (pdan->type == C_DANGERLOCK && nLockPrefix == 0)
                        {
                            break;    // Command harmless without LOCK prefix
                        }
                    if ( m_nIsWindowsNT && pdan->type == C_DANGER95)
                        {
                            break;    // Command harmless under Windows NT
                        }

                    // Dangerous command!
                    if (pdan->type == C_DANGER95)
                        {
                            m_pDisasm->warnings |= DAW_DANGER95;
                        }
                    else
                        {
                            m_pDisasm->warnings |= DAW_DANGEROUS;
                        }
                    break;
                }
        }
    if (m_pDisasm->error == 0 && m_nSoftError != 0)
        {
            m_pDisasm->error = m_nSoftError;    // Error, but still display command
        }

    //
    if (m_nMode >= DISASM_FILE)
        {
            if (m_pDisasm->error != DAE_NOERR)
                {
                    switch (m_pDisasm->error)
                        {
                        case DAE_CROSS:
                        {
                            strcpy(m_pDisasm->comment, ("Command crosses end of memory block"));
                            break;
                        }
                        case DAE_BADCMD:
                        {
                            strcpy(m_pDisasm->comment, ("Unknown command"));
                            break;
                        }
                        case DAE_BADSEG:
                        {
                            strcpy(m_pDisasm->comment, ("Undefined segment register"));
                            break;
                        }
                        case DAE_MEMORY:
                        {
                            strcpy(m_pDisasm->comment, ("Illegal use of register"));
                            break;
                        }
                        case DAE_REGISTER:
                        {
                            strcpy(m_pDisasm->comment, ("Memory address not allowed"));
                            break;
                        }
                        case DAE_INTERN:
                        {
                            strcpy(m_pDisasm->comment, ("Internal OLLYDBG error"));
                            break;
                        }
                        default:
                        {
                            strcpy(m_pDisasm->comment, ("Unknown error"));
                            break;
                        }
                        }
                }
            else if ((m_pDisasm->warnings & DAW_PRIV) != 0 && m_nPrivileged == 0)
                {
                    strcpy(m_pDisasm->comment, ("Privileged command"));
                }
            else if ((m_pDisasm->warnings & DAW_IO) != 0 && m_nIOCommand == 0)
                {
                    strcpy(m_pDisasm->comment, ("I/O command"));
                }
            else if ((m_pDisasm->warnings & DAW_FARADDR) != 0 && m_nFarCalls == 0)
                {
                    if ((m_pDisasm->cmdtype & C_TYPEMASK) == C_JMP)
                        {
                            strcpy(m_pDisasm->comment, ("Far jump"));
                        }
                    else if ((m_pDisasm->cmdtype & C_TYPEMASK) == C_CAL)
                        {
                            strcpy(m_pDisasm->comment, ("Far call"));
                        }
                    else if ((m_pDisasm->cmdtype & C_TYPEMASK) == C_RET)
                        {
                            strcpy(m_pDisasm->comment, ("Far return"));
                        }
                }
            else if ((m_pDisasm->warnings & DAW_SEGMENT) != 0 && m_nFarCalls == 0)
                {
                    strcpy(m_pDisasm->comment, ("Modification of segment register"));
                }
            else if ((m_pDisasm->warnings & DAW_SHIFT) != 0 && m_nBadShift == 0)
                {
                    strcpy(m_pDisasm->comment, ("Shift constant out of range 1..31"));
                }
            else if ((m_pDisasm->warnings & DAW_PREFIX) != 0 && m_nExtraPrefix == 0)
                {
                    strcpy(m_pDisasm->comment, ("Superfluous prefix"));
                }
            else if ((m_pDisasm->warnings & DAW_LOCK) != 0 && m_nLockedbus == 0)
                {
                    strcpy(m_pDisasm->comment, ("LOCK prefix"));
                }
            else if ((m_pDisasm->warnings & DAW_STACK) != 0 && m_nStackAlign == 0)
                {
                    strcpy(m_pDisasm->comment, ("Unaligned stack operation"));
                }
        }
    return (srcsize - m_ulSize);             // Returns number of recognized bytes
}

