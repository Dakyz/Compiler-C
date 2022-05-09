#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <math.h>

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
static struct termios stored_settings;

void set_keypress(void)
{
	struct termios new_settings;

	tcgetattr(0, &stored_settings);

	new_settings = stored_settings;

	/* Disable canonical mode, and set buffer size to 1 byte */
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0, TCSANOW, &new_settings);
	return;
}

void reset_keypress(void)
{
	tcsetattr(0, TCSANOW, &stored_settings);
	return;
}

#define _getch getc
#define Sleep usleep
void system() {}
void exit() {}
void rand() {}
void srand() {}
#endif

int result_int[9] = { -1 }; //-1 ничего 1 крестик 0 нолик
int may_change[9][8] = { 0 };
char result_field[] =
"  X       X  |  X       X  |  X       X  \n";
char result_field2[] = // для получения позиций
"    00000    |    00000    |    00000    \n";
char fields[] =
"     |     |     ||     |     |     ||     |     |     ";
typedef struct place
{
	int mini_result_int[9]; //рез для каждого поля 3х3
	int position; //1-9 
	int symbol_position[9]; //куда ставить 58 все такое
	int up_position;
	int down_position; //границы
	int schet; //не больше 9
} place;
typedef struct result_place
{
	int result;
	int symbol_position_X[9];
	int symbol_position_O[16];
	int schet; //до 9
	int changing; //счетчик изменений не больше 3
} result_place;
void show_result_field()
{
	int hConsole;// = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < strlen(result_field); i++)
	{
		if (result_field[i] == 'X')
		{
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
			printf("%c", result_field[i]);
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
		}
		else if (result_field[i] == '0')
		{
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 9));
			printf("%c", result_field[i]);
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
		}
		else printf("%c", result_field[i]);
	}
}
void show_fields()
{
	int hConsole;// = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < strlen(fields); i++)
	{
		if (fields[i] == 'X')
		{
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
			printf("%c", fields[i]);
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
		}
		else if (fields[i] == 'O')
		{
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 9));
			printf("%c", fields[i]);
			//SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
		}
		else printf("%c", fields[i]);
	}
	printf("\n");
}
void current_place(place A) //выделение поля
{
	int count = 0;
	int i = A.up_position;
	while (count != 3)
	{
		if (fields[i] == '|')
		{
			count++;
			i++;
			continue;
		}
		else
		{
			fields[i] = '-';
		}
		i++;
		if (i - A.up_position == 17) break;
	}
	count = 0;
	i = A.down_position;
	while (count != 3)
	{
		if (fields[i] == '|')
		{
			count++;
			i++;
			continue;
		}
		else
		{
			fields[i] = '-';
		}
		i++;
		if (i - A.down_position == 17) break;
	}
}
void remove_minus() //убирает выделение поля
{
	for (int i = 0; i < strlen(fields); i++)
		if (fields[i] == '-') fields[i] = ' ';
}
void mini_check(place A)
{
	for (int i = 0; i < 9; i++)
	{
		switch (A.mini_result_int[i])
		{
		case -1:
			fields[A.symbol_position[i]] = ' ';
			break;
		case 0:
			fields[A.symbol_position[i]] = 'O';
			break;
		case 1:
			fields[A.symbol_position[i]] = 'X';
			break;
		}
	}
}
void check(result_place A)
{
	switch (A.result)
	{
	case 1:
		for (int i = 0; i < 9; i++)
			result_field[A.symbol_position_X[i]] = 'X';
		break;
	case -1:
		for (int i = 0; i < 9; i++)
			result_field[A.symbol_position_X[i]] = ' ';
		for (int i = 0; i < 16; i++)
			result_field[A.symbol_position_O[i]] = ' ';
		break;
	case 0:
		for (int i = 0; i < 16; i++)
			result_field[A.symbol_position_O[i]] = '0';
		break;
	}
}
void give_symbol(char A, result_place B, int position) //глобальное поле замена
{
	switch (A)
	{
	case 'X':
		if (B.changing < 3 && result_int[position] == -1)
		{
			result_int[position] = 1;
			B.result = 1;
			B.changing++;
			check(B);
		}
		else if (B.changing < 3 && result_int[position] == 0)
		{
			result_int[position] = 1;
			B.result = -1;
			check(B);
			B.changing++;
			B.result = 1;
			check(B);
		}
		break;
	case 'O':
		if (B.changing < 3 && result_int[position] == -1)
		{
			result_int[position] = 0;
			B.result = 0;
			B.changing++;
			check(B);
		}
		else if (B.changing < 3 && result_int[position] == 1)
		{
			result_int[position] = 0;
			B.result = -1;
			check(B);
			B.changing++;
			B.result = 0;
			check(B);
		}
		break;
	}
}
int get_result(place A, result_place B, char C)
{
	if (fields[A.symbol_position[0]] == fields[A.symbol_position[1]] && fields[A.symbol_position[0]] == fields[A.symbol_position[2]] && fields[A.symbol_position[0]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][0] == 1 && may_change[A.position][1] == 1 && may_change[A.position][2] == 1))
	{
		give_symbol(fields[A.symbol_position[0]], B, A.position);
		may_change[A.position][0] = 1;
		may_change[A.position][1] = 1;
		may_change[A.position][2] = 1;
	}
	else if (fields[A.symbol_position[3]] == fields[A.symbol_position[4]] && fields[A.symbol_position[3]] == fields[A.symbol_position[5]] && fields[A.symbol_position[3]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][3] == 1 && may_change[A.position][4] == 1 && may_change[A.position][5] == 1))
	{
		give_symbol(fields[A.symbol_position[3]], B, A.position);
		may_change[A.position][3] = 1;
		may_change[A.position][4] = 1;
		may_change[A.position][5] = 1;
	}
	else if (fields[A.symbol_position[6]] == fields[A.symbol_position[7]] && fields[A.symbol_position[6]] == fields[A.symbol_position[8]] && fields[A.symbol_position[6]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][6] == 1 && may_change[A.position][7] == 1 && may_change[A.position][8] == 1))
	{
		give_symbol(fields[A.symbol_position[6]], B, A.position);
		may_change[A.position][6] = 1;
		may_change[A.position][7] = 1;
		may_change[A.position][8] = 1;
	}
	else if (fields[A.symbol_position[0]] == fields[A.symbol_position[3]] && fields[A.symbol_position[0]] == fields[A.symbol_position[6]] && fields[A.symbol_position[6]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][0] == 1 && may_change[A.position][3] == 1 && may_change[A.position][6] == 1))
	{
		give_symbol(fields[A.symbol_position[0]], B, A.position);
		may_change[A.position][0] = 1;
		may_change[A.position][3] = 1;
		may_change[A.position][6] = 1;
	}
	else if (fields[A.symbol_position[1]] == fields[A.symbol_position[4]] && fields[A.symbol_position[1]] == fields[A.symbol_position[7]] && fields[A.symbol_position[7]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][1] == 1 && may_change[A.position][4] == 1 && may_change[A.position][7] == 1))
	{
		give_symbol(fields[A.symbol_position[1]], B, A.position);
		may_change[A.position][1] = 1;
		may_change[A.position][4] = 1;
		may_change[A.position][7] = 1;
	}
	else if (fields[A.symbol_position[2]] == fields[A.symbol_position[5]] && fields[A.symbol_position[2]] == fields[A.symbol_position[8]] && fields[A.symbol_position[8]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][2] == 1 && may_change[A.position][5] == 1 && may_change[A.position][8] == 1))
	{
		give_symbol(fields[A.symbol_position[2]], B, A.position);
		may_change[A.position][2] = 1;
		may_change[A.position][5] = 1;
		may_change[A.position][8] = 1;
	}
	else if (fields[A.symbol_position[0]] == fields[A.symbol_position[4]] && fields[A.symbol_position[0]] == fields[A.symbol_position[8]] && fields[A.symbol_position[8]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][0] == 1 && may_change[A.position][4] == 1 && may_change[A.position][8] == 1))
	{
		give_symbol(fields[A.symbol_position[0]], B, A.position);
		may_change[A.position][0] = 1;
		may_change[A.position][4] = 1;
		may_change[A.position][8] = 1;
	}
	else if (fields[A.symbol_position[2]] == fields[A.symbol_position[4]] && fields[A.symbol_position[2]] == fields[A.symbol_position[6]] && fields[A.symbol_position[6]] != C && result_int != (fields[A.symbol_position[0]] + 1) % 2 && !(may_change[A.position][2] == 1 && may_change[A.position][4] == 1 && may_change[A.position][6] == 1))
	{
		give_symbol(fields[A.symbol_position[2]], B, A.position);
		may_change[A.position][2] = 1;
		may_change[A.position][4] = 1;
		may_change[A.position][6] = 1;
	}
	else return 0;
	return 1;
}
void win_conditional(result_place * A)
{
	int win = -1;
	if (result_int[0] == result_int[1] && result_int[0] == result_int[2] && result_int[0] != -1)
		win = result_int[0];
	else if (result_int[3] == result_int[4] && result_int[3] == result_int[5] && result_int[3] != -1)
		win = result_int[3];
	else if (result_int[6] == result_int[7] && result_int[8] == result_int[6] && result_int[6] != -1)
		win = result_int[6];
	else if (result_int[0] == result_int[3] && result_int[0] == result_int[6] && result_int[0] != -1)
		win = result_int[0];
	else if (result_int[1] == result_int[4] && result_int[1] == result_int[7] && result_int[1] != -1)
		win = result_int[1];
	else if (result_int[2] == result_int[5] && result_int[2] == result_int[8] && result_int[2] != -1)
		win = result_int[2];
	else if (result_int[0] == result_int[4] && result_int[4] == result_int[8] && result_int[0] != -1)
		win = result_int[0];
	else if (result_int[2] == result_int[4] && result_int[2] == result_int[6] && result_int[2] != -1)
		win = result_int[2];
	switch (win)
	{
	case 1:
		system("cls");
		printf("Выиграли крестики!\n");
		show_result_field();
		getchar();
		exit(0);
		break;
	case 0:
		system("cls");
		printf("Выиграли нолики!\n");
		show_result_field();
		getchar();
		exit(0);
		break;
	}
}
int hod_easy_check(place A)
{
	if (
		(fields[A.symbol_position[0]] == fields[A.symbol_position[1]] && fields[A.symbol_position[0]] == fields[A.symbol_position[2]] && fields[A.symbol_position[0]] != ' ')
		|| (fields[A.symbol_position[3]] == fields[A.symbol_position[4]] && fields[A.symbol_position[3]] == fields[A.symbol_position[5]] && fields[A.symbol_position[3]] != ' ')
		|| (fields[A.symbol_position[6]] == fields[A.symbol_position[7]] && fields[A.symbol_position[6]] == fields[A.symbol_position[8]] && fields[A.symbol_position[6]] != ' ')
		|| (fields[A.symbol_position[0]] == fields[A.symbol_position[3]] && fields[A.symbol_position[0]] == fields[A.symbol_position[6]] && fields[A.symbol_position[6]] != ' ')
		|| (fields[A.symbol_position[1]] == fields[A.symbol_position[4]] && fields[A.symbol_position[1]] == fields[A.symbol_position[7]] && fields[A.symbol_position[7]] != ' ')
		|| (fields[A.symbol_position[2]] == fields[A.symbol_position[5]] && fields[A.symbol_position[2]] == fields[A.symbol_position[8]] && fields[A.symbol_position[8]] != ' ')
		|| (fields[A.symbol_position[0]] == fields[A.symbol_position[4]] && fields[A.symbol_position[0]] == fields[A.symbol_position[8]] && fields[A.symbol_position[8]] != ' ')
		|| (fields[A.symbol_position[2]] == fields[A.symbol_position[4]] && fields[A.symbol_position[2]] == fields[A.symbol_position[6]] && fields[A.symbol_position[6]] != ' ')
		)
		return 1;
	return 0;
}
int hod_easy(place A, result_place B, int C, int D, int level)
{
	int C1;
	for (int i = 0; i < 9; i++) //для победы
	{
		int hod = i;
		if (A.mini_result_int[i] == -1)
		{
			A.mini_result_int[i] = C;
			mini_check(A);
			if (hod_easy_check(A) == 1 && hod != D)
			{
				A.mini_result_int[i] = -1;
				mini_check(A);
				return hod;
			}
			else
			{
				A.mini_result_int[i] = -1;
				mini_check(A);
			}
		}
	}
	switch (C)
	{
	case 1:
		C1 = 0;
		break;
	case 0:
		C1 = 1;
		break;
	}
	for (int i = 0; i < 9; i++) // для того, чтобы не дать победить сопернику
	{
		int hod = i;
		if (A.mini_result_int[i] == -1)
		{
			A.mini_result_int[i] = C1;
			mini_check(A);
			if (hod_easy_check(A) == 1 && hod != D)
			{
				A.mini_result_int[i] = -1;
				mini_check(A);
				return hod;
			}
			else
			{
				A.mini_result_int[i] = -1;
				mini_check(A);
			}
		}
	}
	if (level == '2') return (rand() % 9);
	else return 10;
}
int empty(place A)
{
	int t = 0;
	for (int i = 0; i < 9; i++)
		if (A.mini_result_int[i] != -1)
			t++;
	return t;
}
int win_check(place A)
{
	if (
		(A.mini_result_int[0] == A.mini_result_int[1] && A.mini_result_int[0] == A.mini_result_int[2] && A.mini_result_int[0] == 0) ||
		(A.mini_result_int[3] == A.mini_result_int[4] && A.mini_result_int[3] == A.mini_result_int[5] && A.mini_result_int[3] == 0) ||
		(A.mini_result_int[6] == A.mini_result_int[7] && A.mini_result_int[6] == A.mini_result_int[8] && A.mini_result_int[6] == 0) ||
		(A.mini_result_int[0] == A.mini_result_int[3] && A.mini_result_int[0] == A.mini_result_int[6] && A.mini_result_int[0] == 0) ||
		(A.mini_result_int[1] == A.mini_result_int[4] && A.mini_result_int[1] == A.mini_result_int[7] && A.mini_result_int[1] == 0) ||
		(A.mini_result_int[2] == A.mini_result_int[5] && A.mini_result_int[2] == A.mini_result_int[8] && A.mini_result_int[2] == 0) ||
		(A.mini_result_int[0] == A.mini_result_int[4] && A.mini_result_int[0] == A.mini_result_int[8] && A.mini_result_int[0] == 0) ||
		(A.mini_result_int[2] == A.mini_result_int[4] && A.mini_result_int[2] == A.mini_result_int[6] && A.mini_result_int[2] == 0)
		) return 0;
	else if (
		(A.mini_result_int[0] == A.mini_result_int[1] && A.mini_result_int[0] == A.mini_result_int[2] && A.mini_result_int[0] == 1) ||
		(A.mini_result_int[3] == A.mini_result_int[4] && A.mini_result_int[3] == A.mini_result_int[5] && A.mini_result_int[3] == 1) ||
		(A.mini_result_int[6] == A.mini_result_int[7] && A.mini_result_int[6] == A.mini_result_int[8] && A.mini_result_int[6] == 1) ||
		(A.mini_result_int[0] == A.mini_result_int[3] && A.mini_result_int[0] == A.mini_result_int[6] && A.mini_result_int[0] == 1) ||
		(A.mini_result_int[1] == A.mini_result_int[4] && A.mini_result_int[1] == A.mini_result_int[7] && A.mini_result_int[1] == 1) ||
		(A.mini_result_int[2] == A.mini_result_int[5] && A.mini_result_int[2] == A.mini_result_int[8] && A.mini_result_int[2] == 1) ||
		(A.mini_result_int[0] == A.mini_result_int[4] && A.mini_result_int[0] == A.mini_result_int[8] && A.mini_result_int[0] == 1) ||
		(A.mini_result_int[2] == A.mini_result_int[4] && A.mini_result_int[2] == A.mini_result_int[6] && A.mini_result_int[2] == 1)
		) return 1;
	return -1;
}
int win_result_check(place A[9])
{
	int results[9];
	for (int i = 0; i < 9; i++)
		win_check(A[i]);
	if (
		(results[0] == results[1] && results[0] == results[2] && results[0] == 0) ||
		(results[3] == results[4] && results[3] == results[5] && results[3] == 0) ||
		(results[6] == results[7] && results[6] == results[8] && results[6] == 0) ||
		(results[0] == results[3] && results[0] == results[6] && results[0] == 0) ||
		(results[1] == results[4] && results[1] == results[7] && results[1] == 0) ||
		(results[2] == results[5] && results[2] == results[8] && results[2] == 0) ||
		(results[0] == results[4] && results[0] == results[8] && results[0] == 0) ||
		(results[2] == results[4] && results[2] == results[6] && results[2] == 0)
		) return 0;
	else if (
		(results[0] == results[1] && results[0] == results[2] && results[0] == 1) ||
		(results[3] == results[4] && results[3] == results[5] && results[3] == 1) ||
		(results[6] == results[7] && results[6] == results[8] && results[6] == 1) ||
		(results[0] == results[3] && results[0] == results[6] && results[0] == 1) ||
		(results[1] == results[4] && results[1] == results[7] && results[1] == 1) ||
		(results[2] == results[5] && results[2] == results[8] && results[2] == 1) ||
		(results[0] == results[4] && results[0] == results[8] && results[0] == 1) ||
		(results[2] == results[4] && results[2] == results[6] && results[2] == 1)
		) return 1;
	return -1;
}
int t = 0;
void hod_hard_check(int schet, int A[9], place B[9], int current_number, int previous_number, int number, int antinumber)
{
	for (int i = 0; i < 9 && schet < 8; i++)
	{
		schet++;
		if (B[current_number].mini_result_int[i] == -1 && (i != previous_number || empty(B[current_number]) == 8))
		{
			if (schet % 2 == 1)
				B[current_number].mini_result_int[i] = number;
			else
				B[current_number].mini_result_int[i] = antinumber;
			if (empty(B[i]) < 9)
				hod_hard_check(schet, A, B, i, current_number, number, antinumber);
			else for (int r = 1; r < 9; r++)
				if (empty(B[(i + r) % 9]) < 9)
				{
					hod_hard_check(schet, A, B, (i + r) % 9, current_number, number, antinumber);
					break;
				}
			if (schet == 8) //10
			{
				int temp_number = 0;
				for (int q = 0; q < 9; q++)
				{
					if (B[q].mini_result_int[0] == number && B[q].mini_result_int[4] == number && B[q].mini_result_int[8] == number ||
						B[q].mini_result_int[2] == number && B[q].mini_result_int[4] == number && B[q].mini_result_int[6] == number)
						temp_number += 10;
					else if (B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[8] == antinumber ||
						B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[6] == antinumber)
						temp_number -= 10;
					if (B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[1] == antinumber && B[q].mini_result_int[2] == antinumber ||
						B[q].mini_result_int[3] == antinumber && B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[5] == antinumber ||
						B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[7] == antinumber && B[q].mini_result_int[8] == antinumber ||
						B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[3] == antinumber && B[q].mini_result_int[6] == antinumber ||
						B[q].mini_result_int[1] == antinumber && B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[7] == antinumber ||
						B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[8] == antinumber
						)
						temp_number -= 9;
					if (B[q].mini_result_int[0] == number && B[q].mini_result_int[1] == number && B[q].mini_result_int[2] == number ||
						B[q].mini_result_int[3] == number && B[q].mini_result_int[4] == number && B[q].mini_result_int[5] == number ||
						B[q].mini_result_int[6] == number && B[q].mini_result_int[7] == number && B[q].mini_result_int[8] == number ||
						B[q].mini_result_int[0] == number && B[q].mini_result_int[3] == number && B[q].mini_result_int[6] == number ||
						B[q].mini_result_int[1] == number && B[q].mini_result_int[4] == number && B[q].mini_result_int[7] == number ||
						B[q].mini_result_int[2] == number && B[q].mini_result_int[5] == number && B[q].mini_result_int[8] == number
						)
						temp_number += 9;
					else if (
						(B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[8] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[6] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[2] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[0] != number ||
							B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[4] != number ||
							B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[4] != number) &&
							(B[q].mini_result_int[1] == number || B[q].mini_result_int[3] == number || B[q].mini_result_int[5] == number || B[q].mini_result_int[7] == number))
						temp_number -= 7;
					else if (
						(B[q].mini_result_int[4] == number && B[q].mini_result_int[0] == number && B[q].mini_result_int[8] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[2] == number && B[q].mini_result_int[6] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[6] == number && B[q].mini_result_int[2] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[0] != antinumber ||
							B[q].mini_result_int[0] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[4] != antinumber ||
							B[q].mini_result_int[2] == number && B[q].mini_result_int[6] == number && B[q].mini_result_int[4] != antinumber) &&
							(B[q].mini_result_int[1] == antinumber || B[q].mini_result_int[3] == antinumber || B[q].mini_result_int[5] == antinumber || B[q].mini_result_int[7] == antinumber))
						temp_number += 7;
					else if
						(B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[8] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[6] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[2] != number ||
							B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[0] != number ||
							B[q].mini_result_int[0] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[4] != number ||
							B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[4] != number)
						temp_number -= 8;
					else if
						(B[q].mini_result_int[4] == number && B[q].mini_result_int[0] == number && B[q].mini_result_int[8] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[2] == number && B[q].mini_result_int[6] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[6] == number && B[q].mini_result_int[2] != antinumber ||
							B[q].mini_result_int[4] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[0] != antinumber ||
							B[q].mini_result_int[0] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[4] != antinumber ||
							B[q].mini_result_int[2] == number && B[q].mini_result_int[6] == number && B[q].mini_result_int[4] != antinumber)
						temp_number += 8;
					else if (
						(B[q].mini_result_int[0] == number && (B[q].mini_result_int[3] == number && B[q].mini_result_int[6] != antinumber || B[q].mini_result_int[6] == number && B[q].mini_result_int[3] != antinumber || B[q].mini_result_int[1] == number && B[q].mini_result_int[2] != antinumber || B[q].mini_result_int[2] == number && B[q].mini_result_int[1] != antinumber)) ||
						(B[q].mini_result_int[1] == number && (B[q].mini_result_int[2] == number && B[q].mini_result_int[0] != antinumber || B[q].mini_result_int[4] == number && B[q].mini_result_int[7] != antinumber || B[q].mini_result_int[7] == number && B[q].mini_result_int[4] != antinumber)) ||
						(B[q].mini_result_int[2] == number && (B[q].mini_result_int[5] == number && B[q].mini_result_int[8] != antinumber || B[q].mini_result_int[8] == number && B[q].mini_result_int[5] != antinumber)) ||
						(B[q].mini_result_int[3] == number && (B[q].mini_result_int[4] == number && B[q].mini_result_int[5] != antinumber || B[q].mini_result_int[5] == number && B[q].mini_result_int[4] != antinumber || B[q].mini_result_int[6] == number && B[q].mini_result_int[0] != antinumber)) ||
						(B[q].mini_result_int[4] == number && (B[q].mini_result_int[5] == number && B[q].mini_result_int[3] != antinumber || B[q].mini_result_int[7] == number && B[q].mini_result_int[1] != antinumber)) ||
						(B[q].mini_result_int[5] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[2] != antinumber) ||
						(B[q].mini_result_int[6] == number && (B[q].mini_result_int[7] == number && B[q].mini_result_int[8] != antinumber || B[q].mini_result_int[8] == number && B[q].mini_result_int[7] != antinumber)) ||
						(B[q].mini_result_int[7] == number && B[q].mini_result_int[8] == number && B[q].mini_result_int[6] != antinumber)
						)
						temp_number += 5;
					else if (
						(B[q].mini_result_int[0] == antinumber && (B[q].mini_result_int[3] == antinumber && B[q].mini_result_int[6] != number || B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[3] != number || B[q].mini_result_int[1] == antinumber && B[q].mini_result_int[2] != antinumber || B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[1] != number)) ||
						(B[q].mini_result_int[1] == antinumber && (B[q].mini_result_int[2] == antinumber && B[q].mini_result_int[0] != number || B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[7] != number || B[q].mini_result_int[7] == antinumber && B[q].mini_result_int[4] != antinumber)) ||
						(B[q].mini_result_int[2] == antinumber && (B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[8] != number || B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[5] != number)) ||
						(B[q].mini_result_int[3] == antinumber && (B[q].mini_result_int[4] == antinumber && B[q].mini_result_int[5] != number || B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[4] != number || B[q].mini_result_int[6] == antinumber && B[q].mini_result_int[0] != antinumber)) ||
						(B[q].mini_result_int[4] == antinumber && (B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[3] != number || B[q].mini_result_int[7] == antinumber && B[q].mini_result_int[1] != number)) ||
						(B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[2] != number) ||
						(B[q].mini_result_int[6] == antinumber && (B[q].mini_result_int[7] == antinumber && B[q].mini_result_int[8] != number || B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[7] != number)) ||
						(B[q].mini_result_int[7] == antinumber && B[q].mini_result_int[8] == antinumber && B[q].mini_result_int[6] != number)
						)
						temp_number -= 5;
					else if (
						B[q].mini_result_int[1] == number && (B[q].mini_result_int[3] == number && (B[q].mini_result_int[7] != antinumber || B[q].mini_result_int[0] != antinumber || B[q].mini_result_int[5] != antinumber || B[q].mini_result_int[4] != antinumber) || B[q].mini_result_int[5] == number && (B[q].mini_result_int[4] != antinumber || B[q].mini_result_int[7] != antinumber || B[q].mini_result_int[3] != antinumber || B[q].mini_result_int[2] != antinumber)) ||
						B[q].mini_result_int[3] == number && B[q].mini_result_int[7] == number && (B[q].mini_result_int[6] != antinumber || B[q].mini_result_int[5] != antinumber || B[q].mini_result_int[1] != antinumber) ||
						B[q].mini_result_int[5] == number && B[q].mini_result_int[7] == number && (B[q].mini_result_int[8] != antinumber || B[q].mini_result_int[3] != antinumber || B[q].mini_result_int[1] != antinumber)
						)
						temp_number += 3;
					else if (
						B[q].mini_result_int[1] == antinumber && (B[q].mini_result_int[3] == antinumber && (B[q].mini_result_int[7] != number || B[q].mini_result_int[0] != number || B[q].mini_result_int[5] != number || B[q].mini_result_int[4] != number) || B[q].mini_result_int[5] == antinumber && (B[q].mini_result_int[4] != number || B[q].mini_result_int[7] != number || B[q].mini_result_int[3] != number || B[q].mini_result_int[2] != number)) ||
						B[q].mini_result_int[3] == antinumber && B[q].mini_result_int[7] == antinumber && (B[q].mini_result_int[6] != number || B[q].mini_result_int[5] != number || B[q].mini_result_int[1] != number) ||
						B[q].mini_result_int[5] == antinumber && B[q].mini_result_int[7] == antinumber && (B[q].mini_result_int[8] != number || B[q].mini_result_int[3] != number || B[q].mini_result_int[1] != number)
						)
						temp_number -= 3;
					else
					{
						for (int w = 0; w < 9; w++)
						{
							if (B[q].mini_result_int[w] == number)
								temp_number++;
							if (B[q].mini_result_int[w] == antinumber)
								temp_number--;
						}
					}
				}
				if (win_result_check(B) == number)
					temp_number += 5000;
				if (win_result_check(B) == antinumber)
				{
					A[t] = -20000;
				}
				else if (A[t] > temp_number && A[t] != -20000)
					A[t] = temp_number;
			}
			B[current_number].mini_result_int[i] = -1;
		}
		schet--;
		if (schet == 0)
			t++;
	}
}
int hod_hard(place A[9], int B/*это значение того, чем именено играет бот*/, int C/*номер предыдущей клетки*/, int current_number/*номер текущей клетки*/)
{
	for (int i = 0; i < 9; i++)
	{
		if ((i != C || empty(A[current_number]) == 8) && A[current_number].mini_result_int[i] == -1)
		{
			A[current_number].mini_result_int[i] = B;
			if (B == win_result_check(A))
			{
				A[current_number].mini_result_int[i] = -1;
				return i;
			}
			A[current_number].mini_result_int[i] = -1;
		}
	}
	int antinumber;
	switch (B)
	{
	case 1:
		antinumber = 0;
		break;
	case 0:
		antinumber = 1;
		break;
	}
	int hod_result[9] = { 0 };
	for (int i = 0; i < 9; i++)
		hod_result[i] = 0;
	hod_hard_check(0, hod_result, A, current_number, C, B, antinumber);
	int max = -20000;
	int max_i = -1;
	for (int i = 0; i < 9; i++)
		if (hod_result[i] > max && (i != C || empty(A[current_number]) == 8) && A[current_number].mini_result_int[i] == -1)
		{
			max = hod_result[i];
			max_i = i;
		}
	t = 0;
	return max_i;
}
int main()
{
	system("color F0");
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
#ifndef __linux__
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
#else 
	set_keypress();
#endif
	int current;
	int enemy_current;
	int current_number = 4;
	int krest;
	int buffer[81];
	place digit[9];
	result_place result_digit[9];
	for (int i = 0; i < 9; i++)
		result_int[i] = -1;
	for (int i = 0; i < 9; i++)
		result_digit[i].result = -1;
	for (int i = 0; i < 9; i++)
		result_digit[i].schet = 0;
	for (int i = 0; i < 9; i++)
		result_digit[i].changing = 0;
	for (int i = 0, q = 0; i < strlen(result_field); i++)
	{
		if (result_field[i] == 'X')
		{
			result_digit[q].symbol_position_X[result_digit[q].schet] = i;
			result_digit[q].schet++;
		}
		else if (result_field[i] == '|')
			q++;
		else if (result_field[i] == '\n')
		{
			if (result_field[i + 1] == '=') q++;
			else if (result_field[i - 1] != '=') q -= 2;
		}
	}
	for (int i = 0; i < 9; i++)
		result_digit[i].schet = 0;
	for (int i = 0, q = 0; i < strlen(result_field2); i++)
	{
		if (result_field2[i] == '0')
		{
			result_digit[q].symbol_position_O[result_digit[q].schet] = i;
			result_digit[q].schet++;
		}
		else if (result_field2[i] == '|')
			q++;
		else if (result_field2[i] == '\n')
		{
			if (result_field2[i + 1] == '=') q++;
			else if (result_field2[i - 1] != '=') q -= 2;
		}
	}
	for (int i = 0; i < 9; i++)
		check(result_digit[i]);
	for (int i = 0; i < 9; i++)
		digit[i].schet = 0;
	for (int i = 0; i < 9; i++) // пока на поле ничего нет, -1 это ничего, 0 это нолик, 1 это крестик
	{
		digit[i].position = i;
		for (int q = 0; q < 9; q++)
			digit[i].mini_result_int[q] = -1;
	}
	for (int i = 0, q = 0; i < strlen(fields); i++)
	{
		if (fields[i] == 'O' || fields[i] == 'X')
		{
			buffer[q] = i;
			q++;
		}
	}
	for (int i = 0, schet = 0, count = 0; i < 9; i += 3)
	{
		for (int r = 0; r < 9; r += 3)
		{
			for (int q = 0; q < 3; q++, count++)
				digit[i].symbol_position[q + r] = buffer[count];
			for (int q = 0; q < 3; q++, count++)
				digit[i + 1].symbol_position[q + r] = buffer[count];
			for (int q = 0; q < 3; q++, count++)
				digit[i + 2].symbol_position[q + r] = buffer[count];
		}
	}
	for (int i = 0; i < 9; i++) mini_check(digit[i]);
	digit[0].up_position = 0;
	digit[1].up_position = 19;
	digit[2].up_position = 38;
	digit[0].down_position = 448;
	digit[1].down_position = 467;
	digit[2].down_position = 486;
	digit[3].up_position = 560;
	digit[4].up_position = 579;
	digit[5].up_position = 598;
	digit[3].down_position = 1008;
	digit[4].down_position = 1027;
	digit[5].down_position = 1046;
	digit[6].up_position = 1120;
	digit[7].up_position = 1139;
	digit[8].up_position = 1158;
	digit[6].down_position = 1568;
	digit[7].down_position = 1587;
	digit[8].down_position = 1606;
	printf("Хотите ли пройти краткое обучение перед игрой? Если да нажмите Space, иначе другую кнопку");
	int choice = _getch();
	if (choice == ' ')
	{
		choice = 0;
		system("cls");
		printf("Вам предстоит сыграть в расширенную версию игры крестики-нолики! Игроки ходят по очереди.\n");
		choice = _getch();
		printf("1. Игра начинается с центрального поля. Поставьте там нолик/крестик!\n");
		choice = _getch();
		printf("2. Противник будет делать следующий ход в том поле, в которое вы его отправили. Например, если вы поставили нолик/крестик в правом верхнем углу, игра будет продолжаться в правом верхнем поле.\n");
		choice = _getch();
		printf("3. Нельзя отправлять противника в поле, в котором он сходил свой последний ход (если ход не единственный возможный)!\n");
		choice = _getch();
		printf("4. Когда в каком-то из полей произошел выигрыш (крестики или нолики заняли три клетки по горизонтали, вертикали, диагонали), это поле считается выигрышным для крестика или нолика.\n");
		choice = _getch();
		printf("5. Если в выигрышном поле не заполнены все клетки, в нем может повторно выиграть другой игрок и забрать поле себе.\n");
		choice = _getch();
		printf("6. Существует глобальное поле. Оно заполняется соответственно, если в одном из девяти полей произошел выигрыш.\n");
		choice = _getch();
		printf("7. Игрок победил, если его символ выиграл в глобальном поле.\n");
		choice = _getch();
	}
	system("cls");
	printf("Таким образом выглядит поле игры:\n");
	remove_minus();
	show_fields();
	choice = _getch();
	system("cls");
	current_place(digit[4]);
	printf("Поле, в котором Вы либо ваш соперник ставит крестик/нолик, выделено вот так (в данном случае центральное поле):\n");
	show_fields();
	remove_minus();
	choice = _getch();
	system("cls");
	int ii = 0;//индикатор правиьного ответа
	while (1)
	{
		printf("Вы будете вводить символ, указывая его положение в выделенном поле числом от 1 до 9\n");
		choice = _getch();
		printf("Потренеруйтесь! Поставьте в указанное поле 0 в центр\n");
		current_place(digit[4]);
		show_fields();
		ii = _getch();
		if (ii == '5')
		{
			digit[4].mini_result_int[4] = 0;
			mini_check(digit[4]);
			system("cls");
			printf("Вы будете вводить символ, указывая его положение в выделенном поле числом от 1 до 9\n");
			printf("Потренеруйтесь! Поставьте в указанное поле 0 в центр\n");
			show_fields();
			digit[4].mini_result_int[4] = -1;
			mini_check(digit[4]);
			printf("\nОтличная работа!\n");
			break;
		}
		digit[4].mini_result_int[ii - 49] = 0;
		mini_check(digit[4]);
		system("cls");
		printf("Вы будете вводить символ, указывая его положение в выделенном поле числом от 1 до 9\n");
		printf("Потренеруйтесь! Поставьте в указанное поле 0 в центр\n");
		show_fields();
		digit[4].mini_result_int[ii - 49] = -1;
		mini_check(digit[4]);
		printf("\nУпс! Попробуте еще раз.\n");
		choice = _getch();
		system("cls");
	}
	remove_minus();
	printf("Начнём игру!\n");
	choice = _getch();
	system("cls");
	printf("Выберите уровени сложности:\n1 - супер легкий\n2 - легкий\n3 - средний\n4 - сложный");
	int level;
	level = _getch();
	system("cls");
	if (level >= '1' && level <= '4')
	{
		printf("Вы хотите играть за крестики(1) или за нолики(2)? ");
		krest = _getch();
		system("cls");
		while (1)
		{
			switch (krest)
			{
			case '1':
				current = 1;
				enemy_current = 0;
				break;
			case '2':
				current = 0;
				enemy_current = 1;
				break;
			default:
				printf("Вы хотите играть за крестики(1) или за нолики(2)? ");
				krest = _getch();
				system("cls");
				continue;
			}
			break;
		}
	}
	int hod = -1, previous = -1, count = 0;
	current_place(digit[4]);
	if (level == '1' || level == '2' || level == '3' || level == '4')
	{
		int e = 0;
		while (1)
		{
			switch (current)
			{
			case 0:
				for (e = 0, count = 0; e < 9; e++)
				{
					count += empty(digit[e]);
				}
				if (count == 81)
				{
					printf("\nНИЧЬЯ!\n");
					choice = _getch();
					exit(0);
				}
				current_place(digit[current_number]);
				printf("Ваш ход:\n");
				show_fields();
				show_result_field();
				hod = _getch();
				if (hod - 49 == previous && digit[hod - 49].schet < 9 && empty(digit[current_number]) != 8 && count != 80)
				{
					system("cls");
					continue;
				}
				if (hod >= '1' && hod <= '9')
				{
					if (digit[current_number].mini_result_int[hod - 49] == -1)
						digit[current_number].mini_result_int[hod - 49] = current;
					else
					{
						printf("\nНу что вы!");
						choice = _getch();
						system("cls");
						continue;
					}
					mini_check(digit[current_number]);
					remove_minus();
					digit[current_number].schet++;
					switch (result_int[current_number])
					{
					case 1:
						get_result(digit[current_number], result_digit[current_number], 'X');
						break;
					case 0:
						get_result(digit[current_number], result_digit[current_number], 'O');
						break;
					case -1:
						get_result(digit[current_number], result_digit[current_number], ' ');
					}
					system("cls");
					printf("Ваш ход:\n");
					show_fields();
					show_result_field();
					if (digit[hod - 49].schet < 9)
					{
						previous = current_number;
						current_number = hod - 49;
					}
					else if (digit[hod - 49].schet == 9)
					{
						for (int r = 1; r < 9; r++)
							if (empty(digit[(hod - 49 + r) % 9]) < 9)
							{
								previous = current_number;
								current_number = (hod - 49 + r) % 9;
								break;
							}
					}
					win_conditional(result_digit);
				}
				else
				{
					system("cls");
					continue;
				}
				while (1)
				{
					for (e = 0, count = 0; e < 9; e++)
					{
						count += empty(digit[e]);
					}
					if (count == 81)
					{
						printf("\nНИЧЬЯ!\n");
						choice = _getch();
						exit(0);
					}
					current_place(digit[current_number]);
					if (level > '2')
					{
						system("cls");
						printf("Ход противника:\n");
						show_fields();
						show_result_field();
					}
					if (level == '1') hod = rand() % 9;
					else if (level == '2') hod = hod_easy(digit[current_number], result_digit[current_number], enemy_current, previous, level);
					else if (level == '4') hod = hod_hard(digit, enemy_current, previous, current_number);
					if (level == '1' || level == '2' || level == '4')
					{
						if (hod == previous && digit[hod].schet < 9 && empty(digit[current_number]) != 8 && count != 80)
						{
							system("cls");
							continue;
						}
						if (digit[current_number].mini_result_int[hod] == -1)
							digit[current_number].mini_result_int[hod] = enemy_current;
						else continue;
					}
					if (level < '3')
					{
						system("cls");
						printf("Ход противника:\n");
						show_fields();
						show_result_field();
					}
					if (level == '3')
					{
						int check[9] = { '\0' };
						for (int i = 0; i < 9; i++)
						{
							if (digit[current_number].mini_result_int[i] == -1 && i != previous) check[i] = 1;
							else check[i] = 0;
						}
						int k = 0;
						for (int i = 0; i < 9; i++)
						{
							if (hod_easy(digit[i], result_digit[i], enemy_current, 11, level) != 10 && check[i] != 0) check[i]++;
							if (check[i] == 1) k++;
						}
						hod = hod_easy(digit[current_number], result_digit[current_number], enemy_current, previous, level);
						if (hod != 10 && check[hod] == 1) digit[current_number].mini_result_int[hod] = enemy_current;
						else if (hod != 10 && check[hod] == 2) digit[current_number].mini_result_int[hod] = enemy_current;
						else if (k)
						{
							while (check[hod] != 1) hod = rand() % 9;
							digit[current_number].mini_result_int[hod] = enemy_current;
						}
						if (hod == 10) //очень плохой случай
						{
							for (int i = 0; i < 9; i++)
							{
								if (check[i] == 2)
								{
									hod = i;
									digit[current_number].mini_result_int[hod] = enemy_current;
									break;
								}
							}
						}
						if (hod == 10) digit[current_number].mini_result_int[previous] = enemy_current;
					}
					mini_check(digit[current_number]);
					digit[current_number].schet++;
					switch (result_int[current_number])
					{
					case 1:
						get_result(digit[current_number], result_digit[current_number], 'X');
						break;
					case 0:
						get_result(digit[current_number], result_digit[current_number], 'O');
						break;
					case -1:
						get_result(digit[current_number], result_digit[current_number], ' ');
					}
					if (digit[hod].schet < 9)
					{
						previous = current_number;
						current_number = hod;
					}
					else if (digit[hod].schet == 9)
					{
						Sleep(2000);
						system("cls");
						printf("Ход противника:\n");
						show_fields();
						show_result_field();
						for (int r = 1; r < 9; r++)
							if (empty(digit[(hod + r) % 9]) < 9)
							{
								previous = current_number;
								current_number = (hod + r) % 9;
								break;
							}
					}
					Sleep(2000);
					system("cls");
					printf("Ход противника:\n");
					show_fields();
					show_result_field();
					Sleep(2000);
					system("cls");
					remove_minus();
					break;
				}
				win_conditional(result_digit);
				break;
			case 1:

				while (1)
				{
					while (1)
					{
						for (e = 0, count = 0; e < 9; e++)
						{
							count += empty(digit[e]);
						}
						if (count == 81)
						{
							printf("\nНИЧЬЯ!\n");
							choice = _getch();
							exit(0);
						}
						current_place(digit[current_number]);
						printf("Ход противника:\n");
						show_fields();
						show_result_field();
						if (level == '1') hod = rand() % 9;
						else if (level == '2') hod = hod_easy(digit[current_number], result_digit[current_number], enemy_current, previous, level);
						else if (level == '4') hod = hod_hard(digit, enemy_current, previous, current_number);
						if (level == '1' || level == '2' || level == '4')
						{
							if (hod == previous && digit[hod].schet < 9 && empty(digit[current_number]) != 8 && count != 80)
							{
								system("cls");
								continue;
							}
							if (digit[current_number].mini_result_int[hod] == -1)
								digit[current_number].mini_result_int[hod] = enemy_current;
							else continue;
						}
						if (level == '3')
						{
							int check[9] = { '\0' };
							for (int i = 0; i < 9; i++)
							{
								if (digit[current_number].mini_result_int[i] == -1 && i != previous) check[i] = 1;
								else check[i] = 0;
							}
							int k = 0;
							for (int i = 0; i < 9; i++)
							{
								if (hod_easy(digit[i], result_digit[i], enemy_current, 11, level) != 10 && check[i] != 0) check[i]++;
								if (check[i] == 1) k++;
							}
							hod = hod_easy(digit[current_number], result_digit[current_number], enemy_current, previous, level);
							if (hod != 10 && check[hod] == 1) digit[current_number].mini_result_int[hod] = enemy_current;
							else if (hod != 10 && check[hod] == 2) digit[current_number].mini_result_int[hod] = enemy_current;
							else if (k)
							{
								while (check[hod] != 1) hod = rand() % 9;
								digit[current_number].mini_result_int[hod] = enemy_current;
							}
							if (hod == 10) //очень плохой случай
							{
								for (int i = 0; i < 9; i++)
								{
									if (check[i] == 2)
									{
										hod = i;
										digit[current_number].mini_result_int[hod] = enemy_current;
										break;
									}
								}
							}
							if (hod == 10) digit[current_number].mini_result_int[previous] = enemy_current;
						}
						//system("cls");
						mini_check(digit[current_number]);
						remove_minus();
						digit[current_number].schet++;
						switch (result_int[current_number])
						{
						case 1:
							get_result(digit[current_number], result_digit[current_number], 'X');
							break;
						case 0:
							get_result(digit[current_number], result_digit[current_number], 'O');
							break;
						case -1:
							get_result(digit[current_number], result_digit[current_number], ' ');
						}
						if (digit[hod].schet < 9)
						{
							previous = current_number;
							current_number = hod;
						}
						else if (digit[hod].schet == 9)
						{
							Sleep(2000);
							system("cls");
							printf("Ход противника:\n");
							show_fields();
							show_result_field();
							for (int r = 1; r < 9; r++)
								if (empty(digit[(hod + r) % 9]) < 9)
								{
									previous = current_number;
									current_number = (hod + r) % 9;
									break;
								}
						}
						Sleep(2000);
						system("cls");
						printf("Ход противника:\n");
						show_fields();
						show_result_field();
						Sleep(2000);
						system("cls");
						break;
					}
					win_conditional(result_digit);
					while (1)
					{
						for (e = 0, count = 0; e < 9; e++)
						{
							count += empty(digit[e]);
						}
						if (count == 81)
						{
							printf("\nНИЧЬЯ!\n");
							choice = _getch();
							exit(0);
						}
						current_place(digit[current_number]);
						printf("Ваш ход:\n");
						show_fields();
						show_result_field();
						hod = _getch();
						if (hod - 49 == previous && digit[hod - 49].schet < 9 && empty(digit[current_number]) != 8 && count != 80)
						{
							system("cls");
							continue;
						}
						if (hod >= '1' && hod <= '9')
						{
							if (digit[current_number].mini_result_int[hod - 49] == -1)
								digit[current_number].mini_result_int[hod - 49] = current;
							else
							{
								printf("\nНу что вы!");
								choice = _getch();
								system("cls");
								continue;
							}
							mini_check(digit[current_number]);
							remove_minus();
							digit[current_number].schet++;
							switch (result_int[current_number])
							{
							case 1:
								get_result(digit[current_number], result_digit[current_number], 'X');
								break;
							case 0:
								get_result(digit[current_number], result_digit[current_number], 'O');
								break;
							case -1:
								get_result(digit[current_number], result_digit[current_number], ' ');
							}
							if (digit[hod - 49].schet < 9)
							{
								previous = current_number;
								current_number = hod - 49;
							}
							else if (digit[hod - 49].schet == 9)
							{
								for (int r = 1; r < 9; r++)
									if (empty(digit[(hod - 49 + r) % 9]) < 9)
									{
										previous = current_number;
										current_number = (hod - 49 + r) % 9;
										break;
									}
							}
							system("cls");
							printf("Ваш ход:\n");
							show_fields();
							show_result_field();
							Sleep(2000);
							system("cls");
							win_conditional(result_digit);
						}
						else
						{
							system("cls");
							continue;
						}
						break;
					} //win_conditional(result_digit);
				}
				break;
			}
		}
	}
	Sleep(200);
	system("cls");
	printf("Спасибо за игру! Всего доброго!");
	choice = _getch();
	return 0;
}