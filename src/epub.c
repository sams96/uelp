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

#include <zip.h>
#include <libxml/parser.h>

#include "epub.h"

static void print_element_names (xmlNode * a_node)
{
	xmlNode * cur = NULL;

	for (cur = a_node; cur; cur = cur->next) {
		if (cur->type = XML_ELEMENT_NODE) {
			printf("name: %s\n", cur->name);
		}

		print_element_names(cur->children);
	}
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
		return NULL;
	}

	status = zip_stat(in, content_path, ZIP_FL_NOCASE, sb);
	metad = zip_fopen(in, content_path, ZIP_FL_NOCASE);

	printf("%li\n", sb->size);

	char * buf = malloc((size_t) sb->size);
	zip_fread(metad, buf, sb->size);

	xmlDocPtr data = xmlParseMemory(buf, sb->size);
	if (data == NULL) {
		fprintf(stderr, "Failed to parse %s\n", path);
	}

	xmlNode * root = xmlDocGetRootElement(data);
	print_element_names(root);

	xmlFreeDoc(data);
	free(buf);
	zip_fclose(metad);
	zip_close(in);

	return out;
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
