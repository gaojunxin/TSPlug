//-------------------------------------------------------------------------------------------------------------------------
//
#pragma once



//-------------------------------------------------------------------------------------------------------------------------
//

//
#define REG_EAX        0               // Indexes of general-purpose registers
#define REG_ECX        1               // in t_reg.
#define REG_EDX        2
#define REG_EBX        3
#define REG_ESP        4
#define REG_EBP        5
#define REG_ESI        6
#define REG_EDI        7


//
#define SEG_UNDEF     -1
#define SEG_ES         0               // Indexes of segment/selector registers
#define SEG_CS         1
#define SEG_SS         2
#define SEG_DS         3
#define SEG_FS         4
#define SEG_GS         5




//
#define   C_TYPEMASK	0xF0            // Mask for command type
#define   C_CMD			0x00            // Ordinary instruction
#define   C_PSH			0x10            // 1-word PUSH instruction
#define   C_POP			0x20            // 1-word POP instruction
#define   C_MMX			0x30            // MMX instruction
#define   C_FLT			0x40            // FPU instruction
#define   C_JMP			0x50            // JUMP instruction
#define   C_JMC			0x60            // Conditional JUMP instruction
#define   C_CAL			0x70            // CALL instruction
#define   C_RET			0x80            // RET instruction
#define   C_FLG			0x90            // Changes system flags
#define   C_RTF			0xA0            // C_JMP and C_FLG simultaneously
#define   C_REP			0xB0            // Instruction with REPxx prefix
#define   C_PRI			0xC0            // Privileged instruction
#define   C_DAT			0xD0            // Data (address) doubleword
#define   C_NOW			0xE0            // 3DNow! instruction
#define   C_BAD			0xF0            // Unrecognized command
#define   C_RARE		0x08            // Rare command, seldom used in programs
#define   C_SIZEMASK	0x07            // MMX data size or special flag
#define   C_EXPL		0x01            // (non-MMX) Specify explicit memory size




//
#define   DEC_TYPEMASK		0x1F            // Type of memory byte
#define   DEC_UNKNOWN		0x00            // Unknown type
#define   DEC_BYTE			0x01            // Accessed as byte
#define   DEC_WORD			0x02            // Accessed as short
#define   DEC_NEXTDATA		0x03            // Subsequent byte of code or data
#define   DEC_DWORD			0x04            // Accessed as long
#define   DEC_FLOAT4		0x05            // Accessed as float
#define   DEC_FWORD			0x06            // Accessed as descriptor/long pointer
#define   DEC_FLOAT8		0x07            // Accessed as double
#define   DEC_QWORD			0x08            // Accessed as 8-byte integer
#define   DEC_FLOAT10		0x09            // Accessed as long double
#define   DEC_TBYTE			0x0A            // Accessed as 10-byte integer
#define   DEC_STRING		0x0B            // Zero-terminated ASCII string
#define   DEC_UNICODE		0x0C            // Zero-terminated UNICODE string
#define   DEC_3DNOW			0x0D            // Accessed as 3Dnow operand
#define   DEC_BYTESW		0x11            // Accessed as byte index to switch
#define   DEC_NEXTCODE		0x13            // Subsequent byte of command
#define   DEC_COMMAND		0x1D            // First byte of command
#define   DEC_JMPDEST		0x1E            // Jump destination
#define   DEC_CALLDEST		0x1F            // Call (and maybe jump) destination
#define   DEC_PROCMASK		0x60            // Procedure analysis
#define   DEC_PROC			0x20            // Start of procedure
#define   DEC_PBODY			0x40            // Body of procedure
#define   DEC_PEND			0x60            // End of procedure
#define   DEC_CHECKED		0x80            // Byte was analysed


// Errors detected during command disassembling.
#define DAE_NOERR      0               // No error
#define DAE_BADCMD     1               // Unrecognized command
#define DAE_CROSS      2               // Command crosses end of memory block
#define DAE_BADSEG     3               // Undefined segment register
#define DAE_MEMORY     4               // Register where only memory allowed
#define DAE_REGISTER   5               // Memory where only register allowed
#define DAE_INTERN     6               // Internal error




// Warnings issued by Disasm():
#define DAW_FARADDR    0x0001          // Command is a far jump, call or return
#define DAW_SEGMENT    0x0002          // Command loads segment register
#define DAW_PRIV       0x0004          // Privileged command
#define DAW_IO         0x0008          // I/O command
#define DAW_SHIFT      0x0010          // Shift constant out of range 1..31
#define DAW_PREFIX     0x0020          // Superfluous prefix
#define DAW_LOCK       0x0040          // Command has LOCK prefix
#define DAW_STACK      0x0080          // Unaligned stack operation
#define DAW_DANGER95   0x1000          // May mess up Win95 if executed
#define DAW_DANGEROUS  0x3000          // May mess up any OS if executed





// Special command features.
#define WW             0x01            // Bit W (size of operand)
#define SS             0x02            // Bit S (sign extention of immediate)
#define WS             0x03            // Bits W and S
#define W3             0x08            // Bit W at position 3
#define CC             0x10            // Conditional jump
#define FF             0x20            // Forced 16-bit size
#define LL             0x40            // Conditional loop
#define PR             0x80            // Protected command
#define WP             0x81            // I/O command with bit W

// All possible types of operands in 80x86. A bit more than you expected, he?
#define NNN            0               // No operand
#define REG            1               // Integer register in Reg field
#define RCM            2               // Integer register in command byte
#define RG4            3               // Integer 4-byte register in Reg field
#define RAC            4               // Accumulator (AL/AX/EAX, implicit)
#define RAX            5               // AX (2-byte, implicit)
#define RDX            6               // DX (16-bit implicit port address)
#define RCL            7               // Implicit CL register (for shifts)
#define RS0            8               // Top of FPU stack (ST(0), implicit)
#define RST            9               // FPU register (ST(i)) in command byte
#define RMX            10              // MMX register MMx
#define R3D            11              // 3DNow! register MMx
#define MRG            12              // Memory/register in ModRM byte
#define MR1            13              // 1-byte memory/register in ModRM byte
#define MR2            14              // 2-byte memory/register in ModRM byte
#define MR4            15              // 4-byte memory/register in ModRM byte
#define RR4            16              // 4-byte memory/register (register only)
#define MR8            17              // 8-byte memory/MMX register in ModRM
#define RR8            18              // 8-byte MMX register only in ModRM
#define MRD            19              // 8-byte memory/3DNow! register in ModRM
#define RRD            20              // 8-byte memory/3DNow! (register only)
#define MRJ            21              // Memory/reg in ModRM as JUMP target
#define MMA            22              // Memory address in ModRM byte for LEA
#define MML            23              // Memory in ModRM byte (for LES)
#define MMS            24              // Memory in ModRM byte (as SEG:OFFS)
#define MM6            25              // Memory in ModRm (6-byte descriptor)
#define MMB            26              // Two adjacent memory locations (BOUND)
#define MD2            27              // Memory in ModRM (16-bit integer)
#define MB2            28              // Memory in ModRM (16-bit binary)
#define MD4            29              // Memory in ModRM byte (32-bit integer)
#define MD8            30              // Memory in ModRM byte (64-bit integer)
#define MDA            31              // Memory in ModRM byte (80-bit BCD)
#define MF4            32              // Memory in ModRM byte (32-bit float)
#define MF8            33              // Memory in ModRM byte (64-bit float)
#define MFA            34              // Memory in ModRM byte (80-bit float)
#define MFE            35              // Memory in ModRM byte (FPU environment)
#define MFS            36              // Memory in ModRM byte (FPU state)
#define MFX            37              // Memory in ModRM byte (ext. FPU state)
#define MSO            38              // Source in string op's ([ESI])
#define MDE            39              // Destination in string op's ([EDI])
#define MXL            40              // XLAT operand ([EBX+AL])
#define IMM            41              // Immediate data (8 or 16/32)
#define IMU            42              // Immediate unsigned data (8 or 16/32)
#define VXD            43              // VxD service
#define IMX            44              // Immediate sign-extendable byte
#define C01            45              // Implicit constant 1 (for shifts)
#define IMS            46              // Immediate byte (for shifts)
#define IM1            47              // Immediate byte
#define IM2            48              // Immediate word (ENTER/RET)
#define IMA            49              // Immediate absolute near data address
#define JOB            50              // Immediate byte offset (for jumps)
#define JOW            51              // Immediate full offset (for jumps)
#define JMF            52              // Immediate absolute far jump/call addr
#define SGM            53              // Segment register in ModRM byte
#define SCM            54              // Segment register in command byte
#define CRX            55              // Control register CRx
#define DRX            56              // Debug register DRx

// Pseudooperands (implicit operands, never appear in assembler commands). Must
// have index equal to or exceeding PSEUDOOP.
#define PRN            (PSEUDOOP+0)    // Near return address
#define PRF            (PSEUDOOP+1)    // Far return address
#define PAC            (PSEUDOOP+2)    // Accumulator (AL/AX/EAX)
#define PAH            (PSEUDOOP+3)    // AH (in LAHF/SAHF commands)
#define PFL            (PSEUDOOP+4)    // Lower byte of flags (in LAHF/SAHF)
#define PS0            (PSEUDOOP+5)    // Top of FPU stack (ST(0))
#define PS1            (PSEUDOOP+6)    // ST(1)
#define PCX            (PSEUDOOP+7)    // CX/ECX
#define PDI            (PSEUDOOP+8)    // EDI (in MMX extentions)





//
#define   C_DANGER95     0x01            // Command is dangerous under Win95/98
#define   C_DANGER       0x03            // Command is dangerous everywhere
#define   C_DANGERLOCK   0x07            // Dangerous with LOCK prefix






////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ASSEMBLER FUNCTIONS //////////////////////////////

// Scanner modes.
#define SA_NAME        0x0001          // Don't try to decode labels
#define SA_IMPORT      0x0002          // Allow import pseudolabel


// Types of input tokens reported by scanner.
#define SCAN_EOL       0               // End of line
#define SCAN_REG8      1               // 8-bit register
#define SCAN_REG16     2               // 16-bit register
#define SCAN_REG32     3               // 32-bit register
#define SCAN_SEG       4               // Segment register 段寄存器
#define SCAN_FPU       5               // FPU register FPU寄存器
#define SCAN_MMX       6               // MMX register 扩展寄存器
#define SCAN_CR        7               // Control register
#define SCAN_DR        8               // Debug register
#define SCAN_OPSIZE    9               // Operand size modifier
#define SCAN_JMPSIZE   10              // Jump size modifier 跳转大小
#define SCAN_LOCAL     11              // Address on stack in form LOCAL.decimal 在堆栈中的地址的形式LOCAL.decimal
#define SCAN_ARG       12              // Address on stack in form ARG.decimal 在堆栈中的地址的形式ARG.decimal
#define SCAN_PTR       20              // PTR in MASM addressing statements PTR在MASM解决报表
#define SCAN_REP       21              // REP prefix REP前缀
#define SCAN_REPE      22              // REPE prefix REPE前缀
#define SCAN_REPNE     23              // REPNE prefix REPNE前缀
#define SCAN_LOCK      24              // LOCK prefix LOCK前缀
#define SCAN_NAME      25              // Command or label 命令或标签
#define SCAN_ICONST    26              // Hexadecimal constant 十六进制常量
#define SCAN_DCONST    27              // Decimal constant 十进制常数
#define SCAN_OFS       28              // Undefined constant 未定义的常量
#define SCAN_FCONST    29              // Floating-point constant 浮点常量
#define SCAN_EIP       30              // Register EIP 注册EIP
#define SCAN_SIGNED    31              // Keyword "SIGNED" (in expressions)
#define SCAN_UNSIGNED  32              // Keyword "UNSIGNED" (in expressions)
#define SCAN_CHAR      33              // Keyword "CHAR" (in expressions)
#define SCAN_FLOAT     34              // Keyword "FLOAT" (in expressions)
#define SCAN_DOUBLE    35              // Keyword "DOUBLE" (in expressions)
#define SCAN_FLOAT10   36              // Keyword "FLOAT10" (in expressions)
#define SCAN_STRING    37              // Keyword "STRING" (in expressions)
#define SCAN_UNICODE   38              // Keyword "UNICODE" (in expressions)
#define SCAN_MSG       39              // Pseudovariable MSG (in expressions)

#define SCAN_SYMB      64              // Any other character 任何其他字符
#define SCAN_IMPORT    65              // Import pseudolabel
#define SCAN_ERR       255             // Definitely bad item //错误的信息


// Definition used by Assembler to report command matching errors.
// 错误信息定义
#define MA_JMP         0x0001          // Invalid jump size modifier
#define MA_NOP         0x0002          // Wrong number of operands
#define MA_TYP         0x0004          // Bad operand type  错误的操作数类型
#define MA_NOS         0x0008          // Explicit operand size expected
#define MA_SIZ         0x0010          // Bad operand size	错误的操作数大小
#define MA_DIF         0x0020          // Different operand sizes	不同的操作数大小
#define MA_SEG         0x0040          // Invalid segment register	无效的段寄存器
#define MA_RNG         0x0080          // Constant out of expected range   常量超出预期的范围


