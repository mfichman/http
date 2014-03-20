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

namespace http {

class Cookie {
public:
    Cookie(std::string const& text);
    Cookie() : httpOnly_(false), secure_(false) {};

    std::string const& name() const { return name_; }
    std::string const& value() const { return value_; }
    std::string const& path() const { return path_; }
    bool httpOnly() const { return httpOnly_; }
    bool secure() const { return secure_; }

    void nameIs(std::string const& name) { name_ = name; }
    void valueIs(std::string const& value) { value_ = value; }
    void pathIs(std::string const& path) { path_ = path; }
    void httpOnlyIs(bool httpOnly) { httpOnly_ = httpOnly; }
    void secureIs(bool secure) { secure_ = secure; }

private:
    std::string name_;
    std::string value_;
    std::string path_;
    bool httpOnly_;
    bool secure_;
};

class Cookies {
public:
    typedef std::map<std::string,Cookie> Map;

    Cookie const cookie(std::string const& name) const;
    Map::const_iterator begin() const { return cookie_.begin(); }
    Map::const_iterator end() const { return cookie_.end(); }

    void cookieIs(Cookie const& cookie);

    static std::string const HOST;
    static std::string const CONTENT_LENGTH;
    static std::string const ACCEPT_ENCODING;
    static std::string const CONNECTION;
private:
    Map cookie_;
};

}
