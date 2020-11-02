#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

char allletters[2][11] = { "\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0" };
int count = 0;

inline int count_of_bits(int source) {
	unsigned int i = 0;
	for (; source; i++)
		source &= (source - 1);
	return i;
}

inline int first_bit(int source) {
	unsigned int i = 0;
	for (; source; source >>= 1, i++)
		if (source & 1) return i;
	return 11;
}

int let_num(char letter) {
	for (int i = 0; allletters[0][i] != 0; i++) {
		for (int j = 0; j < 10; j++)
			if (allletters[0][j] == letter) return allletters[1][j] - '0';
	}
}

int convert(char* string) {
	unsigned int result = 0;
	for (int i = 0; i < strlen(string) - 1; i++) {
		result += let_num(string[i]);
		result *= 10;
	}
	result += let_num(string[strlen(string) - 1]);
	return result;
}

inline bool is_firstnull(int argc, char* argv[]) { // начало с 0
	for (int i = 1; i < argc; i += 2) {
		if (let_num(*(argv + i)[0]) == 0) return true;  // начало с 0
	}
	return false;
}

bool check(int argc, char* argv[]) {
	if (is_firstnull(argc, argv)) return false;
	unsigned int result = 0;
	for (int i = 1; i < argc - 1; i += 2)
		result += convert(*(argv + i));

	if (result == convert(*(argv + argc - 1))) {
		for (int i = 1; i < argc; i += 2) {
			printf("%d", convert(*(argv + i)));
			if (i < argc - 1)
				printf(" %s ", (*(argv + i + 1)));
		}
		return true;
	}
	return false;
}

bool do_puzzle(int remain, int argc, char* argv[]) {
	int re = remain;
	int cntb = count_of_bits(remain);
	int fb = first_bit(remain);
	if ((10 - cntb) == count) { return check(argc, argv); }
	for (int j = fb; j != 11; j = first_bit(re)) {
		re = re & ~(1 << j);
		allletters[1][10 - cntb] = j + '0';
		if (do_puzzle(remain & ~(1 << j), argc, argv)) return true;
		allletters[1][10 - cntb] = 0;
	}
	return false;
}

void parse_line(int argc, char* argv[]) {
	for (int i = 1; i < argc; i += 2)
		for (int j = 0; j < strlen(*(argv + i)); j++)
			for (int k = 0; k < 10; k++)
			{
				if (*(*(argv + i) + j) == allletters[0][k]) break;
				if (allletters[0][k] == 0) {
					allletters[0][k] = *(*(argv + i) + j);
					if (++count == 10) return;
					break;
				}
			}
}

int main(int argc, char* argv[])
{
	unsigned int start_time = clock();
	if (argc == 1) { printf("Please, use cmd!"); return 0; }
	parse_line(argc, argv);
	if (!do_puzzle(0b1111111111, argc, argv)) printf("Something went wrong!");

	printf("\n%.3f\n", (clock() - start_time) / 1000.0f);
	getc(stdin);
	return 0;
}