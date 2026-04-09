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

## 游戏截图

<img width="1002" height="1027" alt="image" src="https://github.com/user-attachments/assets/d8d4257d-9bf5-48c5-86c4-c39339fac283" />

消除特效：
<img width="1002" height="1021" alt="image" src="https://github.com/user-attachments/assets/0f9cda13-3e56-4a40-92a1-e46f3c558895" />

哦不我输了 T_T
<img width="1002" height="1021" alt="image" src="https://github.com/user-attachments/assets/e8cf6d01-a3fd-4387-adf6-de995f491d3f" />

## 开发说明

- 运行 `./build.bat` 来编译，编译产物位于 `/bin/` 目录内。
- 渲染使用 raylib 5.5，已作为独立依赖包含在项目中
- 支持 C++20 标准
- Windows 平台编译使用 UTF-8 输入和 GBK 执行字符集