#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define UNSEEN -1 // status
#define FRINGE 0 // status
#define TREE 1 // status
#define ENABLE 2 
#define DISABLE 3

struct node // AdjVertices�� �迭 ���ҿ� ����Ǵ� node
{
	int num; // �ڽ��� ������ ��ȣ
	int num_connect; // �ڽŰ� ����� ������ ��ȣ
	node *next = NULL; // ���� node ������
	int weight; // ����ġ (������ ���)
};

struct vertex // ��������Ʈ�� AdjVertices
{
	 // int num; // �迭�� index�� ��� ���
	int enable = DISABLE;
	node *n = NULL;
};

struct PQnode // priority queue�� ���� node
{
	// int num; // �迭�� index�� ��� ���
	int status = UNSEEN;
	int weight = -1;
	int num_connect = -1; // ���� ����� ����� node�� ��ȣ
};

vector<vertex> AdjVertices(10001); // ��������Ʈ �׷���
vector<PQnode> PQ(10001); // priority queue

void PQ_clear(); // priority queue �ʱ�ȭ �Լ�
void prim(int start, int n); // prim �˰��� �Լ�

// �˰��� ������� 2
// 12161569 ��ǻ�Ͱ��а� �ڵ���

int main()
{
	int n, m, q;
	cin >> n >> m >> q;
	
	int num1, num2, design;
	for (int i = 0; i < m; i++) // ��������Ʈ �׷��� ����
	{
		cin >> num1 >> num2 >> design;

		// node ����
		node *n1 = new node;
		n1->num = num1;
		n1->num_connect = num2;
		n1->weight = design;

		node *n2 = new node;
		n2->num = num2;
		n2->num_connect = num1;
		n2->weight = design;

		// num1�� num2 �ִ��� Ȯ��
		// ������ ����
		node *tmp;
		if (AdjVertices[num1].enable == DISABLE) // ���ٸ� ����
		{
			AdjVertices[num1].enable = ENABLE;
			AdjVertices[num1].n = n2;
		}
		else // �ִٸ� �߰���
		{
			// �������� �߰�
			tmp = AdjVertices[num1].n;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = n2;
		}
		if (AdjVertices[num2].enable == DISABLE) // ���ٸ� ����
		{
			AdjVertices[num2].enable = ENABLE;
			AdjVertices[num2].n = n1;
		}
		else // �ִٸ� �߰���
		{
			// �������� �߰�
			tmp = AdjVertices[num2].n;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = n1;
		}
	}

	char inst; // ��ɾ� 'P' �Է¿�
	int start_num; // ��� ������ ��ȣ

	for (int i = 0; i < q; i++)
	{
		cin >> inst >> start_num;

		// prim �˰��� ����
		prim(start_num, n);
		PQ_clear(); // PQ �ʱ�ȭ
	}

	return 0;
}

void PQ_clear() // priority queue �ʱ�ȭ �Լ�
{
	for (int i = 1; i <= 10000; i++)
	{
		PQ[i].status = UNSEEN;
		PQ[i].weight = -1;
		PQ[i].num_connect = -1;
	}
}

void prim(int start, int n) // prim �˰��� �Լ�
{
	string total_str = ""; // ���� ����� string
	int tmp; // ���� ����Ű�� vertex�� ��ȣ
	int total_weight = 0; // ��å�� ��ġ�� ���� ����� ����

	for (int i = 0; i < n - 1; i++)
	{
		if (i == 0) // start vertex
		{
			PQ[start].weight = 0;
			PQ[start].status = TREE;
			tmp = start;
			total_str = to_string(start) + ' '; // ������ ��ȣ �߰� (���߿� ���)
		} 

		// 1. tmp�� ����� node�� �� ���� ���� -> UNSEEN (status, ��� ����) / FRINGE (��� ����)
		// 2. FRINGE �� �ּ� ��� Ž��
		// 3. �ּ� ��� ���� node TREE�� �ٲٰ�, tmp ����
		// 4. �ش� node (tmp) ��ȣ total_str�� �߰� & ������ ��� ���� ���
		// 5. ��� ���

		// 1. tmp�� ����� node�� �� ���� ���� -> UNSEEN (status, ��� ����) / FRINGE (��� ����)
		node *tmp_n = AdjVertices[tmp].n; // ���� node
		// tmp�� ����� node Ȯ�� �� ���� ����
		while (tmp_n->next != NULL)
		{
			if (PQ[tmp_n->num].status == UNSEEN) // UNSEEN�� �� -> status, ��� ����
			{
				PQ[tmp_n->num].status = FRINGE; // status
				PQ[tmp_n->num].weight = tmp_n->weight; // ���
				PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ ����
			}
			else if (PQ[tmp_n->num].status == FRINGE) // FRINGE�� �� -> �ʿ��ϴٸ� ��� ����
			{
				if (PQ[tmp_n->num].weight > tmp_n->weight) // decrease key ����
				{
					PQ[tmp_n->num].weight = tmp_n->weight; // ���
					PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ
				}
				/*
				else if (PQ[tmp_n->num].weight == tmp_n->weight) // ���� ��� ����� node�� ��ȣ Ȯ��
				{
					if (tmp_n->num_connect < PQ[tmp_n->num].num_connect) // decrease key ����
					{
						PQ[tmp_n->num].weight = tmp_n->weight; // ���
						PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ
					}
				}
				*/
			}

			tmp_n = tmp_n->next;
		}

		// ������ node�� Ȯ��
		if (PQ[tmp_n->num].status == UNSEEN) // UNSEEN�� �� -> status, ��� ����
		{
			PQ[tmp_n->num].status = FRINGE; // status
			PQ[tmp_n->num].weight = tmp_n->weight; // ���
			PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ ����
		}
		else if (PQ[tmp_n->num].status == FRINGE) // FRINGE�� �� -> �ʿ��ϴٸ� ��� ����
		{
			if (PQ[tmp_n->num].weight > tmp_n->weight) // decrease key ����
			{
				PQ[tmp_n->num].weight = tmp_n->weight; // ���
				PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ
			}
			/*
			else if (PQ[tmp_n->num].weight == tmp_n->weight) // ���� ��� ����� node�� ��ȣ Ȯ��
			{
				if (tmp_n->num_connect < PQ[tmp_n->num].num_connect) // decrease key ����
				{
					PQ[tmp_n->num].weight = tmp_n->weight; // ���
					PQ[tmp_n->num].num_connect = tmp_n->num_connect; // ����� node ��ȣ
				}
			}
			*/
		}

		// 2. FRINGE �� �ּ� ��� Ž��
		// �ǳ��� ���� ��ȣ �� ���� �� ���� ó�� -> ��ĵ�ϸ鼭 ���� ���� ���� ���� �����ϸ� �� (���� �� ���� X)
		int min_num; // �ּ� ��� ���� ������ ��ȣ
		int min_weight; // �ּ� ��� (����ġ)
		int j = 1;
		while (PQ[j].status != FRINGE) j++;
			
		// ��ȣ, ��� �ʱ�ȭ
		min_num = j;
		min_weight = PQ[j].weight;
			
		// �ּ� ��� ���� node Ž��
		for (; j <= 10000; j++)
		{
			if (PQ[j].status == FRINGE && PQ[j].weight < min_weight)
			{
				min_num = j;
				min_weight = PQ[j].weight;
			}
		}

		// 3. �ּ� ��� ���� node�� status TREE�� �ٲٰ�, tmp ����
		PQ[min_num].status = TREE;
		tmp = min_num;

		// 4. �ش� node (tmp) ��ȣ total_str�� �߰� & ������ ��� ���� ���
		total_str += (to_string(tmp) + ' '); // ������ ��ȣ �߰� (���߿� ���)	
		total_weight += min_weight; // ��� ���� ���
	}

	// 5. ��� ���
	cout << total_weight << ' ' << total_str << '\n';

}