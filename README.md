# MySQL Connector 8.0.2.8
MySQL Connector in C++ with Demo Functions
 
Compiler: Visual Studio 2019
Build Mode: Release x86
Architecture: x86

## Set-up compile environment:
Needs:
- Visual Studio 2019+ with C++ Compiler
- Ready MySQL Server to connect/testing
- MySQL Connector 8.0.2.8 dependencies | Windows (x86, 32-bit), ZIP Archive | (https://dev.mysql.com/downloads/connector/cpp/8.0.html)

VS Project Configurations:
1. C/C++ → General → Additional Include Directories → Include "jdbc" Folder from "MySQL Connector 8.0.2.8 dependencies". *[Image](https://media.discordapp.net/attachments/652412528793485313/963740614904664104/unknown.png)*
2. C/C++ → General → Preprocessor Definitions → Add "STATIC_CONCPP;". *[Image](https://media.discordapp.net/attachments/652412528793485313/963740413812957214/unknown.png)*
3. C/C++ → Code Generation → Runtime Library → Set to "Multi-threaded DLL (/MD)".
4. C/C++ → Command Line → Additional Options → Add "/utf-8".
5. Linker → General → Additional Library Directories → Include "vs14" Folder from "MySQL Connector 8.0.2.8 dependencies". *[Image](https://media.discordapp.net/attachments/652412528793485313/963740147919241247/unknown.png)*
6. Linker → Input → Additional Dependencies → Add "mysqlcppconn-static.lib". *[Image](https://media.discordapp.net/attachments/652412528793485313/963739744775335976/Capture.PNG)*

Executables:
- Executable file requires "libcrypto-1_1.dll", "libssl-1_1.dll" files to be in the same directory to works. 

(Can be found in "MySQL Connector 8.0.2.8 dependencies" and "Essentials" folder)

Notes:
- Need update AddTable, InsertData function..