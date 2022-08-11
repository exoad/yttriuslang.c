/*
 * File:   unittest.h
 * Author: Kimmy Leo
 *
 */

#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h>

#define TestCase(NAME)								\
	extern void TEST_CASE_FUNCTION_##NAME(void);	\
	class TEST_CASE_CLASS_##NAME					\
	{												\
	public:											\
		TEST_CASE_CLASS_##NAME()					\
		{											\
			printf("%s\n", #NAME);                	\
			TEST_CASE_FUNCTION_##NAME();			\
		}											\
	} TEST_CASE_INSTANCE_##NAME;					\
	void TEST_CASE_FUNCTION_##NAME(void)

#define TestAssert(CONDITION) do{ if (!(CONDITION)) throw 0; } while (0)
#define TestPrint(MESSAGE) printf("%s", MESSAGE);

#define TestRunner() int main(){return 0;}

#endif	/* UNITTEST_H */

