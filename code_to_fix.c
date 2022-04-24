#include <stdlib.h> //changed
#include <string.h> //changed
#include <assert.h> //changed

// s -> ptr_string
char* stringDuplicator(char* ptr_str, int times){
    assert(ptr_str); // Assert check it is null
    assert(times > 0);
    int len = strlen(ptr_str); // Conventions LEN->len
    char* ptr_out = malloc(len * times + 1); // Conventions + off by-one, maybe the sizeof
    // Need to save out head
    assert(ptr_out);

    char* ptr_curr = ptr_out;
    for (int i=0; i< times; i++){ // off by one in I
        // Indentation
        
        strcpy(ptr_curr, ptr_str);
        // Switched order
        ptr_curr = ptr_curr + len;
    }
    return ptr_out;
}
