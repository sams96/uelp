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

#include "book.h"

book_t * get_epub_metadata (const char * path)
{
	int status;
	book_t * out;
	zip_t * in;
	zip_stat_t * sb;
	zip_file_t * metad;

	const char * content_path = "OEPBS/content.opf";
	
	in = zip_open(path, ZIP_RDONLY, &status);
	
	if (status) {
		fprintf(stderr, "Failed to open epub, error %d\n", status);
		return NULL;
	}

	status = zip_stat(in, content_path, ZIP_FL_NOCASE, sb);
	metad = zip_fopen(in, content_path, ZIP_FL_NOCASE);

	char * buf = malloc((size_t) sb->size);
	zip_fread(metad, buf, sb->size);

	xmlDocPtr data = xmlParseMemory(buf, sb->size);

	free(buf);
	zip_fclose(metad);
	zip_close(in);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
