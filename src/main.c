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

static void activate (GtkApplication * app, gpointer user_data)
{
	GtkWidget * window;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "uelp");
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	gtk_widget_show_all(window);
}

int main (int argc, char * argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.github.sams96.uelp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	exit(status);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
