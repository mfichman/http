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
#include "http/Response.hpp"
#include "http/Error.hpp"

namespace http {

template <typename T>
class ParseResult {
public:
    T value;
    char const* ch;
};

template <typename F>
static ParseResult<std::string> parseUntil(char const* str, F func) {
    ParseResult<std::string> result{};
    char const* ch = str;
    for (; *ch && !func(*ch); ++ch) {}
    result.value = std::string(str,ch-str);
    result.ch = ch;
    return result;
}

template <typename F>
static ParseResult<std::string> parseWhile(char const* str, F func) {
    ParseResult<std::string> result{};
    char const* ch = str;
    for (; *ch && func(*ch); ++ch) {}
    result.value = std::string(str,ch-str);
    result.ch = ch;
    return result;
}

static ParseResult<std::string> parseToken(char const* str) {
    auto token = parseUntil(str, isspace);
    token.ch = parseWhile(token.ch, isspace).ch;
    return token;
}

static ParseResult<std::string> parseCrLf(char const* str) {
    auto cr = parseUntil(str, [](char ch) { return ch == '\r'; });
	if (*cr.ch == '\r') {
		cr.ch++;
	}
	return parseWhile(cr.ch, [](char ch) { return isspace(ch) && ch != '\r'; });
}

static ParseResult<Response::Status> parseStatus(char const* str) {
    ParseResult<Response::Status> result{};
    auto code = parseToken(str);

    result.value = (Response::Status)std::atoi(code.value.c_str());
    result.ch = code.ch;
    return result;
}

Response parseResponse(char const* str) {
    // Parse an HTTP response 
    auto version = parseToken(str);
    auto code = parseStatus(version.ch);
    auto message = parseUntil(code.ch, [](char ch) { return ch == '\r'; });
    
    auto response = Response();
    if (version.value != "HTTP/1.1") {
        throw Error("bad HTTP version");
    }
      
    auto ch = parseCrLf(message.ch).ch;
    while (*ch != '\0' && *ch != '\r') {
        auto name = parseUntil(ch, [](char ch) { return ch == ':'; });
        if (*name.ch) {
            name.ch++; // For ":"
        }
        auto ws = parseWhile(name.ch, isspace);
        auto value = parseUntil(ws.ch, [](char ch) { return ch == '\r'; });   
        response.headerIs(name.value, value.value);
        ch = parseCrLf(value.ch).ch;
    }
    ch = parseCrLf(ch).ch;
    
    response.statusIs(code.value);
    response.dataIs(ch); 
    return response;
}

Response::Response(std::string const& response) {
     *this = parseResponse(response.c_str());
}

std::string const Response::header(std::string const& name) const {
    return headers_.header(name);
}

void Response::statusIs(Status status) {
    status_ = status;
}

void Response::dataIs(std::string const& data) {
    data_ = data;
}

void Response::headerIs(std::string const& name, std::string const& value) {
    headers_.headerIs(name, value);
}

}
