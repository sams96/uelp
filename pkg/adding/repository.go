package adding

import (
	"github.com/sams96/uelp/pkg/types"
)

type Service interface {
	AddBook(types.Book) error
}

type Repository interface {
	AddBook(types.Book) error
}

type service struct {
	r Repository
}

func NewService(r Repository) Service {
	return &service{r}
}

func (s *service) AddBook(b types.Book) error {
	return s.r.AddBook(b)
}
