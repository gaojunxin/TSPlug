// MgDisasmBase.h: interface for the CMgDisasmBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGDISASMBASE_H__6DCEBE26_A3B4_476C_ACA1_1B56BA3F0401__INCLUDED_)
#define AFX_MGDISASMBASE_H__6DCEBE26_A3B4_476C_ACA1_1B56BA3F0401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<math.h>

//-------------------------------------------------------------------------------------------------------------------------
//专有类型声明:
typedef unsigned char  uchar;          // Unsigned character (byte)
typedef unsigned short ushort;         // Unsigned short
typedef unsigned int   uint;           // Unsigned integer
typedef unsigned long  ulong;          // Unsigned long


//-------------------------------------------------------------------------------------------------------------------------
//常量声明:

//
#define NEGLIMIT       (-16384)        // Limit to display constans as signed
#define PSEUDOOP       128             // Base for pseudooperands
#define TEXTLEN        256             // Maximal length of text string

#define DISASM_SIZE    0               // Determine command size only
#define DISASM_DATA    1               // Determine size and analysis data
#define DISASM_FILE    3               // Disassembly, no symbols
#define DISASM_CODE    4               // Full disassembly

//------------------------------------------------------------------------------
//------------------ ASSEMBLER, DISASSEMBLER AND EXPRESSIONS -------------------
//------------------汇编，反汇编与表达-------------------

#define MAXCMDSIZE     16              // Maximal length of 80x86 command	80x86的命令的最大长度
#define MAXCALSIZE     8               // Max length of CALL without prefixes	不带前缀的最大长度CALL
#define NMODELS        8               // Number of assembler search models		搜索模式


//-------------------------------------------------------------------------------------------------------------------------
//结构声明
//
typedef		struct		_tagMgAsmCom_CommandData_ 
{
	ulong          mask;                 // Mask for first 4 bytes of the command
	ulong          code;                 // Compare masked bytes with this
	uchar          len;                  // Length of the main command code
	uchar          bits;                 // Special bits within the command
	uchar          arg1,arg2,arg3;       // Types of possible arguments
	uchar          type;                 // C_xxx + additional information
	char           *name;                // Symbolic name for this command
} _MGASM_CMDDATA_,t_cmddata;

//
typedef		struct		_tagMgAsmCom_AddrDec_ 
{
	int            defseg;
	char           *descr;
} _MGASM_ADDRDEC_,t_addrdec;


//-------------------------------------------------------------------------------------------------------------------------
//汇编基础类声明:
class		CMgAsmBase
{
public:
	CMgAsmBase();
	virtual ~CMgAsmBase();

protected:
private:

public:	//-------------------------------------- 结构部分 ----------------------------------

	// Model to search for assembler command
	typedef struct _tagAsmModel 
	{            
unsigned	char           code[MAXCMDSIZE];     // Binary code
unsigned	char           mask[MAXCMDSIZE];     // Mask for binary code (0: bit ignored)
			int            length;               // Length of code, bytes (0: empty)
			int            jmpsize;              // Offset size if relative jump
			int            jmpoffset;            // Offset relative to IP
			int            jmppos;               // Position of jump offset in command
	} _ASMMODEL_,t_asmmodel;

	//
	typedef struct _tagAsmOperand {
		int            type;                 // Operand type, see beginning of file
		int            size;                 // Operand size or 0 if yet unknown
		int            index;                // Index or other register
		int            scale;                // Scale
		int            base;                 // Base register if present
		long           offset;               // Immediate value or offset
		int            anyoffset;            // Offset is present but undefined
		int            segment;              // Segment in address if present
		int            jmpmode;              // Specified jump size
	} _ASMOPERAND_,t_asmoperand;

	/*
	//
	typedef		struct		_tagAddrDec_ 
	{
		int            defseg;
		char           *descr;
	} _ADDRDEC_,t_addrdec;
	*/

	//
	/*
	typedef		struct		_tagCommandData_ 
	{
		ulong          mask;                 // Mask for first 4 bytes of the command
		ulong          code;                 // Compare masked bytes with this
		uchar          len;                  // Length of the main command code
		uchar          bits;                 // Special bits within the command
		uchar          arg1,arg2,arg3;       // Types of possible arguments
		uchar          type;                 // C_xxx + additional information
		char           *name;                // Symbolic name for this command
	} _CMDDATA_,t_cmddata;
	*/

public:		//-------------------------------------- 变量部分 ----------------------------------
	char	*	m_pAsmCmd;              // Pointer to 0-terminated source line
	int			m_nScan;                // Type of last scanned element
	int			m_nPrio;                // Priority of operation (0: highest)
	char		m_sdata[TEXTLEN];     // Last scanned name (depends on type)
	long		m_idata;               // Last scanned value
	long double m_fdata;                // Floating-point number
	char	*	m_pAsmError;            // Explanation of last error, or NULL

public:
	int			m_nIDEAL;                // Force IDEAL decoding mode
	int			m_nSizeSens;             // How to decode size-sensitive mnemonics

protected:  //-------------------------------------- 函数部分 ----------------------------------
	void		Scanasm(int mode);
	void		Parseasmoperand(t_asmoperand *op);
	
public:
	int			Assemble(char *cmd,ulong ip,t_asmmodel *model,int attempt,int constsize,char *errtext);

};

//-------------------------------------------------------------------------------------------------------------------------
//反汇编基础类声明
class CMgDisasmBase  
{
public:
	CMgDisasmBase();
	virtual ~CMgDisasmBase();

public:	//-------------------------------------- 结构部分 ----------------------------------
	
	// Results of disassembling
	typedef		struct		_tagDisasm_ 
	{             
			ulong          ip;                   // Instrucion pointer
			char           dump[TEXTLEN];        // Hexadecimal dump of the command
			char           result[TEXTLEN];      // Disassembled command
			char           comment[TEXTLEN];     // Brief comment
			int            cmdtype;              // One of C_xxx
			int            memtype;              // Type of addressed variable in memory
			int            nprefix;              // Number of prefixes
			int            indexed;              // Address contains register(s)
			ulong          jmpconst;             // Constant jump address
			ulong          jmptable;             // Possible address of switch table
			ulong          adrconst;             // Constant part of address
			ulong          immconst;             // Immediate constant
			int            zeroconst;            // Whether contains zero constant
			int            fixupoffset;          // Possible offset of 32-bit fixups
			int            fixupsize;            // Possible total size of fixups or 0
			int            error;                // Error while disassembling command
			int            warnings;             // Combination of DAW_xxx
	} _DISASM_,t_disasm;

	/*
	//
	typedef		struct		_tagAddrDec_ 
	{
		int            defseg;
		char           *descr;
	} _ADDRDEC_,t_addrdec;
	*/

	//
	/*
	typedef		struct		_tagCommandData_ 
	{
		ulong          mask;                 // Mask for first 4 bytes of the command
		ulong          code;                 // Compare masked bytes with this
		uchar          len;                  // Length of the main command code
		uchar          bits;                 // Special bits within the command
		uchar          arg1,arg2,arg3;       // Types of possible arguments
		uchar          type;                 // C_xxx + additional information
		char           *name;                // Symbolic name for this command
	} _CMDDATA_,t_cmddata;
	*/

public:
	//
	t_disasm	*	m_pDisasm;

	//
	char		*	m_pCmd;                 // Pointer to binary data
	char		*	m_pPFixup;              // Pointer to possible fixups or NULL
	ulong			m_ulSize;                 // Remaining size of the command buffer
	int				m_nMode;                 // Disassembly mode (DISASM_xxx)


public:		//-------------------------------------- 变量部分 ----------------------------------
	
	//
	ulong		m_ulDataSize;			// Size of data (1,2,4 bytes)
	ulong		m_ulAddrSize;			// Size of address (2 or 4 bytes)

	//
	int			m_nSegPrefix;			// Segment override prefix or SEG_UNDEF          
	int			m_nHasRM;				// Command has ModR/M byte
	int			m_nHasSIB;				// Command has SIB byte
	int			m_nDispSize;			// Size of displacement (if any)
	int			m_nImmSize;				// Size of immediate data (if any)
	int			m_nSoftError;			// Noncritical disassembler error
	int			m_nDump;                // Current length of command dump
	int			m_nResult;              // Current length of disassembly
	int			m_nAddComment;			// Comment value of operand

public:
	int			m_nIDEAL;                // Force IDEAL decoding mode
	int			m_nLowercase;            // Force lowercase display
	int			m_nTabArguments;         // Tab between mnemonic and arguments
	int			m_nExtraSpace;           // Extra space between arguments
	int			m_nPutDefSeg;            // Display default segments in listing
	int			m_nShowMemSize;          // Always show memory size
	int			m_nShowNEAR;             // Show NEAR modifiers
	int			m_nShortStringCmds;      // Use short form of string commands
	int			m_nSizeSens;             // How to decode size-sensitive mnemonics
	int			m_nSymbolic;             // Show symbolic addresses in disasm
	int			m_nFarCalls;             // Accept far calls, returns & addresses
	int			m_nDecodeVxd;            // Decode VxD calls (Win95/98)
	int			m_nPrivileged;           // Accept privileged commands
	int			m_nIOCommand;            // Accept I/O commands
	int			m_nBadShift;             // Accept shift out of range 1..31
	int			m_nExtraPrefix;          // Accept superfluous prefixes
	int			m_nLockedbus;            // Accept LOCK prefixes
	int			m_nStackAlign;           // Accept unaligned stack operations
	int			m_nIsWindowsNT;          // When checking for dangers, assume NT

protected:  //-------------------------------------- 函数部分 ----------------------------------
	//
	void		DecodeRG(int index,int datasize,int type);
	void		DecodeST(int index,int pseudoop);
	void		DecodeMX(int index);
	void		DecodeNR(int index);
	void		DecodeMR(int type);
	void		DecodeSO(void);
	void		DecodeDE(void);
	void		DecodeXL(void);
	void		DecodeIM(int constsize,int sxt,int type);
	void		DecodeVX(void);
	void		DecodeC1(void);
	void		DecodeIA(void);
	void		DecodeRJ(ulong offsize,ulong nextip);
	void		DecodeJF(void);
	void		DecodeSG(int index);
	void		DecodeCR(int index);
	void		DecodeDR(int index);

public:
	int			Get3dnowsuffix(void);
	void		Memadr(int defseg,const char *descr,long offset,int dsize);
	int			Decodeaddress(ulong addr,char *symb,int nsymb,char *comment);

public:
	ulong		Disasm( char *src, ulong srcsize, ulong srcip, t_disasm *disasm, int disasmmode );
	ulong		Disassembleback(char *block,ulong base,ulong size,ulong ip,int n);
	ulong		Disassembleforward(char *block,ulong base,ulong size,ulong ip,int n);


};

//-------------------------------------------------------------------------------------------------------------------------
//全局变量声明:
extern	char	*	g_szSegName[8];
extern	char	*	g_szRegName[3][9];
extern	char	*	g_szCRName[9];
extern	char	*	g_szDRName[9];
extern	char	*	g_szFPUName[9];
extern	char	*	g_szMMXName[9];

//
extern	char	*	g_szSizeName[11];

//
extern	t_cmddata	g_CmdData[];
extern	t_cmddata	g_VxdCmd;		// Decoding of VxD calls (Win95/98)

//
extern	t_addrdec	g_addr16[8];
extern	t_addrdec	g_addr32[8];

//-------------------------------------------------------------------------------------------------------------------------
//全局函数声明:


//--------------------------------------------------------------------------------
// Function checks whether 80x86 flags meet condition set in the command.
// Returns 1 if condition is met, 0 if not and -1 in case of error (which is
// not possible).
//--------------------------------------------------------------------------------
int			Checkcondition(int code,ulong flags);

//--------------------------------------------------------------------------------
// Service function, checks whether command at offset addr in data is a valid
// filling command (usually some kind of NOP) used to align code to a specified
// (align=power of 2, 0 means no alignment) border. Returns length of filling
// command in bytes or 0 if command is not a recognized filling.
//--------------------------------------------------------------------------------
int			Isfilling(ulong addr,char *data,ulong size,ulong align);

//--------------------------------------------------------------------------------
// Decodes and prints 64-bit 3DNow! element f into string s (which must be at
// least 30 bytes long). Returns resulting length of the string.
//--------------------------------------------------------------------------------
int			Print3dnow(char *s,char *f);



//--------------------------------------------------------------------------------
// Decodes and prints 80-bit long double ext into string s (at least 32 bytes
// long). Procedure correctly displays all, even invalid, numbers without
// arithmetical exceptions. Returns resulting length of the string.
//--------------------------------------------------------------------------------
int			Printfloat10(char *s,long double ext);


//--------------------------------------------------------------------------------
// Decodes and prints 64-bit double d into string s (at least 25 bytes long).
// Returns resulting length of the string.
//--------------------------------------------------------------------------------
int			Printfloat8(char *s,double d);


//--------------------------------------------------------------------------------
// Decodes and prints 32-bit float f into string s (which must be at least 16
// bytes long). Returns resulting length of the string.
//--------------------------------------------------------------------------------
int			Printfloat4(char *s,float f);

//-------------------------------------------------------------------------------------------------------------------------
//
void message(int aa);
#endif // !defined(AFX_MGDISASMBASE_H__6DCEBE26_A3B4_476C_ACA1_1B56BA3F0401__INCLUDED_)

