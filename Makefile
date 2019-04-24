CC=gcc
FLAGS= -Wall
LDLIBS=-lm 
#a changer
NAME=pair
FILES = net_lib.c pair.c list.c abr.c tlv.c#$(shell find $(SRCIDR) -name '')
OBJ = $(FILES:%.c=%.o)

ALL=$(NAME)
all:$(ALL) clean

%.o: %.c
	$(CC) $(FLAGS) -c $? $(LDLIBS) -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LDLIBS) -o $(NAME)


#To generate the lexical analyser with lex
#lex.yy.c:lexer.l
clean:
	rm -rf $(OBJ)
