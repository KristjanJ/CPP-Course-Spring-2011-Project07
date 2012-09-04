#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

#define BOOST_FILESYSTEM_VERSION 2

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/regex.hpp"

using namespace boost;

void fileSystemBrowser(const filesystem::path& path, const std::string& text, const regex& pattern);
bool findText(const filesystem::path& path, const std::string& text);

int main (int argc, char** argv) 
{
	int exitCode = EXIT_FAILURE;

	try
	{
		program_options::options_description optionsDescription("Program options");
		optionsDescription.add_options() ("folder", program_options::value<std::string>(), "Starting directory") ("text", program_options::value<std::string>(), "Text to be searched") 
			("pattern", program_options::value<std::string>(), "Optional regex pattern to filter filenames");
		
		program_options::variables_map variablesMap;
		program_options::store(program_options::parse_command_line(argc, argv, optionsDescription), variablesMap);
		program_options::notify(variablesMap);

		if (variablesMap.count("folder") == 1 && variablesMap.count("text") == 1)
		{
			std::string folder = variablesMap["folder"].as<std::string>();
			std::string text = variablesMap["text"].as<std::string>();
			std::string pattern = variablesMap.count("pattern") == 1 ? variablesMap["pattern"].as<std::string>() : "";
			filesystem::path currentPath(folder);
			regex regexPattern(pattern != "" ? pattern : ".*");
		
			if (filesystem::exists(currentPath))
			{
				std::cout << "Starting directory: " << folder << std::endl;
				std::cout << "Text to be searched: " << text << std::endl;
				std::cout << "Regex pattern: " << pattern << std::endl;
				std::cout << std::endl;

				fileSystemBrowser(currentPath, text, regexPattern);
				exitCode = EXIT_SUCCESS;
			}
			else
			{
				std::cout << "Error: folder doesn't exist." << std::endl;
			}
		}
		else
		{
			std::cout << "Error: invalid command-line arguments." << std::endl;
			optionsDescription.print(std::cout);
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << "." << std::endl;
	}

	return exitCode;
}

void fileSystemBrowser(const filesystem::path& path, const std::string& text, const regex& pattern)
{
	std::vector<filesystem::path> subFolders;

	filesystem::directory_iterator directoryIterator(path);
	filesystem::directory_iterator endIterator;
	
	for ( ; directoryIterator != endIterator; directoryIterator++)
	{
		filesystem::path newPath = directoryIterator->path();

		if (filesystem::is_directory(newPath))
		{
			subFolders.push_back(newPath);
		}
		else
		{
			std::string fileName = newPath.filename();

			if (regex_match(fileName.begin(), fileName.end(), pattern) && findText(newPath, text))
			{
				std::cout << "File found: " << newPath.string() << std::endl;
			}
		}
	}

	std::size_t subFolderCount = subFolders.size();

	for (unsigned int i = 0; i < subFolderCount; i++)
	{
		fileSystemBrowser(subFolders[i], text, pattern);
	}
}

bool findText(const filesystem::path& path, const std::string& text)
{
	bool result = false;

	std::ifstream inFileStream;
	inFileStream.open(path.string());

	if (inFileStream.is_open())
	{
		unsigned int size = filesystem::file_size(path);

		char* buffer = new char[static_cast<unsigned int>(size)];
		inFileStream.read(buffer, size);
		inFileStream.close();

		std::string data(buffer, size);
		delete [] buffer;
		buffer = 0;

		result = data.find(text) != std::string::npos;
	}

	return result;
}