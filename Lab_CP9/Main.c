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
#define MIN_DATA_ARR_SIZE 10
#define MAX_DATA_ARR_SIZE 100
#define MAX_COUNTRY_NAME_SIZE 41
#define SQUARE_MAX 1e9
#define POPULATION_MAX 1e11

typedef unsigned short ushort;

typedef struct country_data
{
	char* name;
	float square;
	long population;
} country_data;

void create_file(char* path, bool* is_err);

ushort read_option();

void clear_the_input_buffer();

void enter_new_file(char* name);

bool validate_file(char* name, bool* cannot_open);

bool is_empty(char* name, bool* cannot_open);

// returns true if file can be opened
bool read_all_data_from_file(char* path, country_data* data, ushort* capacity, ushort* size);

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
	char* path = malloc(MAX_NAME_SIZE);

	if (path == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		return 0;
	}
	path[0] = '\0';

	ushort capacity = MIN_DATA_ARR_SIZE;
	ushort count_data = MIN_DATA_ARR_SIZE;
	country_data* data = malloc(count_data * sizeof(country_data));

	if (data == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		return 0;
	}

	do
	{
		// choosing option
		do
		{
			printf("Choose option(listed above): ");
			option = read_option();

		} while (option == NON_OPTION);
				
		bool is_err_occured = false;
		FILE* file;
		switch (option)
		{
		case CREATE_FILE:
			do
			{				
				is_err_occured = false;
				enter_new_file(path);
				bool is_err_caught = false;
				create_file(path, &is_err_caught);

				if (is_err_caught)
				{
					is_err_occured = true;
				}

			} while (is_err_occured);
			break;
		case READ_FILE:
			// func prepare_path
			if (path[0] == '\0')
			{
				bool is_valid = false, cannot_open = false;
				do
				{
					enter_new_file(path);
					is_valid = validate_file(path, &cannot_open);
				} while (!is_valid || cannot_open);
			}

			fopen_s(&file, path, "rb");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				bool is_file_opened = read_all_data_from_file(path, data, &capacity, &count_data);
				fclose(file);
			}
			break;
		case DELETE_FILE:
			if (path[0] == '\0')
			{
				bool is_valid = false, cannot_open = false;
				do
				{
					enter_new_file(path);
					is_valid = validate_file(path, &cannot_open);
				} while (!is_valid || cannot_open);
			}

			fopen_s(&file, path, "rb");

			// delete file
			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				printf("Do you really want to delete this file(all information in it will be lost)(y/n): ");
				char option = '\0';
				option = getchar();
				clear_the_input_buffer();

				if (option == 'y' || option == 'Y')
				{
					fclose(file);
					if (remove(path) == 0)
					{
						printf("File successfully deleted\n");
						path[0] = '\0';
					}
					else
					{
						printf("\nError: cannot delete file!\n");
					}
				}
				else
				{
					fclose(file);
				}
			}
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

	} while (option != EXIT_PROG);

	free(path);
	free(data);

	return 0;
}

void create_file(char* path, bool *is_err)
{
	FILE* file = NULL;

	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		fopen_s(&file, path, "wb");

		if (file != NULL)
		{
			printf("The file with .mf extention was successfully created!\n");
			char key[] = "10000";
			fwrite(key, sizeof(key[0]), (sizeof(key) / sizeof(key[0])) - 1, file);
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

void enter_new_file(char* name)
{
	int err = 0;
	do
	{
		err = 1;
		printf("Enter the name of file without file extension(max %d characters): ", MAX_NAME_SIZE - 4);
		err = scanf_s("%s", name, MAX_NAME_SIZE - 3);
		clear_the_input_buffer();

		if (err == 0)
		{
			printf("\nError: too big data entered. Try again!\n");
		}
		else
		{
			strcat_s(name, MAX_NAME_SIZE, FILE_EXT);
			err = 1;
		}

	} while (err == 0);
}

bool validate_file(char* name, bool* cannot_open)
{
	bool is_valid = false;
	*cannot_open = false;

	FILE* file;

	fopen_s(&file, name, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		*cannot_open = true;
		return is_valid;
	}

	bool is_empty_file = is_empty(name, cannot_open);

	if (*cannot_open)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		*cannot_open = true;
		return is_valid;
	}

	if (is_empty_file)
	{
		printf("\nError: this file is not created by the program, we cannot open it. Try another name!\n");
		return is_valid;
	}

	char key = '\0';

	fread(&key, sizeof(key), 1, file);

	if (key == '1')
	{
		printf("File entered successfully!\n");
		is_valid = true;
	}

	fclose(file);

	return is_valid;
}

bool is_empty(char* name, bool* cannot_open)
{
	*cannot_open = false;

	FILE* file;

	fopen_s(&file, name, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		*cannot_open = true;
		return false;
	}

	fseek(file, 0, SEEK_END);
	long pos = ftell(file);
	fclose(file);

	return pos == 0;
}

bool read_all_data_from_file(char* path, country_data* data, ushort* capacity, ushort* size)
{
	FILE* file;

	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		return false;
	}

	fseek(file, 5, SEEK_SET);
	
	char buffer[MAX_COUNTRY_NAME_SIZE];
	/*ushort i = 0;
	while (!feof(file))
	{
		fread(buffer, sizeof(char), )
		i++;
	}*/

	return true;
}
