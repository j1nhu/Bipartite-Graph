/*
 * bipartGraphAdjMat.c
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
	int vertNum1;
	int vertNum2;

	int **vpVertsP1;
	int **vpVertsP2;

	int *subCount1;
	int *subCount2;

	char *vVertExistP1;
	char *vVertExistP2;
};



/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
	int i, j;

	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));
	pGraph->vertNum1 = part1VertNum;
	pGraph->vertNum2 = part2VertNum;

	
	
	pGraph->subCount1 = (int *) safeMalloc(pGraph->vertNum1 * sizeof(int));
	pGraph->vpVertsP1 = (int **) safeMalloc(pGraph->vertNum1 * sizeof(int *));
	pGraph->vVertExistP1 = (char *) safeMalloc(pGraph->vertNum1 * sizeof(char));

	for (i = 0; i < pGraph->vertNum1; ++i) {
		pGraph->vpVertsP1[i] = (int *) safeMalloc(pGraph->vertNum2 * sizeof(int));
		for (j = 0; j< pGraph->vertNum2; j++)
            pGraph->vpVertsP1[i][j] = 0;
		pGraph->subCount1[i] = pGraph->vertNum2;
		pGraph->vVertExistP1[i] = 1;
	}

	pGraph->subCount2 = (int *) safeMalloc(pGraph->vertNum2 * sizeof(int));
	pGraph->vpVertsP2 = (int **) safeMalloc(pGraph->vertNum2 * sizeof(int *));
	pGraph->vVertExistP2 = (char *) safeMalloc(pGraph->vertNum2 * sizeof(char));

	for (i = 0; i < pGraph->vertNum2; ++i) {
		pGraph->vpVertsP2[i] = (int *) safeMalloc(pGraph->vertNum1 * sizeof(int));
		for (j = 0; j< pGraph->vertNum1; j++)
            pGraph->vpVertsP2[i][j] = 0;
		pGraph->subCount2[i] = pGraph->vertNum1;
		pGraph->vVertExistP2[i] = 1;
	}
	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	int i;

	safeFree(pGraph->vVertExistP1, sizeof(char) * pGraph->vertNum1);
	safeFree(pGraph->vVertExistP2, sizeof(char) * pGraph->vertNum2);

	/* free the linked lists */
	for (i = 0; i < pGraph->vertNum1; ++i)
	{
		safeFree(pGraph->vpVertsP1[i],sizeof(int) * (pGraph->subCount1[i]));
	}
	for (i = 0; i < pGraph->vertNum2; ++i)
	{
		safeFree(pGraph->vpVertsP2[i],sizeof(int) * (pGraph->subCount2[i]));
	}

	safeFree(pGraph->vpVertsP1, sizeof(int*) * pGraph->vertNum1);
	safeFree(pGraph->vpVertsP2, sizeof(int*) * pGraph->vertNum2);

	safeFree(pGraph->subCount1, sizeof(int) * pGraph->vertNum1);
	safeFree(pGraph->subCount2, sizeof(int) * pGraph->vertNum2);

	safeFree(pGraph, sizeof(bpGraph_t));
	pGraph = NULL;

} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	int i,j;

	if (partite == 1) {
		if (vertId < pGraph->vertNum1) {
			if (pGraph->vVertExistP1[vertId]) {
				return EXISTING_VERTEX;
			}
			else {			
				pGraph->vVertExistP1[vertId] = 1;
				return NEW_VERTEX;
			}
		}
		else {
			int **pTemp = (int**) safeRealloc(pGraph->vpVertsP1, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum1) * sizeof(int*));
			pGraph->vpVertsP1 = pTemp;
			int *pTemp2 = (int*) safeRealloc(pGraph->subCount1, (vertId+1) * sizeof(int), (vertId+1-pGraph->vertNum1) * sizeof(int));
			pGraph->subCount1 = pTemp2;
            char *pTemp3 = (char*) safeRealloc(pGraph->vVertExistP1, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum1) * sizeof(char));
			pGraph->vVertExistP1 = pTemp3;
			/* initialise all new elements to NULL */
			for (i = pGraph->vertNum1; i < vertId+1; ++i) {
				pGraph->vpVertsP1[i] = NULL;
				pGraph->vVertExistP1[i] = 0;
				pGraph->subCount1[i] = 0;
			}


            pGraph->vertNum1 = vertId+1;

			pGraph->vpVertsP1[vertId] = (int *) safeMalloc(pGraph->vertNum2 * sizeof(int));
            for (j = 0; j< pGraph->vertNum2; j++)
                pGraph->vpVertsP1[vertId][j] = 0;
			pGraph->vVertExistP1[vertId] = 1;
            pGraph->subCount1[vertId] = pGraph->vertNum2;

			return NEW_VERTEX;
		}
	}
	/* insert into partite 1 */
	else if (partite == 2) {
		/* check if 'vertid' is an existing vertex */
		if (vertId < pGraph->vertNum2) {
			if (pGraph->vVertExistP2[vertId]) {
				return EXISTING_VERTEX;
			}
			else {
				pGraph->vVertExistP2[vertId] = 1;
				return NEW_VERTEX
			}
		}
		else {
			/* must be a new vertex, so we need to allocate more space */
			int **pTemp = (int**) safeRealloc(pGraph->vpVertsP2, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum2) * sizeof(int*));
			pGraph->vpVertsP2 = pTemp;
			int *pTemp2 = (int*) safeRealloc(pGraph->subCount2, (vertId+1) * sizeof(int), (vertId+1-pGraph->vertNum2) * sizeof(int));
			pGraph->subCount2 = pTemp2;
            char *pTemp3 = (char*) safeRealloc(pGraph->vVertExistP2, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum2) * sizeof(char));
			pGraph->vVertExistP2 = pTemp3;
			/* initialise all new elements to NULL */
			for (i = pGraph->vertNum2; i < vertId+1; ++i) {
				pGraph->vpVertsP2[i] = NULL;
				pGraph->vVertExistP2[i] = 0;
				pGraph->subCount2[i] = 0;
			}


            pGraph->vertNum2 = vertId+1;
			pGraph->vpVertsP2[vertId] = (int *) safeMalloc(pGraph->vertNum1 * sizeof(int));
            for (j = 0; j< pGraph->vertNum1; j++)
                pGraph->vpVertsP2[vertId][j] = 0;
			pGraph->vVertExistP2[vertId] = 1;
            pGraph->subCount2[vertId] = pGraph->vertNum1;

			return NEW_VERTEX;
		}
	}

	/* Error, unknown partite! */
	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
    int i,j;
	if (srcPartite == 1) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}

		/* Need to check for duplicates */
		if (tarVertId < pGraph->subCount1[srcVertId]) {
            if( pGraph->vpVertsP1[srcVertId][tarVertId]) {
                return EXISTING_EDGE;
            }
            else {
                pGraph->vpVertsP1[srcVertId][tarVertId] = 1;
                return NEW_EDGE;
            }
		} else {
            int *pTemp2 = (int*) safeRealloc(pGraph->vpVertsP1[srcVertId], (pGraph->vertNum2) * sizeof(int), (pGraph->vertNum2-pGraph->subCount1[srcVertId]) * sizeof(int));
			pGraph->vpVertsP1[srcVertId] = pTemp2;

            for(j = pGraph->subCount1[srcVertId] ; j < pGraph->vertNum2; j++)
                pGraph->vpVertsP1[srcVertId][j] = 0;
            pGraph->subCount1[srcVertId] = pGraph->vertNum2;
            pGraph->vpVertsP1[srcVertId][tarVertId] = 1;
            return NEW_EDGE;
		}
	}
	else if (srcPartite == 2) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}

		/* Need to check for duplicates */
		if (tarVertId < pGraph->subCount2[srcVertId]) {
            if( pGraph->vpVertsP2[srcVertId][tarVertId]) {
                return EXISTING_EDGE;
            }
            else {
                pGraph->vpVertsP2[srcVertId][tarVertId] = 1;
                return NEW_EDGE;
            }
		} else {
            int *pTemp2 = (int*) safeRealloc(pGraph->vpVertsP2[srcVertId], (pGraph->vertNum1) * sizeof(int), (pGraph->vertNum1-pGraph->subCount1[srcVertId]) * sizeof(int));
			pGraph->vpVertsP2[srcVertId] = pTemp2;

            for(j = pGraph->subCount2[srcVertId] ; j < pGraph->vertNum1; j++)
                pGraph->vpVertsP2[srcVertId][j] = 0;
            pGraph->subCount2[srcVertId] = pGraph->vertNum1;
            pGraph->vpVertsP2[srcVertId][tarVertId] = 1;
            return NEW_EDGE;
		}
	}

	return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	if (partite == 1) {
		/* check if vertex exists */
		if (pGraph->vVertExistP1[vertId]) {
			/* delete neighbours */
			safeFree(pGraph->vpVertsP1[vertId],sizeof(int)*pGraph->subCount1[vertId]);
			pGraph->vpVertsP1[vertId] = NULL;
			pGraph->vVertExistP1[vertId] = 0;
            pGraph->subCount1[vertId] = 0;
			/* search the in neighbours and delete them */
			

			return FOUND;
		}

		/* vertex not in graph */
		return NOT_FOUND;
	}
	else if (partite == 2) {
		/* check if vertex exists */
		if (pGraph->vVertExistP2[vertId]) {
			/* delete neighbours */
			safeFree(pGraph->vpVertsP2[vertId],sizeof(int)*pGraph->subCount2[vertId]);
			pGraph->vpVertsP2[vertId] = NULL;
			pGraph->vVertExistP2[vertId] = 0;
            pGraph->subCount2[vertId] = 0;
			/* search the in neighbours and delete them */
			

			return FOUND;
		}

		return NOT_FOUND;
	}

	/** Wrong partite. */
	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
	int errorStatus;

	if (srcPartite == 1) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}

		/* delete linked list node */
		if(tarVertId < pGraph->subCount1[srcVertId] && pGraph->vpVertsP1[srcVertId][tarVertId]) {
            pGraph->vpVertsP1[srcVertId][tarVertId] = 0;
            return FOUND;
		}
        else
			return NOT_FOUND;
	}
	else if (srcPartite == 2) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}

		/* delete linked list node */
		if(tarVertId < pGraph->subCount2[srcVertId] && pGraph->vpVertsP2[srcVertId][tarVertId]) {
            pGraph->vpVertsP2[srcVertId][tarVertId] = 0;
            return FOUND;
		}
        else
			return NOT_FOUND;
	}

	return ERROR_VALUE;
} /** end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	if (partite == 1) {
		if (vertId >= pGraph->vertNum1) {
			return NOT_FOUND;
		}

		return FOUND;
	}
	if (partite == 2) {
		if (vertId >= pGraph->vertNum2) {
			return NOT_FOUND;
		}

		return FOUND;
	}

	/* unknown partite */
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* check if vertices exists */
	if (srcPartite == 1) {
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return NOT_FOUND;
		}
        if(tarVertId < pGraph->subCount1[srcVertId] && pGraph->vpVertsP1[srcVertId][tarVertId]) {
            return FOUND;
		}
        else
			return NOT_FOUND;
	}
	else if (srcPartite == 2) {
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return NOT_FOUND;
		}

        if(tarVertId < pGraph->subCount2[srcVertId] && pGraph->vpVertsP2[srcVertId][tarVertId]) {
            return FOUND;
		}
        else
			return NOT_FOUND;
	}

	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
	int i,j;

	/* print vertices */
	printf("Vertices:\n");
	printf("Part 1:\n");
	for (i = 0; i < pGraph->vertNum1; ++i) {
		if (pGraph->vVertExistP1[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");

	printf("Part 2:\n");
	for (i = 0; i < pGraph->vertNum2; ++i) {
		if (pGraph->vVertExistP2[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");

	/* print edges */
	printf("Edges:\n");
	/* partite 1 to partite 2 edges. */
	printf("Part 1 to 2:\n");
	for (i = 0; i < pGraph->vertNum1; ++i) {
		if (pGraph->vpVertsP1[i] != NULL) {
			/* TODO: You are welcome to encapsulate this by creating a new printing function. */
            for (j = 0 ; j < pGraph->subCount1[i]; j++) {
                if(pGraph->vpVertsP1[i][j]) {
                    printf("%d %d\n", i, j);
                }
            }
		}
	}

	/* partite 2 to partite 1 edges. */
	printf("Part 2 to 1:\n");
	for (i = 0; i < pGraph->vertNum2; ++i) {
		if (pGraph->vpVertsP2[i] != NULL) {
			/* TODO: You are welcome to encapsulate this by creating a new printing function. */
            for (j = 0 ; j < pGraph->subCount2[i]; j++) {
                if(pGraph->vpVertsP2[i][j]) {
                    printf("%d %d\n", i, j);
                }
            }
		}
	}
} /* end of bipartGraphPrint() */

/* ********************************************************** */

/* end of findAndDelete() */

