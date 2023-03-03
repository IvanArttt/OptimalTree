#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <cstdlib>


using namespace std;


template <typename T>
int Poisk(T* k, int left, int right, T value)
{
	for (int i = left; i <= right; i++)
	{
		if (k[i] == value)
			return i;
	}
	return -1;
}


template <typename T>
void QuickSort(T* k, int left, int right)
{
	if (right <= left) return;
	int i = left, p = right;
	T temp;
	T x = k[(right + left + 1) / 2];
	while (1)
	{
		while (i < p && k[i] <= x)
			++i;
		while (p >= i && k[p] > x)
			--p;
		if (i < p)
		{
			temp = k[i];
			k[i++] = k[p];
			k[p--] = temp;
		}
		else
			break;
	}
	if (x != k[p]) {
		k[Poisk(k, left, right, x)] = k[p];
		k[p] = x;
	}
	if (p > 0)
		QuickSort(k, left, p - 1);
	if (p <= right)
		QuickSort(k, p + 1, right);
}





template<typename T>
class OptimalTree {

	class TreeNode {
	public:
		TreeNode* _left;
		TreeNode* _right;
		TreeNode* _parent;
		T _value;
		int _T;
		int _weight;
		TreeNode(T value = NULL, int T = 0, int w = 0, TreeNode* left = nullptr, TreeNode* right = nullptr, TreeNode* parent = nullptr)
		{
			this->_value = value;
			this->_T = T;
			this->_weight = w;
			this->_left = left;
			this->_right = right;
			this->_parent = parent;
		}
		TreeNode& operator=(const TreeNode& A)
		{
			this->_value = A._value;
			this->_T = A._T;
			this->_weight = A._weight;
			return (*this);
		}
	};

	TreeNode* _root;
	int _size;
	int TreeWeightLength;


	int Cmp(T v1, T v2)
	{
		if (v1 > v2)
			return 1;
		if (v2 > v1)
			return -1;
		if (v1 == v2)
			return 0;
	}

	bool NoEqualTreeNode(TreeNode* a, TreeNode* b)
	{
		if (a->_value > b->_value)
			return true;
		if (a->_value < b->_value)
			return false;
	}

	void Destructor(TreeNode* a)
	{
		if (a)
		{
			Destructor(a->_left);
			Destructor(a->_right);
			delete a;
		}
	}


	void RoundL(TreeNode* node, int& i, T* a)
	{
		if (node)
		{
			RoundL(node->_left, i, a);
			a[i] = node->val;
			++i;
			RoundL(node->_right, i, a);
		}
	}

	void RoundR(TreeNode* node, int& i, T* a)
	{
		if (node)
		{
			RoundR(node->_right, i, a);
			a[i] = node->val;
			++i;
			RoundR(node->_left, i, a);
		}
	}




	void Print(TreeNode* root, int h)
	{
		if (root)
		{
			Print(root->_right, h + 1);
			for (int i = 0; i < h; i++)
			{
				cout << "   ";
			}
			cout << root->_value << endl;
			Print(root->_left, h + 1);
		}
	}




	void OptimalBuild(T* value, TreeNode* node, int** table, int i, int j, int Size)
	{
		if (j <= 0 || i >= j) return;
		int index = table[i][j];

		/*for (size_t i = 0; i < Size; i++)
		{
			for (size_t j = 0; j < Size; j++) {
				std::cout << table[i][j] << "\t\t";
			}
			std::cout << std::endl;
		}*/
		if ((table[i][table[i][j] - 1] >= 0) && (i < j))
		{
			TreeNode* NewNode = new TreeNode(value[table[i][table[i][j] - 1]]);
			node->_left = NewNode;
			NewNode->_parent = node;
			//return;
			OptimalBuild(value, node->_left, table, i, index - 1, Size);
		}
		//if (node->_left)

		if ((table[table[i][j] + 1][j] >= 0) && (i < j))
		{
			TreeNode* _NewNode = new TreeNode(value[table[table[i][j] + 1][j]]);
			node->_right = _NewNode;
			_NewNode->_parent = node;
			//return;
			OptimalBuild(value, node->_right, table, index + 1, j, Size);
		}
	}


	TreeNode* Remove(TreeNode* node, T val) {
		if (node == nullptr) {
			return nullptr;
		}

		if (val < node->val) {
			node->_left = Remove(node->_left, val);
		}
		else if (val > node->val) {
			node->_right = Remove(node->_right, val);
		}
		else {
			// Node with no children or one child
			if (node->_left == nullptr || node->_right == nullptr) {
				TreeNode* temp = node->_left ? node->_left : node->_right;
				delete node;
				return temp;
			}
			else {
				// Node with two children
				TreeNode* temp = FindMin(node->_right);
				node->val = temp->val;
				node->_right = Remove(node->_right, temp->val);
			}
		}
		return node;
	}

public:

	bool Contains(TreeNode* node, int val) const {
		if (node == nullptr) {
			return false;
		}
		if (val == node->val) {
			return true;
		}
		if (val < node->val) {
			return Contains(node->left, val);
		}
		return Contains(node->right, val);
	}

	OptimalTree()
	{
		this->_root = nullptr;
		this->_Size = 0;
	}




	OptimalTree(T* value, int* Succes, int size)
	{
		//size - количество элементов в массиве value
		int** index = new int* [size];
		int** weight = new int* [size];
		int** sum = new int* [size];
		int j, val;
		for (int i = 0; i < size; i++)
		{
			index[i] = new int[size];
			weight[i] = new int[size];
			sum[i] = new int[size];
		}

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j <= i + 1; j++)
			{
				if (j == size) break;
				index[j][i] = INT_MAX;
				weight[j][i] = INT_MAX;
				sum[j][i] = INT_MAX;
			}
		}

		for (int i = 0; i < size; i++)
		{
			index[i][i] = i;
			weight[i][i] = 0;
			sum[i][i] = Succes[i];
		}

		for (int k = 2; k <= size; k++)
		{
			for (int i = 0; i < size - k + 1; i++)
			{
				j = i + k - 1;
				sum[i][j] = 0;
				for (int l = i; l <= j; l++)
					sum[i][j] += Succes[l];

				for (int n = i; n <= j; n++)
				{
					val = ((n > i) ? (weight[i][n - 1] + sum[i][n - 1]) : 0) + ((n < j) ? (weight[n + 1][j] + sum[n + 1][j]) : 0);
					if (val < weight[i][j])
					{
						weight[i][j] = val;
						int s = 0;
						int p = INT_MIN;
						index[i][j] = 0;
						for (int l = i; l <= j; l++)
						{
							s = Succes[l];
							if (s > p)
							{
								p = s;
								index[i][j] = l;
							}
						}
					}
				}
			}
		}


		_root = new TreeNode(value[index[0][size - 1]]);
		OptimalBuild(value, _root, index, 0, size - 1, size - 1);

		for (int i = 0; i < size; i++)
		{
			delete[] index[i];
			delete[] weight[i];
			delete[] sum[i];
		}
		delete[] index;
		delete[] weight;
		delete[] sum;
	}

	int TWL() { return TreeWeightLength; }


	~OptimalTree()
	{
		Destructor(_root);
	}

	bool Empty()
	{
		return (this->_Size == 0);
	}





	bool Search(T val) const {
		TreeNode* current = _root;
		while (current != nullptr) {
			if (val == current->val) {
				return true;
			}
			current = (val < current->val) ? current->_left : current->_right;
		}
		return false;
	}

	int Size()
	{
		return this->_Size;
	}

	T* RoundLeftRootRight()
	{
		T* arr = new T[this->_size];
		int i = 0;
		RoundL(_root, i, arr);
		return arr;
	}

	T* RoundRightRootLeft()
	{
		T* arr = new T[this->_size];
		int i = 0;
		RoundR(_root, i, arr);
		return arr;
	}


	void Remove(T val) {
		if (!Contains(val)) return;
		_root = Remove(_root, val);
		--_size;
	}



	void Output()
	{
		Print(this->_root, 0);
	}

	void Output(const char option)
	{
		T* tmp = new T[_size];
		switch (option)
		{
		case 'L':
			tmp = this->RoundLeftRootRight();
			for (int i = 0; i < _size; i++)
			{
				printf("%d\t", tmp[i]);
			}
			break;
		case 'R':
			tmp = this->RoundRightRootLeft();
			for (int i = 0; i < _size; i++)
			{
				printf("%d\t", tmp[i]);
			}
			break;
		default:
			printf("Wrong input\n");
			break;
		}
		printf("\n");
		delete[] tmp;
	}

};



int main()
{
	int size1 = 4;
	int* arr1 = new int[size1] {1, 3, 5, 8};
	int* p1 = new int[size1] {6, 2, 8, 7};
	OptimalTree<int> A(arr1, p1, size1);
	A.Output();
	cout << endl << "TWL: " << A.TWL() << endl;
	delete[] arr1, p1;
	return 0;
}
