#include "pch.h"
#include "CppUnitTest.h"
#include "../TinySTL/deque.h"
#include "../TinySTL/utility.h"

#include <deque>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace dequeUnitTest
{
	TEST_CLASS(MultiplicationTests)
	{
	public:
		template <class T, class Alloc1, class Alloc2>
		bool is_equal(std::deque<T, Alloc1> v1,
					  TinySTL::deque<T, Alloc2> v2)
		{
			if (v1.size() != v2.size())return false;
			for (size_t i = 0; i < v1.size(); ++i)
			{
				if (v1[i] != v2[i])
					return false;
			}
			return true;
		}

		template <class T, class Alloc1, class Alloc2>
		bool is_equal(TinySTL::deque<T, Alloc1> v1,
			TinySTL::deque<T, Alloc2> v2)
		{
			if (v1.size() != v2.size())return false;
			for (size_t i = 0; i < v1.size(); ++i)
			{
				if (v1[i] != v2[i])
					return false;
			}
			return true;
		}

		TinySTL::deque<int>
			gettsdeque()
		{
			return TinySTL::deque<int>(100, 100);
		}

		std::deque<int>
			getstddeque()
		{
			return std::deque<int>(100, 100);
		}

		/* deque() */
		TEST_METHOD(TestMethod1)
		{
			std::deque    <int> v1;
			TinySTL::deque<int> v2;
			for (int i = 0; i < 10000; ++i)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* deque(n, val) */
		TEST_METHOD(TestMethod2)
		{
			std::deque    <int> v1(100, 100);
			TinySTL::deque<int> v2(100, 100);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* deque(first, last) */
		TEST_METHOD(TestMethod3)
		{
			int a[100];
			for (int i = 0; i < 100; ++i)
				a[i] = i;
			std::deque    <int> v1(a, a + 100);
			TinySTL::deque<int> v2(a, a + 100);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* deque(rvalue) */
		TEST_METHOD(TestMethod4)
		{
			std::deque<int> v1(getstddeque());
			TinySTL::deque<int> v2(gettsdeque());
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* assign(n, val) */
		TEST_METHOD(TestMethod5)
		{
			std::deque<int> v1(100, 100);
			TinySTL::deque<int> v2(100, 100);
			v1.assign(50, 50);
			v2.assign(50, 50);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* assign(first, last) */
		TEST_METHOD(TestMethod6)
		{
			int a[100];
			for (int i = 0; i < 100; ++i)
				a[i] = i;
			std::deque<int> v1(50, 50);
			TinySTL::deque<int> v2(50, 50);
			v1.assign(a, a + 100);
			v2.assign(a, a + 100);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* clear(), empty() */
		TEST_METHOD(TestMethod7)
		{
			TinySTL::deque<int> v;
			for (int i = 0; i < 100; ++i)
				v.push_back(i);
			v.clear();
			Assert::IsTrue(v.empty());
		}

		/* size */
		TEST_METHOD(TestMethod8)
		{
			TinySTL::deque<int> v;
			for (int i = 0; i < 100; ++i)
				v.push_back(i);
			Assert::IsTrue(v.size() == 100);
		}

		/* shrink_to_fit */
		TEST_METHOD(TestMethod9)
		{
			std::deque<int>     v1(100, 100);
			TinySTL::deque<int> v2(100, 100);
			v1.push_back(1);
			v2.push_back(1);
			v2.shrink_to_fit();
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* insert(pos, count, val), insert(pos, first, last) */
		TEST_METHOD(TestMethod10)
		{
			int a[5] = { 5,4,3,2,1 };
			std::deque<int>     v1(100, 100);
			TinySTL::deque<int> v2(100, 100);
			auto it_1 = v1.end();
			auto it_2 = v2.end();
			--it_1; --it_2;
			v1.insert(it_1, 50, 50);
			v2.insert(it_2, 50, 50);

			it_1 = v1.end();
			it_2 = v2.end();
			--it_1; --it_2;
			v1.insert(it_1, a, a + 50);
			v2.insert(it_2, a, a + 50);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* emplace */
		TEST_METHOD(TestMethod11)
		{
			std::deque<TinySTL::pair<int, int> >     v1;
			TinySTL::deque<TinySTL::pair<int, int> > v2;
			for (int i = 0; i < 50; ++i)
			{
				v1.push_back(TinySTL::make_pair(1, 2));
				v2.push_back(TinySTL::make_pair(1, 2));
			}

			auto it_1 = v1.end();
			auto it_2 = v2.end();
			--it_1; --it_2;
			v1.emplace(it_1, 2, 3);
			v2.emplace(it_2, 2, 3);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* erase */
		TEST_METHOD(TestMethod12)
		{
			int a[100];
			for (int i = 0; i < 100; ++i)
				a[i] = i;
			std::deque<int>     v1(a, a + 100);
			TinySTL::deque<int> v2(a, a + 100);
			auto it_1 = v1.end();
			auto it_2 = v2.end();
			--it_1; --it_2;
			v1.erase(it_1);
			v2.erase(it_2);

			it_1 = v1.begin();
			it_2 = v2.begin();
			++it_1; ++it_2;
			v1.erase(it_1);
			v2.erase(it_2);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* push_back, pop_back, emplace_back */
		TEST_METHOD(TestMethod13)
		{
			std::deque<TinySTL::pair<int, int> >     v1;
			TinySTL::deque<TinySTL::pair<int, int> > v2;
			for (int i = 0; i < 100; ++i)
			{
				v1.push_back(TinySTL::make_pair(i, i));
				v2.push_back(TinySTL::make_pair(i, i));
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 50; ++i)
			{
				v1.pop_back();
				v2.pop_back();
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 50; ++i)
			{
				v1.pop_back();
				v2.pop_back();
			}
			Assert::IsTrue(v2.empty());
			v1.emplace_back(1, 2);
			v2.emplace_back(1, 2);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* push_front, pop_front, emplace_back */
		TEST_METHOD(TestMethod14)
		{
			std::deque<TinySTL::pair<int, int> >     v1;
			TinySTL::deque<TinySTL::pair<int, int> > v2;
			for (int i = 0; i < 100; ++i)
			{
				v1.push_front(TinySTL::make_pair(i, i));
				v2.push_front(TinySTL::make_pair(i, i));
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 50; ++i)
			{
				v1.pop_front();
				v2.pop_front();
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 50; ++i)
			{
				v1.pop_front();
				v2.pop_front();
			}
			Assert::IsTrue(v2.empty());
			v1.emplace_back(1, 2);
			v2.emplace_back(1, 2);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* resize */
		TEST_METHOD(TestMethod15)
		{
			std::deque<int>     v1;
			TinySTL::deque<int> v2;
			for (int i = 0; i < 100; ++i)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			v1.resize(50);
			v2.resize(50);
			Assert::IsTrue(is_equal(v1, v2));
			v1.resize(150);
			v2.resize(150);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* swap */
		TEST_METHOD(TestMethod16)
		{
			TinySTL::deque<int> v1(100, 100);
			TinySTL::deque<int> v2(50, 50);
			TinySTL::deque<int> v1_copy(v1);
			TinySTL::deque<int> v2_copy(v2);
			v1.swap(v2);
			Assert::IsTrue(is_equal(v1, v2_copy));
			Assert::IsTrue(is_equal(v2, v1_copy));
		}
	};
}
