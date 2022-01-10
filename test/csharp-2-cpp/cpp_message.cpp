/* -------------------------------------------------------------------------------
* Copyright (c) 2021, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#include <cstdio>
#include <string>
#include <vector>
#include "gen_message_one.h"

struct Point
{
	int x;
	int y;
};

struct Point3D
{
	int x;
	int y;
	int z;
};

void json2gmq(FILE* input, FILE* output)
{
	mtest::Buffer b;
	b.read_file(input);
	fclose(input);
	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser(riter);

	auto msg = mtest::test_json::MESSAGE_message_one_json_parse(parser);

	mtest::Buffer bOut;
	mtest::GmqComposer composer(bOut);

	mtest::STRUCT_message_one_compose(composer,
		mtest::thirdParam = mtest::CollectionWrapperForComposing([&]() { return msg.thirdParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.thirdParam[ordinal].x, mtest::y = msg.thirdParam[ordinal].y, mtest::z = msg.thirdParam[ordinal].z); }),
		mtest::firstParam = msg.firstParam, mtest::fifthParam = msg.fifthParam, mtest::forthParam = msg.forthParam, mtest::seventhParam = msg.seventhParam,
		mtest::eighthParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point_compose(c, mtest::x = msg.eighthParam.x, mtest::y = msg.eighthParam.y); }),
		mtest::ninethParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.ninethParam.x, mtest::y = msg.ninethParam.y, mtest::z = msg.ninethParam.z); }),
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper(msg.secondParam),
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper(msg.tenthParam),
		mtest::sixthParam = mtest::CollectionWrapperForComposing([&]() { return msg.sixthParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point_compose(c, mtest::x = msg.sixthParam[ordinal].x, mtest::y = msg.sixthParam[ordinal].y); })
	);

	bOut.write_file(output);
	fclose(output);
}


void gmq2json(FILE* input, FILE* output)
{
	mtest::Buffer b;
	b.read_file(input);
	fclose(input);
	auto riter = b.getReadIter();
	mtest::GmqParser<mtest::Buffer> parser(riter);

	auto msg = mtest::test_gmq::MESSAGE_message_one_gmq_parse(parser);

	mtest::Buffer bOut;
	mtest::JsonComposer composer(bOut);

	mtest::STRUCT_message_one_compose(composer,
		mtest::thirdParam = mtest::CollectionWrapperForComposing([&]() { return msg.thirdParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.thirdParam[ordinal].x, mtest::y = msg.thirdParam[ordinal].y, mtest::z = msg.thirdParam[ordinal].z); }),
		mtest::firstParam = msg.firstParam, mtest::fifthParam = msg.fifthParam, mtest::forthParam = msg.forthParam, mtest::seventhParam = msg.seventhParam,
		mtest::eighthParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point_compose(c, mtest::x = msg.eighthParam.x, mtest::y = msg.eighthParam.y); }),
		mtest::ninethParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.ninethParam.x, mtest::y = msg.ninethParam.y, mtest::z = msg.ninethParam.z); }),
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper(msg.secondParam),
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper(msg.tenthParam),
		mtest::sixthParam = mtest::CollectionWrapperForComposing([&]() { return msg.sixthParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point_compose(c, mtest::x = msg.sixthParam[ordinal].x, mtest::y = msg.sixthParam[ordinal].y); })
	);

	bOut.write_file(output);
	fclose(output);
}


void printUsage()
{
	printf("Usage:\n");
	printf("cpp_message [--json|--gmq] path/to/message\n");
	printf("  --json : input file is in json format\n");
	printf("  --gmq  : input file is in gmq format\n");
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printUsage();
		return 1;
	}

	bool isJson = false;
	std::string format = argv[1];
	std::string inputPath = argv[2];

	if (format == "--json")
		isJson = true;
	else if (format != "--gmq")
	{
		printUsage();
		return 1;

	}


	if (inputPath.empty() || inputPath[0] == '-')
	{
		printUsage();
		return 1;
	}
	
	FILE* input = fopen(inputPath.c_str(), "rb");
	if (!input)
	{
		printf("Error opening input file '%s'\n", inputPath.c_str());
		return 1;
	}

	std::string outputPath = inputPath + (isJson ? ".gmq" : ".json");
	FILE* output = fopen(outputPath.c_str(), "wb");
	if (!output)
	{
		printf("Error opening output file '%s'\n", outputPath.c_str());
		return 1;
	}

	// don't catch exceptions so debugger kicks in
	if (isJson)
		json2gmq(input, output);
	else
		gmq2json(input, output);


	return 0;
}
