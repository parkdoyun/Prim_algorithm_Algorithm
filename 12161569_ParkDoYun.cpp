#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define UNSEEN -1 // status
#define FRINGE 0 // status
#define TREE 1 // status
#define ENABLE 2 
#define DISABLE 3

struct node // AdjVertices의 배열 원소에 연결되는 node
{
	int num; // 자신의 조형물 번호
	int num_connect; // 자신과 연결된 조형물 번호
	node *next = NULL; // 다음 node 포인터
	int weight; // 가중치 (디자인 비용)
};

struct vertex // 인접리스트의 AdjVertices
{
	 // int num; // 배열의 index로 대신 사용
	int enable = DISABLE;
	node *n = NULL;
};

struct PQnode // priority queue에 들어가는 node
{
	// int num; // 배열의 index로 대신 사용
	int status = UNSEEN;
	int weight = -1;
	int num_connect = -1; // 현재 연결된 상대편 node의 번호
};

vector<vertex> AdjVertices(10001); // 인접리스트 그래프
vector<PQnode> PQ(10001); // priority queue

void PQ_clear(); // priority queue 초기화 함수
void prim(int start, int n); // prim 알고리즘 함수

// 알고리즘 설계과제 2
// 12161569 컴퓨터공학과 박도윤

int main()
{
	int n, m, q;
	cin >> n >> m >> q;
	
	int num1, num2, design;
	for (int i = 0; i < m; i++) // 인접리스트 그래프 생성
	{
		cin >> num1 >> num2 >> design;

		// node 생성
		node *n1 = new node;
		n1->num = num1;
		n1->num_connect = num2;
		n1->weight = design;

		node *n2 = new node;
		n2->num = num2;
		n2->num_connect = num1;
		n2->weight = design;

		// num1과 num2 있는지 확인
		// 없으면 생성
		node *tmp;
		if (AdjVertices[num1].enable == DISABLE) // 없다면 생성
		{
			AdjVertices[num1].enable = ENABLE;
			AdjVertices[num1].n = n2;
		}
		else // 있다면 추가만
		{
			// 마지막에 추가
			tmp = AdjVertices[num1].n;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = n2;
		}
		if (AdjVertices[num2].enable == DISABLE) // 없다면 생성
		{
			AdjVertices[num2].enable = ENABLE;
			AdjVertices[num2].n = n1;
		}
		else // 있다면 추가만
		{
			// 마지막에 추가
			tmp = AdjVertices[num2].n;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = n1;
		}
	}

	char inst; // 명령어 'P' 입력용
	int start_num; // 출발 조형물 번호

	for (int i = 0; i < q; i++)
	{
		cin >> inst >> start_num;

		// prim 알고리즘 수행
		prim(start_num, n);
		PQ_clear(); // PQ 초기화
	}

	return 0;
}

void PQ_clear() // priority queue 초기화 함수
{
	for (int i = 1; i <= 10000; i++)
	{
		PQ[i].status = UNSEEN;
		PQ[i].weight = -1;
		PQ[i].num_connect = -1;
	}
}

void prim(int start, int n) // prim 알고리즘 함수
{
	string total_str = ""; // 최종 출력할 string
	int tmp; // 지금 가리키는 vertex의 번호
	int total_weight = 0; // 산책로 설치에 들어가는 비용의 총합

	for (int i = 0; i < n - 1; i++)
	{
		if (i == 0) // start vertex
		{
			PQ[start].weight = 0;
			PQ[start].status = TREE;
			tmp = start;
			total_str = to_string(start) + ' '; // 조형물 번호 추가 (나중에 출력)
		} 

		// 1. tmp와 연결된 node들 중 정보 갱신 -> UNSEEN (status, 비용 갱신) / FRINGE (비용 갱신)
		// 2. FRINGE 중 최소 비용 탐색
		// 3. 최소 비용 가진 node TREE로 바꾸고, tmp 갱신
		// 4. 해당 node (tmp) 번호 total_str에 추가 & 디자인 비용 총합 계산
		// 5. 결과 출력

		// 1. tmp와 연결된 node들 중 정보 갱신 -> UNSEEN (status, 비용 갱신) / FRINGE (비용 갱신)
		node *tmp_n = AdjVertices[tmp].n; // 현재 node
		// tmp와 연결된 node 확인 및 정보 갱신
		while (tmp_n->next != NULL)
		{
			if (PQ[tmp_n->num].status == UNSEEN) // UNSEEN일 때 -> status, 비용 갱신
			{
				PQ[tmp_n->num].status = FRINGE; // status
				PQ[tmp_n->num].weight = tmp_n->weight; // 비용
				PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호 갱신
			}
			else if (PQ[tmp_n->num].status == FRINGE) // FRINGE일 때 -> 필요하다면 비용 갱신
			{
				if (PQ[tmp_n->num].weight > tmp_n->weight) // decrease key 수행
				{
					PQ[tmp_n->num].weight = tmp_n->weight; // 비용
					PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호
				}
				/*
				else if (PQ[tmp_n->num].weight == tmp_n->weight) // 같을 경우 상대편 node의 번호 확인
				{
					if (tmp_n->num_connect < PQ[tmp_n->num].num_connect) // decrease key 수행
					{
						PQ[tmp_n->num].weight = tmp_n->weight; // 비용
						PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호
					}
				}
				*/
			}

			tmp_n = tmp_n->next;
		}

		// 마지막 node도 확인
		if (PQ[tmp_n->num].status == UNSEEN) // UNSEEN일 때 -> status, 비용 갱신
		{
			PQ[tmp_n->num].status = FRINGE; // status
			PQ[tmp_n->num].weight = tmp_n->weight; // 비용
			PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호 갱신
		}
		else if (PQ[tmp_n->num].status == FRINGE) // FRINGE일 때 -> 필요하다면 비용 갱신
		{
			if (PQ[tmp_n->num].weight > tmp_n->weight) // decrease key 수행
			{
				PQ[tmp_n->num].weight = tmp_n->weight; // 비용
				PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호
			}
			/*
			else if (PQ[tmp_n->num].weight == tmp_n->weight) // 같을 경우 상대편 node의 번호 확인
			{
				if (tmp_n->num_connect < PQ[tmp_n->num].num_connect) // decrease key 수행
				{
					PQ[tmp_n->num].weight = tmp_n->weight; // 비용
					PQ[tmp_n->num].num_connect = tmp_n->num_connect; // 상대편 node 번호
				}
			}
			*/
		}

		// 2. FRINGE 중 최소 비용 탐색
		// 건너편 정점 번호 더 작은 것 먼저 처리 -> 스캔하면서 현재 값이 작을 때만 갱신하면 됨 (같을 때 갱신 X)
		int min_num; // 최소 비용 갖는 조형물 번호
		int min_weight; // 최소 비용 (가중치)
		int j = 1;
		while (PQ[j].status != FRINGE) j++;
			
		// 번호, 비용 초기화
		min_num = j;
		min_weight = PQ[j].weight;
			
		// 최소 비용 갖는 node 탐색
		for (; j <= 10000; j++)
		{
			if (PQ[j].status == FRINGE && PQ[j].weight < min_weight)
			{
				min_num = j;
				min_weight = PQ[j].weight;
			}
		}

		// 3. 최소 비용 가진 node의 status TREE로 바꾸고, tmp 갱신
		PQ[min_num].status = TREE;
		tmp = min_num;

		// 4. 해당 node (tmp) 번호 total_str에 추가 & 디자인 비용 총합 계산
		total_str += (to_string(tmp) + ' '); // 조형물 번호 추가 (나중에 출력)	
		total_weight += min_weight; // 비용 총합 계산
	}

	// 5. 결과 출력
	cout << total_weight << ' ' << total_str << '\n';

}