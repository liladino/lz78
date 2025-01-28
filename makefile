CC = g++
CFLAGS = -Wall -Wextra -pedantic -MMD

TARGET = lz.out

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

debug: CFLAGS += -g -DDEBUG -DMEMTRACE
debug: $(TARGET)

all: CFLAGS += -O2
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.d
	rm $(TARGET) -f

-include $(DEPS)