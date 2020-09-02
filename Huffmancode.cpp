#include<stdio.h>
#include<stdlib.h>
#define MaxVertexNum 1000
typedef int Vertex; //用顶点下标表示顶点,为整型
//定义边
typedef struct ENode* PtrToEdNode;
struct ENode {
	Vertex V1, V2;//
};
typedef PtrToEdNode Edge;
//邻接点的定义
typedef struct AdjVNode* PtrToAdjVNode;
struct AdjVNode {
	Vertex Adjv;//邻接点下标
	int flag;//查询是否已经被输出
	PtrToAdjVNode Next;//指向下一个邻接点的下标
};
//顶点表头结点的定义
typedef struct VNode{
	PtrToAdjVNode FirstEdge;
}AdjList[MaxVertexNum];
//图结点的定义
typedef struct GNode* PtrToGNode;
struct GNode {
	Vertex Nv;//顶点数
	int Ne;//边数
	AdjList G;//邻接表
};
typedef PtrToGNode LGraph;//邻接表定义
typedef struct QNode* PtrToQNode;
struct QNode{
	PtrToAdjVNode p;
	PtrToQNode Next;
};
typedef struct QueueNode* Queue;
struct QueueNode {
	PtrToQNode Front, Rear;
	int Maxsize;//最大的元素个数
};

LGraph BuildGraph(LGraph Graph,Vertex Vertexnum);//建立一个V个顶点的空图
void Insert(LGraph Graph, Edge E);//将邻接表插入到头结点上
void DFS(LGraph Graph,Vertex V);//DFE输出函数
void BFS(LGraph Graph,Vertex V);//BFS输出函数
void Free(LGraph G);//将申请的内存释放
Queue CreateQueue(Vertex Num);//建立一个最大容量为Num的队列
void Push(Queue Q, PtrToAdjVNode p);
PtrToAdjVNode Pop(Queue Q);
int IsEmpty(Queue Q);
int main()
{
	Vertex V;
	LGraph Graph;
	int  W;
	Edge E;

	scanf_s("%d", &V);//顶点数
	Graph = (LGraph)malloc(sizeof(struct GNode));
	if (!Graph) {
		//验空
		printf("ERROR");
		return -1;
	}
	Graph = BuildGraph(Graph,V);
	//建立只有V个顶点的图
	scanf_s("%d", &(Graph->Ne));//读入边数
	if (Graph->Ne == 0) {
		free(Graph);
		return -1;
	}
	for (W = 0; W < Graph->Ne; W++) {
		E = (Edge)malloc(sizeof(struct ENode));
		if (!E) {
			printf("ERROR");
			return -1;
		}
		scanf_s("%d %d", &(E->V1), &(E->V2));
		Insert(Graph, E);
		free(E);
	}
	for (V = 0; V < Graph->Nv; V++) {
		DFS(Graph,V);
	}
	for (V = 0; V < Graph->Nv; V++) {
		BFS(Graph, V);
	}
	Free(Graph);
	free(Graph);

	return 0;
}

LGraph BuildGraph(LGraph Graph, Vertex Vertexnum)
{
	Vertex v;
	Graph->Nv = Vertexnum;
	for (v = 0; v < Vertexnum; v++) {
		/* 初始化邻接表头指针 */
	/* 注意：这里默认顶点编号从0开始，到(Graph->Nv - 1) */
		Graph->G[v].FirstEdge = NULL;
	}
	return Graph;
}

void Insert(LGraph Graph, Edge E)
{
	PtrToAdjVNode NewNode;
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	if (!NewNode) {
		printf("ERROR");
		return ;
	}
	NewNode->Adjv = E->V2;
	NewNode->flag = 1;
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;

	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	if (!NewNode) {
		printf("ERROR");
		return;
	}
	NewNode->Adjv = E->V1;
	NewNode->Next = Graph->G[E->V2].FirstEdge;
	Graph->G[E->V2].FirstEdge = NewNode;
}

void DFS(LGraph Graph, Vertex V)
{
	PtrToAdjVNode W;
	if (Graph->G[V].FirstEdge->flag) return;
	printf("{");
	for (W = Graph->G[V].FirstEdge; W; W = W->Next) {
		printf("%d ", W->Adjv);
		W->flag = 0;
		DFS(Graph, W->Adjv);
	}
	printf("}\n");
}

void BFS(LGraph Graph,Vertex V)
{
	Queue Q;
	PtrToAdjVNode  W;

	Q = CreateQueue(Graph->Ne);
	Push(Q, Graph->G[V].FirstEdge);
	while (!IsEmpty(Q)) {
		W = Pop(Q);
		if (W->flag == 1) break;
		printf("{ ");
		for (W; W->Next; W = W->Next) {
			printf("%d ", W->Adjv);
			Push(Q, W->Next);
		}
	}
}

void Free(LGraph Graph)
{
	Vertex V;
	PtrToAdjVNode p, W;
	p = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	if (!p) {
		printf("ERROR");
		return;
	}
	p = NULL;
	for (V = 0; V < Graph->Nv; V++) {
		for (p = Graph->G[V].FirstEdge; p->Next; p = p->Next) {
			W = p;
			free(W);
		}
	}
}

Queue CreateQueue(Vertex Num)
{
	Queue Q;
	Q = (Queue)malloc(sizeof(struct QueueNode));
	if (!Q) {
		printf("ERROR");
		return NULL;
	}
	Q->Maxsize = Num;
	return Queue();
}

void Push(Queue Q, PtrToAdjVNode p)
{
	if (p == NULL) return;
	PtrToQNode pos = NULL;
	pos = (PtrToQNode)malloc(sizeof(struct QNode));
	if (!pos) {
		printf("ERROR");
		return ;
	}
	if (!Q->Front && !Q->Rear) {
		pos->p = p;
		pos->Next = NULL;
		Q->Front = Q->Rear = pos;
	}
	else {
		pos->p = p;
		pos->Next = NULL;
		Q->Rear->Next = pos;
		Q->Rear = pos;
	}

}

PtrToAdjVNode Pop(Queue Q)
{
	PtrToQNode FrontCell = NULL;
	PtrToAdjVNode FrontElem;

	FrontCell = Q->Front;
	if (Q->Front == Q->Rear) {
		Q->Front = Q->Rear = NULL;
	}
	else {
		Q->Front = Q->Front->Next;
	}
	FrontElem = FrontCell->p;

	free(FrontCell);
	return FrontElem;
}

int IsEmpty(Queue Q)
{
	if (Q->Front == NULL && Q->Rear == NULL)
		return 0;
	else return 1;
}



