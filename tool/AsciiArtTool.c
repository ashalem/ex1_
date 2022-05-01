#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

RLEList asciiArtRead(FILE* inStream) {
    assert(inStream);
    if (!inStream) {
        return NULL;
    }

    RLEList outList = RLEListCreate();
    char asciiChar = (char)fgetc(inStream);
    while (asciiChar != EOF) {
        if (RLE_LIST_SUCCESS != RLEListAppend(outList, asciiChar)) {
            RLEListDestroy(outList);
            return NULL;
        }

        asciiChar = (char)fgetc(inStream);
    }

    return outList;
}


RLEListResult asciiArtPrint(RLEList list, FILE* outStream)
{
    assert(list);
    assert(outStream);
    if (!list || !outStream) {
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

        if (EOF == fputc(charToWrite, outStream)) {
            return RLE_LIST_ERROR;
        }
    }

    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE* outStream) 
{
    assert(list);
    assert(outStream);
    if (!list || !outStream) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEListResult rlelistRet = RLE_LIST_ERROR;
    char *encodedArt = RLEListExportToString(list, &rlelistRet);
    if (!encodedArt || RLE_LIST_SUCCESS != rlelistRet) {
        return rlelistRet;
    }

    if (EOF == fputs(encodedArt, outStream)) {
        free(encodedArt);
        return RLE_LIST_ERROR;
    }

    free(encodedArt);
    return RLE_LIST_SUCCESS;
}