/*
Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
Требуется отсортировать последовательность. Последовательность может быть очень длинной.
Время работы O(n * log(k)). Память O(k). Использовать слияние.
*/

#include <iostream>
#include <assert.h>
using namespace std;

// Function to print elements of array
template<class T>
void PrintData(T* arr, size_t n) {
	for (size_t i = 0; i < n; i++) std::cout << arr[i] << ' ';
	 cout << endl;
}

// Functor for types with defined "<" operator
template<class T>
class IsLessDefaultFunctor {
public:
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

// SiftDown element of MaxHeap
template<class T, class IsLess = IsLessDefaultFunctor<T> >
void SiftDown(T* arr, size_t n, size_t i, IsLess isLess = IsLess()) {
	assert (i <= n);

	size_t ind_max = i;   // ind of smallest child (if exists)
	size_t left  = 2*i+1; // ind of left child
	size_t right = 2*i+2; // ind of right child

	if (left < n && isLess(arr[i], arr[left])) {
		ind_max = left;
	}
	if (right < n && isLess(arr[ind_max], arr[right])) {
		ind_max = right;
	}
	if (ind_max!=i) {
		swap(arr[i], arr[ind_max]);
		SiftDown(arr, n, ind_max, isLess);
	}
}

// Build array a MaxHeap
template<class T, class IsLess = IsLessDefaultFunctor<T> >
void BuildHeap(T* arr, size_t n, IsLess isLess = IsLess()) {
	for (int i = n / 2 - 1; i>=0; --i) {
		SiftDown(arr, n, i, isLess);
	}
}

// Sort array via HeapSort algorithm
template<class T, class IsLess = IsLessDefaultFunctor<T> >
void HeapSort(T* arr, size_t n, IsLess isLess = IsLess()) {
	size_t heapSize = n;
	BuildHeap(arr, heapSize, isLess);
	while(heapSize > 1) {
		swap(arr[0], arr[heapSize - 1]);
		--heapSize;
		SiftDown(arr, heapSize, 0, isLess);
	}
}

// Merge Sort of two neighboring parts of array with size of k
template<class T, class IsLess = IsLessDefaultFunctor<T> >
void MergeSort(T* arr1, size_t n1, T* arr2, size_t n2, IsLess isLess = IsLess()) {
	assert(n1 + n2 > 0);

	// create buffer to merge sorted arrays
	T* buffer = new T[n1+n2];
	size_t i = 0; size_t i1 = 0; size_t i2 = 0;

    while (i1 < n1 && i2 < n2) {
    	if (!isLess(arr2[i2], arr1[i1])) {
            buffer[i] = arr1[i1];
            i1++;
        } else {
        	buffer[i] = arr2[i2];
            i2++;
        }
        i++;
    }

    while (i1 < n1) {
        buffer[i] = arr1[i1];
        i1++;
        i++;
    }

    while (i2 < n2) {
		buffer[i] = arr2[i2];
		i2++;
		i++;
    }

    // return values to initial arrays
	for (size_t i=0; i < n1; i++) {
		arr1[i] = buffer[i];
	}

	for (size_t i=0; i < n2; i++) {
		arr2[i] = buffer[n1 + i];
	}

	delete [] buffer;
}

template<class T, class IsLess = IsLessDefaultFunctor<T> >
void Sort(T* arr, size_t n, size_t k, IsLess isLess = IsLess()) {
	if (k==0 || k > n) {
		HeapSort(arr, n, isLess);
		return;
	}

	size_t n_parts = n / k; // number of parts of size k
	if (n % k == 0) {
		--n_parts;
	}

	HeapSort(arr, k, isLess);
	for (size_t part_num=1; part_num <= n_parts; part_num++) {

		size_t start_prev = k*(part_num-1); // previous part of size k
		size_t end_prev   = k*part_num-1;

		size_t start_cur = k*part_num;      // current part of size k
		size_t end_cur   = k*(part_num+1)-1;
		if (part_num >= n_parts) {
			end_cur = n-1;
		}

		HeapSort(arr+start_cur, end_cur - end_prev, isLess);
		MergeSort(arr+start_prev, k, arr+start_cur, end_cur - end_prev, isLess);
	}
}


int main() {

	size_t n; size_t k;
	cin >> n >> k;

	int* data = new int[n];
	for (size_t i = 0; i < n; i++) {
		int value;
		cin >> value;
		data[i] = value;
	}

	Sort(data, n, k);
	PrintData(data, n);

	delete [] data;

	return 0;
}
