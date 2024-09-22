CC = gcc
CFLAGS = -I/home/dave/src/libssd1306/include
LDFLAGS = -L/home/dave/src/libssd1306/src/.libs
LIBS = -lssd1306_i2c


all: newdisp

newdisp: newdisp.o
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -c

newdisp.o: newdisp.c

