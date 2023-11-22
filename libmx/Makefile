CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

TARGET = libmx.a

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $(OBJ)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -Iinc -o $@ -c $<

clean:
	rm -rf obj

uninstall: clean
	rm -rf $(TARGET)

reinstall: uninstall all

.PHONY: all clean uninstall reinstall
