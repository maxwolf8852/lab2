#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct vector {
	char letter;
	int number;
	bool is_first;
	struct vector* next;
};

struct vector* setL(struct vector* arr, char l, bool is_first) {
	if (arr == 0) {
		arr = (struct vector*)malloc(sizeof(struct vector));
		arr->letter = l, arr->number = -1;
		arr->next = 0; arr->is_first = is_first;
	}
	else if (arr->letter > l) {
		struct vector* tmp = arr;
		arr = (struct vector*)malloc(sizeof(struct vector));
		arr->letter = l, arr->number = -1;
		arr->next = tmp; arr->is_first = is_first;
	}
	else if (arr->letter < l) arr->next = setL(arr->next, l, is_first);
	return arr;
}

struct vector* arr = 0;

inline int first_bit(int source) {
	unsigned int i = 0;
	for (; source; source >>= 1, i++)
		if (source & 1) return i;
	return 11;
}

inline int let_num(char letter) {
	struct vector* temp = arr;
	for (int i = 0; temp != 0; i++, temp = temp->next)
		if (temp->letter == letter) return temp->number;
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

bool check(int argc, char* argv[]) {
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

bool do_puzzle(int remain, int argc, char* argv[], struct vector* temp) {
	int re = remain;
	int fb = first_bit(remain);
	if (temp == 0) { return check(argc, argv); }
	for (int j = fb; j != 11; re = re & ~(1 << j), j = first_bit(re)) {
		if (temp->is_first && j == 0) continue;
		temp->number = j;
		if (do_puzzle(remain & ~(1 << j), argc, argv, temp->next)) return true;
	}
	return false;
}

void parse_line(int argc, char* argv[]) {
	for (int i = 1; i < argc; i += 2)
		for (int j = 0; j < strlen(*(argv + i)); j++)
			arr = setL(arr, *(*(argv + i) + j), j == 0);
}

int main(int argc, char* argv[])
{
	unsigned int start_time = clock();
	if (argc == 1) { printf("Please, use cmd!"); return 0; }

	parse_line(argc, argv);
	struct vector* temp = arr;
	if (!do_puzzle(0b1111111111, argc, argv, temp)) printf("Something went wrong!");

	printf("\n%.3f\n", (clock() - start_time) / 1000.0f);
	return 0;
}