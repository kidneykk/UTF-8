#include "UTF8.h"

std::string UTF8::encode(const std::u32string& input_str) {
	std::string utf8str;
	unsigned char b0, b1, b2, b3;
	//char32_t utf8 = 0;
	for (const auto& ch : input_str) {
		if (ch <= 0x7F) {
			//utf8 = ch;

			utf8str += ch;
		}
		else if (ch >= 0x80 && ch <= 0x7FF) {
			// 2 bytes
			b0 = (ch & 0x3F) | 0x80;	// 10xxxxxx
			b1 = (ch >> 6) | 0xC0;	// 110xxxxx
			
			utf8str += b1;
			utf8str += b0;

			/*utf8 = b0;
			utf8 |= (short(b1) << 8);*/
		}
		else if (ch >= 0x800 && ch <= 0xFFFF) {
			// 3 bytes
			b0 = (ch & 0x3F) | 0x80;			// 10xxxxxx
			b1 = ((ch >> 6) & 0x3F) | 0x80;	// 10xxxxxx
			b2 = (ch >> 12) | 0xE0;			// 1110xxxx
			
			utf8str += b2;
			utf8str += b1;
			utf8str += b0;

			/*utf8 = b0;
			utf8 |= (short(b1) << 8);
			utf8 |= (char32_t(b2) << 16);*/
		}
		else if (ch >= 0x10000 && ch <= 0x10FFFF) {
			// 4 bytes
			b0 = (ch & 0x3F) | 0x80;			// 10xxxxxx
			b1 = ((ch >> 6) & 0x3F) | 0x80;	// 10xxxxxx
			b2 = ((ch >> 12) & 0x3F) | 0x80;	// 10xxxxxx
			b3 = (ch >> 18) | 0xF0;			// 11110xxx
			
			utf8str += b3;
			utf8str += b2;
			utf8str += b1;
			utf8str += b0;

			/*utf8 = b0;
			utf8 |= (short(b1) << 8);
			utf8 |= (char32_t(b2) << 16);
			utf8 |= (char32_t(b3) << 24);*/
		}
	}

	return utf8str;
}

std::u32string UTF8::decode(const std::string& input_str) {
	std::u32string str;
	using u8 = unsigned char;
	char32_t utf8 = 0;

	u8 b0, b1, b2, b3;

	for (int i = 0; i < input_str.size(); ++i) {
		if ((input_str[i] >> 7) == 0x00) {		// 0xxxxxxx
			// 1 byte
			str += input_str[i];
		}
		else if (((u8)input_str[i] & 0xDF) == (u8)input_str[i]) {	// 110xxxxx
			// 2 bytes
			b0 = (((u8)input_str[i] << 6) | (u8)input_str[i + 1]);
			b1 = (((u8)input_str[i]) & 0x1C);

			utf8 = b0;
			utf8 |= (short(b1) << 6);

			i++;
			str += utf8;
		}
		else if (((u8)input_str[i] & 0xEF) == (u8)input_str[i]) {		// 1110xxxx
			// 2 bytes
			b0 = (((u8)(input_str[i + 1]) << 6) | ((u8)(input_str[i + 2]) & 0x3F));
			b1 = (((u8)input_str[i] << 4) | (((u8)input_str[i + 1] >> 2) & 0xF));

			utf8 = b0;
			utf8 |= (short(b1) << 8);

			i += 2;
			str += utf8;
		}
		else if (((u8)input_str[i] & 0xF7) == (u8)input_str[i]) {		// 11110xxx
			// 3 bytes
			b0 = ((u8)input_str[i + 2] << 6) | ((u8)input_str[i + 3] & 0x3F);
			b1 = (((u8)input_str[i + 1] << 4) | (((u8)input_str[i + 2] >> 2) & 0xF));
			b2 = (((u8)input_str[i] << 5) | (((u8)input_str[i + 1] >> 4) & 0x3));

			utf8 = b0;
			utf8 |= (short(b1) << 8);
			utf8 |= (char32_t(b2) << 16);

			i += 3;
			str += utf8;
		}
	}

	return str;
}
