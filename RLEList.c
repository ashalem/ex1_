#include "RLEList.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define NULL_POINTER -1
#define RLE_NOT_SUCCESS 0

#define ENCODE_FOMAT ("%c%d\n")


struct RLEList_t {
    char letter;
    int letterCounter;
    RLEList next;
};


RLEList RLEListCreate() {
    RLEList listPtr = (RLEList)malloc(sizeof(*listPtr)); 
    if(!listPtr) { 
        return NULL; 
    }

    listPtr->letter = 0;
    listPtr->letterCounter = 0;
    listPtr->next = NULL;
    return listPtr;

}

void RLEListDestroy(RLEList list) {
    assert(list);
    if (!list) {
        return;
    }

    while(list) {
        RLEList toDelete = list;
        list = list->next;
        free(toDelete);
    }

}

static RLEList CreateRLEListWithValue(char value)  {
    RLEList newList = (RLEList)malloc(sizeof(*newList)); 
    if(!newList) {
        return NULL;
    }   

    newList->letter = value;
    newList->letterCounter = 1;
    newList->next = NULL;
    return newList;
}

RLEListResult RLEListAppend(RLEList list, char value) {
    assert(list);
    if(!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    while(list->next) {
        list = list->next;
    }
    if(list->letter == 0 || list->letter == value) { 
        // The char needs to be added to the node.
        list->letter = value;
        list->letterCounter += 1;
        return RLE_LIST_SUCCESS;
    }
    
    RLEList newList = CreateRLEListWithValue(value);
    list->next = newList;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list) {
    assert(list);
    if(!list) {
        return NULL_POINTER;
    }

    int countChars = 0;
    while(list) {
        countChars += list->letterCounter;
        list = list->next;
    }

    return countChars;
}

RLEListResult RLEListRemove(RLEList list, int index) {
    int countChars = 0;
    assert(list);
    assert(index >= 0);
    if(!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList previousNode = list;

    while(list) {
        if ((countChars + list->letterCounter) > index) {
            list->letterCounter--;
            if(0 == list->letterCounter) { 
                // Was a Lone char in it's own RLElist.
                previousNode->next = list->next;
                free(list);
            }
            return RLE_LIST_SUCCESS;
        } 

        countChars += list->letterCounter;
        previousNode = list;
        list = list->next;
    }

    return RLE_LIST_INDEX_OUT_OF_BOUNDS;

}

char RLEListGet(RLEList list, int index, RLEListResult *result) {
//RLEListGet: Returns the character found at a specified index in an RLE list.
    int countChars = 0;
    char letterToGet = 0;
    assert(list);
    assert(index >= 0);

    if(!list) {
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return RLE_NOT_SUCCESS;
    }

    while(list) {
        if((countChars+list->letterCounter) > index) {
            letterToGet = list->letter;
            break;
        }
        
        countChars += list->letterCounter;
        list = list->next;
        
    }

    if (!list) {
        if (result) {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return RLE_NOT_SUCCESS;
    }

    if(result) {
        *result = RLE_LIST_SUCCESS;
    }

    return letterToGet;
}

static int get_digits_len(int num)  {
    int counter = 1;
    while (0 != num / 10 ) {
        counter ++;
        num /= 10;
    }
    return counter;
}

static int calc_encoded_list_len(RLEList list) {
    int len = 0;
    while (list)
    {
        len += 1 + get_digits_len(list->letterCounter) + 1;
        list = list->next;
    }

    return len;
}

char* RLEListExportToString(RLEList list, RLEListResult* result) {
// RLEListExportToString: Returns the characters found in an RLE list as a string.
    assert(list);
    if(!list) {
        if(result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }

    int encodedListLen = calc_encoded_list_len(list);
    char* encodedList = (char *)malloc(encodedListLen + 1);
    char *encodedHead = encodedList;
    int lettersWritten = 0;
    
    while(list) {
        lettersWritten = sprintf(encodedList, ENCODE_FOMAT, list->letter, list->letterCounter);
        encodedList += lettersWritten;
        list = list->next;
    }

    if(result) {
        *result = RLE_LIST_SUCCESS;
    }
    return encodedHead;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function) {
    assert(list);
    assert(map_function);

    if(!list || !map_function) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    while(list) {
        list->letter = map_function(list->letter);
        list = list->next;
    }

    return RLE_LIST_SUCCESS;
}