#include "stdafx.h"
#include <iostream>
#include "json/json.h"

#pragma comment(lib , "json/lib_json.lib")

// write
bool WriteToFile(const char* filename, const char* buffer, int len)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "wb");

	if (fp == nullptr)
	{
		return false;
	}

	size_t fileSize = fwrite(buffer, 1, len, fp);

	fclose(fp);

	return true;
}

// read
bool ReadFromFile(const char* filename, char* buffer, int len)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "rb");

	if (fp == nullptr)
	{
		return false;
	}

	size_t fileSize = fread(buffer, 1, len, fp);

	fclose(fp);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{

	// Toggle comment
	// [write] //*
	// [read] /* 

	/*
	// write test

	Json::Value root;
	Json::Value encoding;
	root["encoding"] = "UTF-8";
	Json::Value plugins;
	plugins.append("python");
	plugins.append("c++");
	plugins.append("ruby");
	root["plug-ins"] = plugins;
	Json::Value indent;
	indent["length"] = 3;
	indent["use_space"] = true;
	root["indent"] = indent;
	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	bool result = WriteToFile("test.json", outputConfig.c_str(), outputConfig.length());


	/*/

	// read test

	const int BufferLength = 1024;
	char readBuffer[BufferLength] = { 0, };

	if (ReadFromFile("test.json", readBuffer, BufferLength) == false)
	{
		return -1;
	}

	std::cout << "Read : " << readBuffer << std::endl;

	std::string config_doc = readBuffer;
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(config_doc, root);

	if (parsingSuccessful == false)
	{
		std::cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
		return -1;
	}

	std::string encoding = root.get("encoding", "").asString();
	std::cout << encoding << std::endl;
	const Json::Value plugins = root["plug-ins"];

	for (unsigned int index = 0; index < plugins.size(); ++index)
	{
		std::cout << plugins[index].asString() << std::endl;
	}

	std::cout << root["indent"].get("length", 0).asInt() << std::endl;
	std::cout << root["indent"]["use_space"].asBool() << std::endl;

	//*/
	return 0;
}

