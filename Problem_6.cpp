/*
Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Рекурсия запрещена.

 Выведите элементы в порядке post-order (снизу вверх).
 */

#include <iostream>
#include <assert.h>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

template<class T>
class Stack {
public:
	Stack();
	~Stack();

	bool is_empty();  // check if stack is empty
	void push_back(const T& elem);
	T pop_back();
	void print_data();

private:
	int len;   // length of buffer
	T* data; // numbers contained in buffer
	int tail;  // index of last elem
};

template<class T>
Stack<T>::Stack() {
	// create zero buffer of min len 16
	len = 16;
	data = new T[len];
	tail = -1;
}

template<class T>
Stack<T>::~Stack() {
	delete [] data;
}

template<class T>
bool Stack<T>::is_empty() {
	return tail==-1;
}

template<class T>
void Stack<T>::push_back(const T& elem) {
	// make buffer twice bigger if full
	if (tail == len-1) {
		len *= 2;
		T* tempData = new T[len];
		memcpy(tempData, data, len * sizeof(T) / 2);
		delete [] data;

		data = tempData;
	}
	tail += 1;
	data[tail] = elem;
}

template<class T>
T Stack<T>::pop_back() {
	assert( !is_empty() );

	T tempVal = data[tail];
	tail -= 1;
	// make buffer twice smaller
	// if it takes only quarter of current size
	if ((tail < len/4) && (tail > 15)) {
		len /= 2;
		T* tempData = new T[len];
		memcpy(tempData, data, len * sizeof(T) / 2);
		delete [] data;

		data = tempData;
	}
	return tempVal;
}

// for debugging
template<class T>
void Stack<T>::print_data() {
	std::cout<< "tail=" << tail << endl;
	for (int i = 0; i < tail+1; i++) cout << data[i] << endl;
}

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

// Functor for types with defined "<" operator
template<class T>
class IsLessDefaultFunctor {
public:
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Node {
	T Data;
	Node* Left;
	Node* Right;
};

// Function to create a new binary tree node having a given key
template<class T>
Node<T>* newNode(const T& key) {
	Node<T>* node = new Node<T>;
	node->Data = key;
	node->Left = node->Right = nullptr;

	return node;
}

template<class T, class IsLess = IsLessDefaultFunctor<T> >
class Tree {
public:
	Tree();
	~Tree();

	void insert(const T& key);
	void postorder_traverse(void (*visit)(const T& key)) const; // traverse to view tree data

private:
	Node<T> *root;
	void delete_tree();
	void postorder_traverse_change(void (*visit)(Node<T>*)); // traverse with ability to change tree

	IsLess isLess;
};

template<class T, class IsLess>
Tree<T, IsLess>::Tree() {
	root = nullptr;
}

template<class T, class IsLess>
Tree<T, IsLess>::~Tree() {
	delete_tree();
}

template<class T, class IsLess>
void Tree<T, IsLess>::delete_tree() {
	postorder_traverse_change([](Node<T>* node) {delete node;});
}

// Function to insert an key into BST
template<class T, class IsLess>
void Tree<T, IsLess>::insert(const T& key)
{
	Node<T> *cur_node = root;
	Node<T> *parent = nullptr;

	if (!root) {
		root = newNode(key);
		return;
	}

	while (cur_node) {
		parent = cur_node;

		if (isLess(key, cur_node->Data))
			cur_node = cur_node->Left;
		else
			cur_node = cur_node->Right;
	}

	if (isLess(key, parent->Data))
		parent->Left = newNode(key);
	else
		parent->Right = newNode(key);
}

// Function to traverse Tree with ability to change Tree
template<class T, class IsLess>
void Tree<T, IsLess>::postorder_traverse_change(void (*visit)(Node<T>*)) {
    Stack<Node<T>*> s1;
    Stack<Node<T>*> s2;

    s1.push_back(root);
    while (!s1.is_empty()) {
        Node<T> *node = s1.pop_back();
        s2.push_back(node);

        if (node->Left)
            s1.push_back(node->Left);
        if (node->Right)
            s1.push_back(node->Right);
    }

    while (!s2.is_empty()) {
    	visit(s2.pop_back());
    }
}

// Function to traverse Tree without ability to change Tree
template<class T, class IsLess>
void Tree<T, IsLess>::postorder_traverse(void (*visit)(const T& key)) const {

    Stack<Node<T>*> s1;
    Stack<Node<T>*> s2;

    s1.push_back(root);
    while (!s1.is_empty()) {
        Node<T> *node = s1.pop_back();
        s2.push_back(node);

        if (node->Left)
            s1.push_back(node->Left);
        if (node->Right)
            s1.push_back(node->Right);
    }

    while (!s2.is_empty()) {
    	visit(s2.pop_back()->Data);
    }

}

int main() {

	size_t n;
	cin >> n;

	Tree<int> *tree = new Tree<int>;
	for (size_t i = 0; i < n; i++) {
		int value;
		cin >> value;
		tree->insert(value);
	}

	tree->postorder_traverse([](const int& Data) {cout << Data << " ";});

	delete tree;

	return 0;
}
