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
