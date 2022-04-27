#include "RLEList.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define NULL_POINTER -1
#define RLE_NOT_SUCCESS 0

#define ENCODE_FOMAT ("%c%d\n")

struct RLEList_t{
    //TODO: implement
    char letter;
    int num_in_row;
    RLEList next;

};

//implement the functions here

RLEList RLEListCreate(){
// RLEListCreate: Allocates a new empty RLE list.
    RLEList ptr = malloc(sizeof(*ptr)); // Dynamic allocation.
    if(!ptr){ // Checks if the pointer is null.
        return NULL; 
    }
    //Allocations worked. Creating the Node.
    ptr->letter = 0;
    ptr->num_in_row = 0;
    ptr->next = NULL;
    return ptr;

}

void RLEListDestroy(RLEList list){
//RLEListDestroy: Deallocates an existing RLE list.
    while(list){
        RLEList toDelete = list; //Enables us to free the entire RLElist.
        list = list->next;
        free(toDelete);
    }

}

RLEListResult RLEListAppend(RLEList list, char value){
//RLEListAppend: add a specified character at the end of the list.
    assert(list);
    assert(value);
    if(!list || !value){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list->next){
            list = list->next;
        }
    if(list->letter == 0 || list->letter == value){ // The char needs to be added to the list.
        list->letter = value;
        list->num_in_row += 1;
        return RLE_LIST_SUCCESS;
    }
    else{
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
}

int RLEListSize(RLEList list){
//RLEListSize: Returns the total number of characters in an RLE list.
    int count_chars = 0;
    assert(list);
    if(!list){
        return NULL_POINTER;
    }
    else{
        while(list){
            count_chars += list->num_in_row;
            list = list->next;
        }
        return count_chars;
    }
}

RLEListResult RLEListRemove(RLEList list, int index){
    //RLEListRemove: Removes a character found at a specified index in an RLE list.
    int count_chars = 0;
    assert(list);
    assert(index >= 0);
    if(!list || !index) {
        return RLE_LIST_NULL_ARGUMENT;
    }

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
        }else {
            count_chars += list->num_in_row;
            list = list->next;
        }
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
