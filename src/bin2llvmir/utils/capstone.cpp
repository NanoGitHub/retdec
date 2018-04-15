/**
* @file src/bin2llvmir/utils/capstone.cpp
* @brief Capstone utilities.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#include "retdec/bin2llvmir/utils/capstone.h"

namespace retdec {
namespace bin2llvmir {
namespace capstone_utils {

bool isNopInstruction(const config::Architecture& arch, cs_insn* insn)
{
	if (arch.isX86())
	{
		return isNopInstruction_x86(insn);
	}
	else if (arch.isMipsOrPic32())
	{
		return isNopInstruction_mips(insn);
	}
	else if (arch.isArmOrThumb())
	{
		return isNopInstruction_arm(insn);
	}
	else if (arch.isPpc())
	{
		return isNopInstruction_ppc(insn);
	}
	else
	{
		assert(false);
		return false;
	}
}

bool isNopInstruction_x86(cs_insn* insn)
{
	cs_x86& insn86 = insn->detail->x86;

	// True NOP variants.
	//
	if (insn->id == X86_INS_NOP
			|| insn->id == X86_INS_FNOP
			|| insn->id == X86_INS_FDISI8087_NOP
			|| insn->id == X86_INS_FENI8087_NOP
			|| insn->id == X86_INS_INT3)
	{
		return true;
	}
	// e.g. lea esi, [esi]
	//
	else if (insn->id == X86_INS_LEA
			&& insn86.disp == 0
			&& insn86.op_count == 2
			&& insn86.operands[0].type == X86_OP_REG
			&& insn86.operands[1].type == X86_OP_MEM
			&& insn86.operands[1].mem.segment == X86_REG_INVALID
			&& insn86.operands[1].mem.index == X86_REG_INVALID
			&& insn86.operands[1].mem.scale == 1
			&& insn86.operands[1].mem.disp == 0
			&& insn86.operands[1].mem.base == insn86.operands[0].reg)
	{
		return true;
	}
	// e.g. mov esi. esi
	//
	else if (insn->id == X86_INS_MOV
			&& insn86.disp == 0
			&& insn86.op_count == 2
			&& insn86.operands[0].type == X86_OP_REG
			&& insn86.operands[1].type == X86_OP_REG
			&& insn86.operands[0].reg == insn86.operands[1].reg)
	{
		return true;
	}

	return false;
}

bool isNopInstruction_mips(cs_insn* insn)
{
	cs_mips& insnMips = insn->detail->mips;

	// True NOP variants.
	//
	if (insn->id == MIPS_INS_NOP
			|| insn->id == MIPS_INS_SSNOP)
	{
		return true;
	}

	return false;
}

bool isNopInstruction_arm(cs_insn* insn)
{
	cs_arm& insnArm = insn->detail->arm;

	// True NOP variants.
	//
	if (insn->id == ARM_INS_NOP)
	{
		return true;
	}

	return false;
}

bool isNopInstruction_ppc(cs_insn* insn)
{
	cs_ppc& insnPpc = insn->detail->ppc;

	// True NOP variants.
	//
	if (insn->id == PPC_INS_NOP
			|| insn->id == PPC_INS_XNOP)
	{
		return true;
	}

	return false;
}

} // namespace capstone_utils
} // namespace bin2llvmir
} // namespace retdec
