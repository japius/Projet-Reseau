CC=gcc
FLAGS= -Wall -g -Wno-unused-parameter 
LDLIBS=-lm -I /usr/local/include/ -l websockets -L /usr/local/lib/
#a changer
NAME=main
FILES = genlist.c list.c abr.c net_lib.c util.c tlv.c websocket.c main.c#$(shell find $(SRCIDR) -name '')
OBJ = $(FILES:%.c=%.o)

ALL=$(NAME) clean
all:$(ALL)

%.o: %.c
	$(CC) $(FLAGS) -c $? $(LDLIBS) -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LDLIBS) -o $(NAME)


#To generate the lexical analyser with lex
#lex.yy.c:lexer.l
clean:
	rm -rf $(OBJ)
