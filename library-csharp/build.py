import os
import sys
import shutil

def build(buildType):
    path = "./build/"
    isExist = os.path.exists(path)
    if not isExist:
        os.makedirs(path)
    os.system("dotnet build global-mq-csharp.csproj --arch any --output build --configuration " + buildType)

buildType = 'Release'

if len(sys.argv) <= 1:
    print('Build Type not set. Release configuration used as a default')
    buildType = 'Release'
else:
    for i in range(1, len(sys.argv)):
        if (sys.argv[i] == "Release"):
            print("Release configuration is set")
            buildType = 'Release'
        elif (sys.argv[i] == "Debug"):
            print("Debug configuration is set")
            buildType = 'Debug'
        else:
            print("Unexpected configuration [" + sys.argv[i] +
                  "]. Only [Release, Debug] configurations are allowed. Terminate")
            exit()

build(buildType)
