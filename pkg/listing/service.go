package listing

import (
	"github.com/sams96/uelp/pkg/adding"
	"log"
)

type Service interface {
}

type Repository interface {
	ReadAll(string)
}

type service struct {
	pR Repository
	l  *log.Logger
}

func NewService(r Repository, l *log.Logger) Service {
	return &service{r, l}
}
