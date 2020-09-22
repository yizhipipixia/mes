CFLAGS = -Wall -o2 -g -std=gnu99 -pthread -I./include -DMYDBG
LDFLAGS = -lpthread -static

APP = ts1
SRCS	 = $(wildcard ./src/*.c)
OBJS	 = $(patsubst %.c, %.o,$(SRCS))

CC = gcc
#CC = arm-linux-gcc
all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $^ $(LDFLAGS)
	rm -f $(OBJS)
	
clean:
	rm -f $(OBJS) $(APP)
