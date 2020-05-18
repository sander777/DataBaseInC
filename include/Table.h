#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define FILE_LEN 32

struct Table {
	char _indexFile[FILE_LEN];
	char _dataFile[FILE_LEN];
	FILE* _indexFile_read;
	FILE* _indexFile_write;
	FILE* _dataFile_read;
	FILE* _dataFile_write;
	uint _size;
};


struct Table* new_Table(char indexFile[FILE_LEN], char dataFile[FILE_LEN], uint size);
void drop(struct Table *self);

int push_data(struct Table *self, void* object);
int write_data(struct Table *self, void* object, uint index);
void* read_data(struct Table *self, int n);
void add_row(struct Table *self, void* object);
void delete_by_id(struct Table *self, uint index);
void* get_by_id(struct Table *self, uint index);
int* get_id_list(struct Table *self, int* size);

#endif