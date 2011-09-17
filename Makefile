################################################################################
# Makefile                                                                     #
#                                                                              #
# Description: This file contains the make rules for a echo server using linux #
#              select() method.                                                              #
#                                                                              #
# Authors: Kaili Li <kailili@andrew.cmu.edu>                                   #
#                                                                              #
################################################################################

CC = gcc
LIB = -L. 

all:lisod 

lisod: server.o 
	$(CC) -o lisod server.o  $(LIB)

clean:
	rm -rf *.o
	rm -rf lisod
