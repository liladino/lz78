CC = g++
CFLAGS = -Wall -pedantic -Wextra

TARGET = lz.out

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -O2 -o $@ $^

debug: $(OBJS)
	$(CC) $(CFLAGS) -g -DDEBUG -DMEMTRACE -o $(TARGET) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o -f
	rm debug -f
	rm all -f
	rm $(TARGET)
