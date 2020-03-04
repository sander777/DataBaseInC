#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void print_author(struct Author a) {
	printf("\n%s\n%s\n%s\n", a.name, a.birthday, a.country);
}

void print_all_author(FILE* f) {
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	struct Author a;
	for(int i = 0; i < sz / sizeof(struct Author); i++) {
		read_data_author(&a, f, i);
		print_author(a);
	}
}

struct Book {
	unsigned int ID;
	char title[name_length];
	int authorID;
};

void write_data_book(struct Book b, FILE* f) {
	fseek(f, 0, SEEK_END);
	fwrite(&b, sizeof(struct Book), 1, f);
}

void change_data_book(struct Book b, FILE* f, int n) {
	fseek(f, sizeof(struct Book) * n, SEEK_SET);
	fwrite(&b, sizeof(struct Book), 1, f);
}

void read_data_book(struct Book* b, FILE* f, int n) {
	fseek(f, sizeof(struct Book) * n, SEEK_SET);
	fread(b, sizeof(struct Book), 1, f);
}

void print_book(struct Book b) {
	printf("%d\n%s\n%s\n\n",b.ID, b.title, b.authorID);
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
	FILE* index = fopen("index.txt", "a+");
	FILE* bin = fopen("bin.txt", "a+");
	print_author(get_author_by_id(1000000003, index, bin));
	//add_author(a, index, bin);


	return(0);
}