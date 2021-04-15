//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Vojtech Eichler <xeichl01@stud.fit.vutbr.cz>
// $Date:       $2021-02-26
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Vojtech Eichler
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
    //goes through the queue and deletes all elements
    Element_t *elem = m_pHead;
    Element_t *tmp = NULL;
    while (elem != NULL) {
        tmp = elem->pNext;
        delete elem;
        elem = tmp;
    }
    
}

void PriorityQueue::Insert(int value)
{
    //if queue is empty, inserts element at the beginning
    if (m_pHead == NULL) {
        m_pHead = new Element_t;
        m_pHead->pNext = NULL;
        m_pHead->value = value;
    } else {
        Element_t *elem = m_pHead;
        Element_t *newElement = new Element_t();
        newElement->value = value;
        //checks if value that's being inserted isn't also supposed to
        //be at the beginning
        if (m_pHead->value <= value) {
            newElement->pNext = m_pHead;
            m_pHead = newElement;
            return;
        }
        //looks for a spot where our element is supposed to be inserted
        while (elem->pNext != NULL) {
            if (elem->pNext->value <= value) {
                
                newElement->pNext = elem->pNext;
                elem->pNext = newElement;
                return;
            }
            elem = elem->pNext;
        }
        //inserted value is also lowest -> should be at the end
        elem->pNext = newElement;
        elem->pNext->pNext = NULL;
    }
    
}

bool PriorityQueue::Remove(int value)
{
    Element_t *del = NULL;
    Element_t *tmp = NULL;
    if (m_pHead == NULL) {
        return false;
    } else {
        //if head is of a value that's supposed to be removed, then it removes it
        if (m_pHead->value == value) {
            tmp = m_pHead->pNext;
            delete m_pHead;
            m_pHead = tmp;
            return true;
        }
        //looking for an element to delete,
        //then rearranging pointers so that the queue is
        //in a correct state
        tmp = m_pHead;
        while (tmp->pNext != NULL) {
            if (tmp->pNext->value == value) {
                del = tmp->pNext;
                tmp->pNext = tmp->pNext->pNext;
                delete del;
                return true;
            }
            tmp = tmp->pNext;
        }
        return false;
    } 
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *elem = m_pHead;
    while (elem != NULL) {
        if (elem->value == value) {
            return elem;
        }
        elem = elem->pNext;
    }
    return NULL;
}

size_t PriorityQueue::Length()
{
    size_t len = 0;
    Element_t *elem = m_pHead;
    while (elem != NULL) {
        len++;
        elem = elem->pNext;
    }
	return len;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
    
}

/*** Konec souboru tdd_code.cpp ***/
