#include <iostream>
#include <exception>
#include <filesystem>
#include <string>
#include <fstream>
#include <iterator>

//Use enums for error code handling
enum class Errors {
	NoError,
	FileNotFound,
	NotRegularFile,
	CannotOpenFile
};

class FileNotFound : public std::runtime_error
{
public:
	explicit FileNotFound(std::filesystem::path filePath)
		: std::runtime_error("File not found: " + filePath.string()) {}

	std::filesystem::path filepath() const
	{
		return fullFileName;
	}

private:
	std::string fullFileName;

};


class NotRegularFile : public std::runtime_error
{

public:
	explicit NotRegularFile(std::filesystem::path filePath)
		: std::runtime_error("Not a regular file: " + filePath.string()) {}
	std::filesystem::path targetPath() const
	{
		return fullFileName;
	}
private:
	std::filesystem::path fullFileName;
};


class CannotOpenFile : public std::runtime_error
{
public:
	explicit CannotOpenFile(std::filesystem::path filePath)
		:std::runtime_error("Cannot open file: " + filePath.string()) {}

	std::filesystem::path targetPath() const
	{
		return fullFileName;
	}
private:
	std::filesystem::path fullFileName;
};


//std::string readFile(const std::filesystem::path& filePath){
Errors readFile(const std::filesystem::path& filePath, std::string& content)
{
	if (!std::filesystem::exists(filePath)) {
		//throw FileNotFound(filePath);
		return Errors::FileNotFound;
	}
	if (!std::filesystem::is_regular_file(filePath)) {
		//throw NotRegularFile(filePath);
		return Errors::NotRegularFile;
	}
	std::ifstream file(filePath);
	if (!file.is_open()) {
		//throw CannotOpenFile(filePath);
		return Errors::CannotOpenFile;
	}
	//return std::string(
		//(std::istreambuf_iterator<char>(file)), 
		//std::istreambuf_iterator<char>());

	//clear any existing content before reading new data
	content.clear();

	//Read file contents
	content.append(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	return Errors::NoError;
}



int main() {
	std::string content;
	switch (readFile("/path/to/file.txt" , content))
	{
	case Errors::NoError:
		std::cout << content << std::endl;
		return 0;

	case Errors::FileNotFound:
		std::cerr << "Can't find file" << std::endl;

	case Errors::NotRegularFile:
		std::cerr << "Not a regular file" << std::endl;

	case Errors::CannotOpenFile:
		std::cerr << "Can't open file" << std::endl;
		return 1;
	}
}
