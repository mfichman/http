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

#pragma once

#include "http/Common.hpp"

namespace http {

class Authority {
public:
    Authority(std::string const& user, std::string const& host, uint16_t port);  
    Authority();

    std::string const& user() const { return user_; }
    std::string const& host() const { return host_; }
    uint16_t port() const { return port_; }

    void userIs(std::string const& user);
    void hostIs(std::string const& host);
    void portIs(uint16_t port);
private:
    std::string user_;
    std::string host_;
    uint16_t port_;
};

class Uri {
public:
    Uri(char* const value);
    Uri(std::string const& value);
    Uri();
    
    std::string const& scheme() const { return scheme_; } 
    Authority const& authority() const { return authority_; }
    std::string const& path() const { return path_; }
    std::string const& host() const { return authority_.host(); }
    uint16_t port() const { return authority_.port(); }

    void schemeIs(std::string const& scheme);
    void authorityIs(Authority const& authority);
    void pathIs(std::string const& path);
private:
    std::string scheme_;
    Authority authority_;
    std::string path_;
};

}
