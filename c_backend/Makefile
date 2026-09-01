CC = gcc

CFLAGS = -g
LDFLAGS = -lcurl

SRCS = main.c database.c databaseManager.c sqlite3.c api.c igdb_wrapper.c cJSON.c
OBJS = $(SRCS:.c=.o)
EXEC = backend.exe

ifeq ($(OS),Windows_NT)
	LDFLAGS += -lws2_32
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)