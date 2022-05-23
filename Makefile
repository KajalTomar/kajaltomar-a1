FLAGS = -Wall -Wextra -Wpedantic -Werror -g
LDLIBS = -lpthread -lcurl
CC = clang

all: a1-elf a1-procs a1-threads

clean:
	rm -f elf
