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
package main

import (
	"encoding/json"
	"github.com/sams96/uelp/pkg/adding"
	"github.com/sams96/uelp/pkg/storage/json"
	"github.com/sams96/uelp/pkg/ui/gtk"
	"github.com/sdomino/scribble"
	"log"
	"os"
)

func main() {
	l := log.New(os.Stderr, "", log.LstdFlags|log.Lshortfile)

	s := json.NewStorage("./")
	adder := adding.NewService(s)

	for _, name := range []string{"book one", "book two", "book three"} {
		adder.AddBook(types.Book{Name: name, Author: "author"})
	}

	gtk.Run(&os.Args, l)
}
