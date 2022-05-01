#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

RLEList asciiArtRead(FILE* in_stream) {
    assert(in_stream);
    if (!in_stream) {
        return NULL;
    }

    RLEList outList = RLEListCreate();
    char asciiChar = (char)fgetc(in_stream);
    while (asciiChar != EOF) {
        if (RLE_LIST_SUCCESS != RLEListAppend(outList, asciiChar)) {
            RLEListDestroy(outList);
            return NULL;
        }

        asciiChar = (char)fgetc(in_stream);
    }

    return outList;
}


RLEListResult asciiArtPrint(RLEList list, FILE* out_stream)
{
    assert(list);
    assert(out_stream);
    if (!list || !out_stream) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    int listSize = RLEListSize(list);
    char charToWrite = 0;
    RLEListResult rlelistRet = RLE_LIST_ERROR;
    for (int i = 0; i < listSize; i++) {
        charToWrite = RLEListGet(list, i, &rlelistRet);
        if (0 == charToWrite) {
            return rlelistRet;
        }

        if (EOF == fputc(charToWrite, out_stream)) {
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

    RLEListResult rlelistRet = RLE_LIST_ERROR;
    char *encodedArt = RLEListExportToString(list, &rlelistRet);
    if (!encodedArt || RLE_LIST_SUCCESS != rlelistRet) {
        return rlelistRet;
    }

    if (EOF == fputs(encodedArt, out_stream)) {
        free(encodedArt);
        return RLE_LIST_ERROR;
    }

    free(encodedArt);
    return RLE_LIST_SUCCESS;
}