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

Response send(Request const& request) {
    // Send an HTTP request.  Auto-fill the content-length headers.
    std::string const string = str(request); 
    
    uint16_t port = 0;
    coro::Ptr<coro::Socket> sd;
    if (request.uri().scheme() == "https") {
        sd.reset(new coro::SslSocket);
        port = 443;
    } else if (request.uri().scheme() == "http") {
        sd.reset(new coro::Socket);
        port = 80;
    } else {
        assert(!"unknown http scheme");
    }
    if (request.uri().port()) {
        port = request.uri().port();
    }
    sd->connect(coro::SocketAddr(request.uri().host(), port));
    sd->writeAll(string.c_str(), string.size());

    std::vector<char> buffer(16384); // 16 KiB
    std::stringstream ss;
    
    while (size_t bytes = sd->read(&buffer[0], buffer.size())) {
        ss.write(&buffer[0], bytes);
    }

    return Response(ss.str());
}

Response get(std::string const& path, std::string const& data) {
    // Shortcut for simple GET requests
    Request request;
    request.methodIs(Request::GET);
    request.uriIs(Uri(path));
    request.dataIs(data);
    return send(request);
}

Response post(std::string const& path, std::string const& data) {
    // Shortcut for simple POST requests
    Request request;
    request.methodIs(Request::POST);
    request.uriIs(Uri(path));
    request.dataIs(data);
    return send(request);
}

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

std::string str(Request const& request) {
    // Serialize a request to a string
    std::stringstream ss;
    auto path = request.path().empty() ? "/" : request.path();
    ss << str(request.method()) << ' ' << path << " HTTP/1.1\n";
    ss << Headers::HOST << ": " << request.uri().host() << "\n";
    ss << Headers::CONTENT_LENGTH << ": " << request.data().size() << "\n";
    ss << Headers::CONNECTION << ": close\n";
    ss << Headers::ACCEPT_ENCODING << ": identity\n";
    for(auto header : request.headers()) {
        ss << header.first << ": " << header.second << "\n";
    }
    ss << "\n";
    ss << request.data();
    return ss.str();
}

}
