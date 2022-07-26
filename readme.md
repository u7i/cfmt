CFMT
----
Simple and flexible python style formatter.  
Requires at least **c++ 20**.  

### Examples:
```c++
auto string = cfmt::format("{}", 123.f); /// std::string { "123" }
auto string = "{}"_cfmt(123.f); /// std::string { "123" }
```

```c++
auto string = cfmt::wformat(L"{}", 123.f); /// std::wstring { L"123" }
```

### String
To use your string class in a formatter it must satisfy **indexable** concept.
