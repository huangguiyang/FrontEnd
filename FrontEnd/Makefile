
CC = g++
CFLAGS = -c -Wall
LDFLAGS = 
TARGET = FrontEnd
SRC = Parser.cpp \
	Lexer.cpp \
	Token.cpp \
	Node.cpp \
	Symbol.cpp \
	Cmd.cpp \
	Error.cpp \
	main.cpp

OBJ = $(SRC: .cpp=.o)
INC = $(wildcard *.h)

all: $(TARGET)

$(TARGET): $(OBJ) $(INC)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o $(TARGET)