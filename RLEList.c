#include "RLEList.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define NULL_POINTER -1
#define RLE_NOT_SUCCESS 0

#define ENCODE_FOMAT ("%c%d\n")

// CR: Fix { spaces
// CR: for each malloc do the casting
// CR: Change to Camel case

struct RLEList_t {
    char letter;
    // CR: Fix name
    int num_in_row;
    RLEList next;
};


RLEList RLEListCreate(){
    // CR: Change to list_ptr
    RLEList ptr = (RLEList)malloc(sizeof(*ptr)); 
    if(!ptr){ 
        return NULL; 
    }

    ptr->letter = 0;
    ptr->num_in_row = 0;
    ptr->next = NULL;
    return ptr;

}

void RLEListDestroy(RLEList list){
    assert(list);
    if (!list) {
        return;
    }

    while(list){
        RLEList toDelete = list;
        list = list->next;
        free(toDelete);
    }

}

RLEListResult RLEListAppend(RLEList list, char value){
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
        list->num_in_row += 1;
        return RLE_LIST_SUCCESS;
    }
    
    // CR: create new static function that adds a new node in an end of the list
    RLEList newList = RLEListCreate();
    if(!newList){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    list->next = newList;
    newList->letter = value;
    newList->num_in_row = 1;
    newList->next = NULL;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list){
    assert(list);
    if(!list){
        return NULL_POINTER;
    }

    int count_chars = 0;
    while(list){
        count_chars += list->num_in_row;
        list = list->next;
    }

    return count_chars;
}

RLEListResult RLEListRemove(RLEList list, int index){
    int count_chars = 0;
    assert(list);
    assert(index >= 0);
    if(!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    // CR: Fix name
    RLEList previus_node = list;

    while(list){
        if ((count_chars + list->num_in_row) > index){
            list->num_in_row--;
            if(0 == list->num_in_row) { 
                // Was a Lone char in it's own RLElist.
                previus_node->next = list->next;
                free(list);
            }
            return RLE_LIST_SUCCESS;
        } else {
            count_chars += list->num_in_row;
            previus_node = list;
            list = list->next;
        }
    }

    return RLE_LIST_INDEX_OUT_OF_BOUNDS;

}

char RLEListGet(RLEList list, int index, RLEListResult *result){
//RLEListGet: Returns the character found at a specified index in an RLE list.
    int count_chars = 0;
    char letter_to_get = 0;
    assert(list);
    assert(index >= 0);

    if(!list){
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return RLE_NOT_SUCCESS;
    }

    while(list){
        if((count_chars+list->num_in_row) > index){
            letter_to_get = list->letter;
            break;
        } 
        count_chars += list->num_in_row;
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

    return letter_to_get;
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
    while (!list)
    {
        len = 1 + get_digits_len(list->num_in_row) + 1;
        list = list->next;
    }

    return len;
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
// RLEListExportToString: Returns the characters found in an RLE list as a string.
    assert(list);
    if(!list){
        if(result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }

    int encoded_list_len = calc_encoded_list_len(list);
    char* encoded_list = (char *)malloc(encoded_list_len + 1);
    char *encoded_head = encoded_list;
    int letters_written = 0;
    
    while(list) {
        letters_written = sprintf(encoded_list, ENCODE_FOMAT, list->letter, list->num_in_row);
        encoded_list += letters_written;
        list = list->next;
    }

    if(result) {
        *result = RLE_LIST_SUCCESS;
    }
    return encoded_head;
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
