CFMT
----
Simple and flexible python style formatter.  
Requires at least **c++ 20**.  

### Template string
```
{id} - named group.
{} - anonymous group ( always has unique id ).

{a} {b} {ab} {} - string with 3 named groups and 1 anonymous.
```

Arguments substituted to group in order they appear for the first time.

### Examples:
```c++
auto string = cfmt::format("{}", 123.f); /// std::string { "123" }
auto string = "{}"_cfmt(123.f); /// std::string { "123" }
```

```c++
auto string = cfmt::wformat(L"{}", 123.f); /// std::wstring { L"123" }
```

### String
To use your string class in a formatter it must satisfy **String** concept.

### TODO
- Write custom __optional__ class ( with builtin reference support ). 
    - Fix group/cache.
- Rewrite formatting algorithm.
- Decrease code amount. 
