#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--TODO: Search by matching prototype
//--TODO: Delete author by id (or visibility of this)
//--??TODO: All fo Book struct
//TODO:Change print_~~~~(FILE*) functions to be dependent on index file not bin
//TODO: Menu



#define name_length 32
#define date_length 10

struct Author {
	char name[name_length];
	char birthday[date_length];
	char country[name_length];
};

void edit_author(struct Author* a) {
	printf("Input Autrhot Name: ");
	scanf("%s", (*a).name);
	printf("Input Autrhot Birhday: ");
	scanf("%s", (*a).birthday);
	printf("Input Autrhot Country: ");
	scanf("%s", (*a).country);
}

int write_data_author(struct Author a, FILE* f) {
	fseek(f, 0, SEEK_END);
	fwrite(&a, sizeof(struct Author), 1, f);
	return(ftell(f) / sizeof(struct Author));
}

void change_data_author(struct Author a, FILE* f, int n) {
	fseek(f, sizeof(struct Author) * n, SEEK_SET);
	fwrite(&a, sizeof(struct Author), 1, f);
}

void read_data_author(struct Author* a, FILE* f, int n) {
	fseek(f, sizeof(struct Author) * n, SEEK_SET);
	fread(a, sizeof(struct Author), 1, f);
}

void add_author(struct Author a, FILE* index, FILE* bin) {
	fseek(index, 0, SEEK_END);
	int sz = ftell(index);
	fseek(index, 0, SEEK_SET);
	
	char* file_str = (char*) malloc(sz);
	for(int i = 0; i < sz; i++) {
		file_str[i] = '\0';
	}
	fread(file_str, sz, 1, index);
	char *file_id_str = strtok(file_str, "\n");
	char id_str[50] = "999999999";
	if(sz != 0) {
		while(file_id_str != NULL) {
			strcpy(id_str, file_id_str);
			file_id_str = strtok(NULL, "\n");
		}
	}
	int id = atoi(id_str) + 1;
	char addr_str[15];
	char str[50];
	sprintf(id_str, "%d", id);
	sprintf(addr_str, "%d", write_data_author(a, bin) - 1);
	strcat(id_str, " ");
	strcat(id_str, addr_str);
	strcpy(str, id_str);
	strcat(str, "\n\0");
	fprintf(index, str);
}

void delete_index(int id, FILE* index, char* file_name) {
	
	fseek(index, 0, SEEK_END);
	int sz = ftell(index);
	fseek(index, 0, SEEK_SET);
	char* file_str = (char*) malloc(sz);
	for(int i = 0; i < sz; i++) {
		file_str[i] = '\0';
	}
	fread(file_str, sz, 1, index);
	for(int i = 0; i < sz; i++) {
		char idx[11];
		itoa(id, idx, 10);
		int is_same = 1;
		for(int j = 0; j < 10; j++) {
			if(idx[j] != file_str[i + j]){
				is_same = 0;
			}
		}
		
		if(is_same) {
			int j = 0;
			while (1) {
				file_str[i+j] = '-';
				j++;
				if(file_str[j] == '\n') {
					file_str[i+j] = '-';
					break;
				}
			}
		}
		else {
			while (1) {
				i++;
				if(file_str[i] == '\n') {
					break;
				}
			}
		}
	}
	fclose(index);
	fopen(file_name, "w+");
	for(int i = 0; i < sz; i++) {
		if(file_str[i] != '-' && ((file_str[i] > 47 && file_str[i] < 58) || file_str[i] == ' ' || file_str[i] == '\n')) {
			fputc(file_str[i], index);
			printf("%c", file_str[i]);
		}
	}
}

struct Author get_author_by_id(int id, FILE* index, FILE* bin) {
	struct Author* result = NULL;

	fseek(index, 0, SEEK_END);
	int sz = ftell(index);
	fseek(index, 0, SEEK_SET);
	
	char* file_str = (char*) malloc(sz);
	for(int i = 0; i < sz; i++) {
		file_str[i] = '\0';
	}
	fread(file_str, sz, 1, index);
	char *file_id_str = strtok(file_str, "\n");
	char id_str[50] = {};
	int addr;
	if(sz != 0) {
		do {
			strcpy(id_str, file_id_str);
			file_id_str = strtok(NULL, "\n");
			addr =  atoi(&id_str[10]);
		} while(atoi(id_str) != id && file_id_str != NULL);
		//TODO: warning when no ID in DataBase
		struct Author a;
		result = &a;
		read_data_author(&a, bin, addr);
	}
	else {
		printf("list is empty");
	}

	return(*result);
}

int* get_author_id_list_by_prototype(struct Author prototype, int* _sz, FILE* f) {
	fseek(f, 0,SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	*_sz = 0;
	struct Author a;
	for(int i = 0; i < sz / sizeof(struct Author); i++) {
		read_data_author(&a, f, i);
		if((strcmp(prototype.name, "!") == 0) ^ (strcmp(a.name, prototype.name) == 0) &&
		   (strcmp(prototype.birthday, "!") == 0) ^ (strcmp(a.birthday, prototype.birthday) == 0) &&
		   (strcmp(prototype.country, "!") == 0) ^ (strcmp(a.name, prototype.country) == 0)) {
			   (*_sz)++;
		   }
	}
	fseek(f, 0, SEEK_SET);
	int* arr = (int*) malloc(*_sz);
	int j = 0;
	for(int i = 0; i < sz / sizeof(struct Author); i++) {
		read_data_author(&a, f, i);
		if((strcmp(prototype.name, "!") == 0) ^ (strcmp(a.name, prototype.name) == 0) &&
		   (strcmp(prototype.birthday, "!") == 0) ^ (strcmp(a.birthday, prototype.birthday) == 0) &&
		   (strcmp(prototype.country, "!") == 0) ^ (strcmp(a.name, prototype.country) == 0)) {
			   arr[j] = 1000000000 + i;
			   j++;
		   }
	}
	return arr;
} 

void print_author(struct Author a) {
	printf("\n%s\n%s\n%s\n", a.name, a.birthday, a.country);
}

void print_all_author(FILE* f) {
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	struct Author a;
	for(int i = 0; i < sz / sizeof(struct Author); i++) {
		read_data_author(&a, f, i);
		print_author(a);
	}
}

struct Book {
	char title[name_length];
	char authorID[11];
};

void edit_book(struct Book* b) {
	printf("Input Title: ");
	scanf("%s", (*b).title);
	printf("Input AutrhotID: ");
	scanf("%i", (*b).authorID);
}

int write_data_book(struct Book b, FILE* f) {
	fseek(f, 0, SEEK_END);
	fwrite(&b, sizeof(struct Book), 1, f);
	return(ftell(f) / sizeof(struct Book));
}

void change_data_book(struct Book b, FILE* f, int n) {
	fseek(f, sizeof(struct Book) * n, SEEK_SET);
	fwrite(&b, sizeof(struct Book), 1, f);
}

void read_data_book(struct Book* b, FILE* f, int n) {
	fseek(f, sizeof(struct Book) * n, SEEK_SET);
	fread(b, sizeof(struct Book), 1, f);
}

void add_book(struct Book b, FILE* index, FILE* bin) {
	fseek(index, 0, SEEK_END);
	int sz = ftell(index);
	fseek(index, 0, SEEK_SET);
	
	char* file_str = (char*) malloc(sz);
	for(int i = 0; i < sz; i++) {
		file_str[i] = '\0';
	}
	fread(file_str, sz, 1, index);
	char *file_id_str = strtok(file_str, "\n");
	char id_str[50] = "999999999";
	if(sz != 0) {
		while(file_id_str != NULL) {
			strcpy(id_str, file_id_str);
			file_id_str = strtok(NULL, "\n");
		}
	}
	int id = atoi(id_str) + 1;
	char addr_str[15];
	char str[50];
	sprintf(id_str, "%d", id);
	sprintf(addr_str, "%d", write_data_book(b, bin) - 1);
	strcat(id_str, " ");
	strcat(id_str, addr_str);
	strcpy(str, id_str);
	strcat(str, "\n\0");
	fprintf(index, str);
}

struct Book get_book_by_id(int id, FILE* index, FILE* bin) {
	struct Book* result = NULL;

	fseek(index, 0, SEEK_END);
	int sz = ftell(index);
	fseek(index, 0, SEEK_SET);
	
	char* file_str = (char*) malloc(sz);
	for(int i = 0; i < sz; i++) {
		file_str[i] = '\0';
	}
	fread(file_str, sz, 1, index);
	char *file_id_str = strtok(file_str, "\n");
	char id_str[50] = {};
	int addr;
	if(sz != 0) {
		do {
			strcpy(id_str, file_id_str);
			file_id_str = strtok(NULL, "\n");
			addr =  atoi(&id_str[10]);
		} while(atoi(id_str) != id && file_id_str != NULL);
		//TODO: warning when no ID in DataBase
		struct Book b;
		result = &b;
		read_data_book(&b, bin, addr);
	}
	else {
		printf("list is empty");
	}

	return(*result);
}

int* get_book_id_list_by_prototype(struct Book prototype, int* _sz, FILE* f) {
	fseek(f, 0,SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	*_sz = 0;
	struct Book b;
	for(int i = 0; i < sz / sizeof(struct Book); i++) {
		read_data_book(&b, f, i);
		if((strcmp(prototype.title, "!") == 0) ^ (strcmp(b.title, prototype.title) == 0) &&
		   (strcmp(prototype.authorID, "!") == 0) ^ (strcmp(b.authorID, prototype.authorID) == 0)) {
			   (*_sz)++;
		   }
	}
	fseek(f, 0, SEEK_SET);
	int* arr = (int*) malloc(*_sz);
	int j = 0;
	for(int i = 0; i < sz / sizeof(struct Book); i++) {
		read_data_book(&b, f, i);
		if((strcmp(prototype.title, "!") == 0) ^ (strcmp(b.title, prototype.title) == 0) &&
		   (strcmp(prototype.authorID, "!") == 0) ^ (strcmp(b.authorID, prototype.authorID) == 0)) {
			   arr[j] = 1000000000 + i;
			   j++;
		   }
	}
	return arr;
} 

void print_book(struct Book b) {
	printf("%s\n%s\n\n", b.title, b.authorID);
}

void print_all_book(FILE* f) {
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	struct Book b;
	for(int i = 0; i < sz / sizeof(struct Book); i++) {
		read_data_book(&b, f, i);
		print_book(b);
	}
}

int main() {
	FILE* book_index = fopen("book_index.txt", "a+");
	while (1) {
		system("cls");
		int answer;
		printf("\t\t\tLibrary DataBase\n1)Book table\n2)Author table\n");
		scanf("%i", answer);
		if(answer == 1) {
			system("cls");
			printf("\t\t\tBookTable\n1)Add\n2)Delete\n");
			print_all_book(book_index);
		}
	}
	
}