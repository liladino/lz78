CC = g++
CFLAGS = -Wall -pedantic -Wextra

ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

TARGET = lz.out

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -O2 -o $@ $^

debug: $(OBJS)
	$(CC) $(CFLAGS) -g -DDEBUG -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o -f
	rm debug -f
	rm all -f
