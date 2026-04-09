# WinLineZ

这是一个基于 C++ 语言和 Raylib 编写的对经典游戏 WinLine 的模仿。

试玩这个项目而不是原版的理由：
- 我实现的版本具有高度优化的显示效果和操作手感，平滑且自然的移动，以及令人激动的记分系统。
- 相信我，实际游玩的感受绝对会比下面的截图更加生动！
- 既然已经看到这里了，为什么不试试呢？

下面是游戏规则（相对原版略有修改：
- 在 9x9 的平面方格上，每回合会在空的格子上随机出现 3 个 7 种颜色之一的球，同时占据这个格子。
- 移动小球并形成长度至少为 5 的 横/竖/斜 的连续段，并消除他们！一步消除 $x$ 个球会让你获得 $2^x$ 分。
- 如果平面被填满了，你就输了！反之如果你在此之前得到了 1024 分，你就获胜了！

## 项目结构

```
winline/
├── build.bat              # Windows 一键构建脚本
├── CMakeLists.txt         # CMake 构建配置
├── Makefile               # 传统 Make 构建
├── src/
│   └── main.cpp          # 程序入口
├── game/                  # 游戏逻辑
│   ├── game.cpp/h        # 游戏主逻辑
│   ├── interact.cpp      # 输入处理
│   └── lib/
│       ├── consts.cpp/h  # 基础类型和常量
│       ├── gradient.h    # 渐变模板类
│       ├── map.cpp/h     # 地图和寻路
│       ├── partical.h    # 粒子系统
│       └── render.cpp/h  # 渲染函数
├── raylib/               # 独立的 raylib 5.5
├── bin/                  # 编译输出
└── build/                # CMake 构建产物
```

## 快速开始

### 依赖项

- MinGW-w64 (g++)
- CMake 3.16+ (可选，但推荐)
- raylib 5.5 (已包含在项目中)

### 编译选项

#### 方法 1: 使用 build.bat (推荐)

```bash
# 编译 Release 版本
build.bat

# 编译 Debug 版本
build.bat debug

# 编译并运行
build.bat run

# 清理构建产物
build.bat clean

# 查看帮助
build.bat help
```

#### 方法 2: 使用 CMake

```bash
# 创建并进入构建目录
mkdir build && cd build

# 配置项目
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . --parallel 8

# 运行
..\bin\mylinez.exe
```

或使用 Makefile 快捷命令：

```bash
make cmake        # 使用 CMake 编译 Release
make cmake-debug  # 使用 CMake 编译 Debug
make run-cmake    # 运行 CMake 编译的版本
make clean-all    # 清理所有构建产物
```

#### 方法 3: 使用传统 Make

```bash
make          # 编译 Release 版本
make debug    # 编译 Debug 版本
make run      # 编译并运行
make clean    # 清理编译产物
```

### 运行

编译成功后，可执行文件位于 `bin/mylinez.exe`，直接运行即可。

## 游戏截图

<img width="1002" height="1027" alt="image" src="https://github.com/user-attachments/assets/d8d4257d-9bf5-48c5-86c4-c39339fac283" />

消除特效：
<img width="1002" height="1027" alt="image" src="https://github.com/user-attachments/assets/35ccaafb-f48a-4f4f-aee8-724ae05b68a4" />

哦不我输了 T_T
<img width="1002" height="1027" alt="image" src="https://github.com/user-attachments/assets/ef9cf513-acfa-4904-a29a-23457505ab82" />

## 开发说明

- 项目采用模块化设计，游戏逻辑集中在 `game/` 目录
- 渲染使用 raylib 5.5，已作为独立依赖包含在项目中
- 支持 C++20 标准
- Windows 平台编译使用 UTF-8 输入和 GBK 执行字符集

计划加入的功能：更多的自定义技能，可持久化的计分，等。
