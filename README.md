# Qt CMake 综合教程

这是一套面向 Qt 6 开发的 CMake 教程与示例仓库，基于 Qt Creator/Qt 生成的标准 `CMakeLists.txt` 写法，扩展到真实工程常见的组织方式：

- 单个 Widgets 应用
- 一个仓库包含多个应用和多个库
- 多项目下面继续嵌套子项目、插件或功能模块
- QML/Qt Quick 模块
- 资源、翻译、安装、部署、VS Code、Qt Creator、CMake Presets、CI 思路

教程正文：[docs/qt-cmake-tutorial.md](docs/qt-cmake-tutorial.md)

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

## 参考来源

本教程的 Qt/CMake 命令说明参考 Qt 6 官方文档与 CMake 官方文档，链接集中列在教程末尾。
