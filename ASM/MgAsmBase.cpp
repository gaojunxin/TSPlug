// MgAsmBase.cpp: implementation of the CMgAsmBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MgAsmCom.h"
#include "MgAsmComDef.h"


//-------------------------------------------------------------------------------------------------------------------------
//全局变量声明:


//-------------------------------------------------------------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMgAsmBase::CMgAsmBase()
{
    //
    m_pAsmCmd		= NULL ;          // Pointer to 0-terminated source line
    m_nScan			= 0 ;             // Type of last scanned element
    m_nPrio			= 0 ;             // Priority of operation (0: highest)

    //
    memset(m_sdata , 0 , TEXTLEN );            // Last scanned name (depends on type)

    //
    m_idata			= 0 ;             // Last scanned value
    m_fdata			= 0 ;             // Floating-point number
    m_pAsmError		= NULL ;		  // Explanation of last error, or NULL

    //
    m_nIDEAL		= 0;             // Force IDEAL decoding mode
    m_nSizeSens		= 0;             // How to decode size-sensitive mnemonics
}

CMgAsmBase::~CMgAsmBase()
{

}


//--------------------------------------------------------------------------------
// Simple and slightly recursive scanner shared by Assemble(). The scanner is
// straightforward and ineffective, but high speed is not a must here. As
// input, it uses global pointer to source line asmcmd. On exit, it fills in
// global variables scan, prio, sdata, idata and/or fdata. If some error is
// detected, asmerror points to error message, otherwise asmerror remains
// unchanged.
//--------------------------------------------------------------------------------
void		CMgAsmBase::Scanasm(int mode)
{
    int i, j, base, maxdigit;
    long decimal, hex;
    long double floating, divisor;
    char s[TEXTLEN], *pcmd;

    m_sdata[0] = '\0';
    m_idata = 0;

    //
    if (m_pAsmCmd == NULL)
        {
            m_pAsmError = ("NULL input line");
            m_nScan = SCAN_ERR;
            return;
        }

    //
    while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
        {
            m_pAsmCmd++;    // Skip leading spaces
        }

    //
    if (*m_pAsmCmd == '\0' || *m_pAsmCmd == ';')
        {
            m_nScan = SCAN_EOL;    // Empty line
            return;
        }

    //
    if (isalpha(*m_pAsmCmd) || *m_pAsmCmd == '_' || *m_pAsmCmd == '@')
        {
            m_sdata[0] = *m_pAsmCmd++;
            i = 1;         // Some keyword or identifier
            while ((isalnum(*m_pAsmCmd) || *m_pAsmCmd == '_' || *m_pAsmCmd == '@') && i < sizeof(m_sdata))
                {
                    m_sdata[i++] = *m_pAsmCmd++;
                }
            if (i >= sizeof(m_sdata))
                {
                    m_pAsmError = ("Too long identifier");
                    m_nScan = SCAN_ERR;
                    return;
                }

            m_sdata[i] = '\0';

            while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                {
                    m_pAsmCmd++;                        // Skip trailing spaces
                }
            strcpy(s, m_sdata);
            strupr(s);

            for (j = 0; j <= 8; j++)
                {
                    // j==8 means "any register"
                    if (strcmp(s, g_szRegName[0][j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_REG8;       // 8-bit register
                    return;
                }

            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szRegName[1][j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_REG16;      // 16-bit register
                    return;
                }

            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szRegName[2][j]) != 0) 
					{
						continue;
					}
					m_idata = j;
                    m_nScan = SCAN_REG32;      // 32-bit register
                    return;
                }

            for (j = 0; j < 6; j++)
                {
                    if (strcmp(s, g_szSegName[j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_SEG;        // Segment register
                    while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                        {
                            m_pAsmCmd++;      // Skip trailing spaces
                        }
                    return;
                }

            if (strcmp(s, "ST") == 0)
                {
                    pcmd = m_pAsmCmd;
                    Scanasm(SA_NAME);   // FPU register
                    if (m_nScan != SCAN_SYMB || m_idata != '(')
                        {
                            m_pAsmCmd = pcmd;                 // Undo last scan
                            m_idata = 0;
                            m_nScan = SCAN_FPU;
                            return;
                        }
                    Scanasm(SA_NAME);
                    j = m_idata;
                    if ((m_nScan != SCAN_ICONST && m_nScan != SCAN_DCONST) || m_idata < 0 || m_idata > 7)
                        {
                            m_pAsmError = ("FPU registers have indexes 0 to 7");
                            m_nScan = SCAN_ERR;
                            return;
                        }

                    Scanasm(SA_NAME);
                    if (m_nScan != SCAN_SYMB || m_idata != ')')
                        {
                            m_pAsmError = ("Closing parenthesis expected");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    m_idata = j;
                    m_nScan = SCAN_FPU;
                    return;
                }
            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szFPUName[j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_FPU;        // FPU register (alternative coding)
                    return;
                }
            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szMMXName[j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_MMX;        // MMX register
                    return;
                }
            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szCRName[j]) != 0)
                        {
                            continue;
                        }
                    m_idata = j;
                    m_nScan = SCAN_CR;         // Control register
                    return;
                }
            for (j = 0; j <= 8; j++)
                {
                    if (strcmp(s, g_szDRName[j]) != 0) continue;
                    m_idata = j;
                    m_nScan = SCAN_DR;         // Debug register
                    return;
                }
            for (j = 0; j < sizeof(g_szSizeName) / sizeof(g_szSizeName[0]); j++)
                {
                    if (strcmp(s, g_szSizeName[j]) != 0)
                        {
                            continue;
                        }
                    pcmd = m_pAsmCmd;
                    Scanasm(SA_NAME);
                    if (m_nScan != SCAN_PTR)            // Fetch non-functional "PTR"
                        {
                            m_pAsmCmd = pcmd;
                        }
                    m_idata = j;
                    m_nScan = SCAN_OPSIZE;     // Operand (data) size in bytes
                    return;
                }
            if (strcmp(s, ("EIP")) == 0)
                {
                    // Register EIP
                    m_nScan = SCAN_EIP;
                    m_idata = 0;
                    return;
                }
            if (strcmp(s, ("SHORT")) == 0)
                {
                    // Relative jump has 1-byte offset
                    m_nScan = SCAN_JMPSIZE;
                    m_idata = 1;
                    return;
                }
            if (strcmp(s, ("LONG")) == 0)
                {
                    // Relative jump has 4-byte offset
                    m_nScan = SCAN_JMPSIZE;
                    m_idata = 2;
                    return;
                }
            if (strcmp(s, ("NEAR")) == 0)
                {
                    // Jump within same code segment
                    m_nScan = SCAN_JMPSIZE;
                    m_idata = 4;
                    return;
                }
            if (strcmp(s, ("FAR")) == 0)
                {
                    // Jump to different code segment
                    m_nScan = SCAN_JMPSIZE;
                    m_idata = 8;
                    return;
                }
            if (strcmp(s, ("LOCAL")) == 0 && *m_pAsmCmd == '.')
                {
                    m_pAsmCmd++;
                    while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                        {
                            m_pAsmCmd++;    // Skip trailing spaces
                        }
                    if (!isdigit(*m_pAsmCmd))
                        {
                            m_pAsmError = ("Integer number expected");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    while (isdigit(*m_pAsmCmd))         // LOCAL index is decimal number!
                        {
                            m_idata = m_idata * 10 + (*m_pAsmCmd++) - '0';
                        }
                    m_nScan = SCAN_LOCAL;
                    return;
                }
            if (strcmp(s, ("ARG")) == 0 && *m_pAsmCmd == '.')
                {
                    m_pAsmCmd++;
                    while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                        {
                            m_pAsmCmd++;    // Skip trailing spaces
                        }
                    if (!isdigit(*m_pAsmCmd))
                        {
                            m_pAsmError = ("Integer number expected");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    while (isdigit(*m_pAsmCmd))         // ARG index is decimal number!
                        {
                            m_idata = m_idata * 10 + (*m_pAsmCmd++) - '0';
                        }
                    m_nScan = SCAN_ARG;
                    return;
                }
            if (strcmp(s, ("REP")) == 0)
                {
                    m_nScan = SCAN_REP;    // REP prefix
                    return;
                }
            if (strcmp(s, ("REPE")) == 0 || strcmp(s, ("REPZ")) == 0)
                {
                    m_nScan = SCAN_REPE;    // REPE prefix
                    return;
                }
            if (strcmp(s, ("REPNE")) == 0 || strcmp(s, ("REPNZ")) == 0)
                {
                    m_nScan = SCAN_REPNE;    // REPNE prefix
                    return;
                }
            if (strcmp(s, ("LOCK")) == 0)
                {
                    m_nScan = SCAN_LOCK;    // LOCK prefix
                    return;
                }
            if (strcmp(s, ("PTR")) == 0)
                {
                    m_nScan = SCAN_PTR;    // PTR in MASM addressing statements
                    return;
                }
            if (strcmp(s, ("CONST")) == 0 || strcmp(s, ("OFFSET")) == 0)
                {
                    m_nScan = SCAN_OFS;    // Present but undefined offset/constant
                    return;
                }
            if (strcmp(s, ("SIGNED")) == 0)
                {
                    m_nScan = SCAN_SIGNED;    // Keyword "SIGNED" (in expressions)
                    return;
                }
            if (strcmp(s, ("UNSIGNED")) == 0)
                {
                    m_nScan = SCAN_UNSIGNED;    // Keyword "UNSIGNED" (in expressions)
                    return;
                }
            if (strcmp(s, ("CHAR")) == 0)
                {
                    m_nScan = SCAN_CHAR;    // Keyword "CHAR" (in expressions)
                    return;
                }
            if (strcmp(s, ("FLOAT")) == 0)
                {
                    m_nScan = SCAN_FLOAT;    // Keyword "FLOAT" (in expressions)
                    return;
                }
            if (strcmp(s, ("DOUBLE")) == 0)
                {
                    m_nScan = SCAN_DOUBLE;    // Keyword "DOUBLE" (in expressions)
                    return;
                }
            if (strcmp(s, ("FLOAT10")) == 0)
                {
                    m_nScan = SCAN_FLOAT10;    // Keyword "FLOAT10" (in expressions)
                    return;
                }
            if (strcmp(s, ("STRING")) == 0)
                {
                    m_nScan = SCAN_STRING;    // Keyword "STRING" (in expressions)
                    return;
                }
            if (strcmp(s, ("UNICODE")) == 0)
                {
                    m_nScan = SCAN_UNICODE;    // Keyword "UNICODE" (in expressions)
                    return;
                }
            if (strcmp(s, ("MSG")) == 0)
                {
                    m_nScan = SCAN_MSG;    // Pseudovariable MSG (in expressions)
                    return;
                }
            if (mode & SA_NAME)
                {
                    m_idata = i;
                    m_nScan = SCAN_NAME;       // Don't try to decode symbolic label
                    return;
                }
            m_pAsmError = ("Unknown identifier");
            m_nScan = SCAN_ERR;
            return;
        }
    else if (isdigit(*m_pAsmCmd))            // Constant
        {
            base = 0;
            maxdigit = 0;
            decimal = hex = 0L;
            floating = 0.0;
            if (m_pAsmCmd[0] == '0' && toupper(m_pAsmCmd[1]) == 'X')
                {
                    base = 16;
                    m_pAsmCmd += 2;
                }           // Force hexadecimal number
            while (1)
                {
                    if (isdigit(*m_pAsmCmd))
                        {
                            decimal = decimal * 10 + (*m_pAsmCmd) - '0';
                            floating = floating * 10.0 + (*m_pAsmCmd) - '0';
                            hex = hex * 16 + (*m_pAsmCmd) - '0';
                            if (maxdigit == 0)
                                {
                                    maxdigit = 9;
                                }
                            m_pAsmCmd++;

                        }
                    else if (isxdigit(*m_pAsmCmd))
                        {
                            hex = hex * 16 + toupper(*m_pAsmCmd++) - 'A' + 10;
                            maxdigit = 15;
                        }
                    else
                        {
                            break;
                        }
                }
            if (maxdigit == 0)
                {
                    m_pAsmError = ("Hexadecimal digits after 0x... expected");
                    m_nScan = SCAN_ERR;
                    return;
                }
            if (toupper(*m_pAsmCmd) == 'H')
                {
                    // Force hexadecimal number
                    if (base == 16)
                        {
                            m_pAsmError = ("Please don't mix 0xXXXX and XXXXh forms");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    m_pAsmCmd++;
                    m_idata = hex;
                    m_nScan = SCAN_ICONST;
                    while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                        {
                            m_pAsmCmd++;
                        }
                    return;
                }
            if (*m_pAsmCmd == '.')
                {
                    // Force decimal number
                    if (base == 16 || maxdigit > 9)
                        {
                            m_pAsmError = ("Not a decimal number");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    m_pAsmCmd++;
                    if (isdigit(*m_pAsmCmd) || toupper(*m_pAsmCmd) == 'E')
                        {
                            divisor = 1.0;
                            while (isdigit(*m_pAsmCmd))
                                {
                                    // Floating-point number
                                    divisor /= 10.0;
                                    floating += divisor * (*m_pAsmCmd - '0');
                                    m_pAsmCmd++;
                                }
                            if (toupper(*m_pAsmCmd) == 'E')
                                {
                                    m_pAsmCmd++;
                                    if (*m_pAsmCmd == '-')
                                        {
                                            base = -1;
                                            m_pAsmCmd++;
                                        }
                                    else
                                        {
                                            base = 1;
                                        }
                                    if (!isdigit(*m_pAsmCmd))
                                        {
                                            m_pAsmError = ("Invalid exponent");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    decimal = 0;
                                    while (isdigit(*m_pAsmCmd))
                                        {
                                            if (decimal < 65536L) decimal = decimal * 10 + (*m_pAsmCmd++) - '0';
                                        }
                                    //floating*=pow10l(decimal*base);
                                    floating *= powl(10, decimal * base);
                                }
                            m_fdata = floating;
                            m_nScan = SCAN_FCONST;
                            return;
                        }
                    else
                        {
                            m_idata = decimal;
                            m_nScan = SCAN_DCONST;
                            while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                                {
                                    m_pAsmCmd++;
                                }
                            return;
                        }
                }
            m_idata = hex;
            m_nScan = SCAN_ICONST;     // Default is hexadecimal
            while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                {
                    m_pAsmCmd++;
                }
            return;
        }
    else if (*m_pAsmCmd == '\'')             // Character constant
        {
            m_pAsmCmd++;
            if (*m_pAsmCmd == '\0' || (*m_pAsmCmd == '\\' && m_pAsmCmd[1] == '\0'))
                {
                    m_pAsmError = ("Unterminated character constant");
                    m_nScan = SCAN_ERR;
                    return;
                }
            if (*m_pAsmCmd == '\'')
                {
                    m_pAsmError = ("Empty character constant");
                    m_nScan = SCAN_ERR;
                    return;
                }
            if (*m_pAsmCmd == '\\')
                {
                    m_pAsmCmd++;
                }
            m_idata = *m_pAsmCmd++;
            if (*m_pAsmCmd != '\'')
                {
                    m_pAsmError = ("Unterminated character constant");
                    m_nScan = SCAN_ERR;
                    return;
                }
            m_pAsmCmd++;
            while (*m_pAsmCmd == ' ' || *m_pAsmCmd == '\t')
                {
                    m_pAsmCmd++;
                }
            m_nScan = SCAN_ICONST;
            return;
        }
    else                                  // Any other character or combination
        {
            m_idata = m_sdata[0] = *m_pAsmCmd++;
            m_sdata[1] = m_sdata[2] = '\0';
            if (m_idata == '|' && *m_pAsmCmd == '|')
                {
                    m_idata = '||';
                    m_nPrio = 10;           // '||'
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '&' && *m_pAsmCmd == '&')
                {
                    m_idata = '&&';
                    m_nPrio = 9;            // '&&'
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '=' && *m_pAsmCmd == '=')
                {
                    m_idata = '==';
                    m_nPrio = 5;            // '=='
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '!' && *m_pAsmCmd == '=')
                {
                    m_idata = '!=';
                    m_nPrio = 5;            // '!='
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '<' && *m_pAsmCmd == '=')
                {
                    m_idata = '<=';
                    m_nPrio = 4;            // '<='
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '>' && *m_pAsmCmd == '=')
                {
                    m_idata = '>=';
                    m_nPrio = 4;            // '>='
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '<' && *m_pAsmCmd == '<')
                {
                    m_idata = '<<';
                    m_nPrio = 3;            // '<<'
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '>' && *m_pAsmCmd == '>')
                {
                    m_idata = '>>';
                    m_nPrio = 3;            // '>>'
                    m_sdata[1] = *m_pAsmCmd++;
                }
            else if (m_idata == '|')
                {
                    m_nPrio = 8;    // '|'
                }
            else if (m_idata == '^')
                {
                    m_nPrio = 7;    // '^'
                }
            else if (m_idata == '&')
                {
                    m_nPrio = 6;    // '&'
                }
            else if (m_idata == '<')
                {
                    if (*m_pAsmCmd == '&')
                        {
                            // Import pseudolabel (for internal use)
                            if ((mode & SA_IMPORT) == 0)
                                {
                                    m_pAsmError = ("Syntax error");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            m_pAsmCmd++;
                            i = 0;
                            while (*m_pAsmCmd != '\0' && *m_pAsmCmd != '>')
                                {
                                    m_sdata[i++] = *m_pAsmCmd++;
                                    if (i >= sizeof(m_sdata))
                                        {
                                            m_pAsmError = ("Too long import name");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                }
                            if (*m_pAsmCmd != '>')
                                {
                                    m_pAsmError = ("Unterminated import name");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            m_pAsmCmd++;
                            m_sdata[i] = '\0';
                            m_nScan = SCAN_IMPORT;
                            return;
                        }
                    else
                        {
                            m_nPrio = 4;    // '<'
                        }
                }
            else if (m_idata == '>')
                {
                    m_nPrio = 4;     // '>'
                }
            else if (m_idata == '+')
                {
                    m_nPrio = 2;     // '+'
                }
            else if (m_idata == '-')
                {
                    m_nPrio = 2;     // '-'
                }
            else if (m_idata == '*')
                {
                    m_nPrio = 1;     // '*'
                }
            else if (m_idata == '/')
                {
                    m_nPrio = 1;     // '/'
                }
            else if (m_idata == '%')
                {
                    m_nPrio = 1;     // '%'
                }
            else if (m_idata == ']')
                {
                    pcmd = m_pAsmCmd;
                    Scanasm(SA_NAME);
                    if (m_nScan != SCAN_SYMB || m_idata != '[')
                        {
                            m_idata = ']';
                            m_pAsmCmd = pcmd;
                            m_nPrio = 0;
                        }
                    else
                        {
                            m_idata = '+';    // Translate '][' to '+'
                            m_nPrio = 2;
                        }
                }
            else
                {
                    m_nPrio = 0;     // Any other character
                }
            m_nScan = SCAN_SYMB;
            return;
        }
}

// Fetches one complete operand from the input line and fills in structure op  with operand's data
// Expects that first token of the operand is already
// scanned. Supports operands in generalized form (for example, R32 means any
// of general-purpose 32-bit integer registers).
void		CMgAsmBase::Parseasmoperand(t_asmoperand *op)
{
    int i, j, bracket, sign, xlataddr;
    int reg, r[9];
    long offset;
    if (m_nScan == SCAN_EOL || m_nScan == SCAN_ERR)
        {
            return;    // No or bad operand
        }

    // Jump or call address may begin with address size modifier(s) SHORT, LONG,
    // NEAR and/or FAR. Not all combinations are allowed. After operand is
    // completely parsed, this function roughly checks whether modifier is
    // allowed. Exact check is done in Assemble().
    if (m_nScan == SCAN_JMPSIZE)
        {
            j = 0;
            while (m_nScan == SCAN_JMPSIZE)
                {
                    j |= m_idata;                      // Fetch all size modifiers
                    Scanasm(0);
                }
            if (
                ((j & 0x03) == 0x03) ||          // Mixed SHORT and LONG
                ((j & 0x0C) == 0x0C) ||          // Mixed NEAR and FAR
                ((j & 0x09) == 0x09)             // Mixed FAR and SHORT
				)
                {
                    m_pAsmError = ("Invalid combination of jump address modifiers");
                    m_nScan = SCAN_ERR;
                    return;
                }
            if ((j & 0x08) == 0)
                {
                    j |= 0x04;    // Force NEAR if not FAR
                }
            op->jmpmode = j;
        }


    // Simple operands are either register or constant, their processing is

    //简单的操作数为寄存器或常数，它们的处理

    // obvious and straightforward.
    if (m_nScan == SCAN_REG8 || m_nScan == SCAN_REG16 || m_nScan == SCAN_REG32)
        {
            op->type = REG;
            op->index = m_idata;   // Integer general-purpose register  整数通用寄存器
            if (m_nScan == SCAN_REG8)
                {
                    op->size = 1;
                }
            else if (m_nScan == SCAN_REG16)
                {
                    op->size = 2;
                }
            else
                {
                    op->size = 4;
                }
        }
    else if (m_nScan == SCAN_FPU)            // FPU register
        {
            op->type = RST;
            op->index = m_idata;
        }
    else if (m_nScan == SCAN_MMX)            // MMX or 3DNow! register
        {
            op->type = RMX;
            op->index = m_idata;
        }
    else if (m_nScan == SCAN_CR)             // Control register
        {
            op->type = CRX;
            op->index = m_idata;
        }
    else if (m_nScan == SCAN_DR)             // Debug register
        {
            op->type = DRX;
            op->index = m_idata;
        }
    else if (m_nScan == SCAN_SYMB && m_idata == '-')
        {
            Scanasm(0);                        // Negative constant
            if (m_nScan != SCAN_ICONST && m_nScan != SCAN_DCONST && m_nScan != SCAN_OFS)
                {
                    m_pAsmError = ("Integer number expected");
                    m_nScan = SCAN_ERR;
                    return;
                }
            op->type = IMM;
            op->offset = -m_idata;
            if (m_nScan == SCAN_OFS)
                {
                    op->anyoffset = 1;
                }

        }
    else if (m_nScan == SCAN_SYMB && m_idata == '+')
        {
            Scanasm(0);                        // Positive constant
            if (m_nScan != SCAN_ICONST && m_nScan != SCAN_DCONST && m_nScan != SCAN_OFS)
                {
                    m_pAsmError = ("Integer number expected");
                    m_nScan = SCAN_ERR;
                    return;
                }
            op->type = IMM;
            op->offset = m_idata;
            if (m_nScan == SCAN_OFS)
                {
                    op->anyoffset = 1;
                }

        }
    else if (m_nScan == SCAN_ICONST || m_nScan == SCAN_DCONST || m_nScan == SCAN_OFS)
        {
            j = m_idata;
            if (m_nScan == SCAN_OFS)
                {
                    op->anyoffset = 1;
                }
            Scanasm(0);
            if (m_nScan == SCAN_SYMB && m_idata == ':')
                {
                    Scanasm(0);                      // Absolute long address (seg:offset)
                    if (m_nScan != SCAN_ICONST && m_nScan != SCAN_DCONST && m_nScan != SCAN_OFS)
                        {
                            m_pAsmError = ("Integer address expected");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    op->type = JMF;
                    op->offset = m_idata;
                    op->segment = j;
                    if (m_nScan == SCAN_OFS) op->anyoffset = 1;
                }
            else
                {
                    op->type = IMM;
                    op->offset = j;    // Constant without sign
                    return;                          // Next token already scanned
                }
        }
    else if (m_nScan == SCAN_FCONST)
        {
            m_pAsmError = ("Floating-point numbers are not allowed in command");
            m_nScan = SCAN_ERR;
            return;
        }
    else if (m_nScan == SCAN_SEG || m_nScan == SCAN_OPSIZE || (m_nScan == SCAN_SYMB && m_idata == '[')) // Segment register or address
        {
            bracket = 0;
            if (m_nScan == SCAN_SEG)
                {
                    j = m_idata;
                    Scanasm(0);
                    if (m_nScan != SCAN_SYMB || m_idata != ':')
                        {
                            op->type = SGM;
                            op->index = j;   // Segment register as operand
                            return;                     // Next token already scanned
                        }
                    op->segment = j;
                    Scanasm(0);
                }

            // Scan 32-bit address. This parser does not support 16-bit addresses.
            // First of all, get size of operand (optional), segment register (optional)
            // and opening bracket (required).
            while (1)
                {
                    if (m_nScan == SCAN_SYMB && m_idata == '[')
                        {
                            if (bracket)
                                {
                                    // Bracket
                                    m_pAsmError = ("Only one opening bracket allowed");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            bracket = 1;
                        }
                    else if (m_nScan == SCAN_OPSIZE)
                        {
                            if (op->size != 0)
                                {
                                    // Size of operand
                                    m_pAsmError = ("Duplicated size modifier");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            op->size = m_idata;
                        }
                    else if (m_nScan == SCAN_SEG)
                        {
                            if (op->segment != SEG_UNDEF)
                                {
                                    // Segment register
                                    m_pAsmError = ("Duplicated segment register");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }

                            op->segment = m_idata;
                            Scanasm(0);
                            if (m_nScan != SCAN_SYMB || m_idata != ':')
                                {
                                    m_pAsmError = ("Semicolon expected");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                        }
                    else if (m_nScan == SCAN_ERR)
                        {
                            return;
                        }
                    else
                        {
                            break;     // None of expected address elements
                        }
                    Scanasm(0);
                }
            if (bracket == 0)
                {
                    m_pAsmError = ("Address expression requires brackets");
                    m_nScan = SCAN_ERR;
                    return;
                }


            // Assembling a 32-bit address may be a kind of nigthmare, due to a large
            // number of allowed forms. Parser collects immediate offset in op->offset
            // and count for each register in array r[]. Then it decides whether this
            // combination is valid and determines scale, index and base. Assemble()
            // will use these numbers to select address form (with or without SIB byte,
            // 8- or 32-bit offset, use segment prefix or not). As a useful side effect
            // of this technique, one may specify, for example, [EAX*5] which will
            // correctly assemble to [EAX*4+EAX].
            for (i = 0; i <= 8; i++)
                {
                    r[i] = 0;
                }
            sign = '+';                        // Default sign for the first operand
            xlataddr = 0;

            while (1)
                {
                    // Get SIB and offset
                    if (m_nScan == SCAN_SYMB && (m_idata == '+' || m_idata == '-'))
                        {
                            sign = m_idata;
                            Scanasm(0);
                        }
                    if (m_nScan == SCAN_ERR)
                        {
                            return;
                        }
                    if (sign == '?')
                        {
                            m_pAsmError = ("Syntax error");
                            m_nScan = SCAN_ERR;
                            return;
                        }

                    // Register AL appears as part of operand of (seldom used) command XLAT.
                    if (m_nScan == SCAN_REG8 && m_idata == REG_EAX)
                        {
                            if (sign == '-')
                                {
                                    m_pAsmError = ("Unable to subtract register");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            if (xlataddr != 0)
                                {
                                    m_pAsmError = ("Too many registers");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            xlataddr = 1;
                            Scanasm(0);
                        }
                    else if (m_nScan == SCAN_REG16)
                        {
                            m_pAsmError = ("Sorry, 16-bit addressing is not supported");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    else if (m_nScan == SCAN_REG32)
                        {
                            if (sign == '-')
                                {
                                    m_pAsmError = ("Unable to subtract register");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            reg = m_idata;
                            Scanasm(0);
                            if (m_nScan == SCAN_SYMB && m_idata == '*')
                                {
                                    Scanasm(0);                  // Try index*scale
                                    if (m_nScan == SCAN_ERR)
                                        {
                                            return;
                                        }
                                    if (m_nScan == SCAN_OFS)
                                        {
                                            m_pAsmError = ("Undefined scale is not allowed");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    if (m_nScan != SCAN_ICONST && m_nScan != SCAN_DCONST)
                                        {
                                            m_pAsmError = ("Syntax error");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    if (m_idata == 6 || m_idata == 7 || m_idata > 9)
                                        {
                                            m_pAsmError = ("Invalid scale");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    r[reg] += m_idata;
                                    Scanasm(0);
                                }
                            else
                                {
                                    r[reg]++;    // Simple register
                                }
                        }
                    else if (m_nScan == SCAN_LOCAL)
                        {
                            r[REG_EBP]++;
                            op->offset -= m_idata * 4;
                            Scanasm(0);
                        }
                    else if (m_nScan == SCAN_ARG)
                        {
                            r[REG_EBP]++;
                            op->offset += (m_idata + 1) * 4;
                            Scanasm(0);
                        }
                    else if (m_nScan == SCAN_ICONST || m_nScan == SCAN_DCONST)
                        {
                            offset = m_idata;
                            Scanasm(0);
                            if (m_nScan == SCAN_SYMB && m_idata == '*')
                                {
                                    Scanasm(0);                  // Try scale*index
                                    if (m_nScan == SCAN_ERR)
                                        {
                                            return;
                                        }
                                    if (sign == '-')
                                        {
                                            m_pAsmError = ("Unable to subtract register");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    if (m_nScan == SCAN_REG16)
                                        {
                                            m_pAsmError = ("Sorry, 16-bit addressing is not supported");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    if (m_nScan != SCAN_REG32)
                                        {
                                            m_pAsmError = ("Syntax error");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    if (offset == 6 || offset == 7 || offset > 9)
                                        {
                                            m_pAsmError = ("Invalid scale");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    r[m_idata] += offset;
                                    Scanasm(0);
                                }
                            else
                                {
                                    if (sign == '-')
                                        {
                                            op->offset -= offset;
                                        }
                                    else
                                        {
                                            op->offset += offset;
                                        }
                                }
                        }
                    else if (m_nScan == SCAN_OFS)
                        {
                            Scanasm(0);
                            if (m_nScan == SCAN_SYMB && m_idata == '*')
                                {
                                    m_pAsmError = ("Undefined scale is not allowed");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            else
                                {
                                    op->anyoffset = 1;
                                }
                        }
                    else
                        {
                            break;    // None of expected address elements
                        }
                    if (m_nScan == SCAN_SYMB && m_idata == ']')
                        {
                            break;
                        }
                    sign = '?';
                }
            if (m_nScan == SCAN_ERR)
                {
                    return;
                }
            if (m_nScan != SCAN_SYMB || m_idata != ']')
                {
                    m_pAsmError = ("Syntax error");
                    m_nScan = SCAN_ERR;
                    return;
                }

            // Process XLAT address separately.
            if (xlataddr != 0)
                {
                    // XLAT address in form [EBX+AX]
                    for (i = 0; i <= 8; i++)
                        {
                            // Check which registers used
                            if (i == REG_EBX)
                                {
                                    continue;
                                }
                            if (r[i] != 0) break;
                        }
                    if (i <= 8 || r[REG_EBX] != 1 || op->offset != 0 || op->anyoffset != 0)
                        {
                            m_pAsmError = ("Invalid address");
                            m_nScan = SCAN_ERR;
                            return;
                        }
                    op->type = MXL;

                }
            else  	// Determine scale, index and base.
                {
                    j = 0;                           // Number of used registers
                    for (i = 0; i <= 8; i++)
                        {
                            if (r[i] == 0)
                                {
                                    continue;    // Unused register
                                }
                            if (r[i] == 3 || r[i] == 5 || r[i] == 9)
                                {
                                    if (op->index >= 0 || op->base >= 0)
                                        {
                                            if (j == 0)
                                                {
                                                    m_pAsmError = ("Invalid scale");
                                                }
                                            else
                                                {
                                                    m_pAsmError = ("Too many registers");
                                                }
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    op->index = op->base = i;
                                    op->scale = r[i] - 1;
                                }
                            else if (r[i] == 2 || r[i] == 4 || r[i] == 8)
                                {
                                    if (op->index >= 0)
                                        {
                                            if (j <= 1)
                                                {
                                                    m_pAsmError = ("Only one register may be scaled");
                                                }
                                            else
                                                {
                                                    m_pAsmError = ("Too many registers");
                                                }
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }
                                    op->index = i;
                                    op->scale = r[i];
                                }
                            else if (r[i] == 1)
                                {
                                    if (op->base < 0)
                                        {
                                            op->base = i;
                                        }
                                    else if (op->index < 0)
                                        {
                                            op->index = i;
                                            op->scale = 1;
                                        }
                                    else
                                        {
                                            m_pAsmError = ("Too many registers");
                                            m_nScan = SCAN_ERR;
                                            return;
                                        }

                                }
                            else
                                {
                                    m_pAsmError = ("Invalid scale");
                                    m_nScan = SCAN_ERR;
                                    return;
                                }
                            j++;
                        }
                    op->type = MRG;
                }
        }
    else
        {
            m_pAsmError = ("Unrecognized operand");
            m_nScan = SCAN_ERR;
            return;
        }

    // In general, address modifier is allowed only with address expression which
    // is a constant, a far address or a memory expression. More precise check
    // will be done later in Assemble().
    if (op->jmpmode != 0 && op->type != IMM && op->type != JMF && op->type != MRG)
        {
            m_pAsmError = ("Jump address modifier is not allowed");
            m_nScan = SCAN_ERR;
            return;
        }
    Scanasm(0);                          // Fetch next token from input line
}


//--------------------------------------------------------------------------------
// Function assembles text into 32-bit 80x86 machine code. It supports imprecise
// operands (for example, R32 stays for any general-purpose 32-bit register).
// This allows to search for incomplete commands. Command is precise when all
// significant bytes in model.mask are 0xFF. Some commands have more than one
// decoding. By calling Assemble() with attempt=0,1... and constsize=0,1,2,3 one
// gets also alternative variants (bit 0x1 of constsize is responsible for size
// of address constant and bit 0x2 - for immediate data). However, only one
// address form is generated ([EAX*2], but not [EAX+EAX]; [EBX+EAX] but not
// [EAX+EBX]; [EAX] will not use SIB byte; no DS: prefix and so on). Returns
// number of bytes in assembled code or non-positive number in case of detected
// error. This number is the negation of the offset in the input text where the
// error encountered. Unfortunately, BC 4.52 is unable to compile the switch
// (arg) in this code when any common subexpression optimization is on. The
// next #pragma statement disables all optimizations.
//--------------------------------------------------------------------------------
int			CMgAsmBase::Assemble(char *cmd, ulong ip, t_asmmodel *model, int attempt, int constsize, char *errtext)
{
    int i, j, k, namelen, nameok, arg, match, datasize, addrsize, bytesize, minop, maxop;
    int rep, lock, segment, jmpsize, jmpmode, longjump;
    int hasrm, hassib, dispsize, immsize;
    int anydisp, anyimm, anyjmp;
    long l, displacement, immediate, jmpoffset;
    char name[32], *nameend;
    unsigned char tcode[MAXCMDSIZE], tmask[MAXCMDSIZE];
    t_asmoperand aop[3], *op;            // 最多允许3个操作数 Allows up to three operands
    t_cmddata *pd;


    if (model != NULL)
        {
            model->length = 0;
        }
    if (cmd == NULL || model == NULL || errtext == NULL)
        {
            if (errtext != NULL) strcpy(errtext, ("Internal OLLYDBG error"));
            return 0;
        }                       // 错误的参数 Wrong parameter

    //
    m_pAsmCmd = cmd;
    rep = lock = 0;
    errtext[0] = '\0';

    //
    Scanasm(SA_NAME);

    if (m_nScan == SCAN_EOL)                // 行结束, nothing to assemble
        {
            return 0;
        }
    while (1)
        {
            // Fetch all REPxx and LOCK prefixes  取所有REPxx和LOCK前缀
            if (m_nScan == SCAN_REP || m_nScan == SCAN_REPE || m_nScan == SCAN_REPNE)
                {
                    if (rep != 0)
                        {
                            strcpy(errtext, ("Duplicated REP prefix(重复REP前缀)"));
                            goto error;
                        }
                    rep = m_nScan;
                }
            else if (m_nScan == SCAN_LOCK)
                {
                    if (lock != 0)
                        {
                            strcpy(errtext, ("Duplicated LOCK prefix(重复LOCK前缀)"));
                            goto error;
                        };
                    lock = m_nScan;
                }
            else
                {
                    break;    // No more prefixes 没有更多的前缀
                }
            Scanasm(SA_NAME);
        }

    if (m_nScan != SCAN_NAME || m_idata > 16)
        {
            strcpy(errtext, ("Command mnemonic expected(预期指令助记符)"));
            goto error;
        }

    //
    nameend = m_pAsmCmd;
    strupr(m_sdata);

    // Prepare full mnemonic (including repeat prefix, if any). 获取操作符
    if (rep == SCAN_REP)
        {
            sprintf(name, ("REP %s"), m_sdata);
        }
    else if (rep == SCAN_REPE)
        {
            sprintf(name, ("REPE %s"), m_sdata);
        }
    else if (rep == SCAN_REPNE)
        {
            sprintf(name, ("REPNE %s"), m_sdata);
        }
    else
        {
            strcpy(name, m_sdata);
        }
    Scanasm(0);

    // Parse command operands (up to 3). Note: jump address is always the first
    // (and only) operand in actual command set.

    //分析指令的操作数（最多3个） 。注：跳转地址永远是第一位

    for (i = 0; i < 3; i++)
        {
            aop[i].type = NNN;                 // No operand 无操作数
            aop[i].size = 0;                   // Undefined size 未定义的大小
            aop[i].index = -1;                 // No index 没有索引
            aop[i].scale = 0;                  // No scale 没有形成规模
            aop[i].base = -1;                  // No base 无基址
            aop[i].offset = 0;                 // No offset 无偏移
            aop[i].anyoffset = 0;              // No offset  无偏移
            aop[i].segment = SEG_UNDEF;        // No segment 在段
            aop[i].jmpmode = 0;              // No jump size modifier
        }

    //
    Parseasmoperand(aop + 0);
    jmpmode = aop[0].jmpmode;
    if (jmpmode != 0)
        {
            jmpmode |= 0x80;
        }

    if (m_nScan == SCAN_SYMB && m_idata == ',')
        {
            Scanasm(0);
            Parseasmoperand(aop + 1);
            if (m_nScan == SCAN_SYMB && m_idata == ',')
                {
                    Scanasm(0);
                    Parseasmoperand(aop + 2);
                }
        }

    if (m_nScan == SCAN_ERR)
        {
            strcpy(errtext, m_pAsmError);
            goto error;
        };
    if (m_nScan != SCAN_EOL)
        {
            strcpy(errtext, ("Extra input after operand"));
            goto error;
        };

    // If jump size is not specified, function tries to use short jump. If
    // attempt fails, it retries with long form.
    longjump = 0;                        // Try short jump on the first pass 在第一轮尝试短跳

    //label:-------------------------------
retrylongjump:


    nameok = 0;
    // Some commands allow different number of operands. Variables minop and
    // maxop accumulate their minimal and maximal counts. The numbers are not
    // used in assembly process but allow for better error diagnostics.

    minop = 3;
    maxop = 0;

    // Main assembly loop: try to find the command which matches all operands,
    // but do not process operands yet.
    namelen = strlen(name);

    for (pd = g_CmdData; pd->mask != 0; pd++)
        {
			//匹配操作符
            if (pd->name[0] == '&')
                {
                    // Mnemonic depends on operand size
                    j = 1;
                    datasize = 2;
                    addrsize = 4;
                    while (1)
                        {
                            // Try all mnemonics (separated by ':')
                            for (i = 0; pd->name[j] != '\0' && pd->name[j] != ':'; j++)
                                {
                                    if (pd->name[j] == '*')
                                        {
                                            if (name[i] == 'W')
                                                {
                                                    datasize = 2;
                                                    i++;
                                                }
                                            else if (name[i] == 'D')
                                                {
                                                    datasize = 4;
                                                    i++;
                                                }
                                            else if (m_nSizeSens == 0)
                                                {
                                                    datasize = 2;
                                                }
                                            else
                                                {
                                                    datasize = 4;
                                                }
                                        }
                                    else if (pd->name[j] == name[i])
                                        {
                                            i++;
                                        }
                                    else
                                        {
                                            break;
                                        }
                                }

                            if (name[i] == '\0' && (pd->name[j] == '\0' || pd->name[j] == ':'))
                                {
                                    break;     // Bingo!
                                }
                            while (pd->name[j] != '\0' && pd->name[j] != ':')
                                {
                                    j++;
                                }
                            if (pd->name[j] == ':')
                                {
                                    j++;    // Retry with 32-bit mnenonic
                                    datasize = 4;
                                }
                            else
                                {
                                    i = 0;    // Comparison failed
                                    break;
                                }
                        }
                    if (i == 0)
                        {
                            continue;
                        }
                }
            else if (pd->name[0] == '$')        // Mnemonic depends on address size
                {
                    j = 1;
                    datasize = 0;
                    addrsize = 2;
                    while (1)
                        {
                            // Try all mnemonics (separated by ':')
                            for (i = 0; pd->name[j] != '\0' && pd->name[j] != ':'; j++)
                                {
                                    if (pd->name[j] == '*')
                                        {
                                            if (name[i] == 'W')
                                                {
                                                    addrsize = 2;
                                                    i++;
                                                }
                                            else if (name[i] == 'D')
                                                {
                                                    addrsize = 4;
                                                    i++;
                                                }
                                            else if (m_nSizeSens == 0)
                                                {
                                                    addrsize = 2;
                                                }
                                            else
                                                {
                                                    addrsize = 4;
                                                }
                                        }
                                    else if (pd->name[j] == name[i])
                                        {
                                            i++;
                                        }
                                    else
                                        {
                                            break;
                                        }
                                }

                            if (name[i] == '\0' && (pd->name[j] == '\0' || pd->name[j] == ':'))
                                {
                                    break;    // Bingo!
                                }
                            while (pd->name[j] != '\0' && pd->name[j] != ':')
                                {
                                    j++;
                                }
                            if (pd->name[j] == ':')
                                {
                                    j++;    // Retry with 32-bit mnenonic
                                    addrsize = 4;
                                }
                            else
                                {
                                    i = 0;    // Comparison failed
                                    break;
                                }
                        }
                    if (i == 0)
                        {
                            continue;
                        }
                }
            else                                // Compare with all synonimes
                {
                    j = k = 0;
                    datasize = 0;                    // Default settings
                    addrsize = 4;
                    while (1)
                        {
                            while (pd->name[j] != ',' && pd->name[j] != '\0')
                                {
                                    j++;
                                }
                            if (j - k == namelen && strnicmp(name, pd->name + k, namelen) == 0)//匹配操作符
                                {
                                    break;
                                }
                            k = j + 1;
                            if (pd->name[j] == '\0')
                                {
                                    break;
                                }
                            j = k;
                        }
                    if (k > j)
                        {
                            continue;
                        }
                }

            // For error diagnostics it is important to know whether mnemonic exists.
            nameok++;
            if (pd->arg1 == NNN || pd->arg1 >= PSEUDOOP)
                {
                    minop = 0;
                }
            else if (pd->arg2 == NNN || pd->arg2 >= PSEUDOOP)
                {
                    if (minop > 1)
                        {
                            minop = 1;
                        }
                    if (maxop < 1)
                        {
                            maxop = 1;
                        }
                }
            else if (pd->arg3 == NNN || pd->arg3 >= PSEUDOOP)
                {
                    if (minop > 2)
                        {
                            minop = 2;
                        }
                    if (maxop < 2)
                        {
                            maxop = 2;
                        }
                }
            else
                {
                    maxop = 3;
                }

            // Determine default and allowed operand size(s).
            if (pd->bits == FF)
                {
                    datasize = 2;    // Forced 16-bit size
                }
            if (pd->bits == WW || pd->bits == WS || pd->bits == W3 || pd->bits == WP)
                {
                    bytesize = 1;     // 1-byte size allowed
                }
            else
                {
                    bytesize = 0;    // Word/dword size only
                }

            // Check whether command operands match specified. If so, variable match
            // remains zero, otherwise it contains kind of mismatch. This allows for
            // better error diagnostics.
            match = 0;
            for (j = 0; j < 3; j++)
                {
                    // Up to 3 operands
                    op = aop + j;
                    if (j == 0)
                        {
                            arg = pd->arg1;
                        }
                    else if (j == 1)
                        {
                            arg = pd->arg2;
                        }
                    else
                        {
                            arg = pd->arg3;
                        }
                    if (arg == NNN || arg >= PSEUDOOP)
                        {
                            if (op->type != NNN)           // No more arguments
                                {
                                    match |= MA_NOP;
                                }
                            break;
                        }
                    if (op->type == NNN)
                        {
                            match |= MA_NOP;    // No corresponding operand
                            break;
                        }

                    //
                    switch (arg)
                        {
                        case REG:                      // Integer register in Reg field
                        case RCM:                      // Integer register in command byte
                        case RAC:                      // Accumulator (AL/AX/EAX, implicit)
                        {
                            if (op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (arg == RAC && op->index != REG_EAX && op->index != 8)
                                {
                                    match |= MA_TYP;
                                }
                            if (bytesize == 0 && op->size == 1)
                                {
                                    match |= MA_SIZ;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (datasize != op->size)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case RG4:                      // Integer 4-byte register in Reg field
                        {
                            if (op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (datasize != op->size)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case RAX:                      // AX (2-byte, implicit)
                        {
                            if (op->type != REG || (op->index != REG_EAX && op->index != 8))
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 2)
                                {
                                    match |= MA_SIZ;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (datasize != op->size)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case RDX:                      // DX (16-bit implicit port address)
                        {
                            if (op->type != REG || (op->index != REG_EDX && op->index != 8))
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 2)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case RCL:                      // Implicit CL register (for shifts)
                        {
                            if (op->type != REG || (op->index != REG_ECX && op->index != 8))
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 1)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case RS0:                      // Top of FPU stack (ST(0))
                        {
                            if (op->type != RST || (op->index != 0 && op->index != 8))
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case RST:                      // FPU register (ST(i)) in command byte
                        {
                            if (op->type != RST)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case RMX:                      // MMX register MMx
                        case R3D:                      // 3DNow! register MMx
                        {
                            if (op->type != RMX)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case MRG:                      // Memory/register in ModRM byte
                        {
                            if (op->type != MRG && op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (bytesize == 0 && op->size == 1)
                                {
                                    match |= MA_SIZ;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (op->size != 0 && op->size != datasize)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case MR1:                      // 1-byte memory/register in ModRM byte
                        {
                            if (op->type != MRG && op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 1)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MR2:                      // 2-byte memory/register in ModRM byte
                        {
                            if (op->type != MRG && op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 2)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MR4:                      // 4-byte memory/register in ModRM byte
                        {
                            if (op->type != MRG && op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case RR4:                      // 4-byte memory/register (register only)
                        {
                            if (op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MRJ:                      // Memory/reg in ModRM as JUMP target
                        {
                            if (op->type != MRG && op->type != REG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            if ((jmpmode & 0x09) != 0)
                                {
                                    match |= MA_JMP;
                                }
                            jmpmode &= 0x7F;
                            break;
                        }
                        case MR8:                      // 8-byte memory/MMX register in ModRM
                        case MRD:                      // 8-byte memory/3DNow! register in ModRM
                        {
                            if (op->type != MRG && op->type != RMX)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 8)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case RR8:                      // 8-byte MMX register only in ModRM
                        case RRD:                      // 8-byte memory/3DNow! (register only)
                        {
                            if (op->type != RMX)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 8)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MMA:                      // Memory address in ModRM byte for LEA
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case MML:                      // Memory in ModRM byte (for LES)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 6)
                                {
                                    match |= MA_SIZ;
                                }
                            if (datasize == 0)
                                {
                                    datasize = 4;
                                }
                            else if (datasize != 4)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case MMS:                      // Memory in ModRM byte (as SEG:OFFS)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 6)
                                {
                                    match |= MA_SIZ;
                                }
                            if ((jmpmode & 0x07) != 0)
                                {
                                    match |= MA_JMP;
                                }
                            jmpmode &= 0x7F;
                            break;
                        }
                        case MM6:                      // Memory in ModRm (6-byte descriptor)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 6)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MMB:                      // Two adjacent memory locations (BOUND)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            k = op->size;
                            if (m_nIDEAL == 0 && k > 1)
                                {
                                    k /= 2;
                                }
                            if (k != 0 && k != datasize)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case MD2:                      // Memory in ModRM byte (16-bit integer)
                        case MB2:                      // Memory in ModRM byte (16-bit binary)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 2)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MD4:                      // Memory in ModRM byte (32-bit integer)
                        case MF4:                      // Memory in ModRM byte (32-bit float)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MD8:                      // Memory in ModRM byte (64-bit integer)
                        case MF8:                      // Memory in ModRM byte (64-bit float)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 8)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MDA:                      // Memory in ModRM byte (80-bit BCD)
                        case MFA:                      // Memory in ModRM byte (80-bit float)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0 && op->size != 10)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MFE:                      // Memory in ModRM byte (FPU environment)
                        case MFS:                      // Memory in ModRM byte (FPU state)
                        case MFX:                      // Memory in ModRM byte (ext. FPU state)
                        {
                            if (op->type != MRG)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->size != 0)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case MSO:                      // Source in string operands ([ESI])
                        {
                            if (op->type != MRG || op->base != REG_ESI || op->index != -1 ||
                                    op->offset != 0 || op->anyoffset != 0)
                                {
                                    match |= MA_TYP;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (op->size != 0 && op->size != datasize)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case MDE:                      // Destination in string operands ([EDI])
                        {
                            if (op->type != MRG || op->base != REG_EDI ||
                                    op->index != -1 || op->offset != 0 || op->anyoffset != 0)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->segment != SEG_UNDEF && op->segment != SEG_ES)
                                {
                                    match |= MA_SEG;
                                }
                            if (datasize == 0)
                                {
                                    datasize = op->size;
                                }
                            if (op->size != 0 && op->size != datasize)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case MXL:                      // XLAT operand ([EBX+AL])
                        {
                            if (op->type != MXL)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case IMM:                      // Immediate data (8 or 16/32)
                        case IMU:                      // Immediate unsigned data (8 or 16/32)
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case VXD:                      // VxD service (32-bit only)
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            if (datasize == 0)
                                {
                                    datasize = 4;
                                }
                            if (datasize != 4)
                                {
                                    match |= MA_SIZ;
                                }
                            break;
                        }
                        case JMF:                      // Immediate absolute far jump/call addr
                        {
                            if (op->type != JMF)
                                {
                                    match |= MA_TYP;
                                }
                            if ((jmpmode & 0x05) != 0)
                                {
                                    match |= MA_JMP;
                                }
                            jmpmode &= 0x7F;
                            break;
                        }
                        case JOB:                      // Immediate byte offset (for jumps)
                        {
                            if (op->type != IMM || longjump)
                                {
                                    match |= MA_TYP;
                                }
                            if ((jmpmode & 0x0A) != 0)
                                {
                                    match |= MA_JMP;
                                }
                            jmpmode &= 0x7F;
                            break;
                        }
                        case JOW:                      // Immediate full offset (for jumps)
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            if ((jmpmode & 0x09) != 0)
                                {
                                    match |= MA_JMP;
                                }
                            jmpmode &= 0x7F;
                            break;
                        }
                        case IMA:                      // Immediate absolute near data address
                        {
                            if (op->type != MRG || op->base >= 0 || op->index >= 0)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case IMX:                      // Immediate sign-extendable byte
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->offset < -128 || op->offset > 127)
                                {
                                    match |= MA_RNG;
                                }
                            break;
                        }
                        case C01:                      // Implicit constant 1 (for shifts)
                        {
                            if (op->type != IMM || (op->offset != 1 && op->anyoffset == 0))
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case IMS:                      // Immediate byte (for shifts)
                        case IM1:                      // Immediate byte
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->offset < -128 || op->offset > 255)
                                {
                                    match |= MA_RNG;
                                }
                            break;
                        }
                        case IM2:                      // Immediate word (ENTER/RET)
                        {
                            if (op->type != IMM)
                                {
                                    match |= MA_TYP;
                                }
                            if (op->offset < 0 || op->offset > 65535)
                                {
                                    match |= MA_RNG;
                                }
                            break;
                        }
                        case SGM:                      // Segment register in ModRM byte
                        {
                            if (op->type != SGM)
                                {
                                    match |= MA_TYP;
                                }
                            if (datasize == 0)
                                {
                                    datasize = 2;
                                }
                            if (datasize != 2)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case SCM:                      // Segment register in command byte
                        {
                            if (op->type != SGM)
                                {
                                    match |= MA_TYP;
                                }
                            break;
                        }
                        case CRX:                      // Control register CRx
                        case DRX:                      // Debug register DRx
                        {
                            if (op->type != arg)
                                {
                                    match |= MA_TYP;
                                }
                            if (datasize == 0)
                                {
                                    datasize = 4;
                                }
                            if (datasize != 4)
                                {
                                    match |= MA_DIF;
                                }
                            break;
                        }
                        case PRN:                      // Near return address (pseudooperand)
                        case PRF:                      // Far return address (pseudooperand)
                        case PAC:                      // Accumulator (AL/AX/EAX, pseudooperand)
                        case PAH:                      // AH (in LAHF/SAHF, pseudooperand)
                        case PFL:                      // Lower byte of flags (pseudooperand)
                        case PS0:                      // Top of FPU stack (pseudooperand)
                        case PS1:                      // ST(1) (pseudooperand)
                        case PCX:                      // CX/ECX (pseudooperand)
                        case PDI:                      // EDI (pseudooperand in MMX extentions)
                        {
                            break;
                        }
                        default:                       // Undefined type of operand
                        {
                            strcpy(errtext, ("Internal Assembler error"));
                            goto error;
                        }
                        }                               // End of switch (arg)

                    if ((jmpmode & 0x80) != 0)
                        {
                            match |= MA_JMP;
                        }
                    if (match != 0)
                        {
                            break;     // Some of the operands doesn't match
                        }
                }                                 // End of operand matching loop
            if (match == 0)
                {
                    // Exact match found
                    if (attempt > 0)
                        {
                            --attempt;    // Well, try to find yet another match
                            nameok = 0;
                        }
                    else
                        {
                            break;
                        }
                }
        }                                  // End of command search loop


    // Check whether some error was detected. If several errors were found
    // similtaneously, report one (roughly in order of significance).
    if (nameok == 0)
        {
            // Mnemonic unavailable
            strcpy(errtext, ("Unrecognized command"));
            m_pAsmCmd = nameend;
            goto error;
        }
    if (match != 0)
        {
            // Command not found
            if (minop > 0 && aop[minop - 1].type == NNN)
                {
                    strcpy(errtext, ("Too few operands"));
                }
            else if (maxop < 3 && aop[maxop].type != NNN)
                {
                    strcpy(errtext, ("Too many operands"));
                }
            else if (nameok > 1)               // More that 1 command
                {
                    strcpy(errtext, ("Command does not support given operands"));
                }
            else if (match & MA_JMP)
                {
                    strcpy(errtext, ("Invalid jump size modifier"));
                }
            else if (match & MA_NOP)
                {
                    strcpy(errtext, ("Wrong number of operands"));
                }
            else if (match & MA_TYP)
                {
                    strcpy(errtext, ("Command does not support given operands"));
                }
            else if (match & MA_NOS)
                {
                    strcpy(errtext, ("Please specify operand size"));
                }
            else if (match & MA_SIZ)
                {
                    strcpy(errtext, ("Bad operand size"));
                }
            else if (match & MA_DIF)
                {
                    strcpy(errtext, ("Different size of operands"));
                }
            else if (match & MA_SEG)
                {
                    strcpy(errtext, ("Invalid segment register"));
                }
            else if (match & MA_RNG)
                {
                    strcpy(errtext, ("Constant out of expected range"));
                }
            else
                {
                    strcpy(errtext, ("Erroneous command"));
                }

            //
            goto error;
        }


    // Exact match found. Now construct the code.
    hasrm = 0;                           // Whether command has ModR/M byte
    hassib = 0;                          // Whether command has SIB byte
    dispsize = 0;                        // Size of displacement (if any)
    immsize = 0;                         // Size of immediate data (if any)
    segment = SEG_UNDEF;                 // Necessary segment prefix
    jmpsize = 0;                         // No relative jumps

    //
    memset(tcode, 0, sizeof(tcode));
    *(ulong *)tcode = pd->code & pd->mask;
    memset(tmask, 0, sizeof(tmask));
    *(ulong *)tmask = pd->mask;

    //
    i = pd->len - 1;                     // Last byte of command itself
    if (rep)
        {
            i++;    // REPxx prefixes count as extra byte
        }

    // In some cases at least one operand must have explicit size declaration (as
    // in MOV [EAX],1). This preliminary check does not include all cases.
    if (pd->bits == WW || pd->bits == WS || pd->bits == WP)
        {
            if (datasize == 0)
                {
                    strcpy(errtext, ("Please specify operand size"));
                    goto error;
                }
            else if (datasize > 1)
                {
                    tcode[i] |= 0x01;    // WORD or DWORD size of operands
					//message(tcode[i]);
                }
            tmask[i] |= 0x01;
        }
    else if (pd->bits == W3)
        {
            if (datasize == 0)
                {
                    strcpy(errtext, ("Please specify operand size"));
                    goto error;
                }
            else if (datasize > 1)
                {
                    tcode[i] |= 0x08;    // WORD or DWORD size of operands
                }
            tmask[i] |= 0x08;
        }

    // Present suffix of 3DNow! command as immediate byte operand.
    if ((pd->type & C_TYPEMASK) == C_NOW)
        {
            immsize = 1;
            immediate = (pd->code >> 16) & 0xFF;
        }

    // Process operands again, this time constructing the code.
    anydisp = anyimm = anyjmp = 0;
    for (j = 0; j < 3; j++)
        {
            // Up to 3 operands
            op = aop + j;
            if (j == 0)
                {
                    arg = pd->arg1;
                }
            else if (j == 1)
                {
                    arg = pd->arg2;
                }
            else
                {
                    arg = pd->arg3;
                }
            if (arg == NNN)
                {
                    break;    // All operands processed
                }
            switch (arg)
                {
                case REG:                        // Integer register in Reg field
                case RG4:                        // Integer 4-byte register in Reg field
                case RMX:                        // MMX register MMx
                case R3D:                        // 3DNow! register MMx
                case CRX:                        // Control register CRx
                case DRX:                        // Debug register DRx
                {
                    hasrm = 1;
                    if (op->index < 8)
                        {
                            tcode[i + 1] |= (char)(op->index << 3);
                            tmask[i + 1] |= 0x38;
                        }
                    break;
                }
                case RCM:                        // Integer register in command byte
                case RST:                        // FPU register (ST(i)) in command byte
                {
                    if (op->index < 8)
                        {
                            tcode[i] |= (char)op->index;
                            tmask[i] |= 0x07;
                        }
                    break;
                }
                case RAC:                        // Accumulator (AL/AX/EAX, implicit)
                case RAX:                        // AX (2-byte, implicit)
                case RDX:                        // DX (16-bit implicit port address)
                case RCL:                        // Implicit CL register (for shifts)
                case RS0:                        // Top of FPU stack (ST(0))
                case MDE:                        // Destination in string op's ([EDI])
                case C01:                        // Implicit constant 1 (for shifts)
                {
                    break;                         // Simply skip implicit operands
                }
                case MSO:                        // Source in string op's ([ESI])
                case MXL:                        // XLAT operand ([EBX+AL])
                {
                    if (op->segment != SEG_UNDEF && op->segment != SEG_DS)
                        {
                            segment = op->segment;
                        }
                    break;
                }
                case MRG:                        // Memory/register in ModRM byte
                case MRJ:                        // Memory/reg in ModRM as JUMP target
                case MR1:                        // 1-byte memory/register in ModRM byte
                case MR2:                        // 2-byte memory/register in ModRM byte
                case MR4:                        // 4-byte memory/register in ModRM byte
                case RR4:                        // 4-byte memory/register (register only)
                case MR8:                        // 8-byte memory/MMX register in ModRM
                case RR8:                        // 8-byte MMX register only in ModRM
                case MRD:                        // 8-byte memory/3DNow! register in ModRM
                case RRD:                        // 8-byte memory/3DNow! (register only)
                {
                    hasrm = 1;
                    if (op->type != MRG)
                        {
                            // Register in ModRM byte
                            tcode[i + 1] |= 0xC0;
                            tmask[i + 1] |= 0xC0;
                            if (op->index < 8)
                                {
                                    tcode[i + 1] |= (char)op->index;
                                    tmask[i + 1] |= 0x07;
                                }
                            break;
                        }
                }                            // Note: NO BREAK, continue with address
                case MMA:                        // Memory address in ModRM byte for LEA
                case MML:                        // Memory in ModRM byte (for LES)
                case MMS:                        // Memory in ModRM byte (as SEG:OFFS)
                case MM6:                        // Memory in ModRm (6-byte descriptor)
                case MMB:                        // Two adjacent memory locations (BOUND)
                case MD2:                        // Memory in ModRM byte (16-bit integer)
                case MB2:                        // Memory in ModRM byte (16-bit binary)
                case MD4:                        // Memory in ModRM byte (32-bit integer)
                case MD8:                        // Memory in ModRM byte (64-bit integer)
                case MDA:                        // Memory in ModRM byte (80-bit BCD)
                case MF4:                        // Memory in ModRM byte (32-bit float)
                case MF8:                        // Memory in ModRM byte (64-bit float)
                case MFA:                        // Memory in ModRM byte (80-bit float)
                case MFE:                        // Memory in ModRM byte (FPU environment)
                case MFS:                        // Memory in ModRM byte (FPU state)
                case MFX:                        // Memory in ModRM byte (ext. FPU state)
                {
                    hasrm = 1;
                    displacement = op->offset;
                    anydisp = op->anyoffset;
                    if (op->base < 0 && op->index < 0)
                        {
                            dispsize = 4;                // Special case of immediate address
                            if (op->segment != SEG_UNDEF && op->segment != SEG_DS)
                                {
                                    segment = op->segment;
                                }
                            tcode[i + 1] |= 0x05;
                            tmask[i + 1] |= 0xC7;

                        }
                    else if (op->index < 0 && op->base != REG_ESP)
                        {
                            tmask[i + 1] |= 0xC0;        // SIB byte unnecessary
                            if (op->offset == 0 && op->anyoffset == 0 && op->base != REG_EBP)
                                {}// [EBP] always requires offset
                            else if ((constsize & 1) != 0 && ((op->offset >= -128 && op->offset < 128) || op->anyoffset != 0))
                                {
                                    tcode[i + 1] |= 0x40;      // Disp8
                                    dispsize = 1;
                                }
                            else
                                {
                                    tcode[i + 1] |= 0x80;      // Disp32
                                    dispsize = 4;
                                }
                            if (op->base < 8)
                                {
                                    if (op->segment != SEG_UNDEF && op->segment != g_addr32[op->base].defseg)
                                        {
                                            segment = op->segment;
                                        }
                                    tcode[i + 1] |= (char)op->base;       // Note that case [ESP] has base<0.
                                    tmask[i + 1] |= 0x07;
                                }
                            else
                                {
                                    segment = op->segment;
                                }
                        }
                    else                            // SIB byte necessary
                        {
                            hassib = 1;
                            if (op->base == REG_EBP &&   // EBP as base requires offset, optimize
                                    op->index >= 0 && op->scale == 1 && op->offset == 0 && op->anyoffset == 0)
                                {
                                    op->base = op->index;
                                    op->index = REG_EBP;
                                }
                            if (op->index == REG_ESP &&  // ESP cannot be an index, reorder
                                    op->scale <= 1)
                                {
                                    op->index = op->base;
                                    op->base = REG_ESP;
                                    op->scale = 1;
                                }
                            if (op->base < 0 &&          // No base means 4-byte offset, optimize
                                    op->index >= 0 && op->scale == 2 && op->offset >= -128 && op->offset < 128 && op->anyoffset == 0)
                                {
                                    op->base = op->index;
                                    op->scale = 1;
                                }

                            if (op->index == REG_ESP)
                                {
                                    // Reordering was unsuccessfull
                                    strcpy(errtext, ("Invalid indexing mode"));
                                    goto error;
                                }

                            if (op->base < 0)
                                {
                                    tcode[i + 1] |= 0x04;
                                    dispsize = 4;
                                }
                            else if (op->offset == 0 && op->anyoffset == 0 && op->base != REG_EBP)
                                {
                                    tcode[i + 1] |= 0x04;     // No displacement
                                }
                            else if ((constsize & 1) != 0 && ((op->offset >= -128 && op->offset < 128) || op->anyoffset != 0))
                                {
                                    tcode[i + 1] |= 0x44;      // Disp8
                                    dispsize = 1;
                                }
                            else
                                {
                                    tcode[i + 1] |= 0x84;      // Disp32
                                    dispsize = 4;
                                }

                            tmask[i + 1] |= 0xC7;        // ModRM completed, proceed with SIB
                            if (op->scale == 2)
                                {
                                    tcode[i + 2] |= 0x40;
                                }
                            else if (op->scale == 4)
                                {
                                    tcode[i + 2] |= 0x80;
                                }
                            else if (op->scale == 8)
                                {
                                    tcode[i + 2] |= 0xC0;
                                }

                            tmask[i + 2] |= 0xC0;
                            if (op->index < 8)
                                {
                                    if (op->index < 0)
                                        {
                                            op->index = 0x04;
                                        }
                                    tcode[i + 2] |= (char)(op->index << 3);
                                    tmask[i + 2] |= 0x38;
                                }
                            if (op->base < 8)
                                {
                                    if (op->base < 0)
                                        {
                                            op->base = 0x05;
                                        }
                                    if (op->segment != SEG_UNDEF && op->segment != g_addr32[op->base].defseg)
                                        {
                                            segment = op->segment;
                                        }
                                    tcode[i + 2] |= (char)op->base;
                                    tmask[i + 2] |= 0x07;
                                }
                            else
                                {
                                    segment = op->segment;
                                }
                        }
                    break;
                }
                case IMM:                        // Immediate data (8 or 16/32)
                case IMU:                        // Immediate unsigned data (8 or 16/32)
                case VXD:                        // VxD service (32-bit only)
                {
                    if (datasize == 0 && pd->arg2 == NNN && (pd->bits == SS || pd->bits == WS))
                        {
                            datasize = 4;
                        }
                    if (datasize == 0)
                        {
                            strcpy(errtext, ("Please specify operand size"));
                            goto error;
                        }
                    immediate = op->offset;
                    anyimm = op->anyoffset;
                    if (pd->bits == SS || pd->bits == WS)
                        {
                            if (datasize > 1 && (constsize & 2) != 0 &&
                                    ((immediate >= -128 && immediate < 128) || op->anyoffset != 0))
                                {
                                    immsize = 1;
                                    tcode[i] |= 0x02;
                                }
                            else
                                {
                                    immsize = datasize;
                                }
                            tmask[i] |= 0x02;
                        }
                    else
                        {
                            immsize = datasize;
                        }
                    break;
                }
                case IMX:                        // Immediate sign-extendable byte
                case IMS:                        // Immediate byte (for shifts)
                case IM1:                        // Immediate byte
                {
                    if (immsize == 2)              // To accomodate ENTER instruction
                        {
                            immediate = (immediate & 0xFFFF) | (op->offset << 16);
                        }
                    else
                        {
                            immediate = op->offset;
                        }
                    anyimm |= op->anyoffset;
                    immsize++;
                    break;
                }
                case IM2:                        // Immediate word (ENTER/RET)
                {
                    immediate = op->offset;
                    anyimm = op->anyoffset;
                    immsize = 2;
                    break;
                }
                case IMA:                        // Immediate absolute near data address
                {
                    if (op->segment != SEG_UNDEF && op->segment != SEG_DS)
                        {
                            segment = op->segment;
                        }
                    displacement = op->offset;
                    anydisp = op->anyoffset;
                    dispsize = 4;
                    break;
                }
                case JOB:                        // Immediate byte offset (for jumps)
                {
                    jmpoffset = op->offset;
                    anyjmp = op->anyoffset;
                    jmpsize = 1;
                    break;
                }
                case JOW:                        // Immediate full offset (for jumps)
                {
                    jmpoffset = op->offset;
                    anyjmp = op->anyoffset;
                    jmpsize = 4;
                    break;
                }
                case JMF:                        // Immediate absolute far jump/call addr
                {
                    displacement = op->offset;
                    anydisp = op->anyoffset;
                    dispsize = 4;
                    immediate = op->segment;
                    anyimm = op->anyoffset;
                    immsize = 2;
                    break;
                }
                case SGM:                        // Segment register in ModRM byte
                {
                    hasrm = 1;
                    if (op->index < 6)
                        {
                            tcode[i + 1] |= (char)(op->index << 3);
                            tmask[i + 1] |= 0x38;
                        }
                    break;
                }
                case SCM:                        // Segment register in command byte
                {
                    if (op->index == SEG_FS || op->index == SEG_GS)
                        {
                            tcode[0] = (char)(0x0F);
                            tmask[0] = (char)(0xFF);
                            i = 1;
                            if (strcmp(name, ("PUSH")) == 0)
                                {
                                    tcode[i] = (char)((op->index << 3) | 0x80);
                                }
                            else
                                {
                                    tcode[i] = (char)((op->index << 3) | 0x81);
                                }
                            tmask[i] = (char)(0xFF);
                        }
                    else if (op->index < 6)
                        {
                            if (op->index == SEG_CS && strcmp(name, ("POP")) == 0)
                                {
                                    strcpy(errtext, ("Unable to POP CS"));
                                    goto error;
                                }
                            tcode[i] = (char)((tcode[i] & 0xC7) | (op->index << 3));
                        }
                    else
                        {
                            tcode[i] &= 0xC7;
                            tmask[i] &= 0xC7;
                        }
                    break;
                }
                case PRN:                        // Near return address (pseudooperand)
                case PRF:                        // Far return address (pseudooperand)
                case PAC:                        // Accumulator (AL/AX/EAX, pseudooperand)
                case PAH:                        // AH (in LAHF/SAHF, pseudooperand)
                case PFL:                        // Lower byte of flags (pseudooperand)
                case PS0:                        // Top of FPU stack (pseudooperand)
                case PS1:                        // ST(1) (pseudooperand)
                case PCX:                        // CX/ECX (pseudooperand)
                case PDI:                        // EDI (pseudooperand in MMX extentions)
                {
                    break;                         // Simply skip preudooperands
                }
                default:                         // Undefined type of operand
                {
                    strcpy(errtext, ("Internal Assembler error"));
                    goto error;
                }
                }
        }

    // Gather parts of command together in the complete command.
    j = 0;
    if (lock != 0)
        {
            // Lock prefix specified
            model->code[j] = (char)0xF0;
            model->mask[j] = (char)0xFF;
            j++;
        }
    if (datasize == 2 && pd->bits != FF)
        {
            // Data size prefix necessary
            model->code[j] = (char)0x66;
            model->mask[j] = (char)0xFF;
            j++;
        }
    if (addrsize == 2)
        {
            // Address size prefix necessary
            model->code[j] = (char)0x67;
            model->mask[j] = (char)0xFF;
            j++;
        }
    if (segment != SEG_UNDEF)
        {
            // Segment prefix necessary
            if (segment == SEG_ES)
                {
                    model->code[j] = 0x26;
                }
            else if (segment == SEG_CS)
                {
                    model->code[j] = 0x2E;
                }
            else if (segment == SEG_SS)
                {
                    model->code[j] = 0x36;
                }
            else if (segment == SEG_DS)
                {
                    model->code[j] = 0x3E;
                }
            else if (segment == SEG_FS)
                {
                    model->code[j] = 0x64;
                }
            else if (segment == SEG_GS)
                {
                    model->code[j] = 0x65;
                }
            else
                {
                    strcpy(errtext, ("Internal Assembler error"));
                    goto error;
                }
            model->mask[j] = (char)0xFF;
            j++;
        }
    if (dispsize > 0)
        {
            memcpy(tcode + i + 1 + hasrm + hassib, &displacement, dispsize);
            if (anydisp == 0)
                {
                    memset(tmask + i + 1 + hasrm + hassib, 0xFF, dispsize);
                }
        }
    if (immsize > 0)
        {
            if (immsize == 1)
                {
                    l = 0xFFFFFF00L;
                }
            else if (immsize == 2)
                {
                    l = 0xFFFF0000L;
                }
            else
                {
                    l = 0L;
                }
            if ((immediate & l) != 0 && (immediate & l) != l)
                {
                    strcpy(errtext, ("Constant does not fit into operand"));
                    goto error;
                }

            memcpy(tcode + i + 1 + hasrm + hassib + dispsize, &immediate, immsize);
            if (anyimm == 0)
                {
                    memset(tmask + i + 1 + hasrm + hassib + dispsize, 0xFF, immsize);
                }
        }

    //
    i = i + 1 + hasrm + hassib + dispsize + immsize;
    jmpoffset = jmpoffset - (i + j + jmpsize);
    model->jmpsize = jmpsize;
    model->jmpoffset = jmpoffset;
    model->jmppos = i + j;

    //
    if (jmpsize != 0)
        {
            if (ip != 0)
                {
                    jmpoffset = jmpoffset - ip;
                    if (jmpsize == 1 && anyjmp == 0 && (jmpoffset < -128 || jmpoffset >= 128))
                        {
                            if (longjump == 0 && (jmpmode & 0x03) == 0)
                                {
                                    longjump = 1;
                                    goto retrylongjump;
                                }
                            sprintf(errtext, ("Relative jump out of range, use %s LONG form"), name);
                            goto error;
                        }
                    memcpy(tcode + i, &jmpoffset, jmpsize);
                }
            if (anyjmp == 0)
                {
                    memset(tmask + i, 0xFF, jmpsize);
                }
            i += jmpsize;
        }

    //
    memcpy(model->code + j, tcode, i);
    memcpy(model->mask + j, tmask, i);
    i += j;
    model->length = i;
    return i;                            // Positive value: length of code


error:
    model->length = 0;
    return cmd - m_pAsmCmd;                 // Negative value: position of error
}
