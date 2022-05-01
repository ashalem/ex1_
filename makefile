CC=gcc
MTM_HEADERS=/home/mtm/public/2122b/ex1
OBJS=RLEList.o tool/AsciiArtTool.o tool/main.o
EXEC=AsciiArtTool
DEBUG_FLAG= -g
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror -I$(MTM_HEADERS) -Itool -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS) -o $@


RLEList.o: RLEList.c $(MTM_HEADERS)/RLEList.h
tool/AsciiArtTool.o: tool/AsciiArtTool.c tool/AsciiArtTool.h $(MTM_HEADERS)/RLEList.h
tool/main.o: tool/main.c tool/AsciiArtTool.h $(MTM_HEADERS)/RLEList.h

clean:
	rm -f $(OBJS) $(EXEC)
