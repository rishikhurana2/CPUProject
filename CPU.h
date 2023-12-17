#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
using namespace std;


class instruction {
public:
	bitset<32> instr;//instruction
	instruction(bitset<32> fetch); // constructor

};

class CPU {
private:
	int dmemory[4096]; //data memory byte addressable in little endian fashion;
	unsigned long PC; //pc

	//Instructions:
	//R-Type
	bitset<7> ADD [3];
	bitset<7> SUB [3];
	bitset<7> XOR [3];
	bitset<7> SRA [3];

	//I-Type
	bitset<7> ANDI [2];
	bitset<7> ADDI [2];
	bitset<7> LW [2];
	bitset<7> JALR [2];

	//S-Type
	bitset<7> SW[2];

	//B-Type
	bitset<7> BLT[2];

	//R-Type groupings
	int n_RTypes;
	bitset<7> R_Types [4];

	//I-Type groupings
	int n_ITypes;
	bitset<7> I_Types [4];

	//B-Type groupings
	int n_BTypes;
	bitset<7> B_Types [1];

	//S-Type groupings
	int n_STypes;
	bitset<7> S_Types [1];

public:

	CPU();
	unsigned long readPC();
	bitset<32> Fetch(bitset<8> *instmem);
	bool Decode(instruction* instr);
	
	//Registers
	int registers [32];

	//retrieve the opcode of a given instruction
	bitset<7> getOpcode(instruction * instr);
	
	//getters for retrieving parts of instruction for R-Type, I-Type, B-Type, etc.
	bitset<7> getFunct7(instruction * instr);
	bitset<3> getFunct3(instruction * instr);
	bitset<5> getRD(instruction * instr);
	bitset<5> getR1(instruction * instr);
	bitset<5> getR2(instruction * instr);
	bitset<12> getImm12(instruction *instr);
	bitset<13> getImm13BType(instruction *instr);
	bitset<12> getImm12SType(instruction *instr);

	//get the type (I-Type, R-Type, etc.) of the current instruction
	string getType(bitset<7> opcode);

	//Execute instructions
	bool RTypeExecution(bitset<7> funct7, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<5> rd, bitset<7> opcode);
	bool ITypeExecution(bitset<12> imm, bitset<5> rs1, bitset<3> funct3, bitset<5> rd, bitset<7> opcode);
	bool BTypeExecution(bitset<13> imm, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<7> opcode);
	bool STypeExecution(bitset<12> imm, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<7> opcode);

	//helper functions
	bitset<3> getFirstThreeBits(bitset<7> in7);
	int getTwosComp12(bitset<12> in12);
	int getTwosComp13(bitset<13> in13);
	int getTwosComp32(bitset<32> in32);
};
