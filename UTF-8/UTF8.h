#ifndef UTF8_H_
#define UTF8_H_

#include <string>

//using ustring = std::basic_string<unsigned char>;

class UTF8 {
public:
	std::string encode(const std::u32string& input_str);
	std::u32string decode(const std::string& input_str);
};

#endif