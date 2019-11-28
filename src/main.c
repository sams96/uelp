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

int main (int argc, char * argv[])
{
	int status = 0;
	int opt;

	sqlite3 * db;
	char * error_msg;
	char * db_fn = (char *) default_db;

	static struct option long_options[] = {
		{"database", 	required_argument, 	0, 	'd'},
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
		} else {
			optind++;
		}
	}

	/* Open DB */
	status = sqlite3_open(default_db, &db);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot open database, SQL error: %s\n",
				sqlite3_errmsg(db));
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

	sqlite3_close(db);

	exit(status);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
