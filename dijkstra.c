#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArrayGraph                               // 인접 행렬을 이용한 그래프를 위한 구조체
{
    int node_count;                                     // 노드의 개수를 나타내는 변수
    int** edge;                                         // 노드간의 간선을 나타내기 위한 이차원 배열
    int graph_type;                                     // 방향 그래프인지, 무방향 그래프인지 나타내는 변수, 0이라면 방향 그래프, 1이라면 무방향 그래프를 나타냄.
} array_graph;

array_graph* create_arraygraph(int n,int type)                   // n(노드의 개수)를 입력받아 노드 n개를 가진 그래프를 만드는 함수.
{
    int i=0;
	int j=0;
    array_graph* r_graph=NULL;                          // 반환을 위한 그래프 포인터

    r_graph=(array_graph*)malloc(sizeof(array_graph));      // 메모리 할당
    
    r_graph->node_count=n;                                  // node_count 변수에 값 할당
    r_graph->graph_type=type;                               // graph_type 변수에 맞는 type 할당
    r_graph->edge=(int **)malloc(sizeof(int *)*n);          // 인접 행렬의 포인터 할당

    for(i=0;i<n;i++)                                        // 인접행렬의 각 행만큼 반복
    {
        r_graph->edge[i]=(int *)malloc(sizeof(int)*n);      // 각 행을 n개 만큼의 열을 만듦.
        for(j=0;j<n;j++)                                    // 각 열에 inf(999)의  값을 할당.
		{
			r_graph->edge[i][j]=999;
		}
    }

    return r_graph;                                         //그래프 포인터 반환.
}

int check_edge(array_graph* gr,int node)                // node로 들어온 수가 조건에 맞는지 확인하는 함수. 
{
    if(gr!=NULL && node<gr->node_count && node>=0)      // 그래프가 존재하는지, node로 들어온 수가 그래프의 노드 개수를 넘는 수 인지, 혹은 음수인지 확인.
    {
        return 1;                                       // 조건에 맞는 수라면 1(true)를 반환.
    } else{
        return 0;                                       // 조건에 맞지 않는 수라면 0(false)을 반환.
    }
}

void add_edge(array_graph* gr,int start_node, int end_node,int weight)   //
{
    if(gr!=NULL                                 // 그래프가 존재하고
       &&check_edge(gr,start_node)              // start_node가 조건에 맞고
       &&check_edge(gr,end_node))               // end_node 또한 조건에 맞다면
    {
        gr->edge[start_node][end_node]=weight;  // start_node가 시작이고 end_node가 종점인 간선 추가.
    } else
    {
        printf("Edge add fail");                // 그래프가 존재하지 않거나 매개변수로 들어온 노드의 수가 조건에 맞지 않는다면 error 메세지 출력.
    }

    if(gr->graph_type==1)                           // 만약 graph_type이 1(무방향 그래프)라면 
    {
        gr->edge[end_node][start_node]=weight;  // end_node가 시작이고 start_node가 종점인 간선 추가.
    }
}

void print_graph(array_graph* gr)               // 그래프를 행렬로 출력하는 함수
{
	int num=gr->node_count;                     // num에 그래프의 노드 수 할당.
	int value;                  

	for(int i=0;i<num;i++)                      // 행을 출력하는 것으로, 노드 수만큼 반복.                
    {
        for(int j=0;j<num;j++)                  // 한 행의 열들을 출력하는 것으로, 노드 수만큼 반복.
        {
			value=gr->edge[i][j];               // value에 노드의 간선 가중치 할당. 간선이 있다면 가중치가, 없다면 999가 할당됨.
			if(value==999){                     // value의 값이 999면 간선이 없는 것이므로
				printf("X    ");                // 간선이 연결 안 됨을 표시
			}else{                              // 999가 아니라면 
				printf("%d    ",value);         // 간선의 가중치를 출력.
			}
            if(j==num-1)                        // 행의 마지막 열이면 줄바꿈.
            {
                printf("\n");
            }
        }
    }
	printf("\n");
}

void print_edge(array_graph* gr)            // 그래프의 간선을 출력하는 함수.
{
	int num=gr->node_count;                 // num에 그래프의 노드 수 할당. 
	int value;                                  

	for(int i=0;i<num;i++)                   // 행을 검사하는 것으로, 노드 수만큼 반복.
    {
        for(int j=0;j<num;j++)              // 한 행의 열들을 검사하는 것으로, 노드 수만큼 반복.
        {
			value=gr->edge[i][j];           // value에 노드의 간선 가중치 할당. 간선이 있다면 가중치가, 없다면 999가 할당됨.
			if(value!=999 && gr->graph_type==0){       // value가 999가 아니면 간선이 있는 것이고 그래프가 방향 그래프라면 
				printf("<%d, %d>\n",i+1,j+1);          // 간선의 시작노드(행), 종점 노드(열)를 방향그래프 방식(<>)으로 출력.
			} else if(value!=999 && gr->graph_type==1 && (i<=j) ){      //value가 999가 아니면 간선이 있는 것이고 그래프가 방향 그래프이며 시작 노드의 값이 
                printf("(%d, %d)\n",i+1,j+1);                           //종점 노드의 값보다 작거나 같다면 간선을 무방향 그래프 방식( '(' ')' )으로 출력.                        
            }
        }
    }
	printf("\n");
}

void dijkstra(int n, array_graph* graph, array_graph* result_graph)  // 다익스트라 알고리즘. 1~n번까지의 노드가 있으나 프로그램 내에선 0~n-1로 표현.
{
    int i;
    int min;
    int vnear;
    int touch[n];

    int length[n];
	int inf=999;                            // inf를 999로 표현. 

    for(i=1;i<n;i++)                        // 2번 노드부터 n번 노드까지 반복.
    {
        touch[i]=0;                 		// 마지막으로 연결된 노드를 1번 노드로 초기화. 
        length[i]=graph->edge[0][i];        // 1번 노드부터의 노드 거리를 초기화.
    }

    for(int j=0;j<n-1;j++)                  // n-1개의 노드를 선택하기 위한 n-1번의 반복.
    {
        min=inf;                            // min 초기화

        for(i=1;i<n;i++)                            // 2번 노드부터 n-1번 노드까지 가장 가까운 거리 탐색
        {
            if(0<=length[i]&&length[i]<min)         // i번째 노드까지의 거리가 0보다 크고 min보다 작다면
            {
                min=length[i];                      // 가장 작은 값을 i번째까지의 거리로 바꾸고
                vnear=i;                            // 가장 가까운 노드도 i로 바꿈.
            }
        }

		add_edge(result_graph,touch[vnear],vnear,graph->edge[touch[vnear]][vnear]);     // 가장 가까운 노드의 마지막 노드 연결노드와 가장 가까운 노드사이의 간선 추가. 

        for(i=1;i<n;i++)                        // 2번 노드부터 n번 노드의 거리와 가까운 거리일때의 마지막 노드를 변경 
        {
            if(length[vnear] + graph->edge[vnear][i]<length[i])         // vnear까지 거리와 vnear에서 i번째 노드까지의 거리의 합이 원래 i번째 노드까지의 거리보다 작다면
            {
                length[i]=length[vnear]+graph->edge[vnear][i];          // i번째 노드까지의 거리를 vnear까지 거리와 vnear에서 i번째 노드까지의 거리의 합으로 변경
                touch[i]=vnear;                                         // 마지막으로 연결된 노드를 vnear로 변경.
            }
        }
        length[vnear]=-1;                       // vnear까지의 거리가 가장 가까운 거리로 선택당하지 않기 위해서(이미 선택해서 사용했기 때문에) 
                                                // 가장 가까운 거리 탐색에 맞지 않는 조건으로 변경.  
    }
}


int main()  // test-case에 대해 실행
{
	int value;
    array_graph* test_graph1=create_arraygraph(5,0);
	array_graph* result_graph=create_arraygraph(5,0);

    add_edge(test_graph1,0,1,7);
    add_edge(test_graph1,0,2,4);
    add_edge(test_graph1,0,3,6);
    add_edge(test_graph1,0,4,1);
    add_edge(test_graph1,2,1,2);
    add_edge(test_graph1,2,3,5);
    add_edge(test_graph1,3,1,3);
    add_edge(test_graph1,4,3,1);

	dijkstra(5,test_graph1,result_graph);

    print_graph(result_graph);
    print_edge(result_graph);
    free(test_graph1);
    free(result_graph);

    array_graph* test_graph2=create_arraygraph(6,1);
    array_graph* result_graph2=create_arraygraph(6,1);

    add_edge(test_graph2,0,1,5);
    add_edge(test_graph2,0,3,2);
    add_edge(test_graph2,0,4,4);
    add_edge(test_graph2,1,3,3);
    add_edge(test_graph2,3,4,1);
    add_edge(test_graph2,1,2,3);
    add_edge(test_graph2,2,3,3);
    add_edge(test_graph2,2,4,4);
    add_edge(test_graph2,2,5,2);
    add_edge(test_graph2,4,5,2);

    dijkstra(6,test_graph2,result_graph2);

    print_graph(result_graph2);
    print_edge(result_graph2);

    free(test_graph2);
    free(result_graph2);

    array_graph* test_graph3=create_arraygraph(5,1);
    array_graph* result_graph3=create_arraygraph(5,1);

    add_edge(test_graph3,0,1,1);
    add_edge(test_graph3,0,2,3);
    add_edge(test_graph3,1,2,3);
    add_edge(test_graph3,1,3,6);
    add_edge(test_graph3,2,3,4);
    add_edge(test_graph3,2,4,2);
    add_edge(test_graph3,3,4,5);

    dijkstra(5,test_graph3,result_graph3);

    print_graph(result_graph2);
    print_edge(result_graph3);

}