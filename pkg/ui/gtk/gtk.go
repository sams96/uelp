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
package gtk

import (
	"github.com/gotk3/gotk3/gtk"
	"log"
)

const WindowName = "window"
const BoxName = "box"
const ToolbarName = "toolbar"
const ListBoxName = "listbox"
const UIMain = "pkg/ui/gtk/glade/main.glade"

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

	err = loadList(bldr, []string{"this", "is", "a", "list"})
	if err != nil {
		return err
	}

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

// TODO: Take object names as parameters
func getWindow(b *gtk.Builder) (*gtk.Window, error) {
	obj, err := b.GetObject(WindowName)
	if err != nil {
		return nil, err
	}

	window, ok := obj.(*gtk.Window)
	if !ok {
		// TODO: return proper errors here
		return nil, err
	}

	return window, nil
}

func getBox(b *gtk.Builder) (*gtk.Box, error) {
	obj, err := b.GetObject(BoxName)
	if err != nil {
		return nil, err
	}

	box, ok := obj.(*gtk.Box)
	if !ok {
		return nil, err
	}

	return box, nil
}

func getToolbar(b *gtk.Builder) (*gtk.Toolbar, error) {
	obj, err := b.GetObject(ToolbarName)
	if err != nil {
		return nil, err
	}

	box, ok := obj.(*gtk.Toolbar)
	if !ok {
		return nil, err
	}

	return box, nil
}

func getListbox(b *gtk.Builder) (*gtk.ListBox, error) {
	obj, err := b.GetObject(ListBoxName)
	if err != nil {
		return nil, err
	}

	box, ok := obj.(*gtk.ListBox)
	if !ok {
		return nil, err
	}

	return box, nil
}

func loadList(b *gtk.Builder, data []string) error {
	lb, err := getListbox(b)
	if err != nil {
		return err
	}

	for i, el := range data {
		lbl, err := gtk.LabelNew(el)
		if err != nil {
			return err
		}

		lbl.SetXAlign(0)
		lbl.SetMarginStart(5)
		row, err := gtk.ListBoxRowNew()
		if err != nil {
			return err
		}

		row.Add(lbl)
		lb.Insert(row, i)
	}

	lb.ShowAll()

	return nil
}
