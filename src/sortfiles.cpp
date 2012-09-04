#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

#define BOOST_FILESYSTEM_VERSION 2

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"

using namespace boost;

void fileSystemBrowser(const filesystem::path& path, const std::string& extension);

int main (int argc, char** argv) 
{
	int exitCode = EXIT_FAILURE;

	try
	{
		program_options::options_description optionsDescription("Program options");
		optionsDescription.add_options() ("folder", program_options::value<std::string>(), "Starting directory") ("extension", program_options::value<std::string>(), " Optional extension of sortable files");
		
		program_options::variables_map variablesMap;
		program_options::store(program_options::parse_command_line(argc, argv, optionsDescription), variablesMap);
		program_options::notify(variablesMap);

		if (variablesMap.count("folder") == 1)
		{
			std::string folder = variablesMap["folder"].as<std::string>();
			std::string extension = variablesMap.count("extension") == 1 ? variablesMap["extension"].as<std::string>() : "";
			filesystem::path currentPath(folder);
		
			if (filesystem::exists(currentPath))
			{
				std::cout << "Starting directory: " << folder << std::endl;
				std::cout << "Extension of sortable files: " << extension << std::endl;

				fileSystemBrowser(currentPath, extension);
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

void fileSystemBrowser(const filesystem::path& path, const std::string& extension)
{
	std::vector<filesystem::path> subFolders;
	std::vector<filesystem::path> newFolders;
	std::vector<filesystem::path> files;

	filesystem::directory_iterator directoryIterator(path);
	filesystem::directory_iterator endIterator;
	
	for ( ; directoryIterator != endIterator; directoryIterator++)
	{
		filesystem::path subPath = directoryIterator->path();

		if (filesystem::is_directory(subPath))
		{
			subFolders.push_back(subPath);
		}
		else
		{
			files.push_back(subPath);
		}
	}

	std::size_t fileCount = files.size();

	for (unsigned int i = 0; i < fileCount; i++)
	{
		filesystem::path filePath = files[i];
		std::string fileName = filePath.filename();
		std::string fileExtension = filePath.extension();
		time_t lastModified = filesystem::last_write_time(filePath);
		tm* timeInfo = localtime(&lastModified);
		int year = timeInfo->tm_year + 1900;
		int month = timeInfo->tm_mon + 1;
		int day = timeInfo->tm_mday;
		
		std::ostringstream outStringStream;
		outStringStream << (timeInfo->tm_year + 1900) << "-";
		outStringStream << (month < 10 ? "0" : "") << month << "-";
		outStringStream << (day < 10 ? "0" : "") << day;

		std::string dateString = outStringStream.str();
		filesystem::path dateFolder = path;
		dateFolder.append(dateString.begin(), dateString.end());

		bool exists = (std::find(subFolders.begin(), subFolders.end(), dateFolder) != subFolders.end()) || 
			(std::find(newFolders.begin(), newFolders.end(), dateFolder) != newFolders.end());

		if (extension == "" || fileExtension == extension)
		{
			if (!exists)
			{
				filesystem::create_directory(dateFolder);
				newFolders.push_back(dateFolder);
			}

			filesystem::path movePath = dateFolder;
			movePath.append(fileName.begin(), fileName.end());
			filesystem::rename(filePath, movePath);
		}
	}

	std::size_t subFolderCount = subFolders.size();

	for (unsigned int i = 0; i < subFolderCount; i++)
	{
		fileSystemBrowser(subFolders[i], extension);
	}
}