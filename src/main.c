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

typedef struct uelp_t
{
	sqlite3 * db;
	GtkListStore * store;
} uelp_t;

static char * default_db = "books.db";

int db_add_item_to_gtk_store_callback (void * model, int argc, char ** argv,
		char **azColName)
{
	GtkTreeIter i;
	gtk_list_store_append(GTK_LIST_STORE(model), &i);
	gtk_list_store_set(GTK_LIST_STORE(model), &i, 0, argv[0], 
			1, argv[1],
			2, argv[2],
			-1);

	return 0;
}

static void add_button_pressed (GtkWidget * widget, gpointer user_data)
{
	int status;
	char * error_msg;
	uelp_t * main = (uelp_t *) user_data;

	status = sqlite3_exec(main->db,
			"INSERT INTO Books(title, author) Values('w', 'v')",
			db_add_item_to_gtk_store_callback, main->store, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot query database, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close((sqlite3 *) user_data);
		exit(status);
	}
}

static void activate (GtkApplication * app, gpointer user_data)
{
	GtkWidget * window;
	uelp_t * main;

	GtkCellRenderer * renderer;
	GtkTreeViewColumn * column;
	GtkWidget * list;

	GtkWidget * box;
	GtkWidget * button;
	GtkTreeSelection * selection;

	char * error_msg;
	int status;

	/* Open DB */
	main->store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	status = sqlite3_open(default_db, &main->db);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot open database, SQL error: %s\n",
				sqlite3_errmsg(main->db));
		sqlite3_close(main->db);
		exit(status);
	}

	/* Create table if it doesn't already exist */
	status = sqlite3_exec(main->db,
			"CREATE TABLE IF NOT EXISTS "
		   		"Books(Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
					"Title TEXT, "
					"Author TEXT)",
			0, 0, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot create table, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close(main->db);
		exit(status);
	}

	/* Load database into GTK store */
	status = sqlite3_exec(main->db, "SELECT * FROM Books",
			db_add_item_to_gtk_store_callback, main->store, &error_msg);

	if (status != SQLITE_OK) {
		fprintf(stderr, "Cannot query database, SQL error: %s\n", error_msg);

		sqlite3_free(error_msg);
		sqlite3_close(main->db);
		exit(status);
	}

	window = gtk_application_window_new(app);
	list = gtk_tree_view_new();

	/* Create column with cell renderer */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text",
			0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Title", renderer, "text",
			1, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Author", renderer, "text",
			2, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(main->store));

	g_object_unref(main->store);

	gtk_window_set_title(GTK_WINDOW(window), "uelp");
	gtk_window_set_default_size(GTK_WINDOW(window), 960, 720);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_box_pack_start(GTK_BOX(box), list, TRUE, TRUE, 5);

	button = gtk_button_new_with_label("+");
	g_signal_connect(G_OBJECT(button), "clicked",
			G_CALLBACK(add_button_pressed), main);
	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(window), box);

	gtk_widget_show_all(window);

	sqlite3_close(main->db);
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
