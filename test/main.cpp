// named_fn_params.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "tests.h"

int main()
{
	runJsonSample();
	runGmqSample();
	testParsingJsonFile();
	testMessageAliases();
	testScopedMessageComposingAndParsing();
	publishableTestOne();
	quickTestForGmqParts();
}
