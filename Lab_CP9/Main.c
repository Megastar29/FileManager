#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define NON_OPTION 0
#define CREATE_FILE 1
#define READ_FILE 2
#define DELETE_FILE 3
#define MK_RECORDS 4
#define READ_DATA 5
#define EDIT_REC 6
#define SORT_REC 7
#define INSERT_DATA 8
#define DELETE_DATA 9
#define EXIT_PROG 10

#define MAX_NAME_SIZE 25
#define FILE_EXT ".mf"

typedef unsigned short ushort;

void create_file(char* path, bool* is_err);

ushort read_option();

void clear_the_input_buffer();

int main()
{
	printf("This is file manager program. It stores data about districts in file. Here are the menu that shows what it can do: \n");
	printf("Menu: \n");
	printf("File commands: \t\tRecord commands:\n");
	printf("1 - create file \t4 - make new records\n");
	printf("2 - read from file \t5 - read data from file\n");
	printf("3 - delete file \t6 - edit records\n");
	printf("\t\t\t7 - sort records\n");
	printf("\t\t\t8 - insert data to file\n");
	printf("\t\t\t9 - delete data from file\n");
	printf("\t\t\t10 - exit program\n");

	ushort option = NON_OPTION;
	do
	{
		// choosing option
		do
		{
			printf("Choose option(listed above): ");
			option = read_option();

		} while (option == NON_OPTION);

		char* path = malloc(MAX_NAME_SIZE);
		int err = 0;
		switch (option)
		{
		case CREATE_FILE:
			do
			{
				printf("Enter the name of file without file extension(max %d characters): ", MAX_NAME_SIZE - 4);
				err = scanf_s("%s", path, MAX_NAME_SIZE - 3);
				clear_the_input_buffer();

				if (err == 0)
				{
					printf("\nError: too big data entered. Try again!\n");
				}
				else
				{
					strcat_s(path, MAX_NAME_SIZE, FILE_EXT);
					bool is_err_caught = false;
					create_file(path, &is_err_caught);

					if (is_err_caught)
					{
						err = 0;
					}
				}

			} while (err == 0);
			break;
		case READ_FILE:
			break;
		case DELETE_FILE:
			break;
		case MK_RECORDS:
			break;
		case READ_DATA:
			break;
		case EDIT_REC:
			break;
		case SORT_REC:
			break;
		case INSERT_DATA:
			break;
		case DELETE_DATA:
			break;
		case EXIT_PROG:
			break;
		default:
			printf("\nError: unhandled option caught!\n");
			break;
		}

		free(path);

	} while (option != EXIT_PROG);

	return 0;
}

void create_file(char* path, bool *is_err)
{
	FILE* file = NULL;

	fopen_s(&file, path, "r");

	if (file == NULL)
	{
		fopen_s(&file, path, "w");

		if (file != NULL)
		{
			printf("The file was successfully created!\n");
			fclose(file);
			*is_err = false;
		}
		else
		{
			printf("Error: can't create the file");
			*is_err = true;
		}
	}
	else
	{
		printf("The file is already exists\n");
	}
}

ushort read_option()
{
	ushort res = NON_OPTION;

	char temp_opt = getchar();
	switch (temp_opt)
	{
		// 1 and 10
	case '1':
		temp_opt = getchar();
		switch (temp_opt)
		{
		case '\n':
			res = CREATE_FILE;
			break;
		case '0':
			res = EXIT_PROG;
			break;
		default:
			printf("\nError: invalid option chosen. Try again!\n");
			clear_the_input_buffer();
			break;
		}
		break;
	case '2':
		res = READ_FILE;
		break;
	case '3':
		res = DELETE_FILE;
		break;
	case '4':
		res = MK_RECORDS;
		break;
	case '5':
		res = READ_DATA;
		break;
	case '6':
		res = EDIT_REC;
		break;
	case '7':
		res = SORT_REC;
		break;
	case '8':
		res = INSERT_DATA;
		break;
	case '9':
		res = DELETE_DATA;
		break;
	default:
		printf("\nError: invalid option chosen. Try again!\n");
		clear_the_input_buffer();
		break;
	}

	if (res != NON_OPTION && res != CREATE_FILE)
	{
		temp_opt = getchar();
		if (temp_opt != '\n')
		{
			printf("\nError: invalid option chosen. Try again!\n");
			res = NON_OPTION;
			clear_the_input_buffer();
		}
	}	

	return res;
}

void clear_the_input_buffer()
{
	char c = '\0';
	while ((c = getchar()) != '\n') {}
}
