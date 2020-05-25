#ifndef STRCUTS_H
#define STRCUTS_H

#include <stdio.h>
#include <stdlib.h>

#define name_length 32
#define date_length 10

struct Author {
	char name[name_length];
	char birthday[date_length];
	char country[name_length];
	char surname[name_length];
};

void edit_author(struct Author* a) {
	printf("Input Autrhot Name: ");
	scanf("%s", (*a).name);
	printf("Input Autrhot Surame: ");
	scanf("%s", (*a).surname);
	printf("Input Autrhot Birhday: ");
	scanf("%s", (*a).birthday);
	printf("Input Autrhot Country: ");
	scanf("%s", (*a).country);
}

void print_author(struct Author a) {
	printf("\n%s\n%s\n%s\n", a.name, a.birthday, a.country);
}

struct Book {
	char title[name_length];
	int author_id;
};

void edit_book(struct Book* b) {
	printf("Input Title: ");
	scanf("%s", (*b).title);
	printf("Input Autrhot Id: ");
	scanf("%i", &((*b).author_id));
}

void print_book(struct Book b) {
	printf("Title: %s\nAuthor Id: %i\n\n", b.title, b.author_id);
}

#endif