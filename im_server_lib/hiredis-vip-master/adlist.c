/* adlist.c - A generic doubly linked redisList implementation
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this redisList of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this redisList of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdlib.h>
#include "adlist.h"
#include "hiutil.h"

/* Create a new redisList. The created redisList can be freed with
 * AlFreeList(), but private value of every node need to be freed
 * by the user before to call AlFreeList().
 *
 * On error, NULL is returned. Otherwise the pointer to the new redisList. */
redisList *listCreate(void)
{
    struct redisList *redisList;

    if ((redisList = hi_alloc(sizeof(*redisList))) == NULL)
        return NULL;
    redisList->head = redisList->tail = NULL;
    redisList->len = 0;
    redisList->dup = NULL;
    redisList->free = NULL;
    redisList->match = NULL;
    return redisList;
}

/* Free the whole redisList.
 *
 * This function can't fail. */
void listRelease(redisList *redisList)
{
    unsigned long len;
    listNode *current, *next;

    current = redisList->head;
    len = redisList->len;
    while(len--) {
        next = current->next;
        if (redisList->free) redisList->free(current->value);
        hi_free(current);
        current = next;
    }
    hi_free(redisList);
}

/* Add a new node to the redisList, to head, containing the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * redisList remains unaltered).
 * On success the 'redisList' pointer you pass to the function is returned. */
redisList *listAddNodeHead(redisList *redisList, void *value)
{
    listNode *node;

    if ((node = hi_alloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (redisList->len == 0) {
        redisList->head = redisList->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = NULL;
        node->next = redisList->head;
        redisList->head->prev = node;
        redisList->head = node;
    }
    redisList->len++;
    return redisList;
}

/* Add a new node to the redisList, to tail, containing the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * redisList remains unaltered).
 * On success the 'redisList' pointer you pass to the function is returned. */
redisList *listAddNodeTail(redisList *redisList, void *value)
{
    listNode *node;

    if ((node = hi_alloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (redisList->len == 0) {
        redisList->head = redisList->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = redisList->tail;
        node->next = NULL;
        redisList->tail->next = node;
        redisList->tail = node;
    }
    redisList->len++;
    return redisList;
}

redisList *listInsertNode(redisList *redisList, listNode *old_node, void *value, int after) {
    listNode *node;

    if ((node = hi_alloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (after) {
        node->prev = old_node;
        node->next = old_node->next;
        if (redisList->tail == old_node) {
            redisList->tail = node;
        }
    } else {
        node->next = old_node;
        node->prev = old_node->prev;
        if (redisList->head == old_node) {
            redisList->head = node;
        }
    }
    if (node->prev != NULL) {
        node->prev->next = node;
    }
    if (node->next != NULL) {
        node->next->prev = node;
    }
    redisList->len++;
    return redisList;
}

/* Remove the specified node from the specified redisList.
 * It's up to the caller to free the private value of the node.
 *
 * This function can't fail. */
void listDelNode(redisList *redisList, listNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        redisList->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        redisList->tail = node->prev;
    if (redisList->free) redisList->free(node->value);
    hi_free(node);
    redisList->len--;
}

/* Returns a redisList iterator 'iter'. After the initialization every
 * call to listNext() will return the next element of the redisList.
 *
 * This function can't fail. */
listIter *listGetIterator(redisList *redisList, int direction)
{
    listIter *iter;

    if ((iter = hi_alloc(sizeof(*iter))) == NULL) return NULL;
    if (direction == AL_START_HEAD)
        iter->next = redisList->head;
    else
        iter->next = redisList->tail;
    iter->direction = direction;
    return iter;
}

/* Release the iterator memory */
void listReleaseIterator(listIter *iter) {
    hi_free(iter);
}

/* Create an iterator in the redisList private iterator structure */
void listRewind(redisList *redisList, listIter *li) {
    li->next = redisList->head;
    li->direction = AL_START_HEAD;
}

void listRewindTail(redisList *redisList, listIter *li) {
    li->next = redisList->tail;
    li->direction = AL_START_TAIL;
}

/* Return the next element of an iterator.
 * It's valid to remove the currently returned element using
 * listDelNode(), but not to remove other elements.
 *
 * The function returns a pointer to the next element of the redisList,
 * or NULL if there are no more elements, so the classical usage patter
 * is:
 *
 * iter = listGetIterator(redisList,<direction>);
 * while ((node = listNext(iter)) != NULL) {
 *     doSomethingWith(listNodeValue(node));
 * }
 *
 * */
listNode *listNext(listIter *iter)
{
    listNode *current = iter->next;

    if (current != NULL) {
        if (iter->direction == AL_START_HEAD)
            iter->next = current->next;
        else
            iter->next = current->prev;
    }
    return current;
}

/* Duplicate the whole redisList. On out of memory NULL is returned.
 * On success a copy of the original redisList is returned.
 *
 * The 'Dup' method set with listSetDupMethod() function is used
 * to copy the node value. Otherwise the same pointer value of
 * the original node is used as value of the copied node.
 *
 * The original redisList both on success or error is never modified. */
redisList *listDup(redisList *orig)
{
    redisList *copy;
    listIter *iter;
    listNode *node;

    if ((copy = listCreate()) == NULL)
        return NULL;
    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;
    iter = listGetIterator(orig, AL_START_HEAD);
    while((node = listNext(iter)) != NULL) {
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                listRelease(copy);
                listReleaseIterator(iter);
                return NULL;
            }
        } else
            value = node->value;
        if (listAddNodeTail(copy, value) == NULL) {
            listRelease(copy);
            listReleaseIterator(iter);
            return NULL;
        }
    }
    listReleaseIterator(iter);
    return copy;
}

/* Search the redisList for a node matching a given key.
 * The match is performed using the 'match' method
 * set with listSetMatchMethod(). If no 'match' method
 * is set, the 'value' pointer of every node is directly
 * compared with the 'key' pointer.
 *
 * On success the first matching node pointer is returned
 * (search starts from head). If no matching node exists
 * NULL is returned. */
listNode *listSearchKey(redisList *redisList, void *key)
{
    listIter *iter;
    listNode *node;

    iter = listGetIterator(redisList, AL_START_HEAD);
    while((node = listNext(iter)) != NULL) {
        if (redisList->match) {
            if (redisList->match(node->value, key)) {
                listReleaseIterator(iter);
                return node;
            }
        } else {
            if (key == node->value) {
                listReleaseIterator(iter);
                return node;
            }
        }
    }
    listReleaseIterator(iter);
    return NULL;
}

/* Return the element at the specified zero-based index
 * where 0 is the head, 1 is the element next to head
 * and so on. Negative integers are used in order to count
 * from the tail, -1 is the last element, -2 the penultimate
 * and so on. If the index is out of range NULL is returned. */
listNode *listIndex(redisList *redisList, long index) {
    listNode *n;

    if (index < 0) {
        index = (-index)-1;
        n = redisList->tail;
        while(index-- && n) n = n->prev;
    } else {
        n = redisList->head;
        while(index-- && n) n = n->next;
    }
    return n;
}

/* Rotate the redisList removing the tail node and inserting it to the head. */
void listRotate(redisList *redisList) {
    listNode *tail = redisList->tail;

    if (listLength(redisList) <= 1) return;

    /* Detach current tail */
    redisList->tail = tail->prev;
    redisList->tail->next = NULL;
    /* Move it as head */
    redisList->head->prev = tail;
    tail->prev = NULL;
    tail->next = redisList->head;
    redisList->head = tail;
}
