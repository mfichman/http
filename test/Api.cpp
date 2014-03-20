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

int main() {

    auto coro = coro::start([]{
        try {
            //http::Response res = http::get("https://192.168.1.154/foob"); 
            //
            http::Response res = http::post("https://192.168.1.154/user/login", 
                "{ \"UserId\": \"matt\", \"Password\": \"matt\"} ");

            auto sessionId = res.cookie("SessionId");
            std::cout << sessionId.value() << std::endl;
            std::cout << sessionId.httpOnly() << std::endl;
            std::cout << sessionId.secure() << std::endl;
            std::cout << sessionId.path() << std::endl;
        } catch (coro::SystemError const& ex) {
            std::cerr << ex.what() << std::endl;
        }
    });
    coro::run(); 
    return 0;
}
