package gtk

import (
	"github.com/gotk3/gotk3/gtk"
)

const WindowName = "window"
const UIMain = "pkg/gui/gtk/glade/main.glade"

func Run(args *[]string, l *log.Logger) error {
	gtk.Init(args)
	bldr, err := getBuilder(UIMain)
	if err != nil {
		l.Println(err)
		return err
	}

	window, err := getWindow(bldr)
	if err != nil {
		l.Println(err)
		return err
	}

	window.SetTitle("uelp")
	window.SetDefaultSize(640, 480)
	_, err = window.Connect("destroy", func() {
		gtk.MainQuit()
	})
	if err != nil {
		l.Println(err)
		return err
	}

	window.ShowAll()

	gtk.Main()
	return nil
}

func getBuilder(filename string) (*gtk.Builder, error) {
	b, err := gtk.BuilderNew()
	if err != nil {
		return nil, err
	}

	if filename != "" {
		err = b.AddFromFile(filename)
		if err != nil {
			return nil, err
		}
	}

	return b, nil
}

func getWindow(b *gtk.Builder) (*gtk.Window, error) {
	obj, err := b.GetObject(WindowName)
	if err != nil {
		return nil, err
	}

	window, ok := obj.(*gtk.Window)
	if !ok {
		return nil, err
	}

	return window, nil
}
