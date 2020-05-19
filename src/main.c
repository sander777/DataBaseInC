#include "Table.h"
#include "structs.h"

//#define DEBUG

int main() {
    struct Table *book_table =
        new_Table("book_index.txt", "book_table.txt",
                  sizeof(struct Book));
    struct Table *author_table =
        new_Table("author_index.txt", "author_table.txt",
                  sizeof(struct Author));

    #ifndef DEBUG
    while (1) {
        printf("Choose what table to open:\n1) Books\n2) Authors\n0-Exit\n> ");
        int asnwer;
        scanf("%i", &asnwer);
        if (asnwer == 0)
            break;
        if (asnwer == 1) {
            while (1) {
                system("cls");
                printf("Book Table\n");
                int size;
                int *list = get_id_list(book_table, &size);
                for (int i = 0; i < size; i++) {
                    struct Book *temp = read_data(book_table, list[i]);
                    print_book(*temp);
                    printf("\n********************\n");
                }

                printf("\n1) Add\n2) "
                       "Delete\n0-Exit\n> ");
                int book_asnwer;
                scanf("%i", &book_asnwer);
                if (book_asnwer == 1) {
                    struct Book add;
                    edit_book(&add);
                    add_row(book_table, &add);
                }
            }
        }

		if (asnwer == 2) {
            while (1) {
                printf("Author Table\n");
                int size;
                int *list = get_id_list(author_table, &size);
                for (int i = 0; i < size; i++) {
                    struct Author *temp = read_data(author_table, list[i]);
                    print_author(*temp);
                    printf("\n********************\n");
                }
                printf("\n1) Add\n2) "
                       "Delete\n0-Exit\n> ");
                int author_asnwer;
                scanf("%i", &author_asnwer);
                if (author_asnwer == 1) {
                    struct Author add;
                    edit_author(&add);
                    add_row(author_table, &add);
                }
            }
        }
    }
    #endif

	drop(book_table);
	drop(author_table);
}
