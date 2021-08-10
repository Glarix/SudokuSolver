CC=gcc
IDIR=include
CFLAGS=-I -Wall$(IDIR)

ODIR=obj
SDIR=src

LIBS=-lSDL2 -lSDL2_ttf -lcurl

_DEPS = SudokuSolv.h readFileData.h menu.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SudokuSolv.o readFileData.o menu.o SDLtest.o
OBJ = $(patsubst %,$(SDIR)/$(ODIR)/%,$(_OBJ))

$(SDIR)/$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Sudoku: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(SDIR)/$(ODIR)/*.o *~ core $(INCDIR)/*~