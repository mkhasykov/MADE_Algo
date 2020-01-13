/*
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<assert.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::sort;

struct Point {
	long double X;
	long double Y;

	Point() : X(0.0), Y(0.0) {}
	Point(long double x, long double y) : X(x), Y(y) {}
};

long double Distance(const Point& l, const Point& r) {
	long double d = sqrt( (r.X - l.X) * (r.X - l.X) + (r.Y - l.Y) * (r.Y - l.Y) );
	return d;
}

class IsLessByAngleOrDist {
public:
	IsLessByAngleOrDist(const Point& _point) : point(_point) {}

	bool operator()(const Point& l, const Point& r)
	{
		Point v1 = Point(l.X - point.X, l.Y - point.Y);
		Point v2 = Point(r.X - point.X, r.Y - point.Y);

		long double cross_product = v1.X * v2.Y - v2.X * v1.Y;

		if ( std::abs(cross_product) <= 1e-6 )
			return Distance( l , point ) > Distance( r , point );

		return cross_product > 1e-6;
	}

private:
	Point point;
};

class IsLessByAngle {
public:
	IsLessByAngle(const Point& _point) : point(_point) {}

	bool operator()(const Point& l, const Point& r)
	{
		Point v1 = Point(l.X - point.X, l.Y - point.Y);
		Point v2 = Point(r.X - point.X, r.Y - point.Y);

		long double cross_product = v1.X * v2.Y - v2.X * v1.Y;

		return cross_product > 1e-6;
	}

private:
	Point point;
};

long double ConvexHullPerim(Point arr[], unsigned int n) {
	assert (n > 2);

	vector<Point> border;
	long double perim = 0.0;

	// choose pivot point as most left bottom point
	Point pivot = arr[0];
	for (unsigned int i=1; i < n; ++i) {
		if (abs(arr[i].Y - pivot.Y) < 1e-6 && arr[i].X < pivot.X) {
			pivot = arr[i];
		}

		if (arr[i].Y < pivot.Y) {
			pivot = arr[i];
		}
	}

	// sort points by angle and distance from pivot
	sort(arr, arr + n, IsLessByAngleOrDist(pivot));

	// determine border points
	border.push_back(pivot);
	border.push_back(arr[0]);
	IsLessByAngle isLessPivot(pivot);
	for (unsigned int i=1; i < n; ++i) {

		// skip point if it's on the same line with farther point
		if ( !isLessPivot( border.back(), arr[i] ) ) {
			continue;
		}

		if (border.size() < 3) {
			border.push_back(arr[i]);
			continue;
		}

		// delete last but one point if it spawns left rotation with a new one
		while (true) {
			IsLessByAngle isLess(border[border.size() - 2]);
			if (isLess(arr[i], border[border.size() - 1])) {
				border.pop_back();
			} else {
				break;
			}
		}

		border.push_back(arr[i]);
	}

	// add start point to make perimeter closed
	border.push_back(border[0]);

	// count perimeter length
	while (border.size() > 1) {
		perim += Distance(border[border.size() - 2], border[border.size() - 1]);
		border.pop_back();
	}

	return perim;
}


int main()
{
	unsigned int n;
	long double x, y;

	cin >> n;
	Point arr[n];
	for (unsigned int i = 0; i < n; i++) {
		cin >> x >> y;
		arr[i] = Point(x, y);
	}

	cout.precision(9);
	cout << ConvexHullPerim(arr, n);

	return 0;
}
