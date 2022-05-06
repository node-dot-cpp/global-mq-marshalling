/* -------------------------------------------------------------------------------
 * Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
 * All rights reserved.
 *
 * Copyright (c) 2020-2021, OLogN Technologies AG
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

#include "idl_generators.h"
#include "idl_tree_common.h"
#include <idl_parser.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        fmt::print("Usage:\n");
        fmt::print(
            "generator path/to/idl path/to/header [--csharp|--cplusplus|--cplusplus-templates] [-m=metascopename]\n");
        fmt::print("  -m : defines metascope name (default=m)\n");
        fmt::print("  --csharp       : generate C# code\n");
        fmt::print("  --cplusplus    : generate C++ code\n");
        fmt::print("  --cpptemplates : generate C++ templatized code\n");
        return 0;
    }

    std::string idlPath = argv[1];
    std::string targetPath = argv[2];
    GenerationConfig config;
    bool isCsharp = false;
    bool isCplusplus = false;
    bool isCppTemplates = false;
    for (int i = 3; i < argc; ++i)
    {
        std::string entry = argv[i];
        if (entry == "--csharp")
        {
            isCsharp = true;
            if (isCplusplus || isCppTemplates)
            {
                fmt::print("Only one generator can be specified\n");
                return 0;
            }
        }
        else if (entry == "--cplusplus")
        {
            isCplusplus = true;
            if (isCsharp || isCppTemplates)
            {
                fmt::print("Only one generator can be specified\n");
                return 0;
            }
        }
        else if (entry == "--cpptemplates")
        {
            isCppTemplates = true;
            if (isCsharp || isCplusplus)
            {
                fmt::print("Only one generator can be specified\n");
                return 0;
            }
        }
        else
        {
            size_t separPos = entry.find_first_of("=");
            if (separPos == std::string::npos)
            {
                fmt::print("unexpected parameter {}\n", entry);
                return 0;
            }

            string key = entry.substr(0, separPos);
            if (key == "-m")
            {
                config.metascope = entry.substr(separPos + 1);
                if (config.metascope == "")
                {
                    fmt::print("metascope (-m) option, if specified, must not be empty {}\n", key);
                    return 0;
                }
            }
            else if (key == "-p")
            {
                config.platformPrefix = entry.substr(separPos + 1);
                if (config.platformPrefix == "")
                {
                    fmt::print("platformprefix (-p) option, if specified, must not be empty {}\n", key);
                    return 0;
                }
            }
            else if (key == "-c")
            {
                config.classNotifierName = entry.substr(separPos + 1);
                if (config.classNotifierName == "")
                {
                    fmt::print("notifier class name (-c) option, if specified, must not be empty {}\n", key);
                    return 0;
                }
            }
            else if (key == "--parser")
            {
                string parserName = entry.substr(separPos + 1);
                if (parserName == "")
                {
                    fmt::print("parser class name (--parser) option, if specified, must not be empty.\n");
                    return 0;
                }
                config.parserNames.push_back(parserName);
            }
            else if (key == "--composer")
            {
                string composerName = entry.substr(separPos + 1);
                if (composerName == "")
                {
                    fmt::print("composer class name (--composer) option, if specified, must not be empty.\n");
                    return 0;
                }
                config.composerNames.push_back(composerName);
            }
            else
            {
                fmt::print("unexpected parameter {}\n", key);
                return 0;
            }
        }
    }

    if ((config.classNotifierName.size() == 0 && config.platformPrefix.size() != 0) ||
        (config.classNotifierName.size() != 0 && config.platformPrefix.size() == 0))
    {
        fmt::print("platformprefix (-p)  and notifier class name (-c) options must either be both specified or not\n");
        return 0;
    }

    if (config.metascope.empty())
        config.metascope = "m";

    size_t lastSlash = targetPath.find_last_of("\\/");
    if (lastSlash == std::string::npos)
        lastSlash = 0;
    string fileName = targetPath.substr(lastSlash);
    if (fileName.size() == 0)
    {
        fmt::print("failed to identify header file name\n");
        return 0;
    }
    for (size_t i = 0; i < fileName.size(); ++i)
    {
        if (!isalnum(fileName[i]))
        {
            fileName.replace(i, 1, "_");
        }
    }

    config.fileName = fileName;
    //	try
    // {
    config.fileChecksum = idlFileChecksum(idlPath);
    Root* root = parseSourceFile(idlPath, false);
    printRoot(*root);

    preprocessRoot(*root);

    FILE* header = fopen(targetPath.c_str(), "wb");

    if (isCsharp)
        generateCsharp(header, *root, config.metascope.c_str());
    else if (isCplusplus)
        generateCplusplus(header, *root, config);
    else
        generateCppTemplates(config.fileName.c_str(), config.fileChecksum, header, config.metascope.c_str(),
                             config.platformPrefix, config.classNotifierName, *root);

    // }
    /*catch ( std::exception& x )
    {
        fmt::print( "Exception happened: {}\n", x.what() );
    }*/

    return 0;
}
