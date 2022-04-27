#include "RLEList.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define NULL_POINTER -1
#define RLE_NOT_SUCCESS 0


typedef struct RLEList_t{
    //TODO: implement
    char letter;
    int num_in_row;
    struct rlelist* next;

}*RLEList; // Name for the pointer of the structor.

//implement the functions here

RLEList RLEListCreate(){
// RLEListCreate: Allocates a new empty RLE list.
    RLEList ptr = malloc(sizeof(*ptr)); // Dynamic allocation.
    if(!ptr){ // Checks if the pointer is null.
        return NULL; //####### need to add the error enum.
    }
    //Allocations worked. Creating the Node.
    ptr->letter = "";
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
    if(list->letter == "" || list->letter == value){ // The char needs to be added to the list. 
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
    assert(index > 0);
    if(!list || !index){
        return RLE_LIST_NULL_ARGUMENT;
    }
    else{
        RLEList previus_list = list;
        while(list){
            if(!list){ // Index is out of bound.
                return RLE_LIST_INDEX_OUT_OF_BOUNDS;
            }
            if((count_chars+list->num_in_row) >= index){
                list->num_in_row--;
                if(!list->num_in_row){ // Was a Lone char in it's own RLElist.
                    previus_list->next = list->next
                    free(list);
                }
                return RLE_LIST_SUCCESS;
            }
            else{
                count_chars += list->num_in_row;
                previus_list = list;
                list = list->next;
            }
        }   
    }

}

char RLEListGet(RLEList list, int index, RLEListResult *result){
//RLEListGet: Returns the character found at a specified index in an RLE list.
    int count_chars = 0;
    char letter_to_get = "";
    assert(list);
    assert(index > 0);
    assert(result);
    if(result){
        if(!list){
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        while(list){
            if(!list){ // Index is out of bound.
                *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
                break;
            }
            else if((count_chars+list->num_in_row) >= index){
                letter_to_get = list->letter;
                *result = RLE_LIST_SUCCESS;
                break;
                }
            else{
                count_chars += list->num_in_row;
                list = list->next;
            }
        }
    }
    if(*result == RLE_LIST_SUCCESS){
            return letter_to_get;
    }
    else{
        return RLE_NOT_SUCCESS;
    }
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
// RLEListExportToString: Returns the characters found in an RLE list as a string.
    assert(list);
    assert(result);
    char* strList = "";
    
    if(result){
        if(!list){
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        while(list){
            
        }
    }
}