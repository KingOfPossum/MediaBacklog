CC = gcc
LDFLAGS = -lcurl -lws2_32

SRCS = main.c database.c databaseManager.c sqlite3.c api.c igdb_wrapper.c cJSON.c
OBJS = $(SRCS:.c=.o)
EXEC = backend

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)