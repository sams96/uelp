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

#include <string.h>
#include <time.h>

#include <zip.h>
#include <libxml/parser.h>

#include "epub.h"

/* Return today's date as a string in the format 'YYYY-MM-DD' */
static int get_date (char * out)
{
	int status = 0;
	time_t t = time(NULL);
	struct tm tm = * localtime(&t);

	status = sprintf(out, "%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1,
			tm.tm_mday);

	return status;
}

static void print_element_names (xmlNode * a_node)
{
	xmlNode * cur = NULL;

	for (cur = a_node; cur; cur = cur->next) {
		if (cur->type == XML_ELEMENT_NODE) {
			printf("name: %s\tcontent: %s\n", cur->name, cur->content);
		}

		print_element_names(cur->children);
	}
}

/* Search the xml tree for a given name and return the content */
static char * search_xml (xmlNode * root, char * name)
{
	xmlNode * cur = NULL;
	char * ret = NULL;
	char * this = NULL;

	/* Loop through nodes on this level */
	for (cur = root; cur; cur = cur->next) {

		if (xmlStrEqual(cur->name, (xmlChar *) name))
			return (char *) cur->children->content;

		/* Search child nodes */
		this = search_xml(cur->children, name);

		/* Pass the return value back up the stack */
		if (this != NULL) ret = this;
	}

	return ret;
}

book_t * get_epub_metadata (const char * path)
{
	int status;
	book_t * out = malloc(sizeof(book_t));
	zip_t * in;
	zip_stat_t * sb = malloc(sizeof(zip_stat_t));
	zip_file_t * metad;

	const char * content_path = "content.opf";
	
	in = zip_open(path, ZIP_RDONLY, &status);

	if (in == NULL) {
		fprintf(stderr, "Failed to open %s, error %d\n", path, status);
		goto free;
	}

	/* Open and read content.opf file */
	status = zip_stat(in, content_path, ZIP_FL_NOCASE, sb);
	metad = zip_fopen(in, content_path, ZIP_FL_NOCASE);
	char * buf = malloc((size_t) sb->size);
	zip_fread(metad, buf, sb->size);

	xmlDocPtr data = xmlParseMemory(buf, sb->size);
	if (data == NULL) {
		fprintf(stderr, "Failed to parse %s\n", path);
		goto free;
	}

	xmlNode * root = xmlDocGetRootElement(data);
	if (root == NULL) {
		fprintf(stderr, "Failed to get root element in %s\n", path);
		goto free;
	}

	/* Search for and store the relevant information */
	char * title = search_xml(root, "title");
	if (title) strcpy(out->title, title);

	char * author = search_xml(root, "creator");
	if (author) strcpy(out->author, author);

	char * series = search_xml(root, "series");
	if (series) strcpy(out->series, series);

	char * publishdate = search_xml(root, "date");
	if (publishdate) {
		strncpy(out->publishdate, publishdate, 10);
		out->publishdate[11] = '\0';
	}

	status = get_date(out->modifydate);

	strcpy(out->epubfile, path);

free:
	xmlFreeDoc(data);
	free(buf);
	zip_fclose(metad);
	zip_close(in);

	return out;
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
