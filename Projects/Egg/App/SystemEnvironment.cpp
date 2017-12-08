#include "stdafx.h"
#include "SystemEnvironment.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include "stdConversions.h"

using namespace Egg;

SystemEnvironment::SystemEnvironment(void)
{
	logStream.open("egg.log");

	arguments["solutionMediaPath"].push_back("./Media");
	arguments["solutionMediaPath"].push_back(".");

	arguments["projectMediaPath"].push_back("./Media");
	arguments["projectMediaPath"].push_back(".");

	arguments["solutionShaderPath"].push_back("./Shader");
	arguments["solutionShaderPath"].push_back(".");

	arguments["projectShaderPath"].push_back("./Shader");
	arguments["projectShaderPath"].push_back(".");

	std::string currentSwitch = "";

	std::string commandLine( GetCommandLineA() );
	using namespace boost;
	escaped_list_separator<char> sep("", " \t:,;", "\"");
	tokenizer< escaped_list_separator<char>, std::string::const_iterator, std::string > tokens(commandLine, sep);
	BOOST_FOREACH(std::string t, tokens)
	{
		if(t.substr(0, 2).compare("--") == 0)
		{
			if(arguments[currentSwitch].empty())
				arguments[currentSwitch].push_back("on");
			currentSwitch = t.substr(2, 64);
		}
		else
		{
			if(currentSwitch.compare("logfile") == 0)
			{
				logStream.close();
				logStream.open(t);
			}
			else
				arguments[currentSwitch].push_back(t);
		}
	}

	arguments["solutionMediaPath"].at(0) = getStringArgument("solutionPath") + "/Media";
	arguments["solutionMediaPath"].at(1) = getStringArgument("solutionPath");

	arguments["solutionShaderPath"].at(0) = getStringArgument("solutionPath") + "/Shaders";
	arguments["solutionShaderPath"].at(1) = getStringArgument("solutionPath");

	arguments["projectMediaPath"].at(0) = getStringArgument("projectPath") + "/Media";
	arguments["projectMediaPath"].at(1) = getStringArgument("solutionPath");

	arguments["projectShaderPath"].at(0) = getStringArgument("projectPath") + "/Shaders";
	arguments["projectShaderPath"].at(1) = getStringArgument("projectPath");

}

SystemEnvironment::~SystemEnvironment(void)
{
	logStream.close();
}

template<class T>
T	SystemEnvironment::getArgument(std::string argumentName)
{
	return fromString<T>(getStringArgument(argumentName));
}

bool	SystemEnvironment::getBoolArgument(std::string argumentName)
{
	return (0 == getStringArgument(argumentName).compare("on"));
}

std::string SystemEnvironment::getStringArgument(std::string argumentName)
{
	const std::vector<std::string>& stringList = getStringListArgument(argumentName);
	if(stringList.empty())
	{
		logWarning("No value for command line argument: " + argumentName, __FILE__, __LINE__);
		arguments[argumentName].push_back(".");
		return ".";
	}

	return stringList.at(0);
}

const std::vector<std::string>& SystemEnvironment::getStringListArgument(std::string argumentName)
{
	ArgumentMap::iterator i = arguments.find(argumentName);
	if(i == arguments.end())
	{
		logWarning("Unspecified command line argument: " + argumentName, __FILE__, __LINE__);
		arguments[argumentName] = std::vector<std::string>();
	}
	return arguments[argumentName];
}

std::string SystemEnvironment::resolveMediaPath(std::string filename)
{
	const std::vector<std::string>& mediaPath = getStringListArgument("projectMediaPath");
	std::vector<std::string>::const_iterator i = mediaPath.begin();
	std::vector<std::string>::const_iterator e = mediaPath.end();
	while(i != e)
	{
		std::string fullpathFilename = *i + "/" + filename;
		unsigned int attrs = GetFileAttributesA(fullpathFilename.c_str());
		if(attrs != 0xffffffff)
		{
			return fullpathFilename;
		}
		i++;
	}
	{
		const std::vector<std::string>& mediaPath = getStringListArgument("solutionMediaPath");
		std::vector<std::string>::const_iterator i = mediaPath.begin();
		std::vector<std::string>::const_iterator e = mediaPath.end();
		while(i != e)
		{
			std::string fullpathFilename = *i + "/" + filename;
			unsigned int attrs = GetFileAttributesA(fullpathFilename.c_str());
			if(attrs != 0xffffffff)
			{
				return fullpathFilename;
			}
			i++;
		}
	}
	logWarning( "Media file name could not be resolved: " + filename, __FILE__, __LINE__);
	return filename;
}

std::string SystemEnvironment::resolveShaderPath(std::string filename)
{
	const std::vector<std::string>& shaderPath = getStringListArgument("projectShaderPath");
	std::vector<std::string>::const_iterator i = shaderPath.begin();
	std::vector<std::string>::const_iterator e = shaderPath.end();
	while (i != e)
	{
		std::string fullpathFilename = *i + "/" + filename;
		unsigned int attrs = GetFileAttributesA(fullpathFilename.c_str());
		if (attrs != 0xffffffff)
		{
			return fullpathFilename;
		}
		i++;
	}
	{
		const std::vector<std::string>& shaderPath = getStringListArgument("solutionShaderPath");
		std::vector<std::string>::const_iterator i = shaderPath.begin();
		std::vector<std::string>::const_iterator e = shaderPath.end();
		while (i != e)
		{
			std::string fullpathFilename = *i + "/" + filename;
			unsigned int attrs = GetFileAttributesA(fullpathFilename.c_str());
			if (attrs != 0xffffffff)
			{
				return fullpathFilename;
			}
			i++;
		}
	}
	logWarning("Shader file name could not be resolved: " + filename, __FILE__, __LINE__);
	return filename;
}

void SystemEnvironment::logWarning(std::string message, std::string filename, unsigned int lineNumber)
{
	logStream << message << "   [" << filename.c_str() << ":" << lineNumber << "]" << std::endl;
}
