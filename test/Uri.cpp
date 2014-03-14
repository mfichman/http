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

#include <http/Common.hpp>
#include "http/http.hpp"

void simple() {
    http::Uri uri("http://www.google.com:80/foo/bar.html");
    assert(uri.scheme()=="http");
    assert(uri.authority().host()=="www.google.com");
    assert(uri.authority().port()==80);
    assert(uri.path()=="/foo/bar.html");
}

void user() {
    http::Uri uri("http://user@www.google.com:80/foo/bar.html");
    assert(uri.scheme()=="http");
    assert(uri.authority().user()=="user");
    assert(uri.authority().host()=="www.google.com");
    assert(uri.authority().port()==80);
    assert(uri.path()=="/foo/bar.html");
}

void noPort() {
    http::Uri uri("http://user@www.google.com/foo/bar.html");
    assert(uri.scheme()=="http");
    assert(uri.authority().user()=="user");
    assert(uri.authority().host()=="www.google.com");
    assert(uri.authority().port()==0);
    assert(uri.path()=="/foo/bar.html");
}

void noHost() {
    http::Uri uri("file:///foo/bar.html");
    assert(uri.scheme()=="file");
    assert(uri.authority().user()=="");
    assert(uri.authority().host()=="");
    assert(uri.authority().port()==0);
    assert(uri.path()=="/foo/bar.html");
}

void fragment() {
    http::Uri uri("file:///foo/bar.html?foo=bar#foo");
    assert(uri.scheme()=="file");
    assert(uri.authority().user()=="");
    assert(uri.authority().host()=="");
    assert(uri.authority().port()==0);
    assert(uri.path()=="/foo/bar.html?foo=bar#foo");
}

int main() {
    simple();
    user();
    noPort();
    noHost();
    fragment();

    return 0;
}
