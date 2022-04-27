#include <stdio.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

typedef enum {
    ARGS_FLAGS = 1,
    ARGS_SOURCE_FILE,
    ARGS_DEST_FILE,
    ARGS_LAST
} user_args_e;

#define ENCODED_FLAG ('e')
#define INVERTED_FLAG ('i')

#define INVERT_RIGHT (' ')
#define INVERT_LEFT ('@')

typedef void (*AsciiModifyFunction)(FILE *, FILE *);

static char invert_char(char char_to_invert) {
    if (INVERT_LEFT == char_to_invert) {
        return INVERT_RIGHT;
    }

    if (INVERT_RIGHT == char_to_invert) {
        return INVERT_LEFT;
    }

    return char_to_invert;
}

static void encode_image(FILE *source_file, FILE *dest_file) {
    if (!source_file || !dest_file) {
        return;
    }

    RLEList asii_art_list = asciiArtRead(source_file);
    if (!asii_art_list) {
        return;
    }
    
    asciiArtPrintEncoded(asii_art_list, dest_file);

    RLEListDestroy(asii_art_list);
}

static void invert_image(FILE *source_file, FILE *dest_file) {
    if (!source_file || !dest_file) {
        return;
    }

    RLEList asii_art_list = asciiArtRead(source_file);
    if (!asii_art_list) {
        return;
    }

    if (RLE_LIST_SUCCESS == RLEListMap(asii_art_list, invert_char)) {
        asciiArtPrint(asii_art_list, dest_file);
    }

    RLEListDestroy(asii_art_list);
}

int main(int argc, char *argv[]) {
    if (ARGS_LAST != argc) {
        return 0;
    }

    AsciiModifyFunction modify_func = NULL;
    switch (argv[ARGS_FLAGS][1]) {
    case ENCODED_FLAG:
        modify_func = encode_image;
        break;
    case INVERTED_FLAG:
        modify_func = invert_image;
        break;
    default:
        // Unknown flag
        return 0;
        break;
    }

    FILE *source_file = fopen(argv[ARGS_SOURCE_FILE], "r");
    if (!source_file) {
        return 0;
    }

    FILE *dest_file = fopen(argv[ARGS_DEST_FILE], "w+");
    if (!dest_file) {
        fclose(source_file);
        return 0;
    }

    modify_func(source_file, dest_file);
   
    fclose(dest_file);
    fclose(source_file);
    return 0;
}
