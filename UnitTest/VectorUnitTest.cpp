#include "pch.h"
#include "CppUnitTest.h"
#include "../TinySTL/vector.h"
#include "../TinySTL/utility.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorUnitTest
{
	TEST_CLASS(MultiplicationTests)
	{
	public:
		template <class T, class Alloc1, class Alloc2>
		bool is_equal(std::vector<T, Alloc1> v1,
						  TinySTL::vector<T, Alloc2> v2)
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
		bool is_equal(TinySTL::vector<T, Alloc1> v1,
					  TinySTL::vector<T, Alloc2> v2)
		{
			if (v1.size() != v2.size())return false;
			for (size_t i = 0; i < v1.size(); ++i)
			{
				if (v1[i] != v2[i])
					return false;
			}
			return true;
		}

		TinySTL::vector<int>
			gettsvector()
		{
			return TinySTL::vector<int>(10, 10);
		}

		std::vector<int>
			getstdvector()
		{
			return std::vector<int>(10, 10);
		}
		
		/* vector() */
		TEST_METHOD(TestMethod1)
		{
			std::vector    <int> v1;
			TinySTL::vector<int> v2;
			for (int i = 0; i < 10; ++i)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* vector(n, val) */
		TEST_METHOD(TestMethod2)
		{
			std::vector    <int> v1(10, 10);
			TinySTL::vector<int> v2(10, 10);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* vector(first, last) */
		TEST_METHOD(TestMethod3)
		{
			int a[10] = { 1, 2, 3, 4, 5, 6, 7, 6, 5, 4 };
			std::vector    <int> v1(a, a + 10);
			TinySTL::vector<int> v2(a, a + 10);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* vector(rvalue) */
		TEST_METHOD(TestMethod4)
		{
			std::vector<int> v1(getstdvector());
			TinySTL::vector<int> v2(gettsvector());
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* assign(n, val) */
		TEST_METHOD(TestMethod5)
		{
			std::vector<int> v1(10,10);
			TinySTL::vector<int> v2(10,10);
			v1.assign(5, 5);
			v2.assign(5, 5);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* assign(first, last) */
		TEST_METHOD(TestMethod6)
		{
			int a[10] = { 1, 2, 3, 4, 5, 6, 7, 6, 5, 4 };
			std::vector<int> v1(5, 5);
			TinySTL::vector<int> v2(5, 5);
			v1.assign(a, a + 10);
			v2.assign(a, a + 10);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* clear(), empty() */
		TEST_METHOD(TestMethod7)
		{
			TinySTL::vector<int> v;
			for (int i = 0; i < 10; ++i)
				v.push_back(i);
			v.clear();
			Assert::IsTrue(v.empty());
		}

		/* size */
		TEST_METHOD(TestMethod8)
		{
			TinySTL::vector<int> v;
			for (int i = 0; i < 10; ++i)
				v.push_back(i);
			Assert::IsTrue(v.size() == 10);
		}

		/* reserve, capacity */
		TEST_METHOD(TestMethod9)
		{
			TinySTL::vector<int> v;
			v.reserve(10);
			Assert::IsTrue(v.capacity()>=10);
		}

		/* shrink_to_fit */
		TEST_METHOD(TestMethod10)
		{
			std::vector<int>     v1(10, 10);
			TinySTL::vector<int> v2(10, 10);
			v1.push_back(1);
			v2.push_back(1);
			auto old_cap = v2.capacity();
			v2.shrink_to_fit();
			auto new_cap = v2.capacity();
			Assert::IsTrue(is_equal(v1, v2));
			Assert::IsTrue(new_cap <= old_cap);
		}

		/* insert(pos, count, val), insert(pos, first, last) */
		TEST_METHOD(TestMethod11)
		{
			int a[5] = { 5,4,3,2,1 };
			std::vector<int>     v1(10, 10);
			TinySTL::vector<int> v2(10, 10);
			auto it_1 = v1.end();
			auto it_2 = v2.end();
			--it_1; --it_2;
			v1.insert(it_1, 5, 5);
			v2.insert(it_2, 5, 5);

			it_1 = v1.end();
			it_2 = v2.end();
			--it_1; --it_2;
			v1.insert(it_1, a, a + 5);
			v2.insert(it_2, a, a + 5);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* emplace */
		TEST_METHOD(TestMethod12)
		{
			std::vector<TinySTL::pair<int, int> >     v1;
			TinySTL::vector<TinySTL::pair<int, int> > v2;
			for (int i = 0; i < 5; ++i)
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
		TEST_METHOD(TestMethod13)
		{
			int a[10] = { 1,2,3,4,5,6,7,6,5,4 };
			std::vector<int>     v1(a, a + 10);
			TinySTL::vector<int> v2(a, a + 10);
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
		TEST_METHOD(TestMethod14)
		{
			std::vector<TinySTL::pair<int, int> >     v1;
			TinySTL::vector<TinySTL::pair<int, int> > v2;
			for (int i = 0; i < 10; ++i)
			{
				v1.push_back(TinySTL::make_pair(i, i));
				v2.push_back(TinySTL::make_pair(i, i));
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 5; ++i)
			{
				v1.pop_back();
				v2.pop_back();
			}
			Assert::IsTrue(is_equal(v1, v2));
			for (int i = 0; i < 5; ++i)
			{
				v1.pop_back();
				v2.pop_back();
			}
			Assert::IsTrue(v2.empty());
			v1.emplace_back(1, 2);
			v2.emplace_back(1, 2);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* push_back, pop_back, emplace_back */
		TEST_METHOD(TestMethod15)
		{
			std::vector<int>     v1;
			TinySTL::vector<int> v2;
			for (int i = 0; i < 10; ++i)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			v1.resize(5);
			v2.resize(5);
			Assert::IsTrue(is_equal(v1, v2));
			v1.resize(15);
			v2.resize(15);
			Assert::IsTrue(is_equal(v1, v2));
		}

		/* swap */
		TEST_METHOD(TestMethod16)
		{
			TinySTL::vector<int> v1(10, 10);
			TinySTL::vector<int> v2(5, 5);
			TinySTL::vector<int> v1_copy(v1);
			TinySTL::vector<int> v2_copy(v2);
			v1.swap(v2);
			Assert::IsTrue(is_equal(v1, v2_copy));
			Assert::IsTrue(is_equal(v2, v1_copy));
		}
	};
}
