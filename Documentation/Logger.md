## Logger
The Logger class is used to log. It is recommended to use it instead of `std::cout` or `printf`.

```cpp
Aleg::Logger* logger = new Aleg::Logger("Class name"); // You should only create one per class (make it static)
logger->log("Hello, World :)");
logger->warn("WARNING !!!!!!!!!");
logger->error("ERROR");
```

**Output:**

<span style="color: green;">[LOG] (Class name):</span> Hello, World :)
<span style="color: yellow;">[WARN] (Class name):</span> WARNING !!!!!!!!!
<span style="color: red;">[ERROR] (Class name):</span> ERROR (always printed, regardless of debug mode)
