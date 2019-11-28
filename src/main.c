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

#include <sqlite3.h>
#include <getopt.h>

const char * default_db = "books.db";
const char * version_string = "v0.0";
const char * prog_name = "uelp";

/* Callback function to print one entry of the database */
int print_entry (void * none, int colc, char ** colv, char ** azcolname)
{
	for (int i = 0; i < colc; i++)
		printf("%s\t", colv[i]);

	printf("\n");

	return 0;
}

/* Print the database */
int print_db (sqlite3 * db)
{
	int status = 0;
	char * error_msg;

	printf("ID\tTitle\tAuthor\n");

	status = sqlite3_exec(db, "SELECT * FROM Books", print_entry, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot query database, SQL error: %s\n", error_msg);
		sqlite3_free(error_msg);
		return status;
	}
}

int main (int argc, char * argv[])
{
	int status = 0;
	int opt;

	sqlite3 * db;
	char * error_msg;
	char * db_fn = (char *) default_db;

	int print_the_db = 0;

	static struct option long_options[] = {
		{"database", 	required_argument, 	0, 	'd'},
		{"print", 		no_argument, 		0, 	'p'},
		{"version", 	no_argument, 		0, 	'V'},
		{"help", 		no_argument, 		0, 	'h'},
		{0, 			0, 					0, 	0}
	};

	if (argc == 1)
		printf("Usage: %s [OPTION]...\
				\nTry '%s --help' for more information.\n",
				prog_name, prog_name);

	while (optind < argc) {
		if ((opt = getopt_long(argc, argv, "V", long_options, &opt)) != -1) {
			switch (opt) {
				case 'd':
					db_fn = optarg;
					break;
				case 'p':
					print_the_db = 1;
					break;
				case 'V':
					printf("%s %s\n", prog_name, version_string);
					break;
				case 'h':
					printf("Usage: &s [OPTION]... [FILE]...\
							\nOptions:\
							\n\t-d,\t--database\tUse a different database file\
							\n\t-V,\t--version\tDisplay program version\
							\n\t-h,\t--help\tDisplay this message\
							\n", prog_name);
					break;
			}
		}
	}

	/* Open DB */
	status = sqlite3_open(default_db, &db);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot open database, SQL error: %s\n",
				sqlite3_errmsg(db));
		sqlite3_free(error_msg);
		sqlite3_close(db);
		exit(status);
	}

	/* Create table if it doesn't already exist */
	status = sqlite3_exec(db,
			"CREATE TABLE IF NOT EXISTS "
		   		"Books(Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
					"Title TEXT, "
					"Author TEXT)",
			0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot create table, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close(db);
		exit(status);
	}

	if (print_the_db) print_db(db);

	sqlite3_close(db);

	exit(status);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
