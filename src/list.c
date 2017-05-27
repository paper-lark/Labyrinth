/**********************************************************************************
 * Copyright (c) 2017 Max Zhuravsky                                               *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

/**********************************************************************************
 *                                 Labyrinth Game                                 *
 *                  Author:  Max Zhuravsky <paperlark@yandex.ru>                  *
 **********************************************************************************/

#include "list.h"
#include <stdlib.h>
#include <assert.h>

int isEmpty(List *list) {
    return list->length == 0;
}

List *nlist() {
    List *temp = malloc( sizeof(List) );
    temp->length = 0;
    temp->next = NULL;
    return temp;
}

void add(List *list, unsigned x, unsigned y, Direction dir) {
    list->length++;
    Entry *temp = malloc( sizeof(Entry) );
    temp->value.x = x;
    temp->value.y = y;
    temp->value.dir = dir;
    temp->next = list->next;
    list->next = temp;
}

Entry *getrandom(List *list) {
    assert(!isEmpty(list));
    int n = rand() % list->length;
    Entry *current = list->next;
    if (n == 0) {
        list->next = current->next;
        current->next = NULL;
        list->length--;
        return current;
    } else {
        Entry *previous = NULL;
        for(int i = 0; i < n; i++) {
            previous = current;
            current = current->next;
        }
        previous->next = current->next;
        current->next = NULL;
        list->length--;
        return current;
    }
}