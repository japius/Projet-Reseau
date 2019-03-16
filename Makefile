CC=gcc
FLAGS= -Wall -g
LDLIBS=-lm -lreadline
#a changer
NAME=mpsh
FILES =$(shell find $(SRCIDR) -name '')
OBJ = $(FILES:%.c=%.o)

ALL=$(NAME)
all:$(ALL)

%.o: %.c
	$(CC) $(FLAGS) -c $? $(LDLIBS) -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LDLIBS) -o $(NAME)


#To generate the lexical analyser with lex
#lex.yy.c:lexer.l
clean:
	rm -rf $(OBJ) $(NAME) 

