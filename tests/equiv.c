#include <stdio.h>
#include <stdlib.h>

#include "../src/parser.h"
#include "../src/opcodes.h"

short Equiv[512] = {0};
short Carry2Equiv[512] = {0};
short Carry3Equiv[512] = {0};

void print_table(FILE *output, short table[], char *name, int size, int flag)
{
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (table[i] == -1)
			continue;
		if (table[i] != i)
			count++;
	}

	fprintf(output, "%s = {\n", name);
	for (int i = 0, j = 0; i < size; i++) {
		if (table[i] == -1)
			continue;
		if (table[i] != i) {
			fprintf(output, "\t0o%03o: 0o%03o", flag | i, table[i]);
			j++;
			if (j < count)
				fputc(',', output);
			fputc('\n', output);
		}
	}
	fputs("};\n\n", output);
}

int main(void)
{
	struct opcode_data data = {0};

	for (int i = 0; i < 512; i++) {
		Equiv[i] = -1;
		Carry2Equiv[i] = -1;
		Carry3Equiv[i] = -1;
	}
	
	puts("Pass 1, generic equivalents");
	for (int i = 0; i < 512; i++) {
		fputc('.', stdout);
		if (i % 64 == 63)
			fputc('\n', stdout);

		data.arg_add = 0;
		data.var = 0;
		data.reg = 0;
		data.arg1 = 0;
		data.arg2 = 0;
		data.carry = 0;
		data.op = 0;
		data.opcode = 0;

		for (int j = 0; _opcodes[j].op; j++) {
			if (_opcodes[j].carry == CARRY_VALUE_0)
				continue;
			if (_opcodes[j].carry == CARRY_VALUE_1)
				continue;
			if (_opcodes[j].opcode == i) {
				data = _opcodes[j];
				break;
			}
		}

		if (!data.op) {
			continue;
		}

		for (int j = 0; _opcodes[j].op; j++) {
			if (_opcodes[j].op != data.op)
				continue;
			if (_opcodes[j].arg_add != data.arg_add)
				continue;
			if (_opcodes[j].var != data.var)
				continue;
			if (_opcodes[j].reg != data.reg)
				continue;
			if (_opcodes[j].arg1 != data.arg1)
				continue;
			if (_opcodes[j].arg2 != data.arg2)
				continue;
			if (_opcodes[j].carry != data.carry)
				continue;

			Equiv[i] = _opcodes[j].opcode;
			break;
		}
	}

	puts("Pass 2, ALUCIN=0 equivalents");
	for (int i = 0; i < 512; i++) {
		fputc('.', stdout);
		if (i % 64 == 63)
			fputc('\n', stdout);

		data.arg_add = 0;
		data.var = 0;
		data.reg = 0;
		data.arg1 = 0;
		data.arg2 = 0;
		data.carry = 0;
		data.op = 0;
		data.opcode = 0;

		for (int j = 0; _opcodes[j].op; j++) {
			if (_opcodes[j].carry != CARRY_VALUE_0)
				continue;
			if (_opcodes[j].opcode == i) {
				data = _opcodes[j];
				break;
			}
		}

		if (!data.op) {
			continue;
		}

		for (int j = 0; _opcodes[j].op; j++) {
			if (_opcodes[j].op != data.op)
				continue;
			if (_opcodes[j].arg_add != data.arg_add)
				continue;
			if (_opcodes[j].var != data.var)
				continue;
			if (_opcodes[j].reg != data.reg)
				continue;
			if (_opcodes[j].arg1 != data.arg1)
				continue;
			if (_opcodes[j].arg2 != data.arg2)
				continue;
			if (_opcodes[j].carry != CARRY_UNUSED)
				continue;

			Carry2Equiv[i] = _opcodes[j].opcode;
			break;
		}
	}


	FILE *equiv = fopen("equiv.py", "wt");
	print_table(equiv, Equiv, "Equiv", 512, 0);
	print_table(equiv, Carry2Equiv, "Carry2Equiv", 512, ATTR_CUSED);
	fclose(equiv);

	return 0;
}
