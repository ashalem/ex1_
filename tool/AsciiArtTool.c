#include <stdio.h>
#include <assert.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

RLEList asciiArtRead(FILE* in_stream) {
    assert(in_stream);
    if (!in_stream) {
        return NULL;
    }

    RLEList out_list = RLEListCreate();

    char ascii_char = (char)fgetc(in_stream);
    while (ascii_char != EOF) {
        if (RLE_LIST_SUCCESS != RLEListAppend(out_list, ascii_char)) {
            RLEListDestroy(out_list);
            return NULL; // TODO: Make sure error handling here is correct
        }

        ascii_char = (char)fgetc(in_stream);
    }

    return out_list;
}


RLEListResult asciiArtPrint(RLEList list, FILE* out_stream)
{
    assert(list);
    assert(out_stream);
    if (!list || !out_stream) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    int list_size = RLEListSize(list);
    char char_to_write = 0;
    RLEListResult rlelist_ret = RLE_LIST_ERROR;
    for (int i = 0; i < list_size; i++) {
        char_to_write = RLEListGet(list, i, &rlelist_ret);
        if (0 == char_to_write) {
            return rlelist_ret;
        }

        // TODO: Check what to do on error
        if (EOF == fputc(char_to_write, out_stream)) {
            return RLE_LIST_ERROR;
        }
    }

    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE* out_stream) 
{
    assert(list);
    assert(out_stream);
    if (!list || !out_stream) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEListResult rlelist_ret = RLE_LIST_ERROR;
    char *encoded_art = RLEListExportToString(list, &rlelist_ret);
    if (RLE_LIST_SUCCESS != rlelist_ret) {
        return rlelist_ret;
    }

    // TODO: Check what to do on error
    if (EOF == fputs(encoded_art, out_stream)) {
        return RLE_LIST_ERROR;
    }

    return RLE_LIST_SUCCESS;
}