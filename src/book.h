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

typedef struct book_t
{
	char title[128];
	char author[128];
	char series[128];

	/* Dates are stored in text as YYYY-MM-DD */
	char publishdate[11];
	char modifydate[11];

	/* Paths to files that contain the actual book */
	char epubfile[256];
	char mobifile[256];
	char pdffile[256];

	/* TODO: remove magic numbers */
	/* TODO: figure out the whole PATH_MAX thing */
} book_t;

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
