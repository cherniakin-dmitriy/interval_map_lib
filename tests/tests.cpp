#include "pch.h"

#include "../interval_map/interval_map.h"

TEST(NoAssign, Simple) {
	interval_map<int, char> im('a');
	EXPECT_EQ(im[10], 'a');
}

TEST(NoAssign, IntCorners) {
	interval_map<int, char> im('a');
	constexpr auto lowest = std::numeric_limits<decltype(im)::key_type>::lowest();
	constexpr auto max = std::numeric_limits<decltype(im)::key_type>::max();
	EXPECT_EQ(im[lowest], 'a');
	EXPECT_EQ(im[max], 'a');
}

TEST(NoAssign, FloatCorners) {
	interval_map<float, char> im('a');
	constexpr auto lowest = std::numeric_limits<decltype(im)::key_type>::lowest();
	constexpr auto max = std::numeric_limits<decltype(im)::key_type>::max();
	EXPECT_EQ(im[lowest], 'a');
	EXPECT_EQ(im[max], 'a');
}

TEST(EmptyRange, InvalidBoundaries) {
	interval_map<int, char> im('a');

	im.assign(1, -1, 'b');
	EXPECT_EQ(im[-1], 'a');
	EXPECT_EQ(im[0], 'a');
	EXPECT_EQ(im[1], 'a');
}

TEST(EmptyRange, SameBoundaries) {
	interval_map<int, char> im('a');

	im.assign(0, 0, 'b');
	EXPECT_EQ(im[0], 'a');
}

TEST(SimpleAssigns, TrivialAssigns) {
	interval_map<int, int> im(1);

	im.assign(1, 11, 2);
	im.assign(11, 21, 3);

	EXPECT_EQ(im[5], 2);
	EXPECT_EQ(im[14], 3);
}

TEST(SimpleAssigns, OneValueAssigns) {
	interval_map<int, char> im('a');

	using KV_t = std::pair<decltype(im)::key_type, decltype(im)::element_type>;
	constexpr static KV_t NEW_VALUES[] = { {1, 'c'}, {3, 'a'}, {5, 'z'}, {2, 'b'} };
	for (auto kv : NEW_VALUES)
		im.assign(kv.first, kv.first + 1, kv.second);
	for (auto kv : NEW_VALUES)
		EXPECT_EQ(im[kv.first], kv.second);
}

TEST(SimpleAssigns, IntersectingIntervals) {
	interval_map<int, char> im('a');

	im.assign(1, 11, 'b');
	im.assign(5, 15, 'c');

	EXPECT_EQ(im[0], 'a');
	EXPECT_EQ(im[1], 'b');
	EXPECT_EQ(im[4], 'b');
	EXPECT_EQ(im[5], 'c');
	EXPECT_EQ(im[14], 'c');
	EXPECT_EQ(im[15], 'a');
}

TEST(SimpleAssigns, OverwriteInterval) {
	interval_map<int, char> im('a');

	im.assign(1, 11, 'b');
	im.assign(1, 11, 'c');

	EXPECT_EQ(im[0], 'a');
	EXPECT_EQ(im[1], 'c');
	EXPECT_EQ(im[4], 'c');
	EXPECT_EQ(im[5], 'c');
	EXPECT_EQ(im[10], 'c');
	EXPECT_EQ(im[11], 'a');
}

TEST(SimpleAssigns, InnerInterval) {
	interval_map<int, char> im('a');

	im.assign(1, 11, 'b');
	im.assign(5, 9, 'c');

	EXPECT_EQ(im[0], 'a');
	EXPECT_EQ(im[1], 'b');
	EXPECT_EQ(im[4], 'b');
	EXPECT_EQ(im[5], 'c');
	EXPECT_EQ(im[10], 'b');
	EXPECT_EQ(im[11], 'a');
}

TEST(SimpleAssigns, ConsecutiveIntersectingIntervals) {
	interval_map<int, char> im('a');

	im.assign(1, 7, 'b');
	im.assign(6, 11, 'c');
	im.assign(-5, 2, 'd');

	EXPECT_EQ(im[-6], 'a');
	EXPECT_EQ(im[-5], 'd');
	EXPECT_EQ(im[1], 'd');
	EXPECT_EQ(im[2], 'b');
	EXPECT_EQ(im[5], 'b');
	EXPECT_EQ(im[6], 'c');
	EXPECT_EQ(im[10], 'c');
	EXPECT_EQ(im[11], 'a');
}

TEST(SimpleAssigns, ConsecutiveIntervals) {
	interval_map<int, char> im('a');

	im.assign(1, 6, 'b');
	im.assign(6, 11, 'c');
	im.assign(-5, 1, 'd');

	EXPECT_EQ(im[-6], 'a');
	EXPECT_EQ(im[-5], 'd');
	EXPECT_EQ(im[0], 'd');
	EXPECT_EQ(im[1], 'b');
	EXPECT_EQ(im[5], 'b');
	EXPECT_EQ(im[6], 'c');
	EXPECT_EQ(im[10], 'c');
	EXPECT_EQ(im[11], 'a');
}

template<typename V>
class dummy_interval_map {
	std::vector<V> data;
public:
	using element_type = V;

	const int size;

	dummy_interval_map(int size, V const& val) : size(size) {
		data.resize(size, val);
	}

	void assign(int const& keyBegin, int const& keyEnd, V const& val) {
		if (!(keyBegin < keyEnd))
			return;

		for (int i = keyBegin; i < keyEnd; ++i)
			data[i] = val;
	}

	V const& operator[](int const& key) const {
		return data[key];
	}
};

TEST(VerifyByDummy, SomeAssigns) {
	constexpr static int SIZE = 100;
	interval_map<int, char> im('a');
	dummy_interval_map<char> dim(SIZE, 'a');

	im.assign(1, 6, 'b');
	im.assign(6, 11, 'c');
	im.assign(54, 68, 'd');
	im.assign(13, 97, 'b');
	im.assign(67, 69, 'z');
	im.assign(13, 51, 'm');
	im.assign(11, 63, 'k');
	im.assign(61, 71, 'b');
	im.assign(80, 87, 't');
	im.assign(31, 47, 'v');
	im.assign(1, 7, 'p');
	im.assign(44, 51, 'b');

	dim.assign(1, 6, 'b');
	dim.assign(6, 11, 'c');
	dim.assign(54, 68, 'd');
	dim.assign(13, 97, 'b');
	dim.assign(67, 69, 'z');
	dim.assign(13, 51, 'm');
	dim.assign(11, 63, 'k');
	dim.assign(61, 71, 'b');
	dim.assign(80, 87, 't');
	dim.assign(31, 47, 'v');
	dim.assign(1, 7, 'p');
	dim.assign(44, 51, 'b');

	for (int i = 0; i < SIZE; ++i)
		EXPECT_EQ(im[i], dim[i]);
}

TEST(VerifyByDummy, RandGen) {
	constexpr static int SIZE = 1000;
	constexpr static int N_INTERVALS = 100000;
	interval_map<int, int> im('a');
	dummy_interval_map<int> dim(SIZE, 'a');

	srand(0);
	for (int i = 0; i < N_INTERVALS; ++i) {
		int p = rand() % SIZE,
			q = rand() % SIZE,
			v = rand();
		if (p > q)
			std::swap(p, q);
		im.assign(p, q, v);
		dim.assign(p, q, v);
	}

	for (int i = 0; i < SIZE; ++i)
		EXPECT_EQ(im[i], dim[i]);
}
