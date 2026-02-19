#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	for (char ch : str) {
		if (ch == delimiter) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else {
			token += ch;
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}
std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delimiter);
	while (end != std::string::npos) {
		tokens.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	if (start < str.length()) {
		tokens.push_back(str.substr(start));
	}
	return tokens;
}

std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters) {
	std::vector<std::string> tokens;
	std::intmax_t start = 0;
	while (start < str.length()) {
		size_t minPos = std::string::npos;
		std::string foundDelimiter;
		for (const auto& delimiter : delimiters) {
			size_t pos = str.find(delimiter, start);
			if (pos != std::string::npos && (minPos == std::string::npos || pos < minPos)) {
				minPos = pos;
				foundDelimiter = delimiter;
			}
		}
		if (minPos == std::string::npos) {
			tokens.push_back(str.substr(start));

			break;
		} else {
			tokens.push_back(str.substr(start, minPos - start));
			start = minPos + foundDelimiter.length();
		}
	}
	return tokens;
}
struct Info {
	std::vector< std::vector<std::string>> Types;
	std::vector<std::vector<std::string>> DataDefines;
	std::vector<std::intmax_t> AlignDefines;
	std::intmax_t ByteBits = -1;
	std::intmax_t DataByteCount = -1;
};
std::vector<std::string> drop(const std::vector<std::string>& vec, const std::string& In) {
	auto vec2 = vec;
	std::intmax_t n = -1;

	L:
	n = 0;
	for (auto& o : vec2) {
		if (o == In) {
			vec2.erase(vec2.begin() + n);
			goto L;
		}
		n++;
	}
	return vec2;

}

const std::vector<std::string>& Find(const std::vector<std::string>& vec, const std::string& In) {
	for (auto& o : vec) {
		if (o == In) {
			return vec;
		}
	}
	return std::vector<std::string>();
}
Info MKInfo(const std::vector<std::vector<std::string>>& types, const std::vector<std::vector<std::string>>& dataDefines, std::intmax_t byteBits = 4) {
	Info info;
	info.Types = types;
	info.DataDefines = dataDefines;
	info.ByteBits = byteBits;

	for (auto& o : info.Types) {
		if (o[0] == "byte1") {
			info.ByteBits = std::atoi(&o[1][0]);
		}
		o = drop(o, std::string(""));
	}
	for (auto& o : info.DataDefines) {
		o = drop(o, std::string(""));
	}

	for (std::intmax_t i = 0; i < info.DataDefines.size(); i++) {
		info.AlignDefines.push_back(std::atoi(&info.DataDefines[i][1][0]));
		if(info.AlignDefines.back()==0&&info.AlignDefines.size()==2) {
			info.AlignDefines.back() = -1;
		}
	}

	for (std::intmax_t i = 0; i < info.AlignDefines.size();i++) {
		if (info.AlignDefines[i] == -1) {
			for (auto& o : info.Types) {
				if (o[0] == info.DataDefines[i][0]) {
					info.AlignDefines[i] = std::atoi(&o[1][0]);
					break;
				}
			}
		}
	}

	for (auto& o : info.AlignDefines) {
		info.DataByteCount += o;
	}


	return info;
}




#define Comment Comment
#define Raw Raw
#define Text Text
#define Data Data

int main() {
	std::string str = "byte1=4b,Char=4,int8=2,uint8=2,int16=4";
	char delimiter = ',';
	std::vector<std::string> tokens = split(str, delimiter);
	std::vector<std::vector<std::string>> typeDefines;
	for (const auto& token : tokens) {
		std::vector<std::string> parsed = split(token, '=');
		if (parsed.size() == 2) {
			typeDefines.push_back(parsed);
		}
	}
	
	std::string  DataD = "Char[16] Name,int16 ID,Data[40] raw,Comment[33] text;";
	std::vector<std::string> delimiters = { ",", ";"};
	std::vector<std::string> tokens2 = split(DataD, delimiters);
	std::vector<std::vector<std::string>> dataDefines;	
	for (const auto& token : tokens2) {
		std::vector<std::string> parsed = split(token, { "[","]"," "});
		//if (parsed.size() == 2) {
			dataDefines.push_back(parsed);
		//}
	}

	Info info = MKInfo(typeDefines, dataDefines);

	return 0;
}


