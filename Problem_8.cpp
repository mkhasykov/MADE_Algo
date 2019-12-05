/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.

Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки
в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.
*/

#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

using std::vector;
using std::string;
using std::cin;
using std::cout;

struct Hasher
{
	unsigned int operator()(const string& str, const unsigned int& a) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < str.length(); i++)
			hash = hash * a + str[i];
		return hash;
	}
};

template<class T, class H>
class HashTable
{
public:
	HashTable(const H& _hasher);
	HashTable(const HashTable& t) = delete;
	HashTable& operator=(const HashTable& t) = delete;
	~HashTable();

	bool Has(const T& key) const;
	bool Add(const T& key);
	bool Delete(const T& key);

private:
	H hasher;
	T* table; // keys
	vector<char> state; // 0 - empty, 1 - with key, 2 - delete
	unsigned int keysCount;

	void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H& _hasher) :
	hasher(_hasher),
	table(new T[8]),
	state(8, '0'),
	keysCount(0)
{
}

template<class T, class H>
HashTable<T, H>::~HashTable()
{
	delete [] table;
}

template<class T, class H>
bool HashTable<T, H>::Has(const T& key) const
{
	unsigned int hsum = hasher(key, 31);
	unsigned int hash = hsum % state.size();
	T elem = table[hash];

	unsigned int i = 0;
	while (state[hash] != '0') {
		if ((elem == key) && (state[hash]=='1'))
			break;
		if (i == state.size()) {
			return false;
		}
		hsum += 2*hasher(key, 17)+1;
		hash = hsum % state.size();
		elem = table[hash];
		++i;
	}
	return state[hash] == '1';
}

template<class T, class H>
bool HashTable<T, H>::Add(const T& key)
{
	if (4 * keysCount >= state.size() * 3) {
		growTable();
	}

	unsigned int hsum = hasher(key, 31);
	unsigned int hash = hsum % state.size();
	T elem = table[hash];

	unsigned int i = 0;
	int first_del = -1;
	while (state[hash] != '0') {
		// remember index of first occurrence of deleted item
		if (state[hash] == '2')
			first_del = hash;
		if ((elem == key) && (state[hash] == '1'))
			return false;
		if (i == state.size()) {
			if (first_del < 0) {
				return false;
			} else {
				break;
			}
		}
		hsum += 2*hasher(key, 17)+1;
		hash = hsum % state.size();
		elem = table[hash];
		++i;
	}

	if (first_del >= 0) {
		hash = first_del;
	}

	table[hash] = key;
	state[hash] = '1';
	++keysCount;
	return true;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
	T* oldTable = table;
	vector<char> oldState = state;

	table = new T[oldState.size() * 2];
	state = vector<char>(oldState.size() * 2, '0');
	keysCount = 0;

	for (unsigned int i = 0; i < oldState.size(); i++) {
		if (oldState[i] == '1')
			Add(oldTable[i]);
	}

	delete [] oldTable;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T& key)
{
	unsigned int hsum = hasher(key, 31);
	unsigned int hash = hsum % state.size();
	T elem = table[hash];

	unsigned int i = 0;
	while (state[hash] != '0') {
		if ((elem == key) && (state[hash]=='1'))
			break;
		if (i == state.size()) {
			return false;
		}
		hsum += 2*hasher(key, 17)+1;
		hash = hsum % state.size();
		elem = table[hash];
		++i;
	}

	if (state[hash] == '0') {
		return false;
	}

	state[hash] = '2';
	--keysCount;
	return true;
}

int main()
{
	Hasher hasher;
	HashTable<string, Hasher> table(hasher);

	char command = 0;
	string word;
	while (cin >> command >> word) {
		switch (command) {
		case '+':
			cout << (table.Add(word) ? "OK" : "FAIL") << "\n";
			break;
		case '-':
			cout << (table.Delete(word) ? "OK" : "FAIL") << "\n";
			break;
		case '?':
			cout << (table.Has(word) ? "OK" : "FAIL") << "\n";
			break;
		default:
			assert(false);
		}
	}
	return 0;
}
