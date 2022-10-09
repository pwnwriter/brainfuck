#include <stdio.h>
#include <stdlib.h>

// Constants
// -----------------------------------------------------------------------------

#define INST_SIZE 1024
#define MEMSIZE   30000

// Structures
// -----------------------------------------------------------------------------

struct Info {
	char *instruct;
	int   size;
};

// Function Declarations
// -----------------------------------------------------------------------------

struct Info
parse(char *file);

void
print(int *block);

int *
compile(struct Info instructions);

// Function Implementations
// -----------------------------------------------------------------------------

struct Info
parse(char *file)
{
	FILE *in = fopen(file, "r");
	if (!in) {
		fprintf(stderr, "Unable to read file: %s\n", file);
		exit(EXIT_FAILURE);
	}

	struct Info ret;
	ret.instruct = (char *)malloc(sizeof(char) * INST_SIZE);

	char *ptr    = ret.instruct;

	while (!feof(in) && ptr - ret.instruct < INST_SIZE) {
		char c = getc(in);
		if (c == '*' || c == '>' || c == '<' || c == '+' || c == '-' ||
		    c == '.' || c == ',' || c == '[' || c == ']')
			*(ptr) = c;
		ptr++;
	}

	if (ptr - ret.instruct > INST_SIZE) {
		fprintf(stderr, "File contains more than %d instructions",
		        INST_SIZE);
	}

	ret.size = ptr - ret.instruct;
	return ret;
}

void
print(int *block)
{
	int range = 1;
	for (int i = 0; i < MEMSIZE; i++)
		if (block[i] != 0)
			range = i + 1;

	for (int i = 0; i < range; i++) {
		printf("[%d]", block[i]);
	}
	printf("\n");
}

int *
compile(struct Info instructions)
{
	int *memory = (int *)malloc(sizeof(int *) * MEMSIZE);
	int *ptr    = memory;
	int  index  = 0;

	while (index < instructions.size) {
		int  cc       = 0;
		char instruct = instructions.instruct[index];
		switch (instruct) {
		case '*':
			print(memory);
			break;
		case '>':
			ptr++;
			break;
		case '<':
			ptr--;
			break;
		case '+':
			*(ptr) += 1;
			break;
		case '-':
			*(ptr) -= 1;
			break;
		case '.':
			printf("%c", *(ptr));
			break;
		case ',': {
			int cin;
			scanf("%d", &cin);
			*(ptr) = (int)cin;
		} break;
		case '[':
			if (!*(ptr)) {
				while (1) {
					index++;
					if (instructions.instruct[index] ==
					    ']') {
						if (cc == 0)
							break;
						cc--;
					} else if (instructions.instruct[index] ==
					           '[') {
						cc++;
					}
				}
			}
			break;
		case ']':
			if (*(ptr)) {
				while (1) {
					index--;
					if (instructions.instruct[index] ==
					    '[') {
						if (cc == 0)
							break;
						cc--;
					} else if (instructions.instruct[index] ==
					           ']') {
						cc++;
					}
				}
				index--;
			}
			break;
		}
		index++;
	}

	return memory;
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct Info instructions = parse(argv[1]);
	int        *block        = compile(instructions);

	if (argc >= 3)
		print(block);

	return EXIT_SUCCESS;
}
