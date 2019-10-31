/*
Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение.Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

1_3. Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
3
3 44
3 50
2 44
YES

2
2 -1
3 10
YES

2
3 44
2 66
NO
*/


#include <iostream>
#include <assert.h>
#include <string>
#include <cstring>

using namespace std;

template<class T>
class Stack {
public:
	Stack();
	~Stack();

	bool is_empty();  // check if stack is empty
	void push_back(T num);
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
void Stack<T>::push_back(T num) {
	// make buffer twice bigger if full
	if (tail == len-1) {
		len *= 2;
		T* tempData = new T[len];
		memcpy(tempData, data, len * sizeof(T) / 2);
		delete [] data;
		data = new T[len];
		memcpy(data, tempData, len * sizeof(T) / 2);
		delete [] tempData;
	}
	tail += 1;
	data[tail] = num;
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
		data = new T[len];
		memcpy(data, tempData, len * sizeof(T) / 2);
		delete [] tempData;
	}
	return tempVal;
}

// for debugging
template<class T>
void Stack<T>::print_data() {
	std::cout<< "tail=" << tail << endl;
	for (int i = 0; i < tail+1; i++) cout << data[i] << endl;
}

template<class T>
class Queue {
public:
	Queue();
	~Queue();

	bool is_empty();  // check if queue is empty
	void push_back(T num);
	T pop_front();

private:
	Stack<T> s1; // to enqueue
	Stack<T> s2; // to dequeue
};

template<class T>
Queue<T>::Queue() {
	Stack<T> s1;
	Stack<T> s2;
}

template<class T>
Queue<T>::~Queue() {
}

template<class T>
bool Queue<T>::is_empty() {
	return s1.is_empty() && s2.is_empty();
}

template<class T>
void Queue<T>::push_back(T num) {
	s1.push_back(num);
}

template<class T>
T Queue<T>::pop_front()
{
	assert (!is_empty());

	if (s2.is_empty()) {
		// move data from s1 to s2
		while (!s1.is_empty()) s2.push_back(s1.pop_back());
	}
	return s2.pop_back();
}

int main()
{
	Queue<int> queue;
	string answer = "YES";

	int n, a, b, value2compar;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a >> b;
		if (a==2) {
			if (queue.is_empty()) {
				value2compar = -1;
			} else {
				value2compar = queue.pop_front();
			}

			if (value2compar!=b) {
				answer = "NO";
			}
		} else {
			queue.push_back(b);
		}
	}

	cout << answer;

	return 0;
}
