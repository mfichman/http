<h1 align="center">C++ HTTP</h1>

<div align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=c%2B%2B&logoColor=A7C7E7&color=303030"/>
  <img src="https://img.shields.io/github/stars/mfichman/http?color=A7C7E7&style=flat-square&labelColor=303030"/>
</div>

<p align="center">
    Simple <b>HTTP</b> asynchronous library for <b>modern C++</b>.<br>
    <sup>Depends on <a href="https://github.com/mfichman/coro">coro</a></sup>
</p>

## 🔧 Usage

Here's an example of library usage:

``` c++
auto coro = coro::start([]{

    try {
    
        auto res = http::post(
            "https://192.168.1.154/user/login",  // Post request URL
             "{ \"UserId\": \"matt\", \"Password\": \"matt\"}"  // Post request data
        );

        auto sessionId = res.cookie("SessionId");  // Getting response "SessionID" cookie
        std::cout << sessionId.value() << std::endl;  // Outputing "SessionID" cookie value
        std::cout << sessionId.httpOnly() << std::endl;  // Outputing "SessionID" httpOnly property
        std::cout << sessionId.secure() << std::endl;  // Outputing "SessionID" secure property
        std::cout << sessionId.path() << std::endl;  // Outputing "SessionID" path
        
    } catch (coro::SystemError const& ex) {
    
        std::cerr << ex.what() << std::endl;  // Catching any errors
        
    }
    
});

coro::run();  // Run
```


