#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct vector {
	char letter;
	int number;
	bool is_first;
	int is_first_end;
	struct vector* next;
};

struct vector* setL(struct vector* arr, char l, bool is_first, int is_first_end) {
	if (arr == 0) {
		arr = (struct vector*)malloc(sizeof(struct vector));
		arr->letter = l, arr->number = -1;
		arr->next = 0; arr->is_first = is_first; arr->is_first_end = is_first_end;
	}
	else if (arr->letter > l) {
		struct vector* tmp = arr;
		arr = (struct vector*)malloc(sizeof(struct vector));
		arr->letter = l, arr->number = -1;
		arr->next = tmp; arr->is_first = is_first; arr->is_first_end = is_first_end;
	}
	else if (arr->letter < l) arr->next = setL(arr->next, l, is_first, is_first_end);
	else if (is_first_end != 0)
		arr->is_first_end = is_first_end;
	return arr;
}

struct vector* arr = 0;
bool evr1 = true;

inline int first_bit(int source) {
	unsigned int i = 0;
	for (; source; source >>= 1, i++)
		if (source & 1) return i;
	return 11;
}

inline int let_num(char letter) {
	struct vector* temp = arr;
	for (; temp != 0; temp = temp->next)
		if (temp->letter == letter) return temp->number;
}

int convert(char* string) {
	register unsigned int result = 0;
	for (int i = 0; i < strlen(string); i++) {
		result *= 10;
		result += let_num(string[i]);
	}
	return result;
}

bool check(int argc, char* argv[]) {
	unsigned int result = 0;
	for (int i = 1; i < argc - 1; i += 2)
		result += convert(*(argv + i));
	if (result == convert(*(argv + argc - 1))) {
		for (int i = 1; i < argc - 2; i += 2) {
			printf("%d", convert(*(argv + i)));
			printf(" %s ", (*(argv + i + 1)));
		}
		printf("%d", convert(*(argv + argc - 1)));
		return true;
	}
	return false;
}

bool do_puzzle(int remain, int argc, char* argv[], struct vector* temp) {
	int re = remain;
	if (temp == 0) { return check(argc, argv); }
	for (int j = first_bit(remain); j != 11; re = re & ~(1 << j), j = first_bit(re)) {
		if ((temp->is_first && j == 0) || (evr1 && temp->is_first_end != 0 && j > temp->is_first_end)) continue;
		temp->number = j;
		if (do_puzzle(remain & ~(1 << j), argc, argv, temp->next)) return true;
	}
	return false;
}

void parse_line(int argc, char* argv[]) {
	int z = argc / 2 - 2;
	int len_end = strlen(*(argv + argc - 1));
	for (int i = 1; i < argc - 1; i += 2) {
		int len = strlen(*(argv + i));
		if (len >= len_end)
			evr1 = false;

		for (int j = 0; j < len; j++)
			arr = setL(arr, *(*(argv + i) + j), j == 0, 0);
	}

	for (int j = 0; j < len_end; j++)
		arr = setL(arr, *(*(argv + argc - 1) + j), j == 0, (j == 0) ? z : 0);
}

int main(int argc, char* argv[])
{
	unsigned int start_time = clock();
	if (argc == 1) { printf("Please, use cmd!"); return 0; }
	parse_line(argc, argv);
	struct vector* temp = arr;
	if (!do_puzzle(0b1111111111, argc, argv, temp)) printf("Something went wrong!");

	printf("\n%.3f", (clock() - start_time) / 1000.0f);
	return 0;
}