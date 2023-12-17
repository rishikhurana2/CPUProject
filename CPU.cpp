#include "CPU.h"

instruction::instruction(bitset<32> fetch)
{
	//cout << fetch << endl;
	instr = fetch;
	//cout << instr << endl;
}

CPU::CPU() {
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM 
	{
		dmemory[i] = (0);
	}
	
	//Initializing the R-Type instructions by Opcodes, Funct3, Funct7
	ADD[0] = bitset<7> (string("0110011")); ADD[1] = bitset<7> (string("0000000")); ADD[2] = bitset<7> (string("0000000"));
	SRA[0] = bitset<7> (string("0110011")); SRA[1] = bitset<7> (string("0000101")); SRA[2] = bitset<7> (string("0100000"));
	XOR[0] = bitset<7> (string("0110011")); XOR[1] = bitset<7> (string("0000100")); XOR[2] = bitset<7> (string("0000000"));
	SUB[0] = bitset<7> (string("0110011")); SUB[1] = bitset<7> (string("0000000")); SUB[2] = bitset<7> (string("0100000"));

	//Initializing the I-Type instructions by Opcodes, Funct3
	ADDI[0] = bitset<7> (string("0010011")); ADDI[1] = bitset<7> (string("0000000"));
	ANDI[0] = bitset<7> (string("0010011")); ANDI[1] = bitset<7> (string("0000111"));
	JALR[0] = bitset<7> (string("1100111")); JALR[1] = bitset<7> (string("0000000"));
	LW[0] = bitset<7> (string("0000011")); LW[1] = bitset<7> (string("0000010"));

	//Intializing the B-Type instructiuons by Opcodes, Funct3
	BLT[0] = bitset<7> (string("1100011")); BLT[1] = bitset<7> (string("0000100"));

	//Intializing the S-Type instructions by Opcodes, Funct3
	SW[0] = bitset<7> (string("0100011")); SW[1] = bitset<7> (string("0000010"));

	//Grouping the R-Types (Opcodes)
	n_RTypes = 4;
	R_Types[0] = ADD[0]; R_Types[1] = SRA[0]; R_Types[2] = XOR[0]; R_Types[3] = SUB[0];

	//Grouping the I-Types (Opcodes)
	n_ITypes = 4;
	I_Types[0] = ADDI[0]; I_Types[1] = ANDI[0]; I_Types[2] = JALR[0]; I_Types[3] = LW[0];

	//Grouping the B-Types
	n_BTypes = 1;
	B_Types[0] = BLT[0];

	//Grouping the S-Types
	n_STypes = 1;
	S_Types[0] = SW[0];

	//initializing registers
	for (int i = 0; i < 32; i++) {
		registers[i] = 0;
	}
}

bitset<32> CPU::Fetch(bitset<8> *instmem) {
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
	PC += 4;//increment PC
	return instr;
}


bool CPU::Decode(instruction* curr) {
	bitset<7> opcode = getOpcode(curr);
	string type = getType(opcode);

	//Procedure for R-Types
	if (type == "R-Type") {
		bitset<7> funct7 = getFunct7(curr);
		bitset<3> funct3 = getFunct3(curr);
		bitset<5> rs1 = getR1(curr);
		bitset<5> rs2 = getR2(curr);
		bitset<5> rd = getRD(curr);
		return RTypeExecution(funct7, rs1, rs2, funct3, rd, opcode);
	}

	//Procedure for I-Types
	if (type == "I-Type") {
		bitset<12> imm = getImm12(curr);
		bitset<5> rs1 = getR1(curr);
		bitset<3> funct3 = getFunct3(curr);
		bitset<5> rd = getRD(curr);
		return ITypeExecution(imm, rs1, funct3, rd, opcode);
	}

	//Procedure for B-Types
	if (type == "B-Type") {
		bitset<13> imm = getImm13BType(curr);
		bitset<5> rs1 = getR1(curr);
		bitset<5> rs2 = getR2(curr);
		bitset<3> funct3 = getFunct3(curr);
		return BTypeExecution(imm, rs2, rs1, funct3, opcode);
	}
	//Procedure for S-Types
	if (type == "S-Type") {
		bitset<12> imm = getImm12SType(curr);
		bitset<5> rs1 = getR1(curr);
		bitset<5> rs2 = getR2(curr);
		bitset<3> funct3 = getFunct3(curr);
		return STypeExecution(imm, rs2, rs1, funct3, opcode);
	}
	return false;
}

unsigned long CPU::readPC() {
	return PC;
}

bitset<7> CPU::getOpcode(instruction* instr) {
	bitset<7> opcode;
	for (int i = 0; i < 7; i++) {
		opcode[i] = instr->instr[i];
	}
	return opcode;
}


string CPU::getType(bitset<7> opcode) {
	 for (int i = 0; i < n_RTypes; i++) {
		if (opcode == R_Types[i]) {
			return "R-Type";
		}
	 }
	 for (int i = 0; i < n_ITypes; i++) {
		if (opcode == I_Types[i]) {
			return "I-Type";
		}
	 }
	 for (int i = 0; i < n_BTypes; i++) {
		if (opcode == B_Types[i]) {
			return "B-Type";
		}
	 }
	 for (int i = 0; i < n_STypes; i++) {
		if (opcode == S_Types[i]) {
			return "S-Type";
		}
	 }
	 return "";
}

bitset<5> CPU::getRD(instruction * instr) {
	bitset<5> rd;
	for (int i = 7; i <= 11; i++) {
		rd[i - 7] = instr->instr[i];
	}
	return rd;
}
bitset<5> CPU::getR1(instruction * instr) {
	bitset<5> r1;
	for (int i = 15; i <= 19; i++) {
		r1[i - 15] = instr->instr[i];
	}
	return r1;
}

bitset<5> CPU::getR2(instruction * instr) {
	bitset<5> r2;
	for (int i = 20; i <= 24; i++) {
		r2[i - 20] = instr->instr[i];
	}
	return r2;
}

bitset<3> CPU::getFunct3(instruction * instr) {
	bitset<3> funct3;
	for (int i = 12; i <= 14; i++) {
		funct3[i - 12] = instr->instr[i];
	}
	return funct3;
}

bitset<7> CPU::getFunct7(instruction * instr) {
	bitset<7> funct7;
	for (int i = 25; i <= 31; i++) {
		funct7[i - 25] = instr->instr[i];
	}
	return funct7;
}

bitset<12> CPU::getImm12(instruction * instr) {
	bitset<12> imm;
	for (int i = 20; i <= 31; i++) {
		imm[i - 20] = instr->instr[i];
	}
	return imm;
}

bitset<13> CPU::getImm13BType(instruction* instr) {
	bitset<13> imm;
	imm[0] = 0;
	for (int i = 8; i <= 11; i++) {
		imm[i - 7] = instr->instr[i];
	}
	for (int i = 25; i <= 30; i++) {
		imm[i - 20] = instr->instr[i];
	}
	imm[11] = instr->instr[7];
	imm[12] = instr->instr[31];
	return imm;
}

bitset<12> CPU::getImm12SType(instruction* instr) {
	bitset<12> imm;
	for (int i = 7; i <= 11; i++) {
		imm[i - 7] = instr->instr[i];
	}
	for (int i = 25; i <= 31; i++) {
		imm[i - 20] = instr->instr[i];
	}

	return imm;
}

bool CPU::RTypeExecution(bitset<7> funct7, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<5> rd, bitset<7> opcode) {
	//Check for ADD
	if (opcode == ADD[0] && funct3 == getFirstThreeBits(ADD[1]) && funct7 == ADD[2]) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] + registers[(int)(rs2.to_ulong())];
		return true;
	}
	//Check for SUB
	if (opcode == SUB[0] && funct3 == getFirstThreeBits(SUB[1]) && funct7 == SUB[2]) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] - registers[(int)(rs2.to_ulong())];
		return true;
	}
	//Check for XOR
	if (opcode == XOR[0] && funct3 == getFirstThreeBits(XOR[1]) && funct7 == XOR[2]) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] ^ registers[(int)(rs2.to_ulong())];
		return true;
	}
	//Check for SRA
	if (opcode == SRA[0] && funct3 == getFirstThreeBits(SRA[1]) && funct7 == SRA[2]) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] << registers[(int)(rs2.to_ulong())];
		return true;
	}
	return false;
}

bool CPU::ITypeExecution(bitset<12> imm, bitset<5> rs1, bitset<3> funct3, bitset<5> rd, bitset<7> opcode) {
	//Check for ADDI
	if (opcode == ADDI[0] && funct3 == getFirstThreeBits(ADDI[1])) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] + getTwosComp12(imm);
		return true;
	}
	//Check for ANDI
	if (opcode == ANDI[0] && funct3 == getFirstThreeBits(ANDI[1])) {
		registers[(int)(rd.to_ulong())] = registers[(int)(rs1.to_ulong())] & getTwosComp12(imm);
		return true;
	}
	//Check for JALR
	if (opcode == JALR[0] && funct3 == getFirstThreeBits(JALR[1])) {
		registers[(int)(rd.to_ulong())] = PC;
		PC = registers[(int)(rs1.to_ulong())] + getTwosComp12(imm);
		return true;
	}
	//Check for LW
	if (opcode == LW[0] && funct3 == getFirstThreeBits(LW[1])) {
		int start_index = (registers[(int)(rs1.to_ulong())] + getTwosComp12(imm))*8;
		int end_index = start_index + 32;
		bitset<32> lw;
		for (int i = start_index; i < end_index; i++) {
			lw[i - start_index] = dmemory[i];
		}
		registers[(int)(rd.to_ulong())] = getTwosComp32(lw);
		return true;
	}
	return false;
}

bool CPU::BTypeExecution(bitset<13> imm, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<7> opcode) {
	//Check for BLT
	if (opcode == BLT[0] && funct3 == getFirstThreeBits(BLT[1])) {
		if (registers[(int)rs1.to_ulong()] < registers[(int)rs2.to_ulong()]) {
			PC = (PC - 4) + getTwosComp13(imm);
		}
		return true;
	}
	return false;
}

bool CPU::STypeExecution(bitset<12> imm, bitset<5> rs2, bitset<5> rs1, bitset<3> funct3, bitset<7> opcode) {
	//Check for SW
	if (opcode == SW[0] && funct3 == getFirstThreeBits(SW[1])) {
		bitset<32> sw (registers[(int)(rs2.to_ulong())]);
		int start_index = (registers[(int)(rs1.to_ulong())] + getTwosComp12(imm))*8;
		int end_index = start_index + 32;
		for (int i = start_index; i < end_index; i++) {
			dmemory[i] = sw[i - start_index];
		}
		return true;
	}

	return false;
}



bitset<3> CPU::getFirstThreeBits(bitset<7> in7) {
	bitset<3> out;
	for (int i = 0; i < 3; i++) {
		out[i] = in7[i];
	}
	return out;
}

int CPU::getTwosComp12(bitset<12> in12) {
	if (in12[11] == 1) {
		return -((int)(in12.flip().to_ulong())  + 1);
	} else {
		return (int) (in12.to_ulong());
	}
}

int CPU::getTwosComp13(bitset<13> in13) {
	if (in13[12] == 1) {
		return -((int)(in13.flip().to_ulong())  + 1);
	} else {
		return (int) (in13.to_ulong());
	}
}

int CPU::getTwosComp32(bitset<32> in32) {
	if (in32[31] == 1) {
		return -((int)(in32.flip().to_ulong())  + 1);
	} else {
		return (int) (in32.to_ulong());
	}
}