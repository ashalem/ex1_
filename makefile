CC=gcc
OBJS=RLEList.o tool/AsciiArtTool.o tool/main.o
EXEC=AsciiArtTool
DEBUG_FLAG=-DNDEBUG 
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror -Itool

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS) -o $@


RLEList.o: RLEList.c tool/RLEList.h
tool/AsciiArtTool.o: tool/AsciiArtTool.c tool/AsciiArtTool.h tool/RLEList.h
tool/main.o: tool/main.c tool/AsciiArtTool.h tool/RLEList.h

clean:
	rm -f $(OBJS) $(EXEC)
