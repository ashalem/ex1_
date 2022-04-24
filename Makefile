CC=gcc
OBJS=RLEList.o AsciiArtTool.o main.o
EXEC=AsciiArtTool
DEBUG_FLAG=-DNDEBUG 
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror 

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS) -o $@


RLEList.o: RLEList.c RLEList.h
AsciiArtTool.o: AsciiArtTool.c AsciiArtTool.h RLEList.h
main.o: main.c AsciiArtTool.h

clean:
	rm -f $(OBJS) $(EXEC)