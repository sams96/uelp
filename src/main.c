/*
	This file is part of uelp,
	copyright (c) 2019 Sam Smith.

	uelp is free software: you can redistribute it and/or modify it under the
	terms of the GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option) any later
	version.

	uelp is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
	details.

	You should have received a copy of the GNU General Public License along with
	uelp. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>
#include <getopt.h>
#include <gtk/gtk.h>

#include "epub.h"

const char * default_db = "books.db";
const char * version_string = "v0.0";
const char * prog_name = "uelp";

enum {
	LIST_ID,
	LIST_TITLE,
	LIST_AUTHOR,
	LIST_SERIES,
	LIST_PUBDATE,
	LIST_MODDATE,
	LIST_EPUB,
	LIST_MOBI,
	LIST_PDF,
	N_COLUMNS
};

const char * const col_titles[] = {"ID", "Title", "Author", "Series",
	"Publish Date", "Modify Date", "EPUB File", "MOBI File", "PDF File"};

/*
 * SQLite callback function to add items to gtk store
 */
int db_add_item_to_store (void * store, UNUSED int colc, char ** colv,
		UNUSED char ** azcolname)
{
	GtkTreeIter i;

	gtk_list_store_append(GTK_LIST_STORE(store), &i);
	gtk_list_store_set(GTK_LIST_STORE(store), &i,
			LIST_ID, 		colv[LIST_ID],
			LIST_TITLE, 	colv[LIST_TITLE],
			LIST_AUTHOR, 	colv[LIST_AUTHOR],
			LIST_SERIES, 	colv[LIST_SERIES],
			LIST_PUBDATE, 	colv[LIST_PUBDATE],
			LIST_MODDATE, 	colv[LIST_MODDATE],
			LIST_EPUB, 		colv[LIST_EPUB],
			LIST_MOBI, 		colv[LIST_MOBI],
			LIST_PDF, 		colv[LIST_PDF],
			-1);

	return 0;
}

/*
 * Print the help message
 */
static inline void print_help (void)
{
	printf("Usage: %s [OPTION]... [FILE]...\n"
			"Options:\n"
			"\t-a,\t--add\t\tAdd the given file to the database\n"
			"\t-r,\t--remove\tRemove book with the given ID\n"
			"\t-p,\t--print\tPrint the DB to console\n"
			"\t-d,\t--database\tUse a different database file\n"
			"\t-V,\t--version\tDisplay program version\n"
			"\t-h,\t--help\t\tDisplay this message\n",
			prog_name);
}

/*
 * Callback function to print one entry of the database
 */
int print_entry (UNUSED void * none, int colc, char ** colv,
		UNUSED char ** azcolname)
{
	for (int i = 0; i < colc; i++)
		printf("%s\t", colv[i]);

	printf("\n");

	return 0;
}

/*
 * Print the database
 */
int print_db (sqlite3 * db)
{
	int status = 0;
	char * error_msg = NULL;

	printf("ID\tTitle\tAuthor\tSeries\tPublish Date\tModify Date\n");

	status = sqlite3_exec(db, "SELECT * FROM Books", print_entry, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot query database, SQL error: %s\n", error_msg);
		sqlite3_free(error_msg);
	}

	return status;
}

/*
 * Add a book to the db
 */
int add_book (sqlite3 * db, book_t * book)
{
	int status = 0;
	char * error_msg = NULL;
	char query[1300]; // TODO: remove magic numbers

	sprintf(query,
			"INSERT INTO Books (title, author, series, publishdate, modifydate, "
				"epubfile, mobifile, pdffile)"
				"Values('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			book->title, book->author, book->series, book->publishdate,
			book->modifydate, book->epubfile, book->mobifile, book->pdffile);

	status = sqlite3_exec(db, query, 0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot add to database, SQL error: %s\n", error_msg);
		sqlite3_free(error_msg);
	}

	return status;
}

/*
 * Remove a book from the db, given it's ID
 */
int remove_book (sqlite3 * db, char * ID)
{
	int status = 0;
	char * error_msg = NULL;
	char query[1300]; // TODO: remove magic numbers

	sprintf(query, "DELETE FROM Books WHERE Id = %s;", ID);
	status = sqlite3_exec(db, query, 0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot remove %s from database, SQL error: %s\n",
				ID, error_msg);
		sqlite3_free(error_msg);
	}

	return status;
}

static void activate (GtkApplication * app, gpointer user_data)
{
	GtkWidget * window;
	sqlite3 * db = (sqlite3 *) user_data;

	GtkCellRenderer * renderer;
	GtkTreeViewColumn * column;
	GtkWidget * list;
	GtkWidget * box;
	GtkListStore * store;

	char * error_msg = NULL;
	int status;

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	
	status = sqlite3_exec(db, "SELECT * FROM Books", db_add_item_to_store,
			store, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Can't query database, SQL error: %s\n", error_msg);
		return;
	}

	window = gtk_application_window_new(app);
	list = gtk_tree_view_new();

	/* Create column with cell renderer */
	// TODO: Use a function/loop for this
	renderer = gtk_cell_renderer_text_new();
	for (int i = 0; i < N_COLUMNS; i++) {
		column = gtk_tree_view_column_new_with_attributes(col_titles[i],
				renderer, "text", i, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

	g_object_unref(store);

	gtk_window_set_title(GTK_WINDOW(window), prog_name);
	gtk_window_set_default_size(GTK_WINDOW(window), 960, 720);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), TRUE);

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(box), list, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(window), box);

	gtk_widget_show_all(window);
}


/*
 * Main
 */
int main (int argc, char * argv[])
{
	int status = 0;
	int opt;

	sqlite3 * db;
	char * error_msg = NULL;
	char * db_fn = (char *) default_db;

	// TODO: find a nicer way to handle these
	char * file_to_add = NULL;
	char * file_to_remove = NULL;

	int print_the_db = 0;

	static struct option long_options[] = {
		{"add", 		required_argument, 	0, 	'a'},
		{"remove", 		required_argument, 	0, 	'r'},
		{"database", 	required_argument, 	0, 	'd'},
		{"print", 		no_argument, 		0, 	'p'},
		{"version", 	no_argument, 		0, 	'V'},
		{"help", 		no_argument, 		0, 	'h'},
		{0, 			0, 					0, 	0}
	};

	while (optind < argc) {
		if ((opt = getopt_long(argc, argv, "a:r:d:pVh", long_options, &opt))
				!= -1) {
			switch (opt) {
				case 'a':
					file_to_add = optarg;
					break;
				case 'r':
					file_to_remove = optarg;
					break;
				case 'd':
					db_fn = optarg;
					break;
				case 'p':
					print_the_db = 1;
					break;
				case 'V':
					printf("%s %s\n", prog_name, version_string);
					exit(EXIT_SUCCESS);
					break;
				case 'h':
					print_help();
					exit(EXIT_SUCCESS);
					break;
			}
		}
	}

	/* Open DB */
	status = sqlite3_open(db_fn, &db);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot open database, SQL error: %s\n",
				sqlite3_errmsg(db));
		goto quit;
	}

	/* Create table if it doesn't already exist */
	status = sqlite3_exec(db,
			"CREATE TABLE IF NOT EXISTS "
		   		"Books(Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
					"title TEXT, "
					"author TEXT, "
					"series TEXT, "
					"publishdate TEXT, "
					"modifydate TEXT, "
					"epubfile TEXT, "
					"mobifile TEXT, "
					"pdffile TEXT)",
			0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot create table, SQL error: %s\n", error_msg);
		goto quit;
	}

	if (file_to_add != NULL) {
		// TODO: Check file type
		book_t * bk = get_epub_metadata(file_to_add);
		add_book(db, bk);
	}

	if (file_to_remove != NULL) {
		remove_book(db, file_to_remove);
	}

	if (print_the_db) print_db(db);

	GtkApplication * app;
	app = gtk_application_new("com.github.sams96.uelp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), db);
	status = g_application_run(G_APPLICATION(app), 0, NULL);
	g_object_unref(app);

quit:
	sqlite3_free(error_msg);
	sqlite3_close(db);
	exit(status);
}
