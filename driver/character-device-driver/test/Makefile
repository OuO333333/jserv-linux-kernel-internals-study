.PHONY: all check clean

CC = gcc
CFLAGS = -std=gnu99 -Wall -g

OBJS = $(FILE_NAME).o
EXEC = $(FILE_NAME)
FILE_NAME = check

OBJS2 = $(FILE_NAME2).o
EXEC2 = $(FILE_NAME2)
FILE_NAME2 = dev

OBJS3 = $(FILE_NAME3).o
EXEC3 = $(FILE_NAME3)
FILE_NAME3 = pthread


all:
	# generate the object file
	$(CC) $(CFLAGS) -c -o $(OBJS) $(FILE_NAME).c
	# generate the executable directly
	$(CC) $(OBJS) -o $(EXEC)
	# generate the object file
	$(CC) $(CFLAGS) -c -o $(OBJS2) $(FILE_NAME2).c
	# generate the executable directly
	$(CC) $(OBJS2) -o $(EXEC2)
	# generate the object file
	gcc pthread.c -lpthread -o pthread

check:
	@./$(EXEC); \
	if [ $$? -eq 0 ] ; then \
		echo "PASS."; \
	else \
		echo "FAIL."; \
	fi

clean:
	rm -f $(OBJS) $(EXEC)
	rm -f $(OBJS2) $(EXEC2)
	rm -f $(OBJS3) $(EXEC3)
