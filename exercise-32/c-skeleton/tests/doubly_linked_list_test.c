#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <liblcthw/doubly_linked_list.h>

#include "minunit.h"

#define TEST_STR_1 "A"
#define TEST_STR_1_LEN strlen(TEST_STR_1)

#define TEST_STR_2 "B"
#define TEST_STR_2_LEN strlen(TEST_STR_2)

#define TEST_STR_3 "C"
#define TEST_STR_3_LEN strlen(TEST_STR_3)

#define TEST_STR_4 "D"
#define TEST_STR_4_LEN strlen(TEST_STR_4)

#define SORT_TEST_STR_1 "5"
#define SORT_TEST_STR_2 "1"
#define SORT_TEST_STR_3 "4"
#define SORT_TEST_STR_4 "2"
#define SORT_TEST_STR_5 "8"

char *test_destroy_emptyList()
{
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_destroy(list);

    return NULL;
}

char *test_destroy_listContainingMallocatedValues()
{
    char *test1 = malloc(sizeof(char) * TEST_STR_1_LEN + 1);
    assert(test1 != NULL);
    strncpy(test1, TEST_STR_1, TEST_STR_1_LEN);

    char *test2 = malloc(sizeof(char) * TEST_STR_2_LEN + 1);
    assert(test2 != NULL);
    strncpy(test2, TEST_STR_2, TEST_STR_2_LEN);

    LCTHW_DoublyLinkedList_List_t *list = List_create(); 
    
    List_push(list, test1);
    List_push(list, test2);

    List_destroy(list);

    free(test1);
    free(test2);
    
    return NULL;
}

char *test_push_pop()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_2;
    char *test3 = TEST_STR_3;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);
    assert((char *) List_last(list) == test1 && "Wrong last value.");

    List_push(list, test2);
    assert((char *) List_last(list) == test2 && "Wrong last value");

    List_push(list, test3);
    assert((char *) List_last(list) == test3 && "Wrong last value.");
    assert(List_count(list) == 3 && "Wrong count on push.");

    char *val = List_pop(list);
    assert(val == test3 && "Wrong value on pop.");

    val = List_pop(list);
    assert(val == test2 && "Wrong value on pop.");

    val = List_pop(list);
    assert(val == test1 && "Wrong value on pop.");
    assert(List_count(list) == 0 && "Wrong count after pop.");
    
    UNUSED(val);
    
    List_destroy(list);

    return NULL;
}

char *test_unshift()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_2;
    char *test3 = TEST_STR_3;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_unshift(list, test1);
    assert((char *) List_first(list) == test1 && "Wrong first value.");

    List_unshift(list, test2);
    assert((char *) List_first(list) == test2 && "Wrong first value");

    List_unshift(list, test3);
    assert((char *) List_first(list) == test3 && "Wrong last value.");
    assert(List_count(list) == 3 && "Wrong count on unshift.");

    List_destroy(list);

    return NULL;
}

char *test_remove()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_2;
    char *test3 = TEST_STR_3;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    char *val = List_remove(list, list->first->next);
    assert(val == test2 && "Wrong removed element.");
    assert(List_count(list) == 2 && "Wrong count after remove.");
    assert((char *) List_first(list) == test1 && "Wrong first after remove.");
    assert((char *) List_last(list) == test3 && "Wrong last after remove.");

    UNUSED(val);

    List_destroy(list);

    return NULL;
}

char *test_shift()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_2;
    char *test3 = TEST_STR_3;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    char *val = List_shift(list);
    assert(val == test1 && "Wrong value on shift.");
    assert(List_count(list) == 2 && "Wrong count after shift.");

    val = List_shift(list);
    assert(val == test2 && "Wrong value on shift.");
    assert(List_count(list) == 1 && "Wrong count after shift.");
    
    val = List_shift(list);
    assert(val == test3 && "Wrong value on shift.");
    assert(List_count(list) == 0 && "Wrong count after shift.");
    
    UNUSED(val);

    List_destroy(list);

    return NULL;
}

char *test_reverse_manyValues()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_2;
    char *test3 = TEST_STR_3;
    char *test4 = TEST_STR_4;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);
    List_push(list, test4);
    
    List_reverse(list);
    
    char *val = List_shift(list);
    assert(val == test4 && "Wrong value on shift.");

    val = List_shift(list);
    assert(val == test3 && "Wrong value on shift.");
    
    val = List_shift(list);
    assert(val == test2 && "Wrong value on shift.");

    val = List_shift(list);
    assert(val == test1 && "Wrong value on shift.");

    UNUSED(val);

    List_destroy(list);

    return NULL;
}

char *test_reverse_oneValue()
{
    char *test1 = TEST_STR_1;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);

    List_reverse(list);
    
    char *val = List_shift(list);
    assert(val == test1 && "Wrong value on shift.");

    UNUSED(val);

    List_destroy(list);

    return NULL;
}

char *test_reverse_twoValues()
{
    char *test1 = TEST_STR_1;
    char *test2 = TEST_STR_1;
    
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, test1);
    List_push(list, test2);

    List_reverse(list);
    
    char *val = List_shift(list);
    assert(val == test2 && "Wrong value on shift.");

    val = List_shift(list);
    assert(val == test1 && "Wrong value on shift.");

    UNUSED(val);

    List_destroy(list);

    return NULL;
}

static int assert_list_is_sorted(LCTHW_DoublyLinkedList_List_t *list)
{
    assert(list != NULL);
    
    LIST_FOREACH(list, first, next, cur) {
        if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
            printf("%s %s\n", (char *) cur->value, (char *) cur->next->value);
            return 0;
        }
    }
    
    return 1;
}

char *test_bubble_sort()
{
    LCTHW_DoublyLinkedList_List_t *list = List_create();
    
    List_push(list, SORT_TEST_STR_1);
    List_push(list, SORT_TEST_STR_2);
    List_push(list, SORT_TEST_STR_3);
    List_push(list, SORT_TEST_STR_4);
    List_push(list, SORT_TEST_STR_5);
    
    /*printf("============ BEFORE SORT ============\n");
    
    LIST_FOREACH(list, first, next, cur) {
        printf("%s\n", (char *) cur->value);
    }*/
    
    List_bubble_sort(list, (sort_callback) strcmp);
    
    /*printf("============ AFTER SORT ============\n");
    
        LIST_FOREACH(list, first, next, cur) {
            printf("cur %s\n", (char *) cur->value);
            printf("\tcur->prev: %s\n", (cur->prev != NULL ? (char *) cur->prev->value : "-"));
            printf("\tcur->next: %s\n", (cur->next != NULL ? (char *) cur->next->value : "-"));
        }
    */
    assert(assert_list_is_sorted(list) == 1 && "Failed to bubble-sort list.");
    
    List_destroy(list);
    
    return NULL;
}

char *all_tests() {
    mu_suite_start();
    
    mu_run_test(test_bubble_sort);
    mu_run_test(test_reverse_manyValues);
    mu_run_test(test_reverse_oneValue);
    mu_run_test(test_reverse_twoValues);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_destroy_emptyList);
    mu_run_test(test_destroy_listContainingMallocatedValues);

    return NULL;
}

RUN_TESTS(all_tests)
