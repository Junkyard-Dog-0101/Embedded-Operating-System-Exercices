CC		= gcc

RM		= rm -f

##CFLAGS		+= -Werror -Wall -Wextra

##CFLAGS		+= -ansi -pedantic

CLIENT_SRCS	= client.c

SERVER_SRCS	= server.c

CLIENT_OBJS	= $(CLIENT_SRCS:.c=.o)

SERVER_OBJS	= $(SERVER_SRCS:.c=.o)

CLIENT_NAME	= client

SERVER_NAME	= server

all: $(CLIENT_NAME) $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_NAME) $(CLIENT_OBJS) $(CFLAGS) $(LDFLAGS)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) -o $(SERVER_NAME) $(SERVER_OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(CLIENT_OBJS) $(SERVER_OBJS)

fclean: clean
	$(RM) $(CLIENT_NAME) $(SERVER_NAME)

re: fclean all

.PHONY: clean fclean all $(CLIENT_NAME) $(SERVER_NAME)
