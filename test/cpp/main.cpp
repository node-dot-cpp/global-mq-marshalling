// named_fn_params.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "tests.h"
#include "comparsers-test.h"

int main()
{
	runGmqSample();
	runJsonSample();
	testParsingJsonFile();
	testMessageAliases();
	testScopedMessageComposingAndParsing();
	// publishableTestOne();
	publishableTestTwo();
	//quickTestForGmqParts();
	comparsers_test();
}
