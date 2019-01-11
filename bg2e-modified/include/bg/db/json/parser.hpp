/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
Some parts of the json namespace are extracted and adapted from JsonBox:
https://github.com/anhero/JsonBox

Copyright (c) 2011 Anhero Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef _bg2e_db_json_parser_hpp_
#define _bg2e_db_json_parser_hpp_

#include <string>
#include <sstream>
#include <fstream>

#include <bg/system/path.hpp>
#include <bg/db/json/value.hpp>
#include <bg/base/exception.hpp>
#include <bg/base/referenced_pointer.hpp>

namespace bg {
namespace db {
namespace json {

class Parser {
public:
	static Value * ParseString(const std::string & str, bool strict = false) {
		std::stringstream stream(str);
		return ParseStream(stream, strict);
	}
	static Value * ParseStream(std::istream & stream, bool strict = false) {
		ptr<Value> val = new Value();
		val->loadFromStream(stream, strict);
		return val.release();
	}
	static Value * ParseFile(const std::string & path, bool strict = false) {
		std::ifstream file;
		file.open(path);
		if (file.is_open()) {
			Value * result = ParseStream(file, strict);
			file.close();
			return result;
		}
		else {
			file.close();
			throw bg::base::NoSuchFileException("Error loading json from file: no such file");
		}
	}

	static Value * ParseFile(const bg::system::Path & path, bool strict = false) {
		return ParseFile(path.text(), strict);
	}
};

}
}
}

#endif