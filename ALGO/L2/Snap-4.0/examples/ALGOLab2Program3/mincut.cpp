#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_OF_TRIALS 1
#define NO_OF_VERTICES 5242

int latestedgeindex = 1;
int besttrail = 0;
int noofcuts = 32000;
const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Edge list format");
struct edgelistformat
{
	int vertex1;
	int vertex2;
	int edgeid;
	struct edgelistformat *nextedge;
};

struct edgelistformat *head = NULL;

struct edgelistformat* createnewnode(int vertex1,int vertex2)
{
	struct edgelistformat *newnode = (struct edgelistformat *)malloc(sizeof(struct edgelistformat));
	newnode->vertex1 = vertex1;
	newnode->vertex2 = vertex2;
	newnode->edgeid = latestedgeindex;
	newnode->nextedge = NULL;
	latestedgeindex++;
	return newnode;
}

void insertedgeintostruct(int vertex1,int vertex2)
{

	struct edgelistformat *newnode = NULL;
	newnode = createnewnode(vertex1,vertex2);
	if(head == NULL)
		head = newnode;
	else
	{
		struct edgelistformat *temp = head;
		while(temp->nextedge)
		{
			temp=temp->nextedge;
		}
		temp->nextedge = newnode;
	}


}

int getnoofedges()
{
	int noofedges = 0;
	struct edgelistformat *temp = head;
	while(temp)
	{
		noofedges++;
		temp=temp->nextedge;
	}
	return noofedges;
}


void printedges()
{
	struct edgelistformat *temp = head;
	int count = 1;
	while(temp)
	{
		printf("(%d,%d)\t--->edge %d\n",temp->vertex1,temp->vertex2,count);
		temp=temp->nextedge;
		count++;
	}
}



int getnoofvertices()
{
	struct edgelistformat *temp = head;
	int *verticesarray = (int *)malloc((NO_OF_VERTICES+1)*sizeof(int));
	int noofvertices = 0;

	while(temp!=NULL)
	{
		int isvertex1presentinarray = 0,isvertex2presentinarray = 0;
		for(int i=0;i<noofvertices;i++)
		{
			if(temp->vertex1 == verticesarray[i])
			{
				isvertex1presentinarray = 1;
			}

		}
		if(!isvertex1presentinarray)
		{
			verticesarray[noofvertices] = temp->vertex1;
			noofvertices++;
		}
		for(int i=0;i<noofvertices;i++)
		{
			if(temp->vertex2 == verticesarray[i])
			{
				isvertex2presentinarray = 1;
			}

		}
		if(!isvertex2presentinarray)
		{
			verticesarray[noofvertices] = temp->vertex2;
			noofvertices++;
		}
		temp=temp->nextedge;
	}

	return noofvertices;
}



struct edgelistformat* getaddressofrandomedge(int num)
{
	struct edgelistformat* temp = head;
	int count = 1;
	while(temp)
	{
		if(num == count)
			return temp;

		temp = temp->nextedge;
		count++;
	}
	return NULL;
}

struct edgelistformat* selectrandomedge()
{
	struct edgelistformat* edgepointer = NULL;
	int randomedgeid = rand() % getnoofedges()+1;
	printf("Randmonly selected edge no. %d\n",randomedgeid);
	edgepointer = getaddressofrandomedge(randomedgeid);
	return edgepointer;

}

void remove(struct edgelistformat* edgepointer)
{
	struct edgelistformat *temp = NULL;
	if(edgepointer == head)
	{
		temp = head;
		head = head->nextedge;
		free(temp);
	}
	else
	{
		struct edgelistformat *temp = head;
		while(temp)
		{
			if(temp->nextedge == edgepointer)
			{
				temp->nextedge = edgepointer->nextedge;
				free(edgepointer);
			}
			temp= temp->nextedge;
		}
	}

}

void removeloops(struct edgelistformat *edgepointer)
{
	struct edgelistformat *temp = head;
	struct edgelistformat *edgetoremove = NULL;
	while(temp)
	{
		if(temp->vertex1 == edgepointer->vertex1 && temp->vertex2 == edgepointer->vertex2 && temp->edgeid != edgepointer->edgeid)
		{
			edgetoremove = temp;
			temp = temp->nextedge;
			remove(edgetoremove);
		}
		else
			temp = temp->nextedge;

	}
}

void mergeandremove(struct edgelistformat *edgepointer)
{
	struct edgelistformat *temp = head;
	latestedgeindex++;
	while(temp)
	{
		if(temp->vertex1 == edgepointer->vertex1 && temp->edgeid != edgepointer->edgeid)
			temp->vertex1 = latestedgeindex;
		if(temp->vertex2 == edgepointer->vertex1 && temp->edgeid != edgepointer->edgeid)
			temp->vertex2 = latestedgeindex;
		if(temp->vertex1 == edgepointer->vertex2 && temp->edgeid != edgepointer->edgeid)
			temp->vertex1 = latestedgeindex;
		if(temp->vertex2 == edgepointer->vertex2 && temp->edgeid != edgepointer->edgeid)
			temp->vertex2 = latestedgeindex;

		temp = temp->nextedge;
	}
	removeloops(edgepointer);
	remove(edgepointer);
}



void removeallselfloopsingraph()
{
	struct edgelistformat *temp = head;
	struct edgelistformat *edgetoremove = NULL;
	while(temp)
	{
		if(temp->vertex1 == temp->vertex2)
		{
			edgetoremove = temp;
			temp = temp->nextedge;
			remove(edgetoremove);
		}
		else
			temp = temp->nextedge;
	}
}

int dowehavemorethantwovertices()
{
	struct edgelistformat *temp = head;
	int a = temp->vertex1;
	int b = temp->vertex2;
	while(temp->nextedge)
	{
		temp=temp->nextedge;
		int c=temp->vertex1;
		int d=temp->vertex2;
		if( !((a == c && b==d) || (a==d && b==c)))
		{
			return 1;
		}

	}
	return 0;

}

void startalgo()
{
	struct edgelistformat* edgepointer;
	while(dowehavemorethantwovertices())
	{
		printf("---------------------------------------------\n");
		//printedges();
		//printf("no of vertices %d\n",getnoofvertices());
		//printf("no of edges %d\n",getnoofedges());
		edgepointer = selectrandomedge();
		if(edgepointer != NULL)
		{
			mergeandremove(edgepointer);
			printf("Removing edge,merging both nodes and redirecting all other edges to new merged node \n");
		}
		else
			printf("NULL came\n");

	}
	printf("---------------------------------------------\n");
	printedges();
	//printf("no of vertices %d\n",getnoofvertices());
	printf("no of edges %d\n",getnoofedges());
	printf("Removing self loops if any\n");
	removeallselfloopsingraph();
	printf("---------------------------------------------\n");
	printedges();
	//printf("no of vertices %d\n",getnoofvertices());
	printf("no of edges %d\n",getnoofedges());
	printf("---------------------------------------------\n");


}

int main()
{
	typedef PNEGraph PGraph;  //   directed multigraph
	int count = 1;
	latestedgeindex = getnoofvertices();
	while(count <= NO_OF_TRIALS)
	{
			printf("+++++++++++++++++++++++++++ Trial No : %d +++++++++++++++++++++++++\n",count);
			printf("Creating graph:\n");
			head = NULL;
			PGraph G = PGraph::TObj::New();
			for (int n = 0; n < NO_OF_VERTICES; n++) {
			    G->AddNode(); // if no parameter is given, node ids are 0,1,...,9
			  }
			printf("Vertices Added\n");
			G = TSnap::LoadEdgeList<PNEGraph>(InFNm);
			for (typename PGraph::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++)
			{
				insertedgeintostruct(ei.GetSrcNId(), ei.GetDstNId());
			}
			latestedgeindex++;
			startalgo();
			if(noofcuts > getnoofedges())
			{
				noofcuts = getnoofedges();
				besttrail = count;
			}
			printf("Min cut is %d\n",getnoofedges());
			if(getnoofedges() == 0)
				printf("So Graph is already Disconnected by default\n");
			count++;
	}

	printf("+++++++++++++++++++++++++++ End Of Trials +++++++++++++++++++++++++++++++++\n");
	printf("Best Min cut is %d and best trail is %d\n",noofcuts,besttrail);
	if(getnoofedges() == 0)
		printf("So Graph is already Disconnected by default\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

}




















