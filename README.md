# Qt CMake 综合课程

这是一套专注 Qt 6 + CMake 开发的系统课程与示例仓库，基于 Qt Creator/Qt 生成的标准 `CMakeLists.txt` 写法，扩展到真实 Qt 工程常见的组织方式：

- 单个 Widgets 应用
- Qt 项目必须掌握的 CMake 语法、作用域、target 依赖逐项拆解
- 单应用也保留根 `CMakeLists.txt`，应用 target 放到子目录，方便未来无痛扩展
- 一个仓库包含多个应用和多个库
- 多项目下面继续嵌套子项目、插件或功能模块
- QML/Qt Quick 模块
- 资源、翻译、安装、部署、VS Code、Qt Creator、CMake Presets、CI 思路
- Qt 项目常用实战技巧与现代 CMake 标杆做法

教程正文：[docs/qt-cmake-tutorial.md](docs/qt-cmake-tutorial.md)

## 学习目标

这套课程按“从 0 基础到能维护大型 Qt+CMake 工程”的路径组织：先跑通 Qt Widgets/QML 示例，再看懂必要的 CMake 语法，最后掌握多应用、多库、资源、部署、测试和团队规范。

## 快速运行示例

示例默认不参与根工程构建，方便读者按需启用：

```powershell
cmake -S . -B build -DSTUDY_BUILD_WIDGETS_BASIC=ON
cmake --build build
```

多项目示例：

```powershell
cmake -S . -B build-multi -DSTUDY_BUILD_MULTI_PROJECT=ON
cmake --build build-multi
```

QML 示例：

```powershell
cmake -S . -B build-qml -DSTUDY_BUILD_QML_APP=ON
cmake --build build-qml
```

> 需要 Qt 6.5 或更新版本，并确保 CMake 能找到 Qt，例如设置 `CMAKE_PREFIX_PATH` 指向 Qt 安装目录。

## 示例目录

```text
examples/
  01_widgets_basic/        # Qt Widgets 单应用
  02_multi_project/        # 一个仓库多个 app/lib，并包含嵌套子项目
  03_qml_app/              # Qt Quick/QML 应用与 QML 模块
```

## 推荐标准

即使当前只有一个 Qt 应用，也推荐使用“根工程 + 子项目”的标准结构：

```text
MyQtProject/
  CMakeLists.txt
  apps/
    MyApp/
      CMakeLists.txt
      main.cpp
      mainwindow.h
      mainwindow.cpp
      mainwindow.ui
```

根 `CMakeLists.txt` 只负责项目级配置和 `add_subdirectory()`，应用自己的 `CMakeLists.txt` 只负责创建和链接自己的 target。这样以后添加第二个应用、公共库、插件、测试工程时，不需要重写已有结构。

## 参考来源

本教程的 Qt/CMake 命令说明参考 Qt 6 官方文档与 CMake 官方文档，链接集中列在教程末尾。
