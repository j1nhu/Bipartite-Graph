/*
 * bpGraphAdjMat_AA.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "bipartiteGraph.h"
#include "linkedList.h"
#include "memoryUtil.h"



struct implBipartGraph_t
{
    /** Vertex number of partite 1. */
    int vertNum1;
    /** Vertex number of partite 2. */
    int vertNum2;    
    vtNode_t *vpVpertsP1;
    vtNode_t *vpVpertsP2;
};


/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{    
    int i;
    vtNode_t *currentNode = NULL;
    
    bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));
    pGraph->vertNum1 = part1VertNum;
    pGraph->vertNum2 = part2VertNum;
    pGraph->vpVpertsP1 = NULL;
    pGraph->vpVpertsP2 = NULL;   
    /* initialize linked list */   
    for(i = 0; i < part1VertNum; i++){
        if(pGraph->vpVpertsP1 == NULL){
            pGraph->vpVpertsP1 = safeMalloc(sizeof(vtNode_t));
            currentNode = pGraph->vpVpertsP1;
        }else{
            currentNode->next = safeMalloc(sizeof(vtNode_t));
            currentNode = currentNode->next;
        }
        currentNode->element = i;        
        currentNode->vVertExist = 1;
        currentNode->sub_linkedlist = createList();
        currentNode->next = NULL;       
    }
    
    for(i = 0; i < part2VertNum; i++){
        if(pGraph->vpVpertsP2 == NULL){
            pGraph->vpVpertsP2 = safeMalloc(sizeof(vtNode_t));
            currentNode = pGraph->vpVpertsP2;
        }else{
            currentNode->next = safeMalloc(sizeof(vtNode_t));
            currentNode = currentNode->next;
        }
        currentNode->element = i;
        currentNode->vVertExist = 1;
        currentNode->sub_linkedlist = createList();
        currentNode->next = NULL;      
    }   
    return pGraph;       
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	vtNode_t *currentNode1, *tempNode1;
	vtNode_t *currentNode2, *tempNode2;
	currentNode1 = pGraph->vpVpertsP1;
	currentNode2 = pGraph->vpVpertsP2;
	while(currentNode1 != NULL)
	{
		destroyList(currentNode1->sub_linkedlist);
      	tempNode1 = currentNode1;
      	currentNode1 = currentNode1->next;
      	safeFree(tempNode1, sizeof(vtNode_t));
      	tempNode1 = NULL;     	
	}

	while(currentNode2 != NULL)
	{
		destroyList(currentNode2->sub_linkedlist);
      	tempNode2 = currentNode2;
      	currentNode2 = currentNode2->next;
      	safeFree(tempNode2, sizeof(vtNode_t));
      	tempNode2 = NULL;
	}
	
	pGraph->vpVpertsP1 = NULL;
	pGraph->vpVpertsP2 = NULL;
	pGraph->vertNum1 = 0;
	pGraph->vertNum2 = 0;
	safeFree(pGraph, sizeof(bpGraph_t));

} /* end of bipartGraphDestroy() */

vtNode_t* findVTNode(bpGraph_t* pGraph, int vertId, int partite)
{

    if(pGraph == NULL) {
    	return NULL;
    }

    if (partite == 1)
    {
    	vtNode_t *pCurrNode = pGraph->vpVpertsP1;
		while (pCurrNode != NULL) {
			if (pCurrNode->element == vertId) {
				return pCurrNode;
			}
			pCurrNode = pCurrNode->next;
		}
		return NULL;   	
    }

    if (partite == 2)
    {
    	vtNode_t *pCurrNode = pGraph->vpVpertsP2;
		while (pCurrNode != NULL) {
			if (pCurrNode->element == vertId) {
				return pCurrNode;
			}
			pCurrNode = pCurrNode->next;
		}
		return NULL;
    }    
} 


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	if(partite == 1) {				
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, vertId, 1))!= NULL) {
			return EXISTING_VERTEX;
		} else {
			vtNode_t *newNode = (vtNode_t*) safeMalloc(sizeof(vtNode_t));
			newNode->element = vertId;
			newNode->next = pGraph->vpVpertsP1;	
			pGraph->vpVpertsP1 = newNode;
			newNode->sub_linkedlist = createList();	
			newNode->vVertExist = 1;		
			pGraph->vertNum1 += 1;
			return NEW_VERTEX;
		}
	} else if (partite == 2) {
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, vertId, 2))!= NULL) {
			return EXISTING_VERTEX;
		} else {
			vtNode_t *newNode = (vtNode_t*) safeMalloc(sizeof(vtNode_t));
			newNode->element = vertId;
			newNode->next = pGraph->vpVpertsP2;	
			pGraph->vpVpertsP2 = newNode;
			newNode->sub_linkedlist = createList();	
			newNode->vVertExist = 1;		
			pGraph->vertNum2 += 1;
			return NEW_VERTEX;
		}
	}	
	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* TODO: Implement me! */
	if (srcPartite == 1) {
        vtNode_t *currentNode = NULL;
        vtNode_t *tarNode = NULL;
        if ((currentNode=findVTNode(pGraph, srcVertId, 1))!= NULL && (tarNode=findVTNode(pGraph, tarVertId, 2))!= NULL) {
        	if (!findElement(currentNode->sub_linkedlist, tarVertId)) {
        		addNode(currentNode->sub_linkedlist, tarVertId);
            	return NEW_EDGE;
        	}
        	return EXISTING_EDGE;          
        } else {
        	return NOT_FOUND;
        }
	} else if (srcPartite == 2) {
        vtNode_t *currentNode = NULL;
        vtNode_t *tarNode = NULL;
        if ((currentNode=findVTNode(pGraph, srcVertId, 2))!= NULL && (tarNode=findVTNode(pGraph, tarVertId, 1))!= NULL) {
            if (!findElement(currentNode->sub_linkedlist, tarVertId)) {
        		addNode(currentNode->sub_linkedlist, tarVertId);
            	return NEW_EDGE;
        	}
        	return EXISTING_EDGE; 
        } else {
        	return NOT_FOUND;
        }       
	}

	/* TODO: Replace placeholder. */
	return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


void deleteSublistNode(bpGraph_t *pGraph, int vertId, int partite)
{
	/* search for all elements in in-neighbourhood. */
	if (partite == 1) {
        vtNode_t *currentNode = pGraph->vpVpertsP2;
        while (currentNode != NULL) {
            if(currentNode->sub_linkedlist) {
                deleteNode(currentNode->sub_linkedlist,vertId);
            }
            currentNode = currentNode->next;
        }
	}
	else if (partite == 2) {
        vtNode_t *currentNode = pGraph->vpVpertsP1;
        while (currentNode != NULL) {
            if(currentNode->sub_linkedlist) {
                deleteNode(currentNode->sub_linkedlist,vertId);
            }
            currentNode = currentNode->next;
        }
	}
} /* end of findAndDelete() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	/* TODO: Implement me! */
	vtNode_t *currentNode = NULL, *previousNode = NULL;
    vtNode_t *tempNode = NULL;	
	if (partite == 1) {
		currentNode = pGraph->vpVpertsP1;
		while(currentNode != NULL) {
			if (currentNode->element == vertId) {				
    			if(previousNode == NULL)
    			{
    				destroyList(currentNode->sub_linkedlist);
    				tempNode = currentNode;
    				pGraph->vpVpertsP1 = currentNode->next;
    				safeFree(tempNode, sizeof(vtNode_t));   				
    			}
    			else
    			{
    				destroyList(currentNode->sub_linkedlist);
    				tempNode = currentNode;
    				previousNode->next = currentNode->next;
    				safeFree(tempNode, sizeof(vtNode_t));
    			}
    			deleteSublistNode(pGraph, vertId, 2);
    			return SUCCESS;
			}
			previousNode = currentNode;
    		currentNode = currentNode->next;
		}
		return FAILED;
	}else if (partite == 2) {
		currentNode = pGraph->vpVpertsP2;
		while(currentNode != NULL) {
			if (currentNode->element == vertId) {				
    			if(previousNode == NULL)
    			{
    				destroyList(currentNode->sub_linkedlist);
    				tempNode = currentNode;
    				pGraph->vpVpertsP2 = currentNode->next;
    				safeFree(tempNode, sizeof(vtNode_t));   				
    			}
    			else
    			{
    				destroyList(currentNode->sub_linkedlist);
    				tempNode = currentNode;
    				previousNode->next = currentNode->next;
    				safeFree(tempNode, sizeof(vtNode_t));
    			}
    			deleteSublistNode(pGraph, vertId, 1);
    			return SUCCESS;
			}
			previousNode = currentNode;
    		currentNode = currentNode->next;
    	}
    	return FAILED;
	}
	/* TODO: Replace placeholder. */
	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* TODO: Implement me! */
	int errorStatus;

	if(srcPartite == 1) {				
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, srcVertId, 1)) != NULL) {
			linkedList_t *currentList = currentNode->sub_linkedlist;
			errorStatus = deleteNode(currentList, tarVertId);
		} 

		if (!errorStatus) {
			return NOT_FOUND;
		}
		/* vertex not in graph */
		return FOUND;
	}

	if(srcPartite == 2) {				
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, srcVertId, 2)) != NULL) {
			linkedList_t *currentList = currentNode->sub_linkedlist;
			errorStatus = deleteNode(currentList, tarVertId);
		} 
		
		if (!errorStatus) {
			return NOT_FOUND;
		}
		/* vertex not in graph */
		return FOUND;
	}
	return ERROR_VALUE;
} /** end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	if(pGraph == NULL) {
    	return NULL;
    }

    if (partite == 1)
    {
    	vtNode_t *pCurrNode = pGraph->vpVpertsP1;
		while (pCurrNode != NULL) {
			if (pCurrNode->element == vertId) {
				return FOUND;
			}
			pCurrNode = pCurrNode->next;
		}
		return NOT_FOUND;   	
    }

    if (partite == 2)
    {
    	vtNode_t *pCurrNode = pGraph->vpVpertsP2;
		while (pCurrNode != NULL) {
			if (pCurrNode->element == vertId) {
				return FOUND;
			}
			pCurrNode = pCurrNode->next;
		}
		return NOT_FOUND;   	
    }	
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	if(pGraph == NULL) {
    	return NULL;
    }

	if (srcPartite == 1) {
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, srcVertId, 1)) != NULL) {
			return findElement(currentNode->sub_linkedlist, tarVertId);
		} else {
			return NOT_FOUND;
		}
	}

	if (srcPartite == 2) {
		vtNode_t *currentNode = NULL;
		if ((currentNode=findVTNode(pGraph, srcVertId, 2)) != NULL) {
			return findElement(currentNode->sub_linkedlist, tarVertId);
		} else {
			return NOT_FOUND;
		}
	}
	/* TODO: Replace placeholder. */
	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
	vtNode_t *currentNode = NULL;
	/* print vertices */
	printf("Vertices:\n");
	printf("Part 1:\n");
	currentNode = pGraph->vpVpertsP1;
	while(currentNode) {
        printf("%d ",currentNode->element);
        currentNode = currentNode->next;
	}
	printf("\n");

	printf("Part 2:\n");
	currentNode = pGraph->vpVpertsP2;
	while(currentNode) {
        printf("%d ",currentNode->element);
        currentNode = currentNode->next;
	}
	printf("\n");

	/* print edges */
	printf("Edges:\n");
	/* partite 1 to partite 2 edges. */
	printf("Part 1 to 2:\n");
	currentNode = pGraph->vpVpertsP1;
	while(currentNode) {
		if (currentNode->sub_linkedlist) {
			linkedList_t *currentList = currentNode->sub_linkedlist;
        	llNode_t *currentHead = currentList->pHead;
        	while (currentHead != NULL) {
        		printf("%d %d\n", currentNode->element, currentHead->element);
				currentHead = currentHead->pNext;
        	}
		}
        currentNode = currentNode->next;
	}
	
	/* partite 2 to partite 1 edges. */
	printf("Part 2 to 1:\n");
	currentNode = pGraph->vpVpertsP2;
	while(currentNode) {
		if (currentNode->sub_linkedlist) {
			linkedList_t *currentList = currentNode->sub_linkedlist;
        	llNode_t *currentHead = currentList->pHead;
        	while (currentHead != NULL) {
        		printf("%d %d\n", currentNode->element, currentHead->element);
				currentHead = currentHead->pNext;
        	}
		}
        currentNode = currentNode->next;
	}
	printf("\n");
} /* end of bipartGraphPrint() */

