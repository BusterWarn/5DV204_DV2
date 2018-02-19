/**
  * Compiles with:gcc -std=c99 -g -lm bitset.c test_bitset.c -o test_bitset.o
  * Just a show of usage of datatype bitset. 
  *
  *
  * 2017-02-10 Extended the the test to make uninitialised bits both between 
  *            and after last byte. Fixed prints to give more information.
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bitset.h"


void check_bitset(bitset *b);
void add_bit_to_bitset(bitset *b, int index, int value);


/* Will add 8 values to the bitset, check the bitset,
 * Then will add another value, checks the bitset again.
 */
int main(void) {
    char *bitsetArray;
    int index = 0;
    bool value[8];
    bitset *b = bitset_empty();

    
    value[index++] = 1;
    value[index++] = 0;
    value[index++] = 0;
    value[index++] = 1;
    value[index++] = 1;
    value[index++] = 1;
    value[index++] = 1;
    value[index++] = 1;

    for(int i = 0; i < index; i++) {
        add_bit_to_bitset(b, i, value[i]);
    }

    check_bitset(b);

    printf("\n");
    /* Add another bit, will be on index 8, 9, 10 (will be byte 2) */
    add_bit_to_bitset(b, index++, 0);
    add_bit_to_bitset(b, index++, 1);
    add_bit_to_bitset(b, index, 0);
    printf("\n"); 
    check_bitset(b);
    
    /* Add members on byte 4 */
    add_bit_to_bitset(b, 30, 1);    
    add_bit_to_bitset(b, 31, 1);
    printf("\n"); 
    check_bitset(b);
    
    
    /* Add member on byte 8 */
    printf("\n");     
    add_bit_to_bitset(b, 62, 1); 
    check_bitset(b);
    
    /* Add member on byte 16 */
    printf("\n");     
    add_bit_to_bitset(b, 122, 0);
    check_bitset(b);
    bitset_free(b);
    
    
    /* New bitset, only add a 0 in the last byte(8) but first index (58).*/
    printf("\n\nCreate new bitset, only add a 0 on place 58\n");
    b = bitset_empty();
    add_bit_to_bitset(b, 58, 0);
    check_bitset(b);
    bitset_free(b);
    
    
    
    /* New bitset, only add a 1 in the last byte(8) but first index (58).*/
    printf("\n\nCreate new bitset, only add a 1 on place 58\n");
    b = bitset_empty();
    add_bit_to_bitset(b, 58, 1);
    check_bitset(b);
    bitset_free(b);
 

}


/* Prints current bitsets content.
 * @param bitset    The bitset that will be printed.
 * Notice:  The uninitialised members that has not been touched will be yellow
 *          in the terminal.
 */
void check_bitset(bitset *b) {
    char *bitsetArray = bitset_toByteArray(b);
    int bits = bitset_size(b);
    int bytes = (int)ceil(bits/8.0);
    int totBits = bytes*8;

    printf("Values in the bitset:");
    for(int i = 0; i < bits; i++) {  
        printf("%d", bitset_memberOf(b, i));
    }
    for(int i = bits; i < totBits; i++) {
        printf("\x1b[33m" "%d" "\x1b[0m", bitset_memberOf(b, i));
    }
    printf("\n");  
    printf("Bitsize Array:%d bytes, %d bits touched, %d bits exits in array\n", 
                                                        bytes, 
                                                        bits,
                                                        totBits);
    free(bitsetArray);
}

/* Adds a value to the bitset on given bitset (and prints it)
 * @param b         The bitset.
 * @param index     The index the value in the bitset.
 * @param value     The value that will be added
 */
void add_bit_to_bitset(bitset *b, int index, int value) {    
    printf("Adding %d to bitset[index]: %d\n", value, index);
    bitset_setBitValue(b, index, value);
}
