// named_fn_params.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "tests.h"

int main()
{
	runGmqSample();
	runGmqSample2();
	runJsonSample();
	runJsonSample2();
	testParsingJsonFile();
	testParsingJsonFile2();
	testMessageAliases();
	testMessageAliases2();
	testScopedMessageComposingAndParsing();
	testScopedMessageComposingAndParsing2();
	// publishableTestOne();
	publishableTestTwo();
	//quickTestForGmqParts();
}
