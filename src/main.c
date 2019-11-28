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

#include <gtk/gtk.h>
#include <sqlite3.h>

static char * default_db = "books.db";

int main (int argc, char * argv[])
{
	char * error_msg;
	int status;

	sqlite3 * db;

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
