/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "http/Common.hpp"

namespace http {

template <typename T>
class ParseResult {
public:
    T value;
    char const* ch;
};

template <typename F>
static inline ParseResult<std::string> parseUntil(char const* str, F func) {
    ParseResult<std::string> result{};
    char const* ch = str;
    for (; *ch && !func(*ch); ++ch) {}
    result.value = std::string(str,ch-str);
    result.ch = ch;
    return result;
}

template <typename F>
static inline ParseResult<std::string> parseWhile(char const* str, F func) {
    ParseResult<std::string> result{};
    char const* ch = str;
    for (; *ch && func(*ch); ++ch) {}
    result.value = std::string(str,ch-str);
    result.ch = ch;
    return result;
}

static inline ParseResult<std::string> parseToken(char const* str) {
    auto token = parseUntil(str, isspace);
    token.ch = parseWhile(token.ch, isspace).ch;
    return token;
}

static inline ParseResult<std::string> parseCrLf(char const* str) {
    auto cr = parseUntil(str, [](char ch) { return ch == '\r'; });
	if (*cr.ch == '\r') {
		cr.ch++;
	}
	return parseWhile(cr.ch, [](char ch) { return isspace(ch) && ch != '\r'; });
}

static inline ParseResult<std::string> parseWhitespace(char const* str) {
    return parseWhile(str, isspace);
}


}
