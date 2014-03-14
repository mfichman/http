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
#include "http/http.hpp"

namespace http {

/*
Response get(std::string const& path, std::string const& data) {

}

Response post(std::string const& path, std::string const& data) {

}
*/

std::string str(Request::Method method) {
    switch (method) {
    case Request::GET: return "GET";
    case Request::HEAD: return "HEAD";
    case Request::POST: return "POST";
    case Request::PUT: return "PUT";
    case Request::DELETE: return "DELETE";
    case Request::TRACE: return "TRACE";
    case Request::CONNECT: return "CONNECT";
    default: assert(!"unknown request method");
    }
    return "";
}

/*
std::string str(Request const& request) {
    //std::stringstream ss;
        

    //ss << str(request.method() << ' ' << request.path() << " HTTP/1.1\n";
    //ss << host
    return "";
}
*/

}
