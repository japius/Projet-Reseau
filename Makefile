CC=gcc
FLAGS= -Wall -g
LDLIBS=-lm 
#a changer
NAME=main
FILES = genlist.c list.c abr.c net_lib.c util.c tlv.c main.c#$(shell find $(SRCIDR) -name '')
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
