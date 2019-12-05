/*
Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
Напишите нерекурсивный алгоритм. Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).

Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

#include <iostream>
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

// Partition with two iterators running from end to beginning of array
template<class T, class IsLess = IsLessDefaultFunctor<T> >
size_t Partition(T* arr, size_t n, IsLess isLess = IsLess()) {
	size_t pivot_ind = rand() % n;
	T pivot = arr[pivot_ind];
	swap(arr[0], arr[pivot_ind]);

	size_t i = n-1;

	while (isLess(pivot, arr[i])) {
		--i;
	}

	if (i==0) {
		return i;
	}

	size_t j = i-1;

	while (j > 0) {
		if (!isLess(arr[j], pivot)) {
				swap(arr[i], arr[j]);
			--i;
		}
		--j;
	}
	swap(arr[0], arr[i]);

	return i;
}

template<class T>
int KStatDC(T* arr, size_t n, size_t k) {
	size_t left = 0;    // index of left elem of considered array
	size_t right = n-1; // index of right elem

	size_t pivot_ind = Partition(arr, n);
	size_t candidate_ind = Partition(arr, n); // index of candidate for answer

	// change borders of considered array
	// if candidate answer is wrong
	while (candidate_ind!=k) {
		if (candidate_ind < k) {
			// move border to the right side of candidate
			left = candidate_ind + 1;
		} else {
			// move border to the left side
			right = candidate_ind - 1;
		}
		pivot_ind = Partition(arr+left, right - left + 1);
		candidate_ind = left + pivot_ind;
	}

	return arr[candidate_ind];
}

int main() {

	srand(time(0));

	size_t n; size_t k;
	cin >> n >> k;

	int* data = new int[n];
	for (size_t i = 0; i < n; i++) {
		int value;
		cin >> value;
		data[i] = value;
	}

	int k_stat = KStatDC(data, n, k);
	cout << k_stat;

	delete [] data;

	return 0;
}
