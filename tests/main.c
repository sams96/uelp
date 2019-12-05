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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/epub.h"

static void test_ext_match (void ** state)
{
	assert_true(ext_match("this_is_a_test.epub", ".epub"));
	assert_true(ext_match("this_is_a_test.EPUB", ".epub"));
	assert_true(ext_match("this_is_a_test.epub", ".ePuB"));

	assert_true(ext_match("this_is_a_test.mobi", ".mobi"));
	assert_true(ext_match("this/is/a/test.azw3", ".azw3"));

	assert_false(ext_match("this_is_a_test.mobi", ".epub"));
	assert_false(ext_match("this_is_a_test.mobi_123", ".mobi"));
	assert_false(ext_match("this_is_a_test.m.o.b.i", ".mobi"));
}

int main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_ext_match),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
