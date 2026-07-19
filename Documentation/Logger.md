# Alegengine docs
## Logger

The Logger class is used to log. It is recommended to use it instead of std::cout or printf.
```
Aleg::Logger* logger = new Aleg::Logger("Class name"); // You should only create one per class (make it static)
logger->log("Hello, World :)"); // Prints : <span style="color: green;">[LOG] (Class name) : </span> Hello, World :)
logger->warn("WARNING !!!!!!!!!"); // Prints : <span style="color: yellow;">[WARN] (Class name) : </span> WARNING !!!!!!!!!
logger->error("ERROR"); // Prints (regardless of test flag or not) : <span style="color: red;">[ERROR] (Class name) : </span> ERROR
```
