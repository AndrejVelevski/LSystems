#pragma once

#include <windows.h>
#include <codecvt>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

namespace Utils
{
	bool contains(const std::string& string, char c)
	{
		return string.find(c) != std::string::npos;
	}

	std::string toUpper(std::string string)
	{
		std::for_each(string.begin(), string.end(), [&](char& c) {
			c = toupper(c);
		});
		return string;
	}

	std::string toLower(std::string string)
	{
		std::for_each(string.begin(), string.end(), [&](char& c) {
			c = tolower(c);
		});
		return string;
	}

	std::vector<std::string> split(std::string string, std::string delimiter)
	{
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = string.find(delimiter, pos_start)) != std::string::npos)
		{
			token = string.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(string.substr(pos_start));
		return res;
	}

	std::wstring s2ws(const std::string& string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(string);
	}

	std::string ws2s(const std::wstring& wstring)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstring);
	}

	std::string openFileDialog()
	{
		OPENFILENAME ofn;
		wchar_t fileName[MAX_PATH] = L"";
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = L"L-System Files(*.ls)\0*.ls\0";
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_NOCHANGEDIR;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = L"";

		std::wstring fileNameStr;

		if (GetOpenFileName(&ofn))
			fileNameStr = fileName;

		return ws2s(fileNameStr);
	}
}