/*
 Порядковые статистики.
 Дано число N и N строк.
 Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
 Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
 Запрос на получение k-ой порядковой статистики задается числом k.
 Вариант 7_3.  Требуемая скорость выполнения запроса - O(log n) в среднем. В реализации используйте декартово дерево.
*/

#include <iostream>
#include <assert.h>

using std::cin;
using std::cout;
using std::endl;

// Functor for types with defined "<" operator
template<class T>
class IsLessDefaultFunctor {
public:
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

// Treap Node Struct
template<class T>
struct Node {
	T Data;
	int Priority;
	size_t Size; // size of subtree
	Node* Left;
	Node* Right;
};

// Function to create a new Treap Node having given key
template<class T>
Node<T>* newNode(const T& key) {
	Node<T>* node = new Node<T>;
	node->Data = key;
	node->Priority = rand() % 100;

	node->Size = 1;
	node->Left = node->Right = nullptr;
	return node;
}

// Function to get size of subtree with a given root node
template<class T>
size_t get_size(const Node<T>* node) {
	return node == nullptr ? 0 : node->Size;
}

// Function to calculate size of tree by sizes of left and right subtrees
template<class T>
void calc_size(Node<T>*& node) {
	if (node) {node->Size = get_size(node->Left) + get_size(node->Right) + 1;}
}

// Treap Class
template<class T, class IsLess = IsLessDefaultFunctor<T> >
class Treap {
public:
	Treap();
	~Treap();

	void insert(const T& key);
	void remove(const T& key);
	void postorder_print() const;
	T KStat(size_t k) const;

private:
	Node<T>* root;

	Node<T>* Merge(Node<T>* left, Node<T>* right);
	void Split(Node<T>* node, T key, Node<T>*& left, Node<T>*& right);
	void insert(Node<T>*& node, Node<T>*& new_node);
	void remove(Node<T>*& node, const T& key);
	void delete_tree(Node<T> *node);
	void delete_subtree(Node<T> *node);
	void postorder_print(Node<T> *node) const;

	IsLess isLess;
};

template<class T, class IsLess>
Treap<T, IsLess>::Treap() {
	root = nullptr;
}

template<class T, class IsLess>
Treap<T, IsLess>::~Treap() {
	delete_tree(root);
}

// Function to Split Treap by key
template<class T, class IsLess>
void Treap<T, IsLess>::Split(Node<T>* node, T key, Node<T>*& left, Node<T>*& right) {
	if (!node) {
		left = right = nullptr;
	} else if (!isLess(key, node->Data)) {
		Split(node->Right, key, node->Right, right);
		left = node;
		calc_size(left);
	} else {
		Split(node->Left, key, left, node->Left);
		right = node;
		calc_size(right);
	}
}

// Function to Merge two Treaps
template<class T, class IsLess>
Node<T>* Treap<T, IsLess>::Merge(Node<T>* left, Node<T>* right) {

	if(!left || !right) { return left == nullptr ? right : left; }

	if(left->Priority > right->Priority) {
		left->Right = Merge(left->Right, right);
		calc_size(left);
		return left;
	} else {
		right->Left = Merge(left, right->Left);
		calc_size(right);
		return right;
	}
}

template<class T, class IsLess>
void Treap<T, IsLess>::delete_tree(Node<T> *node) {
	if (node) {
		delete_tree(node->Left);
		delete_tree(node->Right);
		delete node;
	}
}

// Method to insert a key into Treap
template<class T, class IsLess>
void Treap<T, IsLess>::insert(const T& key) {
	Node<T>* new_node = newNode<T>(key);
	insert(root, new_node);
}

template<class T, class IsLess>
void Treap<T, IsLess>::insert(Node<T>*& node, Node<T>*& new_node) {
	if (!node) {
		node = new_node;
	} else if (new_node->Priority > node->Priority) {
		Split(node, new_node->Data, new_node->Left, new_node->Right);
		node = new_node;
	} else {
		insert(isLess(new_node->Data, node->Data) ? node->Left : node->Right, new_node);
	}
	calc_size(node);
}

// Method to remove an element from Treap
template<class T, class IsLess>
void Treap<T, IsLess>::remove(const T& key) {
	remove(root, key);
}

template<class T, class IsLess>
void Treap<T, IsLess>::remove(Node<T>*& node, const T& key) {
	if (!isLess(node->Data, key) && !isLess(key, node->Data)) {
		node = Merge(node->Left, node->Right);
	} else {
		remove(isLess(key, node->Data) ? node->Left : node->Right, key);
	}
	calc_size(node);
}

template<class T, class IsLess>
void Treap<T, IsLess>::postorder_print() const {
	cout << "tree: " ;
	postorder_print(root);
	cout << endl;
}

template<class T, class IsLess>
void Treap<T, IsLess>::postorder_print(Node<T> *node) const {
	if (node) {
		postorder_print(node->Left);
		postorder_print(node->Right);
		cout << "(" << node->Data << "; " << node->Priority << ")" << " ";
	}
}

// Function to get k-th statistic
template<class T, class IsLess>
T Treap<T, IsLess>::KStat(size_t k) const {
	assert(k <= get_size(root));

	Node<T>* node = root;

    while (node != nullptr) {
        size_t sizeLeft = get_size(node->Left);

        if (sizeLeft == k) { return node->Data; }

        node = sizeLeft > k ? node->Left : node->Right;
        if (sizeLeft < k) { k -= sizeLeft + 1; }
    }
    return 0;
}

int main() {

	srand(time(0));

	size_t n;
	cin >> n;

	Treap<int> *tree = new Treap<int>;
	int value;
	size_t k;

	for (size_t i = 0; i < n; i++) {

		cin >> value >> k;

		if (value >= 0) {
			tree->insert(value);
		} else {
			tree->remove(-value);
		}

		cout << tree->KStat(k) << endl;
	}

	delete tree;

	return 0;
}

