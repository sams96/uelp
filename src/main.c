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

int db_add_item_to_gtk_store_callback (void * model, int argc, char ** argv,
		char **azColName)
{
	GtkTreeIter i;
	gtk_list_store_append(GTK_LIST_STORE(model), &i);
	gtk_list_store_set(GTK_LIST_STORE(model), &i, 0, argv[0], -1);

	return 0;
}

static void activate (GtkApplication * app, gpointer user_data)
{
	GtkWidget * window;

	GtkCellRenderer * renderer;
	GtkTreeViewColumn * column;
	GtkListStore * store;
	GtkWidget * list;

	GtkWidget * box;
	GtkWidget * label;
	GtkTreeSelection * selection;

	sqlite3 * db;
	char * error_msg;
	int status;

	/* Open DB */
	store = gtk_list_store_new(1, G_TYPE_UINT);
	status = sqlite3_open(default_db, &db);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot open database, SQL error: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(status);
	}

	/* Create table if it doesn't already exist */
	status = sqlite3_exec(db,
			"CREATE TABLE IF NOT EXISTS Books(Id INT PRIMARY KEY)",
			0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot create table, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close(db);
		exit(status);
	}

	/* Load database into GTK store */
	status = sqlite3_exec(db, "SELECT * FROM Books",
			db_add_item_to_gtk_store_callback, store, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot query database, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close(db);
		exit(status);
	}

	sqlite3_close(db);

	window = gtk_application_window_new(app);
	list = gtk_tree_view_new();

	/* Create column with cell renderer */
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0,
			NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

	g_object_unref(store);

	gtk_window_set_title(GTK_WINDOW(window), "uelp");
	gtk_window_set_default_size(GTK_WINDOW(window), 960, 720);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	box = gtk_box_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(box), list, TRUE, TRUE, 5);

	label = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(window), box);

	gtk_widget_show_all(window);
}

int main (int argc, char * argv[])
{
	GtkApplication * app;
	int status;

	app = gtk_application_new("com.github.sams96.uelp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	exit(status);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
