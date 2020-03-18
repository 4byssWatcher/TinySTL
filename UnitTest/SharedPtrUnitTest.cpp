#include "pch.h"
#include "CppUnitTest.h"
#include "../TinySTL/memory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedPtrUnitTest
{
	int foo_in;
	int foo_out;

	TEST_CLASS(MultiplicationTests)
	{
	public:
		struct foo
		{
			int data = 1;
			foo() { ++foo_in; }
			~foo() { ++foo_out; }
		};

		template <class T, class U>
		bool is_equal(TinySTL::shared_ptr<T> p1, U* p2)
		{
			return (p1.get() == p2);
		}

		template <class T>
		bool is_equal(TinySTL::shared_ptr<T> p1,
					  TinySTL::shared_ptr<T> p2)
		{
			return (p1.get() == p2.get());
		}

		/* shared_ptr(pointer), get() */
		TEST_METHOD(TestMethod1)
		{
			int* p = new int;
			TinySTL::shared_ptr<int> p1(p);
			Assert::IsTrue(is_equal(p1, p));
		}

		/* shared_ptr<array> */
		void bar()
		{
			TinySTL::shared_ptr<foo[]> p(new foo[5]);
		}

		/* con/des */
		TEST_METHOD(TestMethod2)
		{
			foo_in  = 0;
			foo_out = 0;
			bar();
			Assert::IsTrue(foo_in  == 5);
			Assert::IsTrue(foo_out == 5);
		}

		/* operator= */
		TEST_METHOD(TestMethod3)
		{
			int* p = new int;
			TinySTL::shared_ptr<int>p1(p);
			TinySTL::shared_ptr<int>p2 = p1;
			Assert::IsTrue(is_equal(p1, p2));
		}

		/* operator[] */
		TEST_METHOD(TestMethod4)
		{
			TinySTL::shared_ptr<foo[]> p(new foo[5]);
			Assert::IsTrue(p[4].data == 1);
		}

		/* operator* */
		TEST_METHOD(TestMethod5)
		{
			TinySTL::shared_ptr<int> p(new int);
			*p = 1;
			Assert::IsTrue(*p == 1);
		}
	};
}
