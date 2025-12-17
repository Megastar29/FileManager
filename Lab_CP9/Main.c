#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

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
#define KEY_SYMBOLS 6
#define MIN_DATA_ALLOC 2
#define MAX_DATA_POSSIBLE_ALLOC 100
#define MIN_COUNT_REC 1
#define MAX_COUNT_REC 100
#define MAX_COUNTRY_NAME_SIZE 41
#define SQUARE_MIN 1e-7f
#define SQUARE_MAX 1e9f
#define POPULATION_MIN 0
#define POPULATION_MAX 100000000000
#define SORT_ASCEND '1'
#define SORT_DESCEND '2'
#define SORT_PAR_NAME '1'
#define SORT_PAR_SQUARE '2'
#define SORT_PAR_POPULAION '3'

#define EPS 1e-7f

typedef unsigned short ushort;

typedef struct country_data
{
	char* name;
	float square;
	size_t population;
} country_data;

void create_file(char* path, bool* is_err);

ushort read_option();

void clear_the_input_buffer();

void enter_new_file(char* name);

bool validate_file(char* name, bool* cannot_open);

bool is_empty(char* name, bool* cannot_open);

// returns true if file can be opened and memory allocated successfully
bool read_all_data_from_file(FILE* file, country_data** data, ushort* size);

bool valid_input(ushort data, ushort min, ushort max, char inv_data);

bool valid_long_input(size_t data, size_t min, size_t max, char inv_data);

void input_float(float* result, bool* is_data_valid, float min_val, float max_val);

void print_data(country_data* data, ushort size, bool print_data_stack_num);

void prepare_path(char** path);

bool read_display_data_from_file(char* path, ushort start_rec, ushort end_rec);

bool edit_data_in_file(FILE* file, char* path, country_data obj, ushort index);

void write_data_to_file(FILE* file, country_data* data, ushort size);

bool clear_the_file(char* path);

bool delete_data_from_file(FILE* file, char* path, ushort index);

bool sort_data_in_file(FILE* file, char* path, char option_order, char option_paremetr);

void sort_data(country_data* data, ushort size, char* key, char option_order, char option_par);

bool pred_name_asc(country_data el1, country_data el2);

bool pred_name_desc(country_data el1, country_data el2);

bool pred_square_asc(country_data el1, country_data el2);

bool pred_square_desc(country_data el1, country_data el2);

bool pred_population_asc(country_data el1, country_data el2);

bool pred_population_desc(country_data el1, country_data el2);

void swap(country_data* el1, country_data* el2);

bool insert_data_in_file(FILE* file, char* path, country_data obj, bool* is_sorted);

int main()
{
	printf("This is file manager program. It stores data about districts in file. Here are the menu that shows what it can do: \n");
	printf("Menu: \n");
	printf("File commands: \t\t\tRecord commands:\n");
	printf("1 - create file \t\t4 - make new records\n");
	printf("2 - read all data from file \t5 - read data from file\n");
	printf("3 - delete file \t\t6 - edit records\n");
	printf("\t\t\t\t7 - sort records\n");
	printf("\t\t\t\t8 - insert data to file(sorting needed)\n");
	printf("\t\t\t\t9 - delete data from file\n");
	printf("\t\t\t\t10 - exit program\n");
	printf("\nWarning: all numbers with floating point must have less than 7 digits after dot\n");

	ushort option = NON_OPTION;
	char* path = malloc(MAX_NAME_SIZE);

	if (path == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		printf("Press enter to exit...");
		_getch();
		return 0;
	}
	path[0] = '\0';

	ushort count_data = 0;
	country_data* data = NULL;

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
			prepare_path(&path);
			fopen_s(&file, path, "rb");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				if (data != NULL)
				{
					for (ushort i = 0; i < count_data; i++)
					{
						if (data[i].name != NULL)
						{
							free(data[i].name);
							data[i].name = NULL;
						}
					}

					free(data);
					data = NULL;
				}
				bool is_correct_work = read_all_data_from_file(file, &data, &count_data);

				if (!is_correct_work)
				{
					printf("Press enter to exit...");
					_getch();
					fclose(file);
					return 0;
				}

				print_data(data, count_data, true);

				fclose(file);
			}
			break;
		case DELETE_FILE:
			prepare_path(&path);
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
					printf("File deleting canceled\n");
					fclose(file);
				}
			}
			break;
		case MK_RECORDS:
			prepare_path(&path);
			fopen_s(&file, path, "ab");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				// idea: check what is in file (count written data) 
				ushort count_rec = 0;
				char inv_data = '\0';
				do
				{
					printf("Enter count of records(from %d to %d): ", MIN_COUNT_REC, MAX_COUNT_REC);
					scanf_s("%hu%c", &count_rec, &inv_data, 1);

				} while (!valid_input(count_rec, MIN_COUNT_REC, MAX_COUNT_REC, inv_data));

				if (data != NULL)
				{
					for (ushort i = 0; i < count_data; i++)
					{
						if (data[i].name != NULL)
						{
							free(data[i].name);
							data[i].name = NULL;
						}						
					}

					country_data* temp = realloc(data, sizeof(country_data) * count_rec);

					if (temp == NULL)
					{
						printf("\nError: the memory can't be allocated!\n");
						printf("Press enter to exit...");
						_getch();
						fclose(file);
						return 0;
					}
					data = temp;
					temp = NULL;
				}
				else
				{
					data = malloc(sizeof(country_data) * count_rec);
				}				
				count_data = count_rec;

				if (data == NULL)
				{
					printf("\nError: the memory can't be allocated!\n");
					printf("Press enter to exit...");
					_getch();
					fclose(file);
					return 0;
				}

				for (ushort i = 0; i < count_rec; i++)
				{
					data[i].name = malloc(MAX_COUNTRY_NAME_SIZE * sizeof(char));
					if (data[i].name == NULL)
					{
						printf("\nError: the memory can't be allocated!\n");
						printf("Press enter to exit...");
						_getch();
						fclose(file);
						return 0;
					}
				}

				for (ushort i = 0; i < count_rec; i++)
				{
					// func enter data object
					printf("Enter the name of the region #%hu (max %d characters): ", (i + 1), MAX_COUNTRY_NAME_SIZE - 1);
					int err = scanf_s("%s", data[i].name, MAX_COUNTRY_NAME_SIZE);
					clear_the_input_buffer();

					if (err == 0)
					{
						printf("\nError: too big data entered. Try again!\n");
						i--;
					}
					else
					{
						printf("Enter the square of the region #%hu (min: %f, max: %f): ", (i + 1), SQUARE_MIN, SQUARE_MAX);
						bool is_input_valid = false;
						input_float(&data[i].square, &is_input_valid, SQUARE_MIN, SQUARE_MAX);

						if (!is_input_valid)
						{
							i--;
						}
						else
						{
							inv_data = '\0';
							printf("Enter the population of the region #%hu (min: %lu, max: %llu): ", (i + 1), POPULATION_MIN, POPULATION_MAX);
							scanf_s("%llu%c", &data[i].population, &inv_data, 1);

							if (!valid_long_input(data[i].population, POPULATION_MIN, POPULATION_MAX, inv_data))
							{
								i--;
							}
						}
					}
				}

				fclose(file);
				fopen_s(&file, path, "r+b");
				if (file == NULL)
				{
					printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
				}
				else
				{
					fseek(file, 0, SEEK_SET);
					char* key = "100000";
					fwrite(key, sizeof(char), KEY_SYMBOLS, file);

					fclose(file);
					fopen_s(&file, path, "ab");
					if (file == NULL)
					{
						printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
					}
					else
					{
						write_data_to_file(file, data, count_rec);

						fclose(file);
					}
				}
			}			
			
			break;
		case READ_DATA:
			prepare_path(&path);
			fopen_s(&file, path, "rb");

			// read data from file
			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				ushort start_rec_num = 0;
				char inv_data = '\0';
				do
				{
					printf("Enter the number of record to start read from file(from %d to %d): ", MIN_COUNT_REC, MAX_COUNT_REC);
					scanf_s("%hu%c", &start_rec_num, &inv_data, 1);

				} while (!valid_input(start_rec_num, MIN_COUNT_REC, MAX_COUNT_REC, inv_data));

				ushort count_rec = 0;
				inv_data = '\0';
				if (MAX_COUNT_REC != start_rec_num)
				{
					do
					{
						printf("Enter counts of records to read from starting record(from %d to %d): ", MIN_COUNT_REC, MAX_COUNT_REC - start_rec_num);
						scanf_s("%hu%c", &count_rec, &inv_data, 1);

					} while (!valid_input(count_rec, MIN_COUNT_REC, MAX_COUNT_REC - start_rec_num, inv_data));
				}

				if (!read_display_data_from_file(path, start_rec_num - 1, start_rec_num + count_rec - 1))
				{
					printf("Press enter to exit...");
					_getch();
					fclose(file);
					return 0;
				}

				fclose(file);
			}
			break;
		case EDIT_REC:
			prepare_path(&path);
			fopen_s(&file, path, "r+b");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				ushort change_rec_num = 0;
				char inv_data = '\0';
				do
				{
					printf("Enter the number of record to change(from %d to %d): ", MIN_COUNT_REC, MAX_COUNT_REC);
					scanf_s("%hu%c", &change_rec_num, &inv_data, 1);

				} while (!valid_input(change_rec_num, MIN_COUNT_REC, MAX_COUNT_REC, inv_data));

				country_data temp;
				temp.name = NULL;

				temp.name = malloc(MAX_COUNTRY_NAME_SIZE * sizeof(char));
				if (temp.name == NULL)
				{
					printf("\nError: the memory can't be allocated!\n");
					printf("Press enter to exit...");
					fclose(file);
					_getch();
					return 0;
				}

				bool is_data_input_correct = true;
				do
				{
					is_data_input_correct = true;
					printf("Enter the name of the region #%hu (max %d characters): ", change_rec_num, MAX_COUNTRY_NAME_SIZE - 1);
					int err = scanf_s("%s", temp.name, MAX_COUNTRY_NAME_SIZE);
					clear_the_input_buffer();

					if (err == 0)
					{
						printf("\nError: too big data entered. Try again!\n");
						is_data_input_correct = false;
					}
					else
					{
						printf("Enter the square of the region #%hu (min: %f, max: %f): ", change_rec_num, SQUARE_MIN, SQUARE_MAX);
						bool is_input_valid = false;
						input_float(&temp.square, &is_input_valid, SQUARE_MIN, SQUARE_MAX);

						if (!is_input_valid)
						{
							is_data_input_correct = false;
						}
						else
						{
							inv_data = '\0';
							printf("Enter the population of the region #%hu (min: %lu, max: %llu): ", change_rec_num, POPULATION_MIN, POPULATION_MAX);
							scanf_s("%llu%c", &temp.population, &inv_data, 1);

							if (!valid_long_input(temp.population, POPULATION_MIN, POPULATION_MAX, inv_data))
							{
								is_data_input_correct = false;
							}
						}
					}
				} while (!is_data_input_correct);

				if (!edit_data_in_file(file, path, temp, change_rec_num - 1))
				{
					printf("Press enter to exit...");
					_getch();
					fclose(file);
					return 0;
				}

				if (temp.name != NULL)
				{
					free(temp.name);
				}

				fclose(file);
			}
			break;
		case SORT_REC:
			prepare_path(&path);
			fopen_s(&file, path, "r+b");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{
				char sort_option_order = '\0';
				bool is_correct = false;
				do
				{
					printf("Choose the order to sort(1 - by ascending, 2 - by descending): ");
					sort_option_order = getchar();
					clear_the_input_buffer();

					if (sort_option_order == SORT_ASCEND || sort_option_order == SORT_DESCEND)
					{
						is_correct = true;
					}
					else
					{
						printf("\nError: invalid option chosen. Try again!\n");
						is_correct = false;
					}

				} while (!is_correct);

				char sort_option_par = '\0';
				is_correct = false;
				do
				{
					printf("Choose the sort parametr(1 - name, 2 - square, 3 - population): ");
					sort_option_par = getchar();
					clear_the_input_buffer();

					if (sort_option_par == SORT_PAR_NAME || sort_option_par == SORT_PAR_SQUARE || sort_option_par == SORT_PAR_POPULAION)
					{
						is_correct = true;
					}
					else
					{
						printf("\nError: invalid option chosen. Try again!\n");
						is_correct = false;
					}

				} while (!is_correct);

				if (!sort_data_in_file(file,path, sort_option_order, sort_option_par))
				{
					printf("Press enter to exit...");
					fclose(file);
					_getch();
					return 0;
				}

				fclose(file);
			}
			break;
		case INSERT_DATA:
			prepare_path(&path);
			fopen_s(&file, path, "r+b");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else
			{				
				char inv_data = '\0';
				country_data temp;
				temp.name = NULL;

				temp.name = malloc(MAX_COUNTRY_NAME_SIZE * sizeof(char));
				if (temp.name == NULL)
				{
					printf("\nError: the memory can't be allocated!\n");
					printf("Press enter to exit...");
					_getch();
					return 0;
				}

				bool is_data_input_correct = true;
				do
				{
					is_data_input_correct = true;
					printf("Enter the name of the region #n (max %d characters): ", MAX_COUNTRY_NAME_SIZE - 1);
					int err = scanf_s("%s", temp.name, MAX_COUNTRY_NAME_SIZE);
					clear_the_input_buffer();

					if (err == 0)
					{
						printf("\nError: too big data entered. Try again!\n");
						is_data_input_correct = false;
					}
					else
					{
						printf("Enter the square of the region #n (min: %f, max: %f): ", SQUARE_MIN, SQUARE_MAX);
						bool is_input_valid = false;
						input_float(&temp.square, &is_input_valid, SQUARE_MIN, SQUARE_MAX);

						if (!is_input_valid)
						{
							is_data_input_correct = false;
						}
						else
						{
							inv_data = '\0';
							printf("Enter the population of the region #n (min: %lu, max: %llu): ", POPULATION_MIN, POPULATION_MAX);
							scanf_s("%llu%c", &temp.population, &inv_data, 1);

							if (!valid_long_input(temp.population, POPULATION_MIN, POPULATION_MAX, inv_data))
							{
								is_data_input_correct = false;
							}
						}
					}
				} while (!is_data_input_correct);

				bool is_sorted = false;
				bool result = insert_data_in_file(file, path, temp, &is_sorted);
				if (!is_sorted)
				{
					printf("\nError: data in file is unsorted. Sort it first!\n");
				}
				else if (!result)
				{
					printf("Press enter to exit...");
					fclose(file);
					_getch();
					return 0;
				}

				fclose(file);
			}
			break;
		case DELETE_DATA:
			prepare_path(&path);
			fopen_s(&file, path, "r+b");

			if (file == NULL)
			{
				printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
			}
			else 
			{
				ushort delete_rec_num = 0;
				char inv_data = '\0';
				do
				{
					printf("Enter the number of record to delete(from %d to %d): ", MIN_COUNT_REC, MAX_COUNT_REC);
					scanf_s("%hu%c", &delete_rec_num, &inv_data, 1);

				} while (!valid_input(delete_rec_num, MIN_COUNT_REC, MAX_COUNT_REC, inv_data));

				if (!delete_data_from_file(file, path, delete_rec_num - 1))
				{
					printf("Press enter to exit...");
					_getch();
					return 0;
				}

				fclose(file);
			}
			break;
		case EXIT_PROG:
			break;
		default:
			printf("\nError: unhandled option caught!\n");
			break;
		}

	} while (option != EXIT_PROG);

	free(path);
	path = NULL;
	if (data != NULL)
	{
		for (ushort i = 0; i < count_data; i++)
		{
			if (data[i].name != NULL)
			{
				free(data[i].name);
			}
		}

		free(data);
		data = NULL;
	}

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
			char key[] = "100000";
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

bool read_all_data_from_file(FILE* file, country_data** data, ushort* size)
{
	fseek(file, KEY_SYMBOLS, SEEK_SET);
	
	ushort counter = 0;
	ushort allocated = MIN_DATA_ALLOC;
	*data = malloc(allocated * sizeof(country_data));
	if (*data == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		return false;
	}

	// read loop stops when cannot read a valid name_length
	bool is_continue_working = true;
	do
	{
		size_t name_length = 0;
		if (fread(&name_length, sizeof(name_length), 1, file) != 1)
		{			
			is_continue_working = false;
		}

		if (is_continue_working)
		{
			if (counter >= allocated)
			{
				allocated *= 2;
				if (allocated > MAX_DATA_POSSIBLE_ALLOC)
				{
					allocated = MAX_DATA_POSSIBLE_ALLOC;
				}

				country_data* temp = realloc(*data, allocated * sizeof(country_data));
				if (temp == NULL)
				{
					printf("\nError: the memory can't be allocated!\n");
					for (ushort i = 0; i < counter; i++)
					{
						free((*data)[i].name);
					}
					free(*data);
					*data = NULL;
					return false;
				}

				*data = temp;
				temp = NULL;
			}

			(*data)[counter].name = malloc((name_length + 1) * sizeof(char));
			if ((*data)[counter].name == NULL)
			{
				printf("\nError: the memory can't be allocated!\n");
				for (ushort i = 0; i < counter; i++)
				{
					free((*data)[i].name);
				}
				free(*data);
				*data = NULL;
				return false;
			}

			if (name_length > 0)
			{
				// reading region name
				if (fread((*data)[counter].name, sizeof(char), name_length, file) != name_length)
				{
					printf("\nError: data is corrupted (name read failed)!\n");
					free((*data)[counter].name);
					for (ushort i = 0; i < counter; i++)
					{
						free((*data)[i].name);
					}
					free(*data);
					*data = NULL;
					return false;
				}
			}
			(*data)[counter].name[name_length] = '\0';

			// reading square
			if (fread(&(*data)[counter].square, sizeof((*data)[counter].square), 1, file) != 1)
			{
				printf("\nError: data is corrupted (square read failed)!\n");
				free((*data)[counter].name);
				for (ushort i = 0; i < counter; i++)
				{
					free((*data)[i].name);
				}
				free(*data);
				*data = NULL;
				return false;
			}

			// reading population
			if (fread(&(*data)[counter].population, sizeof((*data)[counter].population), 1, file) != 1)
			{
				printf("\nError: data is corrupted (population read failed)!\n");
				free((*data)[counter].name);
				for (ushort i = 0; i < counter; i++)
				{
					free((*data)[i].name);
				}
				free(*data);
				*data = NULL;
				return false;
			}

			counter++;
		}
	} while (is_continue_working);

	if (counter == 0)
	{
		printf("The file has no data inside.\n");
	}

	*size = counter;

	return true;
}

bool valid_input(ushort data, ushort min, ushort max, char inv_data)
{
	bool res = false;

	if (inv_data != '\n')
	{
		printf("\nError: NOT a number has been inputed. Try again!\n");

		clear_the_input_buffer();
	}
	else
	{
		if (data >= min && data <= max)
		{
			res = true;
		}
		else
		{
			printf("\nError: the number is out of range. Try again!\n");
		}
	}

	return res;
}

bool valid_long_input(size_t data, size_t min, size_t max, char inv_data)
{
	bool res = false;

	if (inv_data != '\n')
	{
		printf("\nError: NOT a number has been inputed. Try again!\n");

		clear_the_input_buffer();
	}
	else
	{
		if (data >= min && data <= max)
		{
			res = true;
		}
		else
		{
			printf("\nError: the number is out of range. Try again!\n");
		}
	}

	return res;
}

void input_float(float* result, bool* is_data_valid, float min_val, float max_val)
{
	*result = 0.0;
	// + - 12345678.1234567 \n \0
	char buffer[21];

	fgets(buffer, sizeof(buffer), stdin);

	// buffer overflow error handler
	ushort buffer_size = (ushort)strlen(buffer);
	if (buffer[buffer_size - 1] != '\n')
	{
		printf("\nError: too big data entered. The buffer overflow has occured. Try to enter data again!\n");
		clear_the_input_buffer();
		*is_data_valid = false;
		return;
	}

	// nothing inputed error handler
	if (buffer_size == 1)
	{
		printf("\nError: nothing has been entered. Try again!\n");
		*is_data_valid = false;
		return;
	}

	char* data_after_number = NULL;
	float number = strtof(buffer, &data_after_number);

	if (data_after_number[0] != '\n')
	{
		printf("\nError: not a number has been put. Try again!\n");
		*is_data_valid = false;
		return;
	}

	// boundary validation
	if ((number - min_val < EPS) || (number - max_val > EPS))
	{
		printf("\nError: the element is out of range. Try again!\n");
		*is_data_valid = false;
		return;
	}

	// 7 digits after . validation
	// case with e: 1e-10

	char* e_p = strchr(buffer, 'e');
	char* E_p = strchr(buffer, 'E');
	char* pdot = strchr(buffer, '.');

	if (E_p != NULL)
	{
		buffer[E_p - buffer] = 'e';
		e_p = E_p;
	}

	if (e_p != NULL && pdot == NULL)
	{
		// - x1 x2 \0
		char after_e_number[4];

		ushort e_index = e_p - buffer;
		ushort slash_n_index = strchr(buffer, '\n') - buffer;

		snprintf(after_e_number, sizeof(after_e_number), "%.*s", slash_n_index - e_index - 1, buffer + e_index + 1);

		ushort after_e_number_i = abs(atoi(after_e_number));

		if (after_e_number_i > 7)
		{
			printf("\nError: the number has more than 7 digits after dot. Try to enter again!\n");
			*is_data_valid = false;
			return;
		}
	}
	// non e case: 2.123456789
	else if (e_p == NULL && pdot != NULL)
	{
		ushort dot_index = pdot - buffer;
		bool valid = false;

		for (ushort i = dot_index + 1; i < (dot_index + 9); i++)
		{
			if (!valid)
			{
				if (buffer[i] == '\n')
				{
					valid = true;
				}
			}
		}

		if (!valid)
		{
			printf("\nError: the number has more than 7 digits after dot. Try to enter again!\n");
			*is_data_valid = false;
			return;
		}
	}
	// e case and dot case: 1.0000001e-5
	else if (e_p != NULL && pdot != NULL)
	{
		ushort dot_index = pdot - buffer;
		ushort e_index = e_p - buffer;

		ushort digits_of_number = 0;

		digits_of_number += (e_index - dot_index - 2);

		char after_e_number[4];
		ushort slash_n_index = strchr(buffer, '\n') - buffer;

		snprintf(after_e_number, sizeof(after_e_number), "%.*s", slash_n_index - e_index - 1, buffer + e_index + 1);

		digits_of_number += abs(atoi(after_e_number));

		if (digits_of_number > 7)
		{
			printf("\nError: the number has more than 7 digits after dot. Try to enter again!\n");
			*is_data_valid = false;
			return;
		}
	}

	*is_data_valid = true;
	*result = number;
}

void print_data(country_data* data, ushort size, bool print_data_stack_num)
{
	for (ushort i = 0; i < size; i++)
	{
		if (print_data_stack_num)
		{
			printf("Data stack #%hu\n", i + 1);
		}		
		printf("Region: %s\n", data[i].name);
		printf("Square: %f\n", data[i].square);
		printf("Population: %llu\n", data[i].population);
		printf("\n");
	}
}

void prepare_path(char** path)
{
	if ((*path)[0] == '\0')
	{
		bool is_valid = false, cannot_open = false;
		do
		{
			enter_new_file(*path);
			is_valid = validate_file(*path, &cannot_open);
		} while (!is_valid || cannot_open);
	}
}

bool read_display_data_from_file(char* path, ushort start_rec, ushort end_rec)
{
	FILE* file;

	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		return false;
	}

	fseek(file, KEY_SYMBOLS, SEEK_SET);

	ushort counter = 0;
	bool is_continue_working = true;
	country_data one_rec;
	one_rec.name = NULL;
	do
	{		
		size_t name_length = 0;
		if (fread(&name_length, sizeof(name_length), 1, file) != 1)
		{
			is_continue_working = false;
		}

		if (is_continue_working)
		{
			if (counter >= start_rec && counter <= end_rec)
			{
				one_rec.name = malloc((name_length + 1) * sizeof(char));
				if (one_rec.name == NULL)
				{
					printf("\nError: the memory can't be allocated!\n");
					free(one_rec.name);
					one_rec.name = NULL;
					fclose(file);
					return false;
				}

				if (name_length > 0)
				{
					// reading region name
					if (fread(one_rec.name, sizeof(char), name_length, file) != name_length)
					{
						printf("\nError: data is corrupted (name read failed)!\n");
						free(one_rec.name);
						one_rec.name = NULL;
						fclose(file);
						return false;
					}
				}
				one_rec.name[name_length] = '\0';

				// reading square
				if (fread(&one_rec.square, sizeof(one_rec.square), 1, file) != 1)
				{
					printf("\nError: data is corrupted (square read failed)!\n");
					free(one_rec.name);
					one_rec.name = NULL;
					fclose(file);
					return false;
				}

				// reading population
				if (fread(&one_rec.population, sizeof(one_rec.population), 1, file) != 1)
				{
					printf("\nError: data is corrupted (population read failed)!\n");
					free(one_rec.name);
					one_rec.name = NULL;
					fclose(file);
					return false;
				}

				printf("Data stack #%hu\n", counter + 1);
				print_data(&one_rec, 1, false);
			}
			else
			{
				// skip bytes
				fseek(file, name_length * sizeof(char), SEEK_CUR);
				fseek(file, sizeof(float), SEEK_CUR);
				fseek(file, sizeof(size_t), SEEK_CUR);
			}

			counter++;
		}

	} while (is_continue_working);

	if (start_rec > counter)
	{
		printf("\Warning: the number of start record is bigger than the total number of records in file: %d\n", counter);
	}
	else if (counter < end_rec)
	{
		printf("\Warning: the number of end record is bigger than the total number of records in file: %d\n", counter);
	}

	if (one_rec.name != NULL)
	{
		free(one_rec.name);
	}

	fclose(file);

	return true;
}

bool edit_data_in_file(FILE* file, char* path, country_data obj, ushort index)
{
	char* key = malloc((KEY_SYMBOLS + 1) * sizeof(char));
	if (key == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		return false;
	}

	fread(key, sizeof(char), KEY_SYMBOLS, file);
	key[KEY_SYMBOLS] = '\0';
	country_data* data_from_file;
	ushort size = 0;

	if (!read_all_data_from_file(file, &data_from_file, &size))
	{
		free(key);
		return false;
	}

	if (index >= size)
	{
		printf("\nError: index out of range.\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	ushort new_len = strlen(obj.name);
	char* new_name = malloc(new_len + 1);
	if (new_name == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}
	memcpy(new_name, obj.name, new_len + 1);

	// reassigning new name
	free(data_from_file[index].name);
	data_from_file[index].name = new_name;
	data_from_file[index].square = obj.square;
	data_from_file[index].population = obj.population;

	fclose(file);

	if (!clear_the_file(path))
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	fopen_s(&file, path, "ab");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	fwrite(key, sizeof(char), KEY_SYMBOLS, file);
	write_data_to_file(file, data_from_file, size);

	if (data_from_file != NULL)
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}

		free(data_from_file);
		data_from_file = NULL;
	}

	free(key);
	fclose(file);

	printf("Data edited correctly\n");

	return true;
}

void write_data_to_file(FILE* file, country_data* data, ushort size)
{
	fseek(file, KEY_SYMBOLS, SEEK_SET);
	for (ushort i = 0; i < size; i++)
	{
		size_t size_of_word = strlen(data[i].name);
		fwrite(&size_of_word, sizeof(size_of_word), 1, file);
		fwrite(data[i].name, sizeof(char), size_of_word, file);
		fwrite(&data[i].square, sizeof(data[i].square), 1, file);
		fwrite(&data[i].population, sizeof(data[i].population), 1, file);
	}
}

bool clear_the_file(char* path)
{
	FILE* file;

	fopen_s(&file, path, "wb");

	if (file == NULL)
	{
		printf("\nError: cannot clear the file!\n");
		return false;
	}

	fclose(file);

	return true;
}

bool delete_data_from_file(FILE* file, char* path, ushort index)
{
	char* key = malloc(KEY_SYMBOLS * sizeof(char));
	if (key == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		return false;
	}

	fread(key, sizeof(char), KEY_SYMBOLS, file);
	country_data* data_from_file;
	ushort size = 0;

	if (!read_all_data_from_file(file, &data_from_file, &size))
	{
		free(key);
		return false;
	}

	if (index >= size)
	{
		printf("\nError: index out of range.\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	fclose(file);

	if (!clear_the_file(path))
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	fopen_s(&file, path, "ab");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		data_from_file = NULL;
		free(key);
		return false;
	}

	fwrite(key, sizeof(char), KEY_SYMBOLS, file);

	for (ushort i = 0; i < size; i++)
	{
		if (i != index)
		{
			size_t size_of_word = strlen(data_from_file[i].name);
			fwrite(&size_of_word, sizeof(size_of_word), 1, file);
			fwrite(data_from_file[i].name, sizeof(char), size_of_word, file);
			fwrite(&data_from_file[i].square, sizeof(data_from_file[i].square), 1, file);
			fwrite(&data_from_file[i].population, sizeof(data_from_file[i].population), 1, file);
		}		
	}

	if (data_from_file != NULL)
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}

		free(data_from_file);
		data_from_file = NULL;
	}

	free(key);
	fclose(file);

	printf("Data deleted correctly\n");

	return true;
}

bool sort_data_in_file(FILE* file, char* path, char option_order, char option_paremetr)
{
	fseek(file, KEY_SYMBOLS, SEEK_SET);
	country_data* data_from_file;
	ushort size = 0;

	if (!read_all_data_from_file(file, &data_from_file, &size))
	{
		return false;
	}

	// x1x2x3x4x5 + \0
	char* key = malloc((KEY_SYMBOLS + 1) * sizeof(char));
	if (key == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		fclose(file);
		return false;
	}

	sort_data(data_from_file, size, key, option_order, option_paremetr);

	fclose(file);

	if (!clear_the_file(path))
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		free(key);
		fclose(file);
		data_from_file = NULL;
		return false;
	}

	fopen_s(&file, path, "ab");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}
		free(data_from_file);
		free(key);		
		data_from_file = NULL;
		return false;
	}
	
	// new key written
	fwrite(key, sizeof(char), KEY_SYMBOLS, file);
	write_data_to_file(file, data_from_file, size);

	if (data_from_file != NULL)
	{
		for (ushort i = 0; i < size; i++)
		{
			free(data_from_file[i].name);
			data_from_file[i].name = NULL;
		}

		free(data_from_file);
		data_from_file = NULL;
	}

	if (key != NULL)
	{
		free(key);
	}

	fclose(file);

	printf("Data sorted successfully\n");

	return true;
}

void sort_data(country_data* data, ushort size,	char* key, char option_order, char option_par)
{
	bool (*pred)(country_data el1, country_data el2);

	switch (option_par)
	{
	case SORT_PAR_NAME:
		switch (option_order)
		{
		case SORT_ASCEND:
			pred = pred_name_asc;
			strncpy_s(key, KEY_SYMBOLS + 1, "111000", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		case SORT_DESCEND:
			pred = pred_name_desc;
			strncpy_s(key, KEY_SYMBOLS + 1, "111001", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		default:
			printf("\nError: unexpected option_parametr caught\n");
			return;
			break;
		}
		break;
	case SORT_PAR_SQUARE:
		switch (option_order)
		{
		case SORT_ASCEND:
			pred = pred_square_asc;
			strncpy_s(key, KEY_SYMBOLS + 1, "110100", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		case SORT_DESCEND:
			pred = pred_square_desc;
			strncpy_s(key, KEY_SYMBOLS + 1, "110101", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		default:
			printf("\nError: unexpected option_parametr caught\n");
			return;
			break;
		}
		break;
	case SORT_PAR_POPULAION:
		switch (option_order)
		{
		case SORT_ASCEND:
			pred = pred_population_asc;
			strncpy_s(key, KEY_SYMBOLS + 1, "110010", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		case SORT_DESCEND:
			pred = pred_population_desc;
			strncpy_s(key, KEY_SYMBOLS + 1, "110011", KEY_SYMBOLS);
			key[KEY_SYMBOLS] = '\0';
			break;
		default:
			printf("\nError: unexpected option_parametr caught\n");
			return;
			break;
		}
		break;
	default:
		printf("\nError: unexpected option_parametr caught\n");
		return;
		break;
	}

	for (ushort i = 0; i < size - 1; i++)
	{
		for (ushort j = 0; j < size - i - 1; j++)
		{
			if ((*pred)(data[j], data[j + 1]))
			{
				swap(&data[j], &data[j + 1]);
			}
		}
	}
}

bool pred_name_asc(country_data el1, country_data el2)
{
	if (strcmp(el1.name, el2.name) > 0)
	{
		return true;
	}

	return false;
}

bool pred_name_desc(country_data el1, country_data el2)
{
	return !(pred_name_asc(el1, el2));
}

bool pred_square_asc(country_data el1, country_data el2)
{
	if (el1.square - el2.square > EPS)
	{
		return true;
	}

	return false;
}

bool pred_square_desc(country_data el1, country_data el2)
{
	return !(pred_square_asc(el1, el2));
}

bool pred_population_asc(country_data el1, country_data el2)
{
	if (el1.population > el2.population)
	{
		return true;
	}

	return false;
}

bool pred_population_desc(country_data el1, country_data el2)
{
	return !(pred_population_asc(el1, el2));
}

void swap(country_data* el1, country_data* el2)
{
	country_data temp;
	temp.name = (*el1).name;
	temp.square = (*el1).square;
	temp.population = (*el1).population;

	(*el1).name = (*el2).name;
	(*el1).square = (*el2).square;
	(*el1).population = (*el2).population;

	(*el2).name = temp.name;
	(*el2).square = temp.square;
	(*el2).population = temp.population;
}

bool insert_data_in_file(FILE* file, char* path, country_data obj, bool* is_sorted)
{
    fseek(file, 1, SEEK_SET);
    char sort_key;
	fread(&sort_key, sizeof(char), 1, file);

    if (sort_key != '1')
    {
        *is_sorted = false;
        return false;
    }
	else
	{
		*is_sorted = true;
		fseek(file, 0, SEEK_SET);
		char* key = malloc((KEY_SYMBOLS + 1) * sizeof(char));
		if (key == NULL)
		{
			printf("\nError: the memory can't be allocated!\n");
			return false;
		}

		fread(key, sizeof(char), KEY_SYMBOLS, file);
		key[KEY_SYMBOLS] = '\0';

		country_data* data_from_file = NULL;
		ushort size = 0;
		if (!read_all_data_from_file(file, &data_from_file, &size))
		{
			free(key);
			return false;
		}

		ushort insert_index = 0;
		bool is_last = false;
		if (key[2] == '1')
		{
			if (key[5] == '0')
			{
				is_last = true;
				for (ushort i = 0; i < size - 1; i++)
				{
					if (strcmp(obj.name, data_from_file[i].name) >= 0 && 
						strcmp(obj.name, data_from_file[i + 1].name) <= 0)
					{
						insert_index = i + 1;
						is_last = false;
					}
				}

				if (is_last)
				{
					insert_index = size - 1;
				}
			}
			else
			{
				is_last = true;
				for (ushort i = 0; i < size - 1; i++)
				{
					if (strcmp(data_from_file[i].name, obj.name) >= 0 &&
						strcmp(data_from_file[i + 1].name, obj.name) <= 0)
					{
						insert_index = i + 1;
						is_last = false;
					}
				}

				if (is_last)
				{
					insert_index = size - 1;
				}
			}
		}
		else if (key[3] == '1')
        {
			if (key[5] == '0')
			{
				is_last = true;
				for (ushort i = 0; i < size - 1; i++)
				{
					if (obj.square - data_from_file[i].square >= EPS &&
						data_from_file[i + 1].square - obj.square >= EPS)
					{
						insert_index = i + 1;
						is_last = false;
        }
				}

				if (is_last)
        {
					insert_index = size - 1;
        }

				if (is_last)
				{
					insert_index = size - 1;
				}
			}
		}
		else if (key[4] == '1')
		{
			if (key[5] == '0')
			{
				is_last = true;
				for (ushort i = 0; i < size - 1; i++)
				{
					if (obj.square >= data_from_file[i].square &&
						data_from_file[i + 1].square >= obj.square)
					{
						insert_index = i + 1;
						is_last = false;
					}
				}

				if (is_last)
				{
					insert_index = size - 1;
				}
			}
		}
        else
        {
			is_last = true;
			for (ushort i = 0; i < size - 1; i++)
			{
				if (obj.square <= data_from_file[i].square &&
					data_from_file[i + 1].square <= obj.square)
				{
					insert_index = i + 1;
					is_last = false;
				}
			}

			if (is_last)
			{
				insert_index = size - 1;
			}
		
		}

    fclose(file);

    if (!clear_the_file(path))
    {
        for (ushort i = 0; i < size; i++) 
		{ 
			free(data_from_file[i].name); 
		}
        free(data_from_file);
        free(key);
        return false;
    }

    fopen_s(&file, path, "ab");
    if (file == NULL)
    {
        printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
        for (ushort i = 0; i < size; i++) 
		{
			free(data_from_file[i].name);
		}
        free(data_from_file);
        free(key);
        return false;
    }

    fwrite(key, sizeof(char), KEY_SYMBOLS, file);

    // write elements: [0, insert_index)
    for (ushort i = 0; i < insert_index; i++)
    {
        size_t size_of_word = strlen(data_from_file[i].name);
        fwrite(&size_of_word, sizeof(size_of_word), 1, file);
        fwrite(data_from_file[i].name, sizeof(char), size_of_word, file);
        fwrite(&data_from_file[i].square, sizeof(data_from_file[i].square), 1, file);
        fwrite(&data_from_file[i].population, sizeof(data_from_file[i].population), 1, file);
    }

    // write new object
    size_t size_of_obj_name = strlen(obj.name);
    fwrite(&size_of_obj_name, sizeof(size_of_obj_name), 1, file);
    fwrite(obj.name, sizeof(char), size_of_obj_name, file);
    fwrite(&obj.square, sizeof(obj.square), 1, file);
    fwrite(&obj.population, sizeof(obj.population), 1, file);

    // write remaining elements: [insert_index, size)
    for (ushort i = insert_index; i < size; i++)
    {
        size_t size_of_word = strlen(data_from_file[i].name);
        fwrite(&size_of_word, sizeof(size_of_word), 1, file);
        fwrite(data_from_file[i].name, sizeof(char), size_of_word, file);
        fwrite(&data_from_file[i].square, sizeof(data_from_file[i].square), 1, file);
        fwrite(&data_from_file[i].population, sizeof(data_from_file[i].population), 1, file);
    }

		if (is_last)
		{
			size_t size_of_obj_name = strlen(obj.name);
			fwrite(&size_of_obj_name, sizeof(size_of_obj_name), 1, file);
			fwrite(obj.name, sizeof(char), size_of_obj_name, file);
			fwrite(&obj.square, sizeof(obj.square), 1, file);
			fwrite(&obj.population, sizeof(obj.population), 1, file);
		}

		if (data_from_file != NULL)
		{
    for (ushort i = 0; i < size; i++)
    {
        free(data_from_file[i].name);
        data_from_file[i].name = NULL;
    }
    free(data_from_file);
    data_from_file = NULL;

    free(key);
    fclose(file);

    printf("Data inserted correctly\n");

    return true;
}
