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

struct control {
	int32_t type;
	union {
		struct {
			int32_t flag;
			int32_t a_then;
			int32_t a_else;
		} If;
		struct {
			int32_t a_goto;
		} Goto;
		struct {
			int32_t value;
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
int program_set_input(struct program *program, int address, int value);
int program_set_annotation(struct program *program, int address, char *annotation);
int program_move_annotation(struct program *program, int address, sds *p_annotation);
int program_output(struct program *program, char *u8path);
int program_free(struct program *program);

#endif
