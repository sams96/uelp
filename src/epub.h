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

#ifndef _EPUB_H
#define _EPUB_H

#include <stdbool.h>
#include "book.h"

// TODO: move this macro somewhere more universal
#if defined __GNUC__ || defined __clang__
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED
#endif

bool check_date_format (char * date);
bool ext_match (const char * path, const char * ext);
int get_date (char * out);
book_t * get_epub_metadata (const char * path);

#endif // _EPUB_H
