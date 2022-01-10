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

using globalmq.marshalling;
using System;

namespace csharp_message
{
	class Program
	{
		static void json2gmq(string input, string output)
		{
			SimpleBuffer buffer = SimpleBuffer.readFromFile(input);
			JsonParser parser = new JsonParser(buffer.getReadIterator());

			mtest.message_one msg;

			mtest.message_one.parse(parser, out msg);

			SimpleBuffer buffer2 = new SimpleBuffer();
			GmqComposer composer = new GmqComposer(buffer2);

			mtest.message_one.compose(composer,
				firstParam: msg.firstParam,
				secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
				thirdParam: new CollectionWrapperForComposing(
					() => { return msg.thirdParam.Count; },
					(ComposerBase composer, int ordinal) => { point3D.compose(composer, msg.thirdParam[ordinal]); }),
				forthParam: msg.forthParam,
				fifthParam: msg.fifthParam,
				sixthParam: new CollectionWrapperForComposing(
					() => { return msg.sixthParam.Count; },
					(ComposerBase composer, int ordinal) => { point.compose(composer, msg.sixthParam[ordinal]); }),
				seventhParam: msg.seventhParam,
				eighthParam: new MessageWrapperForComposing(
					(ComposerBase composer) => { point.compose(composer, msg.eighthParam); }),
				ninethParam: new MessageWrapperForComposing(
					(ComposerBase composer) => { point3D.compose(composer, msg.ninethParam); }),
				tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam)
			);

			buffer2.writeToFile(output);
		}

		static void gmq2json(string input, string output)
		{
			SimpleBuffer buffer = SimpleBuffer.readFromFile(input);
			GmqParser parser = new GmqParser(buffer.getReadIterator());

			mtest.message_one msg;

			mtest.message_one.parse(parser, out msg);

			SimpleBuffer buffer2 = new SimpleBuffer();
			JsonComposer composer = new JsonComposer(buffer2);

			mtest.message_one.compose(composer,
				firstParam: msg.firstParam,
				secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
				thirdParam: new CollectionWrapperForComposing(
					() => { return msg.thirdParam.Count; },
					(ComposerBase composer, int ordinal) => { point3D.compose(composer, msg.thirdParam[ordinal]); }),
				forthParam: msg.forthParam,
				fifthParam: msg.fifthParam,
				sixthParam: new CollectionWrapperForComposing(
					() => { return msg.sixthParam.Count; },
					(ComposerBase composer, int ordinal) => { point.compose(composer, msg.sixthParam[ordinal]); }),
				seventhParam: msg.seventhParam,
				eighthParam: new MessageWrapperForComposing(
					(ComposerBase composer) => { point.compose(composer, msg.eighthParam); }),
				ninethParam: new MessageWrapperForComposing(
					(ComposerBase composer) => { point3D.compose(composer, msg.ninethParam); }),
				tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam)
			);

			buffer2.writeToFile(output);
		}



		static void printUsage()
		{
			Console.WriteLine("Usage:");
			Console.WriteLine("cpp_message [--json|--gmq] path/to/message");
			Console.WriteLine("  --json : input file is in json format");
			Console.WriteLine("  --gmq  : input file is in gmq format");
		}

		static void Main(string[] args)
		{
			if (args.Length != 2)
			{
				printUsage();
				return;
			}

			bool isJson = false;
			string format = args[0];
			string inputPath = args[1];

			if (format == "--json")
				isJson = true;
			else if (format != "--gmq")
			{
				printUsage();
				return;

			}


			if (inputPath.Length == 0 || inputPath[0] == '-')
			{
				printUsage();
				return;
			}

			//FILE* input = fopen(inputPath.c_str(), "rb");
			//if (!input)
			//{
			//	printf("Error opening input file '%s'\n", inputPath.c_str());
			//	return 1;
			//}

			string outputPath = inputPath + (isJson ? ".gmq" : ".json");
			//FILE* output = fopen(outputPath.c_str(), "wb");
			//if (!output)
			//{
			//	printf("Error opening output file '%s'\n", outputPath.c_str());
			//	return 1;
			//}


			if (isJson)
				json2gmq(inputPath, outputPath);
			else
				gmq2json(inputPath, outputPath);

			return;
		}
	}
}
