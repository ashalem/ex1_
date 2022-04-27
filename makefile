CC=gcc
OBJS=RLEList.o tool/AsciiArtTool.o tool/main.o
EXEC=AsciiArtTool
DEBUG_FLAG=-DNDEBUG 
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror -I. -Itool

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS) -o $@


RLEList.o: RLEList.c RLEList.h
tool/AsciiArtTool.o: tool/AsciiArtTool.c tool/AsciiArtTool.h RLEList.h
tool/main.o: tool/main.c tool/AsciiArtTool.h RLEList.h

clean:
	rm -f $(OBJS) $(EXEC)
