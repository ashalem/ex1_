//
// Created by Ariel on 25/04/2022.
//

#ifndef HW1_ASCIARTTOOL_H
#define HW1_ASCIARTTOOL_H

#include <stdio.h>
#include <stdbool.h>
#include "RLEList.h"

/**
* Reading and writing of ascii arts
*
* The following functions are available:
*   asciiArtRead	        - Reads an ascii art from file.
*   asciiArtPrint		    - Writes an ascii art image to a file.
*   asciiArtPrintEncoded    - Writes an encoded ascii art image to a file.
**/

/**
* asciiArtRead: Reads an ascii art from file.
*
* @param in_stream - FILE* object containing the image to read.
* @return A list of type RLEList containing all the information of the ascii art
*/
RLEList asciiArtRead(FILE* in_stream);

/**
* asciiArtPrint: Writes an ascii art to file.
*
* @param list - A list of type RLEList containing all the information of the ascii art.
* @param out_stream - FILE* object to contain the ascii art.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters
* 	RLE_LIST_SUCCESS if the character has been inserted successfully
*/
RLEListResult asciiArtPrint(RLEList list, FILE* out_stream);

/**
* asciiArtRead: Writes an encoded ascii art info to file.
*
* @param list - A list of type RLEList containing all the information of the ascii art.
* @param out_stream - FILE* object to contain the ascii art encoding.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters
* 	RLE_LIST_SUCCESS if the character has been inserted successfully
*/
RLEListResult asciiArtPrintEncoded(RLEList list, FILE* out_stream);

#endif // HW1_ASCIARTTOOL_H
