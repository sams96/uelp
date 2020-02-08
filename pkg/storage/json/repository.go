package json

import (
	"github.com/sams96/uelp/pkg/types"
	"github.com/sdomino/scribble"
)

type Storage struct {
	db     *scribble.Driver
	dbname string
}

func NewStorage(dir string) (*Storage, error) {
	db, err := scribble.New(dir, nil)
	return &Storage{db: db, dbname: "books"}, err
}

func (s *Storage) AddBook(b types.Book) error {
	return s.db.Write(s.dbname, b)
}
