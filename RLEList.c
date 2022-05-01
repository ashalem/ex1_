#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "RLEList.h"

#define NULL_POINTER (-1)
#define RLE_NOT_SUCCESS (0)

#define ENCODE_FOMAT ("%c%d\n")

struct RLEList_t {
    char letter;
    int letterCounter;
    RLEList next;
};

static void RLEListCopyNodes(RLEList node1, RLEList node2) {
    assert(node1);
    assert(node2);
    if (!node1 || !node2) { 
        return; 
    }

    node1->letter = node2->letter;
    node1->letterCounter = node2->letterCounter;
    node1->next = node2->next;
}

static void RLEListMergeSuccessiveNodes(RLEList node) {
    assert(node);
    assert(node->next);
    if (!node || !node->next) { 
        return; 
    }

    node->letterCounter += node->next->letterCounter;
    RLEList deprecatedNode = node->next;
    node->next = node->next->next;
    free(deprecatedNode);
}

RLEList RLEListCreate() {
    RLEList listPtr = (RLEList)malloc(sizeof(*listPtr)); 
    if (!listPtr) { 
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

    while (list) {
        RLEList toDelete = list;
        list = list->next;
        free(toDelete);
    }
}

static RLEList RLEListCreateNodeWithValue(char value)  {
    RLEList newList = (RLEList)malloc(sizeof(*newList)); 
    if (!newList) {
        return NULL;
    }   

    newList->letter = value;
    newList->letterCounter = 1;
    newList->next = NULL;
    return newList;
}

static void RLEListRemoveFirstNode(RLEList list) {
    if (list->next) {
        RLEList deprecatedNode = list->next;
        RLEListCopyNodes(list, deprecatedNode);
        free(deprecatedNode);
    } else {
        // Last node in list
        list->letter = 0;
    }
}

RLEListResult RLEListAppend(RLEList list, char value) {
    assert(list);
    if (!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    while (list->next) {
        list = list->next;
    }

    if (list->letter == 0 || list->letter == value) { 
        // The char needs to be added to the node.
        list->letter = value;
        list->letterCounter += 1;
        return RLE_LIST_SUCCESS;
    }
    
    RLEList newNode = RLEListCreateNodeWithValue(value);
    if (!newNode) {
        return RLE_LIST_OUT_OF_MEMORY;
    }

    list->next = newNode;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list) {
    assert(list);
    if (!list) {
        return NULL_POINTER;
    }

    int countChars = 0;
    while (list) {
        countChars += list->letterCounter;
        list = list->next;
    }

    return countChars;
}

RLEListResult RLEListRemove(RLEList list, int index) {
    assert(list);
    assert(index >= 0);
    if (!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    if (index < 0) {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    RLEList previousNode = list;
    int countChars = 0;

    while (list) {
        if ((countChars + list->letterCounter) <= index) {
            countChars += list->letterCounter;
            previousNode = list;
            list = list->next;
            continue;
        }

        list->letterCounter--;
        if(0 != list->letterCounter) {  
            return RLE_LIST_SUCCESS;
        }
        
        if (previousNode == list) {
            RLEListRemoveFirstNode(list);
            return RLE_LIST_SUCCESS;
        } 

        // Not first node
        previousNode->next = list->next;
        if (list->next && previousNode->letter == previousNode->next->letter) {
            RLEListMergeSuccessiveNodes(previousNode);
        }
        free(list);
        return RLE_LIST_SUCCESS;
    }

    return RLE_LIST_INDEX_OUT_OF_BOUNDS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result) {
    assert(list);
    assert(index >= 0);
    if (!list) {
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return RLE_NOT_SUCCESS;
    }

    if (index < 0) {
        if (result) {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return RLE_NOT_SUCCESS;
    }

    int countChars = 0;
    char letterToGet = 0;
    while (list) {
        if ((countChars + list->letterCounter) > index) {
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

    if (result) {
        *result = RLE_LIST_SUCCESS;
    }

    return letterToGet;
}

static int getDigitsLen(int num)  {
    int counter = 1;
    while (0 != num / 10 ) {
        counter ++;
        num /= 10;
    }

    return counter;
}

static int calcEncodedListLen(RLEList list) {
    int len = 0;
    while (list) {
        // A letter len + digits + NewLine
        len += 1 + getDigitsLen(list->letterCounter) + 1;
        list = list->next;
    }

    return len;
}

char* RLEListExportToString(RLEList list, RLEListResult* result) {
    assert(list);
    if(!list) {
        if(result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }

    int encodedListLen = calcEncodedListLen(list);
    char* encodedList = (char *)malloc(encodedListLen + 1);
    char *encodedHead = encodedList;
    int lettersWritten = 0;
    
    while(list) {
        lettersWritten = sprintf(encodedList, ENCODE_FOMAT, list->letter, list->letterCounter);
        if (lettersWritten < 0) {
            if (result) {
                *result = RLE_LIST_ERROR;
            }

            free(encodedHead);
            return NULL;
        }
        encodedList += lettersWritten;
        list = list->next;
    }

    if (result) {
        *result = RLE_LIST_SUCCESS;
    }
    return encodedHead;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function) {
    assert(list);
    assert(map_function);

    if (!list || !map_function) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList previousNode = list;
    while (list) {
        list->letter = map_function(list->letter);

        if (previousNode != list && previousNode->letter == list->letter) {
            RLEListMergeSuccessiveNodes(previousNode);
            list = previousNode->next;
        } else {
            previousNode = list;
            list = list->next;
        }
    }

    return RLE_LIST_SUCCESS;
}