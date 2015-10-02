/*
 * bipartGraphAdjMat.c
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
 */

#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "bipartiteGraph.h"
#include "memoryUtil.h"


struct implBipartGraph_t
{
	int vertNum1;
	int vertNum2;

	char *vVertExistP1;
	char *vVertExistP2;

	int **vpVertsP1;
	int **vpVertsP2;

	int *subCount1;
	int *subCount2;
};


/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
	/* TODO: Implement me! */

	int i, j;
	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));

	pGraph->vVertExistP1 = (char *) safeMalloc(part1VertNum * sizeof(char));
	pGraph->vpVertsP1 = (int **) safeMalloc(part1VertNum * sizeof(int *));
	pGraph->subCount1 = (int *) safeMalloc(part1VertNum * sizeof(int));

	for (i = 0; i < part1VertNum; ++i) {
		pGraph->vpVertsP1[i] = (int *) safeMalloc(part2VertNum * sizeof(int));
		for (j = 0; j< part2VertNum; j++) {
			printf("allocated pointer for edges1\n");
            pGraph->vpVertsP1[i][j] = 0;

		}
		pGraph->vVertExistP1[i] = 1;
		pGraph->subCount1 = part2VertNum;
	}

	pGraph->vVertExistP2 = (char *) safeMalloc(part2VertNum * sizeof(char));
	pGraph->vpVertsP2 = (int **) safeMalloc(part2VertNum * sizeof(int *));
	pGraph->subCount2 = (int *) safeMalloc(part2VertNum * sizeof(int));

	for (i = 0; i < part2VertNum; ++i) {
		pGraph->vpVertsP2[i] = (int *) safeMalloc(part1VertNum * sizeof(int));
		for (j = 0; j< part1VertNum; j++){
            pGraph->vpVertsP2[i][j] = 0;
		}
		pGraph->vVertExistP2[i] = 1;
		pGraph->subCount2 = part1VertNum;
	}

	pGraph->vertNum1 = part1VertNum;
	pGraph->vertNum2 = part2VertNum;
	
	/* TODO: Replace placeholder. */
	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	/* TODO: Implement me! */
	int i;

	safeFree(pGraph->vVertExistP1, sizeof(pGraph->vertNum1 * sizeof(char)));
	safeFree(pGraph->vVertExistP2, sizeof(pGraph->vertNum2 * sizeof(char)));

	for (i = 0; i < pGraph->vertNum1; ++i)
	{
		safeFree(pGraph->vpVertsP1[i], sizeof(pGraph->vertNum2 * sizeof(int)));
	}
	for (i = 0; i < pGraph->vertNum2; ++i)
	{
		safeFree(pGraph->vpVertsP2[i], sizeof(pGraph->vertNum1 * sizeof(int)));
	}

	safeFree(pGraph->vpVertsP1, sizeof(pGraph->vertNum1 * sizeof(int *)));	
	safeFree(pGraph->vpVertsP2, sizeof(pGraph->vertNum2 * sizeof(int *)));	
	safeFree(pGraph->subCount1, sizeof(pGraph->vertNum1 * sizeof(int)));
	safeFree(pGraph->subCount2, sizeof(pGraph->vertNum2 * sizeof(int)));
	safeFree(pGraph, sizeof(bpGraph_t));
	pGraph = NULL;

} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	/* TODO: Implement me! */
	int **pTemp;
	char *pTemp2;
	int *pTemp3;
	int i;
	int j;
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
			pTemp = (int **) safeRealloc(pGraph->vpVertsP1, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum1) * sizeof(int*));
			pGraph->vpVertsP1 = pTemp;
			pTemp2 = (char *) safeRealloc(pGraph->vVertExistP1, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum1) * sizeof(char));
			pGraph->vVertExistP1 = pTemp2;
			printf("hello\n");
			printf("%d\n", pGraph->subCount1);
			pTemp3 = (int *) safeRealloc(pGraph->subCount1, (vertId+1) * sizeof(int), (vertId+1-pGraph->vertNum1) * sizeof(int));
			printf("world\n");
			pGraph->subCount1 = pTemp3;
			

			for (i = pGraph->vertNum1; i < vertId+1; ++i) {
				pGraph->vpVertsP1[i] = NULL;
				pGraph->vVertExistP1[i] = 0;
				pGraph->subCount1[i] = 0;
			}

            pGraph->vertNum1 = vertId+1;
			pGraph->vpVertsP1[vertId] = (int *) safeMalloc(pGraph->vertNum2 * sizeof(int));

            for (j = 0; j< pGraph->vertNum1; j++) {
                pGraph->vpVertsP1[vertId][j] = 0;
            }
			pGraph->vVertExistP1[vertId] = 1;
			pGraph->subCount1[vertId] = pGraph->vertNum2;

			return NEW_VERTEX;

		}

	} else if (partite == 2) {
		if (vertId < pGraph->vertNum2) {
			if (pGraph->vVertExistP2[vertId]) {
				return EXISTING_VERTEX;
			}
			else {			
				pGraph->vVertExistP2[vertId] = 1;
				return NEW_VERTEX;
			}
		}
		else {
			pTemp = (int**) safeRealloc(pGraph->vpVertsP2, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum2) * sizeof(int*));
			pGraph->vpVertsP2 = pTemp;
			pTemp2 = (char*) safeRealloc(pGraph->vVertExistP2, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum2) * sizeof(char));
			pGraph->vVertExistP2 = pTemp2;

			pTemp3 = (int*) safeRealloc(pGraph->subCount2, (vertId+1) * sizeof(int), (vertId+1-pGraph->vertNum2) * sizeof(int));
			pGraph->subCount2 = pTemp3;

			for (i = pGraph->vertNum2; i < vertId+1; ++i) {
				pGraph->vpVertsP2[i] = NULL;
				pGraph->vVertExistP2[i] = 0;
				pGraph->subCount2[i] = 0;
			}

            pGraph->vertNum2 = vertId+1;
			pGraph->vpVertsP2[vertId] = (int *) safeMalloc(pGraph->vertNum1 * sizeof(int));

            for (j = 0; j< pGraph->vertNum1; j++) {
                pGraph->vpVertsP2[vertId][j] = 0;
            }
			pGraph->vVertExistP2[vertId] = 1;
			pGraph->subCount2[vertId] = pGraph->vertNum1;

			return NEW_VERTEX;

		}

	} 
	/* TODO: Replace placeholder. */
	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* TODO: Implement me! */
	if (srcPartite == 1) {
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}
		if (pGraph->vVertExistP1[srcVertId]) { 
			if (pGraph->vpVertsP1[srcVertId][tarVertId]) {
				return EXISTING_EDGE;
			} else {
				pGraph->vpVertsP1[srcVertId][tarVertId] = tarVertId;
				return NEW_EDGE;
			}
		}
		return ERROR_VALUE;
	}
	else if (srcPartite == 2) {
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}
		if (pGraph->vVertExistP2[srcVertId]) { 
			if (pGraph->vpVertsP2[srcVertId][tarVertId]) {
				printf("%d\n", tarVertId);
				printf("passed found number\n");
				return EXISTING_EDGE;
			} else {
				pGraph->vpVertsP2[srcVertId][tarVertId] = tarVertId;
				return NEW_EDGE;
			}
		}
		return ERROR_VALUE;
	}
	return ERROR_VALUE;

} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* graph, int vertId, int partite)
{
	/* TODO: Implement me! */

	/* TODO: Replace placeholder. */
	return 0;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
	/* TODO: Implement me! */

	/* TODO: Replace placeholder. */
	return 0;
} /** end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	/* TODO: Implement me! */

	/* TODO: Replace placeholder. */
	return 0;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* graph, int srcVertId, int tarVertId, int srcPartite)
{
	/** TODO: Implement me! */

	/* TODO: Replace placeholder. */
	return 0;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
	int i;
	int j;
	int *pCurrNode = NULL;

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
	for (i = 0; i < pGraph->vertNum1; i++) {
		if (pGraph->vVertExistP1[i]) {
			for (j = 0; j < pGraph->vertNum2; j++) {
				if (pGraph->vpVertsP1[i][j]) {
					printf("%d %d\n", i, j);
					printf("%d \n", pGraph->subCount1[i]);					
				}								
			}
		}
	}
	
	/* partite 2 to partite 1 edges. */
	printf("Part 2 to 1:\n");
	for (i = 0; i < pGraph->vertNum2; i++) {
		if (pGraph->vVertExistP2[i]) {
			for (j = 0; j < pGraph->vertNum1; j++) {
				if (pGraph->vpVertsP2[i][j]) {
					printf("%d %d\n", i, j);
				}								
			}
		}
	}
	
	/* TODO: Implement me! */
} /* end of bipartGraphPrint() */

