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
	"github.com/sams96/uelp/pkg/gui/gtk"
	"log"
	"os"
)

func main() {
	l := log.New(os.Stderr, "", log.LstdFlags|log.Lshortfile)

	gtk.Run(&os.Args, l)
}
