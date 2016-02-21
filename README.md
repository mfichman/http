http
====

Simple HTTP asynchronous library for modern C++. Depends on [coro](https://github.com/mfichman/coro.git). Here's the basic usage:

```
auto coro = coro::start([]{
    try {
        auto res = http::post(
            "https://192.168.1.154/user/login", 
             "{ \"UserId\": \"matt\", \"Password\": \"matt\"} "
        );

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
```


