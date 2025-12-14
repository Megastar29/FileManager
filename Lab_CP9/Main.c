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
#define MIN_DATA_ALLOC 2
#define MAX_DATA_POSSIBLE_ALLOC 100
#define MIN_COUNT_REC 1
#define MAX_COUNT_REC 100
#define MAX_COUNTRY_NAME_SIZE 41
#define SQUARE_MIN 1e-7f
#define SQUARE_MAX 1e9f
#define POPULATION_MIN 0
#define POPULATION_MAX 100000000000

#define EPS 1e-7f

typedef unsigned short ushort;
typedef unsigned long long ullong;

typedef struct country_data
{
	char* name;
	float square;
	ullong population;
} country_data;

void create_file(char* path, bool* is_err);

ushort read_option();

void clear_the_input_buffer();

void enter_new_file(char* name);

bool validate_file(char* name, bool* cannot_open);

bool is_empty(char* name, bool* cannot_open);

// returns true if file can be opened and memory allocated successfully
bool read_all_data_from_file(char* path, country_data** data, ushort* size);

bool valid_input(ushort data, ushort min, ushort max, char inv_data);

bool valid_long_input(ullong data, ullong min, ullong max, char inv_data);

void input_float(float* result, bool* is_data_valid, float min_val, float max_val);

void print_data(country_data* data, ushort size, bool print_data_stack_num);

void prepare_path(char** path);

void read_display_data_from_file(char* path, ushort start_rec, ushort end_rec);

int main()
{
	printf("This is file manager program. It stores data about districts in file. Here are the menu that shows what it can do: \n");
	printf("Menu: \n");
	printf("File commands: \t\t\tRecord commands:\n");
	printf("1 - create file \t\t4 - make new records\n");
	printf("2 - read all data from file \t5 - read data from file\n");
	printf("3 - delete file \t\t6 - edit records\n");
	printf("\t\t\t\t7 - sort records\n");
	printf("\t\t\t\t8 - insert data to file\n");
	printf("\t\t\t\t9 - delete data from file\n");
	printf("\t\t\t\t10 - exit program\n");

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
				bool is_correct_work = read_all_data_from_file(path, &data, &count_data);

				if (!is_correct_work)
				{
					printf("Press enter to exit...");
					_getch();
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
						return 0;
					}
				}

				for (ushort i = 0; i < count_rec; i++)
				{
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

				// writing to file
				fseek(file, 5, SEEK_SET);
				for (ushort i = 0; i < count_rec; i++)
				{
					size_t size_of_word = strlen(data[i].name);
					fwrite(&size_of_word, sizeof(size_of_word), 1, file);
					fwrite(data[i].name, sizeof(char), size_of_word, file);
					fwrite(&data[i].square, sizeof(data[i].square), 1, file);
					fwrite(&data[i].population, sizeof(data[i].population), 1, file);
				}

				fclose(file);
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

				read_display_data_from_file(path, start_rec_num - 1, start_rec_num + count_rec - 1);

				fclose(file);
			}
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

bool read_all_data_from_file(char* path, country_data** data, ushort* size)
{
	FILE* file;

	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		return false;
	}

	fseek(file, 5, SEEK_SET);
	
	ushort counter = 0;
	ushort allocated = MIN_DATA_ALLOC;
	*data = malloc(allocated * sizeof(country_data));
	if (*data == NULL)
	{
		printf("\nError: the memory can't be allocated!\n");
		fclose(file);
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
					fclose(file);
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
				fclose(file);
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
					fclose(file);
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
				fclose(file);
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
				fclose(file);
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

	fclose(file);

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

bool valid_long_input(ullong data, ullong min, ullong max, char inv_data)
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

void read_display_data_from_file(char* path, ushort start_rec, ushort end_rec)
{
	FILE* file;

	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("\nError: cannot open the file! The file is not in the directory or the name of the file is wrong. Make sure that the file has extention .mf\n");
		return false;
	}

	fseek(file, 5, SEEK_SET);

	ushort counter = 0;
	bool is_continue_working = true;
	do
	{
		country_data one_rec;
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
				fseek(file, sizeof(ullong), SEEK_CUR);
			}

			counter++;
		}

	} while (is_continue_working);

	if (start_rec > counter)
	{
		printf("\nError: the number of start record is bigger than the total number of records in file: %d\n", counter);
	}
	else if (counter < end_rec)
	{
		printf("\nError: the number of end record is bigger than the total number of records in file: %d\n", counter);
	}

	fclose(file);
}
