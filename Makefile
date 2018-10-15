OBJS = $(wildcard src/*.c)

CC = clang

CFLAGS = -Wall -Wpedantic -Wextra

LDFLAGS = 

TARGET = brainf

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
