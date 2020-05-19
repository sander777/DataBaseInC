#ifndef TABLE_C
#define TABLE_C

#include "Table.h"
#include <stdio.h>
#include <stdlib.h>

struct Table *new_Table(char indexFile[FILE_LEN], char dataFile[FILE_LEN],
                        uint size) {
    struct Table *result = malloc(sizeof(struct Table));
    strcpy((*result)._indexFile, indexFile);
    strcpy((*result)._dataFile, dataFile);
    (*result)._dataFile_stream = fopen(dataFile, "a+");
    (*result)._indexFile_stream = fopen(indexFile, "a+");
    (*result)._size = size;

    return result;
}

void drop(struct Table *self) {
    fclose((*self)._dataFile_stream);
    fclose((*self)._indexFile_stream);
}

int push_data(struct Table *self, void *object) {
    fseek((*self)._dataFile_stream, 0, SEEK_END);
    fwrite(object, (*self)._size, 1, (*self)._dataFile_stream);
    return (ftell((*self)._dataFile_stream) / (*self)._size);
}

int write_data(struct Table *self, void *object, uint index) {
    char *ptr = object;
    fseek((*self)._dataFile_stream, (*self)._size * index, SEEK_SET);
    for (uint i = 0; i < (*self)._size; i++) {
        fseek((*self)._dataFile_stream, 0, SEEK_CUR);
        fputc((int)(*(ptr++)), (*self)._dataFile_stream);
        fflush((*self)._dataFile_stream);
    }
    return 0;
}

void *read_data(struct Table *self, int index) {
    //fflush((*self)._dataFile_stream);
    void *result = malloc((*self)._size);
    fseek((*self)._dataFile_stream, (*self)._size * index, SEEK_SET);
    fread(result, (*self)._size, 1, (*self)._dataFile_stream);
    printf(ferror((*self)._dataFile_stream));
    return result;
}

int read_index(struct Table *self, int pos) {
    int result;
    if (fseek((*self)._indexFile_stream, (sizeof(int) * pos), SEEK_SET) != 0) {
        return -2;
    }
    fread(&result, sizeof(int), 1, (*self)._indexFile_stream);
    if (feof((*self)._indexFile_stream))
        return -2;
    return result;
}

void write_index(struct Table *self, int index, int pos) {
    char *ptr = &index;
    fseek((*self)._indexFile_stream, sizeof(int) * pos, SEEK_SET);
    for (uint i = 0; i < sizeof(int); i++) {
        fseek((*self)._indexFile_stream, 0, SEEK_CUR);
        fputc((int)(*(ptr++)), (*self)._indexFile_stream);
        fflush((*self)._indexFile_stream);
    }
}

void add_row(struct Table *self, void *object) {
    int i = 0;
    while (1) {
        int temp = read_index(self, i);
        if (temp == -2 || temp == -1)
            break;
        i++;
    }
    write_index(self, i, i);
    write_data(self, object, i);
}

void delete_by_id(struct Table *self, uint index) {
    write_index(self, -1, index);
}

int *get_id_list(struct Table *self, int *size) {
    int i = 0;
    int j = 0;
    while (1) {
        if (read_index(self, j) == -2)
            break;
        if (read_index(self, j) >= 0)
            i++;
        j++;
    }
    int *list = malloc(sizeof(int) * (i));
    *size = i;
    i = 0;
    j = 0;
    while (1) {
        int temp = read_index(self, j);
        if (temp == -2)
            break;
        if (temp >= 0) {
            list[i] = temp;
            i++;
        }
        j++;
    }

    return list;
}

#endif