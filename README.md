# uelp
untitled ebook library program - meant to be something of a lightweight
replacement to [calibre](https://calibre-ebook.com) without the python2 (or
inevitably soon, python3) dependency hell.

## Build instructions
Have these dependencies installed:
* sqlite3
* libzip
* libxml2

Run `make`.

To run the test suite also install cmocka and run `make test`.
