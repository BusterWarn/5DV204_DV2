/**
  * Compiles with:gcc -std=c99 -Wall -g -lm list.c prioqueue.c test_prioqueue.c -o test_prioqueue.o
  * Just a show of usage of datatype prioqueue. 
  *
  *
  * @author: Lena Kallin Westin <kallin@cs.umu.se>
  * @since:  2018-01-13 
  */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "pqueue.h"

typedef struct {
    int prio;
    char val;   
} queue_elem;


/* Function for comparing two prio-elements
 *
 * \param [in] elem1 - the first value
 * \param [in] elem1 - the second value
 *
 * \return > 0   if elem1 > elem2
 *         < 0   if elem1 < elem2
 *          0    if elem1 = elem2
*/
int lessthan(void *elem1, void *elem2){
    return ((queue_elem *)elem1)->prio - ((queue_elem *)elem2)->prio;
}

pqueue *createprioq(int choice){
     queue_elem *qelem;
     int numbers[7] = {5,3,9,7,1,0,5};
     char chars[7] = {'a','a','a','a','a','a','b'};
     pqueue *prioq;
     
     prioq = pqueue_empty(lessthan);
     
     for (int i = 0; i < 7; i++){
          qelem = malloc(sizeof qelem);
          qelem->prio = numbers[i];
          qelem->val = chars[i];
          pqueue_insert(prioq, qelem);
     }
     return prioq;
}

/*
 * Prints and empties a priorityqueue (but does not remove the queue)
 *
 * \param [in] prioq - the priority queue
 */
 void print_and_empty(pqueue *prioq){
    queue_elem *qelem;
    while (!pqueue_is_empty(prioq)){
        qelem = (queue_elem *)pqueue_inspect_first(prioq);
        pqueue_delete_first(prioq);
        printf("(%d, %c) ",qelem->prio, qelem->val);
        free(qelem);
    }
    printf("\n");
}


/** The test creates a priority queue and adds the tuples (5,a) (3,a) (9,a) (7,a) (1,a) (0,a) (5,b) into it. 
  * after that it empties the queue and writes the tuples to the screeen.
  */
 int main(){
     pqueue *prioq;
     
     printf("Create a priorityqueue with the elements (5,a) (3,a) (9,a) (7,a) (1,a) (0,a) (5,b) added.\n");
     printf("The result of printing the queue using < as priority should be:\n");
     printf("(0, a) (1, a) (3, a) (5, a) (5, b) (7, a) (9, a)\n");
     prioq = createprioq(1);
     print_and_empty(prioq);
     pqueue_kill(prioq);
     return 0;         
 }
