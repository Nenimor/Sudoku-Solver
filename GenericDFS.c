#include "GenericDFS.h"
#include <math.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "SudokuTree.h"

#define FOUND_BEST 0
#define BEST_NOT_FOUND 1
#define MIN_VALUE 0


/**
 * @brief getBestRecursive This recursive function updates the best valued node in a tree using
 * DFS algorithm.
 * @param head The head of the tree
 * @param getChildren A function that gets a node and a pointer to array of nodes.
 * the function allocates memory for an array of all the children of the node, and
 * returns the number of the children.
 * (the array might be initialized, but the function can't know how many children
 * the node has)
 * @param getVal A function that gets a node and returns its value, as int
 * @param freeNode A function that frees node from memory.
 * this function will be called for each Node returns by getChilds.
 * @param best The best available value for a node, when the function encounters
 * a node with that value it stops looking and returns it.
 * If the best value can't be determined, pass UINT_MAX (defined in limits.h)
 * for that param.
 * @param copy A function that do deep copy of Node.
 * @param maxValNode An updating node, which holds the current best valued node in the tree
 * @param maxVal an updating int, holds the current best value found in the tree
 * If some nodes shares the best valued, the function keeps the first one it encounters.
 * @return returns 0 for success (FOUND_BEST), -1 for memory failure (MEMORY_ERROR) and 1 for failure (BEST_NOT_FOUND)
 */
int getBestRecursive(pNode head, getNodeChildrenFunc getChildren, getNodeValFunc getVal,
                     freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best, pNode* maxValNode,
                     unsigned int* maxVal)
{
    if (getVal(head) > *maxVal)
    {
        *maxVal = getVal(head);

        if (*maxValNode != NULL)
        {
            freeNode(*maxValNode);
        }
        *maxValNode = copy(head);
        if (*maxValNode == NULL) //failed to copy
        {
            return MEMORY_ERROR;
        }
    }

    if (*maxVal == best)
    {
        return FOUND_BEST;
    }

    pNode *children = NULL;
    int numOfChildren, result;

    numOfChildren = getChildren(head, &children);
    if(numOfChildren == MEMORY_ERROR) //in case of an error, return NULL
    {
        return MEMORY_ERROR;
    }
    assert(children != NULL);
    for (int i = 0 ; i < numOfChildren ; i++)
    {
        assert(children[i] != NULL);
        result = getBestRecursive(children[i], getChildren, getVal, freeNode, copy, best,
                                 maxValNode, maxVal);
        if (result == MEMORY_ERROR || result == FOUND_BEST)
        {
            for (int j = i; j < numOfChildren; ++j)
            {
                assert(children[j] != NULL);
                freeNode(children[j]);
                children[j] = NULL;
            }
            free(children);
            children = NULL;
            return result;
        }
        freeNode(children[i]);
        children[i] = NULL;
    }
    if (children != NULL)
    {
        free(children);
        children = NULL;
    }
    return BEST_NOT_FOUND;
}


/**
 * @brief getBest This function returns the best valued node in a tree using
 * DFS algorithm.
 * @param head The head of the tree
 * @param getChildren A function that gets a node and a pointer to array of nodes.
 * the function allocates memory for an array of all the children of the node, and
 * returns the number of the children.
 * (the array might be initialized, but the function can't know how many children
 * the node has)
 * @param getVal A function that gets a node and returns its value, as int
 * @param freeNode A function that frees node from memory.
 * this function will be called for each Node returns by getChilds.
 * @param best The best available value for a node, when the function encounters
 * a node with that value it stops looking and returns it.
 * If the best value can't be determined, pass UINT_MAX (defined in limits.h)
 * for that param.
 * @param copy A function that do deep copy of Node.
 * @return The best valued node in the tree
 * In case of an error, or when all the nodes in the tree valued zero the returns
 * Node is NULL.
 * If some nodes shares the best valued, the function returns the first one it encounters.
 */
pNode getBest(pNode head, getNodeChildrenFunc getChildren, getNodeValFunc getVal, freeNodeFunc
              freeNode, copyNodeFunc copy, unsigned int best)
{
    assert (head != NULL);
    pNode bestValNode = NULL;
    unsigned int maxValue = MIN_VALUE;
    int result = getBestRecursive(head, getChildren, getVal, freeNode, copy, best, &bestValNode,
                                  &maxValue);
    //returns NULL in case of an error OR all nodes valued 0
    if (result == MEMORY_ERROR || maxValue == MIN_VALUE)
    {
        if (bestValNode != NULL)
        {
            freeNode(bestValNode);
            bestValNode = NULL;
        }
        return NULL;
    }
    assert(bestValNode != NULL);
    return bestValNode;
}

