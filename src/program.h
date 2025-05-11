#ifndef _program_h_
#define _program_h_

#include <stdint.h>

#include <sds.h>

#define N_INSTRUCTIONS 1024
#define MAX_LABELS     1024
#define LABEL_FIRST    N_INSTRUCTIONS
#define LABEL_NEXT     (N_INSTRUCTIONS + MAX_LABELS)
#define LABEL_INVALID  (LABEL_NEXT + 1)

#define NOP  154
#define NOP2 186
#define ATTR_BREAK 0x8000
#define ATTR_CARRY 0x4000
#define ATTR_CUSED 0x2000

#define CARRY_UNUSED     0
#define CARRY_INDEFINITE 4
#define CARRY_VALUE_0    6
#define CARRY_VALUE_1    7

enum control_type {
	CT_NONE,
	CT_IF,
	CT_GOTO,
	CT_INPUT,
};

enum control_if_flags {
	CF_NONE,
	CF_CO3,
	CF_CO2,
	CF_CO1,
	CF_CO0,
	CF_NOT_SHL1,
	CF_NOT_SHR1,
	CF_NOT_SHL2,
	CF_NOT_SHR2,
	CF_XWR0,
	CF_XWR3,
	CF_A15,
	CF_B15,

	CF_MAX,

	CF_NOT_FLAG = 32,
	CF_NOT_CO3,
	CF_NOT_CO2,
	CF_NOT_CO1,
	CF_NOT_CO0,
	CF_SHL1,
	CF_SHR1,
	CF_SHL2,
	CF_SHR2,
	CF_NOT_XWR0,
	CF_NOT_XWR3,
	CF_NOT_A15,
	CF_NOT_B15,
};

enum control_input_format {
	CI_NONE,
	CI_UNSIGNED,
	CI_SIGNED,
	CI_HEX,
	CI_BINARY,
	CI_GROUPED_BINARY,
};

struct control {
	int16_t type;
	union {
		struct {
			int16_t flag;
			int16_t a_then;
			int16_t a_else;
		} If;
		struct {
			int16_t a_goto;
		} Goto;
		struct {
			int16_t format;
			uint16_t value;
		} Input;
	};
};

struct program {
	int32_t opcode[N_INSTRUCTIONS];
	int32_t label[MAX_LABELS];
	struct control control[N_INSTRUCTIONS];
	sds annotation[N_INSTRUCTIONS];
};

int program_init(struct program *program);
int program_set_label(struct program *program, int label, int address);
int program_set_opcode(struct program *program, int address, int opcode, int brk, int carry);
int program_set_if(struct program *program, int address, int32_t flag, int32_t a_then, int32_t a_else);
int program_set_goto(struct program *program, int address, int32_t a_goto);
int program_set_input(struct program *program, int address, enum control_input_format format, int value);
int program_set_annotation(struct program *program, int address, char *annotation);
int program_move_annotation(struct program *program, int address, sds *p_annotation);
int program_output(struct program *program, char *u8path);
int program_free(struct program *program);

#endif
