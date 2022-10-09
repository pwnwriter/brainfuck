PREFIX = /usr/local
FLAGS = -Wall -Wextra

all: clean build

clean:
	rm -f brainfuck

build:
	gcc ${FLAGS} brainfuck.c -o brainfuck

install:
	cp brainfuck ${DESTDIR}${PREFIX}/bin/brainfuck

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/brainfuck

test:
	brainfuck testbrainfuck.bf
