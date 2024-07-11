CPPFLAGS=-Wall -O3 -std=c++11

EXEC=tictactoe

LIB=

$(EXEC): $(LIB)

.PHONY: clean

clean:
	-rm -f $(EXEC)
