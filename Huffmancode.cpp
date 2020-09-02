#include<stdio.h>
#include<stdlib.h>
#define MaxVertexNum 1000
typedef int Vertex; //�ö����±��ʾ����,Ϊ����
//�����
typedef struct ENode* PtrToEdNode;
struct ENode {
	Vertex V1, V2;//
};
typedef PtrToEdNode Edge;
//�ڽӵ�Ķ���
typedef struct AdjVNode* PtrToAdjVNode;
struct AdjVNode {
	Vertex Adjv;//�ڽӵ��±�
	int flag;//��ѯ�Ƿ��Ѿ������
	PtrToAdjVNode Next;//ָ����һ���ڽӵ���±�
};
//�����ͷ���Ķ���
typedef struct VNode{
	PtrToAdjVNode FirstEdge;
}AdjList[MaxVertexNum];
//ͼ���Ķ���
typedef struct GNode* PtrToGNode;
struct GNode {
	Vertex Nv;//������
	int Ne;//����
	AdjList G;//�ڽӱ�
};
typedef PtrToGNode LGraph;//�ڽӱ���
typedef struct QNode* PtrToQNode;
struct QNode{
	PtrToAdjVNode p;
	PtrToQNode Next;
};
typedef struct QueueNode* Queue;
struct QueueNode {
	PtrToQNode Front, Rear;
	int Maxsize;//����Ԫ�ظ���
};

LGraph BuildGraph(LGraph Graph,Vertex Vertexnum);//����һ��V������Ŀ�ͼ
void Insert(LGraph Graph, Edge E);//���ڽӱ���뵽ͷ�����
void DFS(LGraph Graph,Vertex V);//DFE�������
void BFS(LGraph Graph,Vertex V);//BFS�������
void Free(LGraph G);//��������ڴ��ͷ�
Queue CreateQueue(Vertex Num);//����һ���������ΪNum�Ķ���
void Push(Queue Q, PtrToAdjVNode p);
PtrToAdjVNode Pop(Queue Q);
int IsEmpty(Queue Q);
int main()
{
	Vertex V;
	LGraph Graph;
	int  W;
	Edge E;

	scanf_s("%d", &V);//������
	Graph = (LGraph)malloc(sizeof(struct GNode));
	if (!Graph) {
		//���
		printf("ERROR");
		return -1;
	}
	Graph = BuildGraph(Graph,V);
	//����ֻ��V�������ͼ
	scanf_s("%d", &(Graph->Ne));//�������
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
		/* ��ʼ���ڽӱ�ͷָ�� */
	/* ע�⣺����Ĭ�϶����Ŵ�0��ʼ����(Graph->Nv - 1) */
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



