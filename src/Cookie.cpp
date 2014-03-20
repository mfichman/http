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
#include "http/Cookies.hpp"
#include "http/Parse.hpp"

namespace http {

ParseResult<std::string> parseName(char const* str) {
    return parseUntil(str, [](char ch) { return isspace(ch) || ch == '='; });
}

ParseResult<std::string> parseValue(char const* str) {
    return parseUntil(str, [](char ch) { return ch == ';' || ch == '='; });
}

ParseResult<std::string> parseSeparator(char const* str) {
    if (*str) {
        assert(*str==';'||*str=='='); 
        return parseWhitespace(str+1);
    } else {
        auto result = ParseResult<std::string>{};
        result.ch = str;
        return result;
    }
}

Cookie parseCookie(char const* str) {
    auto name = parseName(str);
    auto ch = parseSeparator(name.ch).ch;
    auto value = parseValue(ch);
    ch = parseSeparator(value.ch).ch;

    auto cookie = Cookie();
    cookie.nameIs(name.value);
    cookie.valueIs(value.value);
    while (*ch) {
        auto flag = parseValue(ch);
        if (flag.value == "Path") {
            ch = parseSeparator(flag.ch).ch;
            flag = parseValue(ch);
            cookie.pathIs(flag.value);
        } else if (flag.value == "HttpOnly") {
            cookie.httpOnlyIs(true);
        } else if (flag.value == "Secure") {
            cookie.secureIs(true);
        }
        ch = parseSeparator(flag.ch).ch;
    }
    return cookie;
}

Cookie::Cookie(std::string const& text) {
    *this = parseCookie(text.c_str());
}

Cookie const Cookies::cookie(std::string const& name) const {
    auto i = cookie_.find(name);
    return (i == cookie_.end()) ? Cookie() : i->second;
}

void Cookies::cookieIs(Cookie const& cookie) {
    cookie_[cookie.name()] = cookie;
}

}

