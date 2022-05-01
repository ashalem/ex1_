#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

typedef enum {
    ARGS_FLAGS = 1,
    ARGS_SOURCE_FILE,
    ARGS_DEST_FILE,
    ARGS_LAST
} user_args_e;

#define ENCODED_FLAG ("-e")
#define INVERTED_FLAG ("-i")

#define INVERT_RIGHT (' ')
#define INVERT_LEFT ('@')

typedef void (*AsciiModifyFunction)(FILE *, FILE *);

static char invert_char(char charToInvert) {
    if (INVERT_LEFT == charToInvert) {
        return INVERT_RIGHT;
    }

    if (INVERT_RIGHT == charToInvert) {
        return INVERT_LEFT;
    }

    return charToInvert;
}

static void encodeImage(FILE *sourceFile, FILE *destFile) {
    assert(sourceFile);
    assert(destFile);
    if (!sourceFile || !destFile) {
        return;
    }

    RLEList asiiArtList = asciiArtRead(sourceFile);
    if (!asiiArtList) {
        return;
    }
    
    asciiArtPrintEncoded(asiiArtList, destFile);

    RLEListDestroy(asiiArtList);
}

static void invertImage(FILE *sourceFile, FILE *destFile) {
    if (!sourceFile || !destFile) {
        return;
    }

    RLEList asiiArtList = asciiArtRead(sourceFile);
    if (!asiiArtList) {
        return;
    }

    if (RLE_LIST_SUCCESS == RLEListMap(asiiArtList, invert_char)) {
        asciiArtPrint(asiiArtList, destFile);
    }

    RLEListDestroy(asiiArtList);
}

int main(int argc, char *argv[]) {
    if (ARGS_LAST != argc) {
        return 0;
    }

    AsciiModifyFunction modifyFunc = NULL;
    char *flag = argv[ARGS_FLAGS];

    if (0 == strncmp(flag, ENCODED_FLAG, sizeof(ENCODED_FLAG))) {
        modifyFunc = encodeImage;
    } else if (0 == strncmp(flag, INVERTED_FLAG, sizeof(INVERTED_FLAG))) {
        modifyFunc = invertImage;
    } else {
        // Unknown flag
        return 0;
    }

    FILE *sourceFile = fopen(argv[ARGS_SOURCE_FILE], "r");
    if (!sourceFile) {
        return 0;
    }

    FILE *destFile = fopen(argv[ARGS_DEST_FILE], "w+");
    if (!destFile) {
        fclose(sourceFile);
        return 0;
    }

    modifyFunc(sourceFile, destFile);
   
    fclose(destFile);
    fclose(sourceFile);
    return 0;
}
