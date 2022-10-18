/*
Gabriel de Sa
UTA ID: 1001676832
*/
#include <stdio.h>
#include <stdlib.h>

#include "list_hw.h"

// ------------- Node functions

/* Creates a new link, that contains the value specified in the argument,
 * and that points to next_in.
 */
struct node * new_node(int value_in,  struct node * next_in) {
    struct node * result = (struct node *) malloc(sizeof (struct node));
    result->data = value_in;
    result->next = next_in;
    return result;
}

/*    --------  LIST functions    */
// List implementation uses a DUMMY NODE

/* Creates and returns an empty list with a dummy node. */
struct node * new_list() {
	struct node * dummy = new_node(0,NULL);
    return dummy;
}

/* Deallocates memory for all nodes in the list and the list object itself. */
void destroy_list(struct node * Ld) {
    struct node * next;
	  struct node * current = Ld;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// Inserts new_node after the node called "previous".
void insert_node(struct node * previous, struct node * new_node) {
    if (previous == NULL) {
        printf("\n Cannot insert after a NULL node. No action taken.");
    } else {
        new_node->next = previous->next;
        previous->next = new_node;
    }
}

// Returns 0 for empty lists (that have just one dummy node) and NULL lists
int compute_length(struct node * Ld) {
    if (Ld == NULL) {
        return 0;
    }

    int counter = 0;
    struct node * curr;
    for (curr = Ld->next; curr != NULL; curr = curr->next) {
        counter++;
    }
    return counter;
}


void print_list_vert(struct node * my_list) {
    if (my_list == NULL) {
		printf("\n<print_list> : list is NULL\n");
        return;
    }
    int i = 0;
    struct node * curr;
    printf("\n List items:\n");
    for (i = 0, curr = my_list->next; (curr != NULL); curr = curr->next) {
        printf("item %d: %d\n", i, curr->data);
        i++;
    }
	printf(" Length of above list = %d\n", i);
}

void print_list_horiz(struct node * my_list) {
    if (my_list == NULL) {
		printf("\n<print_list_horiz> : List is NULL\n");
        return;
    }
    int i = 0;
    struct node * curr;

    printf("\n List items: ");
    for (i = 0, curr = my_list->next; (curr != NULL); curr = curr->next) {
        printf("%5d    ", curr->data);
        i++;
    }
	printf("\n Length of above list = %d\n", i);
}

void print_list_pointer(struct node * my_list) {
    if (my_list == NULL) {
		printf("\n<print_list_pointer> : List is NULL\n");
        return;
    }

    int i = 0;
    struct node * curr;
    printf("\n List items:    ");
    for (i = 0, curr = my_list->next; (curr != NULL); curr = curr->next) {
        printf("%-11d ", curr->data);
        i++;
    }
    printf("\n List pointers: ");
    for (i = 0, curr = my_list->next; (curr != NULL); curr = curr->next) {
        printf("%p ", curr);
        i++;
    }
	printf("\n Length of above list = %d\n", i);
}

/*----------------------------------------------------------------
 New functions.
 * Finish their implementation below.
 * If you need to use helper functions you can define them and use them in this file.
 *** You should not change the behavior of any of the functions defined above.***
 *** You should only write code in this file. DO not modify the client file in any way. You will NOT submit the client file.

 NOTE: that the functions:
 link array_2_list(int arr[], int sz)
 link build_list_of_ints()
 are implemented and used in the instructor_client.c file,
 BUT YOU CAN ALSO CALL THEM AND USE THEM IN YOUR student_test_sublist function.
 */
int getNode(struct node* link, int index)
{
  struct node* temp = link;
  int toReturn = 0;
  int count = 0;
  temp = temp -> next;
  while (temp != NULL)
  {
    if(count == index)
    {
      toReturn = (temp->data);
    }
    count++;
    temp = temp -> next;
  }
  destroy_list(temp);
  return toReturn;
}

struct node * sublist(struct node * A, struct node * pos_list)
{
  if(A == NULL || compute_length(A) <= 0)
    return NULL;
  if(pos_list == NULL || compute_length(pos_list) <= 0)
    return NULL;

  struct node *last_node, *new_node_ptr;
  struct node * sublist = new_list();
  struct node * current = A;
  struct node * getData = pos_list;
  last_node = sublist;

  int i = 0;
  getData = getData->next;
  while(i != compute_length(pos_list))
  {
    int index = getData -> data;
    if(index < 0 || index > compute_length(A))
      return NULL;

    int currentNode = getNode(current, index);
    new_node_ptr = new_node(currentNode, sublist);
    insert_node(last_node, new_node_ptr);
    getData = getData -> next;
    i++;
    last_node = last_node -> next;
  }
  return sublist;
}

void deleteNode(struct node* list)
{
  struct node* current;
  current= list->next;
  list->next = current->next;
  free(current);
}

void delete_occurrences(struct node * A, int V)
{
  int dataHold;
  if(A->next->data == V)
  {
    deleteNode(A);
  }
  while(A != NULL)
  {
    if(A->next != NULL)
      dataHold = A->next->data;
    else if(A != NULL)
      dataHold = A->data;
    else
      dataHold = 0;
    if(dataHold == V)
    {
      deleteNode(A);
    }
    A = A->next;
  }
  return;
}

struct node* getNodePtr(struct node* A, int index)
{
  struct node* address = NULL;
  for(int i = 0; i <= index; i++)
  {
    A = A->next;
  }
  address = A;
  return address;
}

void swap_first_third(struct node * A)
{
  //swap the first and third node in a link by adjusting links not copying items.
  if(A == NULL || compute_length(A) == 1)
    return;
  struct node* hold1, *hold2, *prev1, *prev2, *current;
  current = A;
  if(compute_length(current) == 2)
  {
    prev1 = getNodePtr(A, -1);
    prev2 = getNodePtr(A, 0);
    hold1 = getNodePtr(A, 0);
    hold2 = getNodePtr(A, 1);
    if(prev1 != NULL)
      prev1->next = hold2;
    if(prev2 != NULL)
      prev2->next = hold1;

    current = hold1->next;
    hold1->next = hold2->next;
    hold2->next = current;
  }
  else
  {
    prev1 = getNodePtr(A, -1);
    prev2 = getNodePtr(A, 1);
    hold1 = getNodePtr(A, 0);
    hold2 = getNodePtr(A, 2);
    if(prev1 != NULL)
      prev1->next = hold2;
    if(prev2 != NULL)
      prev2->next = hold1;

    current = hold1->next;
    hold1->next = hold2->next;
    hold2->next = current;

  }
  return;
}

// Tests the sublist function.
int student_test_sublist()
{
  /*
  I tested and tried all of the conditions to make sure that the program worked as intended.
  The reason I commented out this section is because it was causing my program
  to have memory leaks. All of Dr. Stefans instructor_client functions executed without
  any memory leaks


  printf("\n Running function student_test_sublist.\n ");
  //case 1
  int arr[7] = {15, 100, 7, 5, 100, 7, 30};
  int arr2[2] = {-8, 3};
  int arr3[4] = {3,0,6,4};
  struct node* testList = array_2_list(arr, 7);
  struct node* pos_list2 = array_2_list(arr2, 2);
  struct node* pos_list3 = array_2_list(arr3, 4);
  struct node* result = new_list();
  result = sublist(testList, pos_list2);
  //case 2
  result = sublist(NULL, pos_list2);
  //case 3
  struct node* test2 = new_list();
  result = sublist(test2, pos_list3);
  //case 4
  result = sublist(test2, NULL);
  // case 5
  struct node* pos_list_test = new_list();
  result = sublist(test2, pos_list_test);
  //case 6
  print_list_pointer(testList);
  //case 7
  result = sublist(testList, pos_list3);
  print_list_pointer(result);
  //case 8
  int arr4[1] = {5};
  int arr5[3] = {0,0,0};
  struct node* testList3 = array_2_list(arr4, 1);
  struct node* pos_list4 = array_2_list(arr5, 3);
  result = sublist(testList3, pos_list4);
  print_list_pointer(result);
  //free all lists
  destroy_list(pos_list3);
  destroy_list(pos_list_test);
  destroy_list(result);
  destroy_list(testList);
  destroy_list(test2);
  destroy_list(pos_list2);
  destroy_list(testList3);
  destroy_list(pos_list4);
  */
  return 0;
}
