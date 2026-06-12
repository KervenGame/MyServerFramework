# MyServerFramework

一个配套 **MyFramework Unity 客户端框架** 使用的 C++ 游戏服务器框架。

项目地址：

https://github.com/ZHOURUIH/MyServerFramework

配套客户端框架：

https://github.com/ZHOURUIH/MyFramework

---

## 项目定位

**MyServerFramework** 是一个偏传统游戏服务器开发思路的 C++ 服务端框架。

它不是单纯的 Socket Demo，也不是只用于演示网络连接的小项目，而是围绕实际游戏项目开发流程整理出来的一套服务器框架，主要用于配合 Unity 客户端框架 **MyFramework** 使用。

框架重点关注：

* TCP 服务器
* UDP 服务器
* WebSocket 服务器
* HTTP 服务
* 客户端 / 服务器协议生成
* MySQL 数据库访问
* SQLite 配置数据读取
* 配置表代码生成
* 对象池
* 命令系统
* 事件系统
* 角色管理
* 组件系统
* 状态机
* 定时任务
* 日志系统
* Windows / Linux 编译部署

---

## 与 MyFramework 的关系

**MyFramework** 是 Unity 客户端框架。

**MyServerFramework** 是与它配套的 C++ 服务器框架。

两者并不是完全割裂的两个项目，而是围绕同一套游戏开发流程设计的客户端和服务器工程。

主要配套点包括：

* 客户端和服务器使用同一套协议定义
* 协议配置可生成客户端消息代码
* 协议配置可生成服务器消息代码
* 配置表结构可生成客户端读取代码
* 配置表结构可生成服务器读取代码
* 客户端和服务器可以保持消息结构一致
* 客户端 Demo 可连接服务器进行登录、Ping、攻击等基础交互

适合希望研究 **Unity 客户端 + C++ 服务器配套开发流程** 的项目。

---

## 核心特点

### 1. C++ 游戏服务器框架

框架主体使用 C++ 编写，整体代码风格偏传统游戏服务器和引擎开发思路。

主要目标不是追求最少代码，而是让服务器逻辑具备明确的模块划分、生命周期和可维护性。

---

### 2. 多种网络通信方式

框架包含多种网络通信模块：

* TCP Server
* UDP Server
* WebSocket Server
* HTTP Server
* TCP Client
* HTTP Client

对应目录：

```text
MyServerFramework_Frame/Frame/Net
├── TCPServer
├── TCPClient
├── UDPServer
├── WebSocketServer
└── Http
```

其中 TCP 通信用于常规客户端长连接。

UDP 可用于实时性更高的通信场景。

WebSocket 可用于 Web 平台或需要 WebSocket 接入的客户端。

HTTP 可用于短连接请求、后台接口或辅助服务。

---

### 3. 客户端 / 服务器协议代码生成

仓库中包含协议定义文件：

```text
#自动生成代码/PacketCS.txt
#自动生成代码/PacketSC.txt
#自动生成代码/PacketStruct.txt
```

协议文件中定义客户端到服务器、服务器到客户端以及公共结构体。

例如：

```text
CSServerCheckPing
CSLogin
CSAttack

SCServerCheckPing
SCCharacterFullGameData
SCGetItemTip
SCAttack

NetStructItemInfo
```

协议生成工具可以根据这些配置生成：

* 客户端消息代码
* 服务器消息代码
* 消息 ID 定义
* 消息注册代码
* 序列化代码
* 反序列化代码

这样可以减少客户端和服务器协议不一致的问题。

---

### 4. MySQL 数据库支持

框架内置 MySQL 访问相关封装。

相关目录：

```text
MyServerFramework_Frame/Frame/DataBase/MySQL
```

包含：

* MySQLManager
* MySQLTable
* MySQLTableT
* MySQLData
* MySQLCacheData
* MySQLCacheTable
* PreStatementRead
* PreStatementWrite
* QueryScope
* QueryParam

仓库中也包含 MySQL 表结构定义示例：

```text
#文档/micro_legend.sql
```

当前 Demo 中包含账号表 `account`，用于基础登录测试。

---

### 5. SQLite 配置数据支持

框架内置 SQLite 读取支持。

相关目录：

```text
MyServerFramework_Frame/Frame/DataBase/SQLite
```

包含：

* SQLiteManager
* SQLiteTable
* SQLiteTableBase
* SQLiteData
* SQLiteDataReader
* SQLiteTableParam

SQLite 主要用于读取配置类数据，配合配置表生成代码使用。

---

### 6. 配置表系统

框架内置 Excel / 配置表管理模块。

相关目录：

```text
MyServerFramework_Frame/Frame/DataBase/Excel
```

包含：

* ExcelManager
* ExcelTable
* ExcelTableBase
* ExcelData

项目层对应注册代码：

```text
MyServerFramework/Game/DataBase/Excel/ExcelRegister.cpp
MyServerFramework/Game/DataBase/Excel/ExcelRegister.h
```

配置表系统用于和客户端保持一致的数据结构，适合生成客户端和服务器两端的配置读取代码。

---

### 7. 命令系统

框架包含命令系统，用于封装具有完整逻辑意义的操作。

相关目录：

```text
MyServerFramework_Frame/Frame/CommandSystem
```

包含：

* CommandSystem
* CommandReceiver
* GameCommand
* DelayCommand
* DelayCallSystem
* CmdHttpGet
* CmdHttpPost
* CmdNetServerLogoutAccount
* CmdNetServerReceiveUDPPacket
* CmdCharacterManagerDestroyCharacter

命令系统可以用于：

* 模块解耦
* 延迟执行
* 网络事件处理
* 角色销毁
* HTTP 请求
* 主流程调度

---

### 8. 事件系统

框架包含事件系统。

相关目录：

```text
MyServerFramework_Frame/Frame/EventSystem
```

包含：

* EventSystem
* GameEvent
* EventInfo
* EventProcess
* IEventListener

事件系统用于模块之间的通知和解耦，避免所有逻辑直接互相调用。

---

### 9. 对象池与内存控制

框架包含大量对象池和临时内存管理工具。

相关目录：

```text
MyServerFramework_Frame/Frame/ClassPool
```

包含：

* ClassPool
* ClassTypePool
* ClassKeyPool
* ArrayPool
* VectorPool
* Thread Pool
* Packet Pool
* Command Pool
* Character Pool
* State Pool

框架也包含内存追踪相关模块：

```text
MyServerFramework_Frame/Frame/Memory
```

这些设计主要用于减少频繁 new/delete 带来的开销，并让服务器长期运行时的内存行为更可控。

---

### 10. 角色与组件系统

框架包含基础角色、移动对象和组件系统。

相关目录：

```text
MyServerFramework_Frame/Frame/Character
MyServerFramework_Frame/Frame/Component
```

项目层角色代码位于：

```text
MyServerFramework/Game/Character
```

当前 Demo 中包含：

* CharacterGame
* CharacterPlayer
* CharacterGameData
* CharacterPlayerData
* CharacterRegister

适合作为游戏角色、玩家对象和服务器实体系统的基础。

---

### 11. 状态机与 AI 基础

框架包含状态机和部分 AI 相关基础组件。

相关目录：

```text
MyServerFramework_Frame/Frame/StateManager
MyServerFramework_Frame/Frame/Character/Component
```

包括：

* COMCharacterStateMachine
* COMMonsterDecisionTree
* StateManager
* State
* StateParam

可用于角色状态、怪物行为、AI 决策等服务器逻辑。

---

### 12. 定时任务与时间点系统

框架包含时间任务系统和时间点系统。

相关目录：

```text
MyServerFramework_Frame/Frame/TimeTaskSystem
MyServerFramework_Frame/Frame/TimePointSystem
```

可用于：

* 延迟任务
* 定时任务
* 周期任务
* 游戏内定时刷新
* 活动时间判断
* 服务器时间点触发

---

### 13. 日志系统

框架包含日志系统。

相关目录：

```text
MyServerFramework_Frame/Frame/Log
```

包含：

* LogSystem
* LogInfo
* LogFileInfo
* GameLogWrap

用于服务器运行日志、错误定位和调试输出。

---

### 14. 单元测试

框架包含大量单元测试代码。

相关目录：

```text
MyServerFramework_Frame/Frame/UnitTest
```

覆盖内容包括：

* 字符串工具
* 数学工具
* 文件工具
* 二进制工具
* 容器扩展
* 对象池
* 命令系统
* 事件系统
* 序列化
* 网络消息
* MySQL 缓存表
* 双缓冲
* 状态机
* 时间工具
* 内存追踪

单元测试用于保证基础工具函数和核心模块长期稳定。

---

## 功能概览

| 模块               | 状态 |
| ---------------- | -- |
| TCP Server       | 支持 |
| UDP Server       | 支持 |
| WebSocket Server | 支持 |
| HTTP Server      | 支持 |
| TCP Client       | 支持 |
| HTTP Client      | 支持 |
| 协议生成             | 支持 |
| 客户端 / 服务器消息结构生成  | 支持 |
| MySQL            | 支持 |
| SQLite           | 支持 |
| 配置表读取            | 支持 |
| 对象池              | 支持 |
| 命令系统             | 支持 |
| 事件系统             | 支持 |
| 角色管理             | 支持 |
| 组件系统             | 支持 |
| 状态机              | 支持 |
| 定时任务             | 支持 |
| 日志系统             | 支持 |
| 单元测试             | 支持 |
| Visual Studio 工程 | 支持 |
| CMake            | 支持 |
| Linux 编译部署       | 支持 |

---

## 目录结构

```text
MyServerFramework
├── MyServerFramework
│   └── Game
│       ├── Character
│       ├── Common
│       ├── Component
│       ├── Config
│       ├── DataBase
│       ├── Game
│       ├── Socket
│       └── Utility
│
├── MyServerFramework_Frame
│   └── Frame
│       ├── Character
│       ├── ClassPool
│       ├── CommandSystem
│       ├── Common
│       ├── Component
│       ├── Config
│       ├── DataBase
│       ├── Dependency
│       ├── EventSystem
│       ├── Factory
│       ├── InputSystem
│       ├── KeyframeManager
│       ├── Log
│       ├── Math
│       ├── Memory
│       ├── Net
│       ├── Param
│       ├── Serialize
│       ├── ServerFramework
│       ├── StateManager
│       ├── Thread
│       ├── Ticker
│       ├── TimePointSystem
│       ├── TimeTaskSystem
│       ├── UnitTest
│       └── Utility
│
├── ToolProject
│   ├── GenerateMakeLists
│   └── GenerateVSProject
│
├── #自动生成代码
│   ├── PacketCS.txt
│   ├── PacketSC.txt
│   ├── PacketStruct.txt
│   ├── MySQL.txt
│   └── 自动生成代码.exe
│
├── #文档
│   ├── 环境安装说明.txt
│   ├── linux编译.txt
│   ├── linux命令.txt
│   ├── linux错误解决日志.txt
│   └── micro_legend.sql
│
├── lib
├── DLL
├── media
├── CMakeLists.txt
└── MyServerFramework.sln
```

---

## 快速开始

### Windows

1. 安装 Visual Studio。

2. 安装 OpenSSL。

可从以下地址下载安装 Win64 OpenSSL：

```text
https://slproweb.com/products/Win32OpenSSL.html
```

安装后添加环境变量：

```text
OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
```

3. 安装 MySQL 8.0。

可从以下地址下载安装 MySQL Installer：

```text
https://dev.mysql.com/downloads/installer/
```

安装后添加环境变量：

```text
MYSQL_ROOT=C:\Program Files\MySQL\MySQL Server 8.0
```

4. 使用 Visual Studio 打开：

```text
MyServerFramework.sln
```

5. 编译并运行服务器。

6. 启动配套 Unity 客户端工程：

```text
https://github.com/ZHOURUIH/MyFramework
```

客户端启动后即可连接服务器进行基础测试。

---

## Linux 编译

仓库中包含 CMakeLists 文件，可部署到 Linux 编译。

根目录：

```text
CMakeLists.txt
```

包含两个子工程：

```text
MyServerFramework_Frame
MyServerFramework
```

Linux 环境需要安装相关依赖，例如：

```text
mysql
libevent
openssl
libssl
zlib
cmake
build-essential
```

常见依赖安装示例：

```bash
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y cmake
sudo apt-get install -y zlib1g-dev
sudo apt-get install -y libssl-dev
sudo apt-get install -y mysql-server
sudo apt-get install -y libmysqlclient-dev
```

编译示例：

```bash
mkdir build
cd build
cmake ..
make
```

如果修改了工程中的文件结构，可以使用仓库提供的工具更新 CMakeLists。

---

## 自动生成 CMakeLists

仓库中包含自动生成 CMakeLists 的工具：

```text
GenerateMakeLists.exe
GenerateMakeLists_Config.txt
```

用于在 Windows 上修改完代码后，自动更新 CMakeLists。

适合以下情况：

* 新增 cpp / h 文件
* 删除 cpp / h 文件
* 新增目录
* 删除目录
* 调整工程结构

这样可以减少手动维护 Linux 编译文件的成本。

---

## 自动生成 Visual Studio 工程

仓库中包含 Visual Studio 工程生成工具：

```text
#自动生成代码/生成VS工程.exe
#自动生成代码/GenerateVSProject_Config.txt
```

用于根据配置自动更新 Visual Studio 工程文件。

适合在项目文件增删较多时保持工程文件同步。

---

## 协议生成说明

协议定义位于：

```text
#自动生成代码/PacketCS.txt
#自动生成代码/PacketSC.txt
#自动生成代码/PacketStruct.txt
```

其中：

```text
PacketCS.txt
```

定义客户端发送到服务器的消息。

```text
PacketSC.txt
```

定义服务器发送到客户端的消息。

```text
PacketStruct.txt
```

定义客户端和服务器共用的网络结构体。

生成工具：

```text
#自动生成代码/自动生成代码.exe
```

生成后会更新服务器和客户端对应的协议代码。

---

## MySQL 生成说明

MySQL 表结构定义位于：

```text
#自动生成代码/MySQL.txt
```

示例：

```text
Account:account
{
    string Account;
    string Password;
    llong CharacterGUID;
index:
    Account
}
```

说明：

* `Account` 表示生成的 C++ 类名。
* `account` 表示数据库中的表名。
* 每个表默认包含一个 `llong ID` 字段。
* `index` 用于声明索引字段。

生成后会生成对应 MySQL 数据类和注册代码。

---

## 当前 Demo 包含的基础消息

客户端到服务器：

```text
CSCheckPacketVersion
CSServerCheckPing
CSAttack
CSLogin
```

服务器到客户端：

```text
SCCheckPacketVersion
SCServerCheckPing
SCCharacterFullGameData
SCGetItemTip
SCAttack
```

公共结构：

```text
NetStructItemInfo
```

这些消息主要用于演示：

* 协议版本检查
* Ping / 延迟检测
* 登录
* 攻击
* 角色数据返回
* 物品奖励提示

---

## 当前 Demo 包含的数据库表

仓库文档中包含基础 MySQL 表结构：

```text
#文档/micro_legend.sql
```

当前示例包含账号表：

```text
account
```

字段包括：

```text
ID
Account
Password
CharacterGUID
```

用于基础账号登录测试。

---

## 设计理念

### 1. 强控制

服务器框架倾向于显式生命周期和显式调度。

系统初始化、更新、销毁都由框架统一管理，减少隐藏流程。

### 2. 工程化

框架不仅提供运行时模块，也提供协议生成、MySQL 生成、工程生成、CMakeLists 生成等工具。

目标是减少长期项目中的重复劳动。

### 3. 客户端 / 服务器一致性

服务器框架与 Unity 客户端框架配套设计。

协议、配置、消息结构尽量通过工具生成，减少两端手写导致的不一致。

### 4. 长期维护

框架中包含对象池、日志系统、单元测试、工程检查思路和大量基础工具，目标是支持长期运行和持续迭代。

---

## 与 MyFramework 客户端配套使用

推荐同时查看客户端仓库：

```text
https://github.com/ZHOURUIH/MyFramework
```

客户端框架包含：

* HybridCLR 热更新
* Obfuz 混淆
* UI 自动生成
* 配置表工具链
* 协议生成
* 资源管理
* 对象池
* 特效系统
* 多语言
* 工程检查工具

服务器仓库和客户端仓库配合后，可以形成一套完整的开发流程：

```text
协议配置
    ↓
生成客户端消息代码
    ↓
生成服务器消息代码
    ↓
客户端发送请求
    ↓
服务器处理逻辑
    ↓
服务器返回消息
    ↓
客户端刷新表现
```

配置表流程：

```text
CSV 配置表
    ↓
配置表工具链检查
    ↓
生成客户端配置代码
    ↓
生成服务器配置代码
    ↓
客户端和服务器共用同一套配置结构
```

---

## 文档

仓库中包含部分环境和部署文档：

```text
#文档/环境安装说明.txt
#文档/linux编译.txt
#文档/linux命令.txt
#文档/linux错误解决日志.txt
#文档/micro_legend.sql
```

---

## 后续计划

后续会继续补充：

* 更完整的服务器启动说明
* 客户端连接服务器说明
* 协议生成工具说明
* MySQL 生成工具说明
* Linux 部署文档
* 网络 benchmark 测试
* Demo 业务流程说明
* 与 MyFramework 客户端的联调教程

---

## License

MIT
