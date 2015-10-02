/*
 * bpGraphAdjList_BL.c
 *
 * Double Linked list implementation of an adjacency list representation.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "linkedList.h"
#include "memoryUtil.h"
#include "binaryTree.h"

#include <stddef.h>
#include "bipartiteGraph.h"


struct implBipartGraph_t
{
	int vertNum1;
	int vertNum2;
	binTreeNode_t *vpVertsP1;
	binTreeNode_t *vpVertsP2;
};


/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
	/* TODO: Implement me! */	
	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));
	pGraph->vertNum1 = part1VertNum;
	pGraph->vertNum2 = part2VertNum;
	pGraph->vpVertsP1 = NULL;
	pGraph->vpVertsP2 = NULL;
	int i;
	int j;
	for (i = 0; i < part1VertNum; i++) {
		if(pGraph->vpVertsP1 == NULL) {
            pGraph->vpVertsP1 = createTreeNode(i, createList());
        } else {
        	binTreeNode_t *pNewNode = createTreeNode(i, createList());
        	insertTreeNode(pGraph->vpVertsP1, pNewNode);            
        }
	}

	for (j = 0; j < part2VertNum; j++) {
		if(pGraph->vpVertsP2 == NULL) {
            pGraph->vpVertsP2 = createTreeNode(j, createList());
        } else {
        	binTreeNode_t *pNewNode = createTreeNode(j, createList());
        	insertTreeNode(pGraph->vpVertsP2, pNewNode);            
        }
	}
	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	/* TODO: Implement me! */
	destroyTree(pGraph->vpVertsP1);
	destroyTree(pGraph->vpVertsP2);
	safeFree(pGraph, sizeof(bpGraph_t));
	pGraph = NULL;
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
		/* TODO: Implement me! */
	binTreeNode_t *currentNode = NULL, *left = NULL, *right = NULL;	

	if (partite == 1) {
		if(pGraph->vpVertsP1 == NULL) {			
            pGraph->vpVertsP1 = createTreeNode(vertId, createList());
            pGraph->vertNum1 += 1;
            return NEW_VERTEX;
        } else {
        	binTreeNode_t *pNewNode = createTreeNode(vertId, createList());       	      	
        	if (insertTreeNode(pGraph->vpVertsP1, pNewNode))  {
        		pGraph->vertNum1 += 1;
        		return NEW_VERTEX;
        	}
        	return EXISTING_VERTEX;
        }
	}

	if (partite == 2) {
		if(pGraph->vpVertsP2 == NULL) {
            pGraph->vpVertsP2 = createTreeNode(vertId, createList());
            pGraph->vertNum2 += 1;
            return NEW_VERTEX;
        } else {
        	binTreeNode_t *pNewNode = createTreeNode(vertId, createList());
        	if (insertTreeNode(pGraph->vpVertsP2, pNewNode))  {
        		pGraph->vertNum2 += 1;
        		return NEW_VERTEX;
        	}         	    
        	return EXISTING_VERTEX; 
        }
	}
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* TODO: Implement me! */
	binTreeNode_t *parent = NULL, *pNode1 = NULL, *pNode2 = NULL, *left = NULL, *right = NULL;
	int leftChild = 0;
	if (srcPartite == 1) {
		if ((pNode1 = searchDeleteNode(pGraph->vpVertsP1, srcVertId, &parent, &leftChild)) != NULL && (pNode2 = searchDeleteNode(pGraph->vpVertsP2, tarVertId, &parent, &leftChild)) != NULL) {
			if (!findElement(pNode1->value, tarVertId)) {
				addNode(pNode1->value, tarVertId);
				return NEW_EDGE;
			} else {
				return EXISTING_EDGE;
			}
		}        
		return ERROR_VALUE;
	}
	else if (srcPartite == 2) {
        if ((pNode2 = searchDeleteNode(pGraph->vpVertsP2, srcVertId, &parent, &leftChild)) != NULL && (pNode1 = searchDeleteNode(pGraph->vpVertsP1, tarVertId, &parent, &leftChild)) != NULL) {
			if (!findElement(pNode2->value, tarVertId)) {
				addNode(pNode2->value, tarVertId);
				return NEW_EDGE;
			} else {
				return EXISTING_EDGE;
			}
		}        
		return ERROR_VALUE;
	}
	return ERROR_VALUE;
	/* TODO: Replace placeholder. */
} /* end of bipartGraphInsertEdge() */

void deleteSublistNode (binTreeNode_t *pTree, int tarVertId) {
	if (pTree != NULL) {
		if (pTree->value) {
			if (findElement(pTree->value, tarVertId)) {				
				deleteNode(pTree->value, tarVertId);
			}
		}
		deleteSublistNode(pTree->left, tarVertId);
		deleteSublistNode(pTree->right, tarVertId);
	}
}


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	binTreeNode_t *parent, *pNode1 = NULL, *pNode2 = NULL;
	
	if (partite == 1) {
		parent = NULL;
		int leftChild = 0;
		if ((pNode1 = searchDeleteNode(pGraph->vpVertsP1, vertId, &parent, &leftChild)) != NULL) {
			deleteSublistNode(pGraph->vpVertsP2, vertId);
			deleteTreeNode(pGraph->vpVertsP1, pNode1, parent, leftChild);			
			return SUCCESS;
		}
		return FAILED;
	}
	if (partite == 2) {
		parent = NULL;
		int leftChild = 0;
		if ((pNode2 = searchDeleteNode(pGraph->vpVertsP2, vertId, &parent, &leftChild)) != NULL) {
			deleteSublistNode(pGraph->vpVertsP1, vertId);
			deleteTreeNode(pGraph->vpVertsP2, pNode2, parent, leftChild);
			return SUCCESS;
		}
		return FAILED;
	}
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
	int errorStatus;
    binTreeNode_t *pNode = NULL, *left = NULL, *right = NULL, *parent=NULL;
    int leftChild = 0;
	if (srcPartite == 1) {
        if ((pNode = searchDeleteNode(pGraph->vpVertsP1, srcVertId, &parent, &leftChild)) != NULL) {
        	return deleteNode(pNode->value, tarVertId);
        } else {
        	return NOT_FOUND;
        }        
	}
	else if (srcPartite == 2) {
        if ((pNode = searchDeleteNode(pGraph->vpVertsP2, srcVertId, &parent, &leftChild)) != NULL) {
        	return deleteNode(pNode->value, tarVertId);
        } else {
        	return NOT_FOUND;
        }        
	}

	return ERROR_VALUE;

} /** end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	/* TODO: Implement me! */

	/* TODO: Replace placeholder. */
	binTreeNode_t *parent = NULL;
	int leftChild = 0;
	if (partite == 1) {
		if (searchDeleteNode(pGraph->vpVertsP1, vertId, &parent, &leftChild) != NULL) {
			return FOUND;
		} else {
			return NOT_FOUND;
		}
	}

	if (partite == 2) {
		if (searchDeleteNode(pGraph->vpVertsP2, vertId, &parent, &leftChild) != NULL) {
			return FOUND;
		} else {
			return NOT_FOUND;
		}
	}
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/** TODO: Implement me! */
	binTreeNode_t *parent = NULL, *pNode1 = NULL, *pNode2 = NULL;
	int leftChild = 0;
	if (srcPartite == 1) {
		if ((pNode1 = searchDeleteNode(pGraph->vpVertsP1, srcVertId, &parent, &leftChild)) != NULL) {
			return findElement(pNode1->value, tarVertId);
		}
		return NOT_FOUND;
	}
	if (srcPartite == 2) {
		if ((pNode2 = searchDeleteNode(pGraph->vpVertsP2, srcVertId, &parent, &leftChild)) != NULL) {
			return findElement(pNode2->value, tarVertId);
		}
		return NOT_FOUND;
	}
	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */

void printLList(binTreeNode_t *pTree);
void printLList(binTreeNode_t *pTree) {
	if (pTree != NULL) {
		if (pTree->value) {
			linkedList_t *currentList = pTree->value;
			llNode_t *currentHead = currentList->pHead;
			while (currentHead != NULL) {
        		printf("%d %d\n", pTree->key, currentHead->element);
				currentHead = currentHead->pNext;
        	}
		}
		printLList(pTree->left);
		printLList(pTree->right);
	}
}


void bipartGraphPrint(bpGraph_t *pGraph)
{
	printf("Vertices:\n");
	printf("Part 1:\n");
    printTree(pGraph->vpVertsP1);
    printf("\n");

	printf("Part 2:\n");
    printTree(pGraph->vpVertsP2);
	printf("\n");

	printf("Edges:\n");
	/* partite 1 to partite 2 edges. */
	printf("Part 1 to 2:\n");
	printLList(pGraph->vpVertsP1);
	
	/* partite 2 to partite 1 edges. */
	printf("Part 2 to 1:\n");
	printLList(pGraph->vpVertsP2);
} /* end of bipartGraphPrint() */
