#include <iostream>
//#include <ccstint>

using namespace std;

typedef union 
{
    int32_t value;
    struct 
    {
        int32_t imm: 18;
		int32_t src : 4;
		int32_t dst : 4;
		int32_t opCode : 6;
    };
} INSTR4_4_18;

typedef union 
{
    int32_t value;
    struct 
    {
        int32_t imm: 21;
		int32_t dst : 5;
		int32_t opCode : 6;
	};
} INSTR5_21;

typedef union 
{
    int32_t value;
    struct 
    {
        int32_t imm: 24;
		int32_t type : 2;
		int32_t opCode : 6;
    };
} INSTR2_24;

typedef union 
{
    int32_t value;
    struct 
    {
		int32_t nu : 13;
		int32_t imm : 5;
		int32_t src : 4;
		int32_t dst : 4;
        int32_t opCode: 6;
    };
} INSTR4_4_5_13;

typedef union 
{
    int32_t value;
    struct 
    {
        int32_t nu: 22;
		int32_t src : 4;
		int32_t opCode : 6;
    };
} INSTR4_22;

typedef union 
{
    int32_t value;
    struct 
    {
        int32_t nu: 5;
		int32_t c : 16;
		int32_t hl : 1;
		int32_t dst : 4;
		int32_t opCode : 6;
    };
} INSTR4_1_16_5;