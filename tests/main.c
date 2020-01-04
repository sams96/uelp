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
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <cmocka.h>

#include "../src/epub.h"

static void test_check_date_format (UNUSED void ** state)
{
	assert_true(check_date_format("2020-01-03"));
	assert_true(check_date_format("2020-02-29"));
	assert_true(check_date_format("2020-03-31"));
	assert_true(check_date_format("2000-02-29"));

	assert_false(check_date_format("2020-04-31"));
	assert_false(check_date_format("2019-02-29"));
	assert_false(check_date_format("1900-02-29"));

	assert_false(check_date_format("40000-01-03"));
	assert_false(check_date_format("2020-13-03"));
	assert_false(check_date_format("2020-01-99"));
	assert_false(check_date_format("2020_01_03"));
}

static void test_ext_match (UNUSED void ** state)
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

static void test_get_date (UNUSED void ** state)
{
	char * date = malloc(sizeof(char) * 11);

	get_date(date);

	assert_int_equal(strlen(date), 10);
	assert_int_equal((int) date[10], (int) '\0');

	int year = atoi(strtok(date, "-"));
	int month = atoi(strtok(NULL, "-"));
	int day = atoi(strtok(NULL, "-"));

	time_t t = time(NULL);
	struct tm tm = * localtime(&t);

	assert_int_equal(year, tm.tm_year + 1900);
	assert_int_equal(month, tm.tm_mon + 1);
	assert_int_equal(day, tm.tm_mday);

}

int main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_check_date_format),
		cmocka_unit_test(test_ext_match),
		cmocka_unit_test(test_get_date),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

// vim: tabstop=4 softtabstop=0 noexpandtab shiftwidth=4
