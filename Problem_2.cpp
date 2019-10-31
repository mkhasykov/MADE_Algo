/*
Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
((1+2)+3) -> 1+2 + 3+3 = 9
((1+3)+2) -> 1+3 + 4+2 = 10
((2+3)+1) -> 2+3 + 5+1 = 11
Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.
Формат входных данных. Вначале вводится n - количество чисел. Затем вводится n строк - значения чисел (значение каждого числа не превосходит 10^9, сумма всех чисел не превосходит 2*10^9).
Формат выходных данных. Натуральное число - минимальное время.

5
5 2 3 4 6
45

5
3 7 6 1 9
56
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <assert.h>
using namespace std;

// MinHeap class
template<class T>
class Heap {
public:
	Heap(const T* input_arr, int input_n);
	~Heap();

	void Add(T value);  // add elem to heap
	T GetMin();         // extract minimum from heap
	int GetNum();       // Get number of elements in heap
	void PrintData();

private:
	T* arr;      // array of heap elems
	int size; // ind of last elem
	void SiftDown(int i);
	void SiftUp(int i);
	void BuildHeap();
};

template<class T>
Heap<T>::Heap(const T* input_arr, int input_n) {
	arr = new T[input_n];
	memcpy(arr, input_arr, input_n * sizeof(T));
	size = input_n - 1;
	BuildHeap();
}

template<class T>
Heap<T>::~Heap() {
	delete [] arr;
}

template<class T>
void Heap<T>::SiftDown(int i) {
	assert (i <= size);

	int ind_min = i;   // ind of smallest child (if exists)
	int left  = 2*i+1; // ind of left chid
	int right = 2*i+2; // ind of right child

	if (left <= size && arr[left] < arr[i]) {
		ind_min = left;
	}
	if (right <= size && arr[right] < arr[ind_min]) {
		ind_min = right;
	}

	if (ind_min!=i) {
		swap(arr[i], arr[ind_min]);
		SiftDown(ind_min);
	}
}

template<class T>
void Heap<T>::SiftUp(int i) {
	assert (i <= size);

	if (i > 0) {
		if (arr[i] < arr[(i-1)/2]) {
			swap(arr[i], arr[(i-1)/2]);
		}
		SiftUp((i-1)/2);
	}
}

template<class T>
void Heap<T>::BuildHeap() {
	for (int i = (size+1) / 2 - 1; i>=0; --i) {
		SiftDown(i);
	}
}

template<class T>
void Heap<T>::Add(T value) {
	size += 1;
	arr[size] = value;
	SiftUp(size);
}

template<class T>
T Heap<T>::GetMin() {
	assert (size >= 0);

	T root = arr[0];
	arr[0] = arr[size];
	--size;
	if (size>0) {
		SiftDown(0);
	}
	return root;
}

template<class T>
int Heap<T>::GetNum() {
	return size+1;
}

template<class T>
void Heap<T>::PrintData() {
	for (int j = 0; j <= size; j++) cout << arr[j] << ' ';
	cout << endl;
}

template<class T>
T count_time(const T* arr, size_t n) {
	T time = 0;
	T sum_cur = 0;
	Heap<T> heap(arr, n);

	while (heap.GetNum()>1) {
		sum_cur = heap.GetMin() + heap.GetMin();
		time += sum_cur;
		heap.Add(sum_cur);
	}
	return time;
}


int main()
{
	int n;
	cin >> n;

	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		int value;
		cin >> value;
		arr[i] = value;
	}

	cout << count_time(arr, n);

	delete [] arr;

    return 0;
}

