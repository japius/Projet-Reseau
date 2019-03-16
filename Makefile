CC=gcc

CFLAGS= -Wall -std=c11

LDLIBS= -lm


all : pair

pair : pair.c struct.h


clear : 
	clear
	