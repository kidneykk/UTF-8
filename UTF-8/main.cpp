#include <iostream>
#include <fstream>

#include "UTF8.h"

using namespace std;

int main() {
	UTF8 utf8;

	u32string str{ U"漢字" };
	string str_enc = utf8.encode(str);	// utf-8 str
	u32string str_dec = utf8.decode(str_enc);	// initial str

	ofstream fout("output.txt", ios::binary);
	fout.write((char*)&str_dec[0], str_dec.size());
	fout.close();


	return 0;
}