CFMT
----
Simple and flexible python style formatter.  
Requires **c++ 20**.  

### Examples:
```c++
auto string = cfmt::format("{}", 123.f); /// std::string { "123" }
auto string = "{}"_cfmt(123.f); /// std::string { "123" }
```

```c++
auto string = cfmt::wformat(L"{}", 123.f); /// std::wstring { L"123" }
```

### Adapters
Adapters is a thing that provides an interface between **formatter** and **custom string types**.  
Adapters must satisfy the **cfmt::adapter** concept.  
  
Library comes with 1 adapter for **stdlib strings**.