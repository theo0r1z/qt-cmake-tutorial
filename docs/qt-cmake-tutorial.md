# Qt CMake 综合教程：从单窗口到多项目工程

本文面向已经会写一点 Qt/C++，但想把 CMake 工程组织做扎实的开发者。目标不是背命令，而是建立一套可落地的工程模板：小项目能快，大项目能拆，多个应用、多个库、嵌套子项目、QML、资源、安装部署都能自然扩展。

![VS Code 中的 Qt CMake 工程](assets/screenshots/vscode-overview.png)

## 1. 从标准 Qt CMakeLists 开始

Qt Creator 创建 Widgets 工程时常见的标准写法大致如下：

```cmake
cmake_minimum_required(VERSION 3.19)
project(untitled30 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(untitled30
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(untitled30
    PRIVATE
        Qt::Core
        Qt::Widgets
)
```

这份文件已经体现了现代 Qt CMake 的几个关键点：

- `find_package(Qt6 ... COMPONENTS ...)`：声明需要哪些 Qt 模块。
- `qt_standard_project_setup()`：启用 Qt 常用默认行为，例如 AUTOMOC、AUTOUIC、安装目录变量等。
- `qt_add_executable()`：创建 Qt 应用目标，并处理 Qt target finalization。
- `target_link_libraries()`：把依赖绑定到 target，而不是全局设置 include/link path。
- `install()` 与 `qt_generate_deploy_app_script()`：把安装和部署纳入 CMake。

推荐你把“工程等于 targets 的集合”作为主线理解 CMake。目录只是组织方式，真正决定编译、链接、安装、IDE 展示的是 target。

## 2. 推荐目录结构

小工程可以平铺，但中大型 Qt 项目最好从一开始就分层：

```text
StudyCMake/
  CMakeLists.txt
  CMakePresets.json
  apps/
    DesignerTool/
    ImageViewer/
  libs/
    CoreKit/
    UiKit/
  plugins/
  qml/
  resources/
  tests/
  docs/
```

本仓库示例使用：

```text
examples/
  01_widgets_basic/
  02_multi_project/
    apps/
      DesignerTool/
        plugins/ColorPanel/
      ImageViewer/
    libs/
      CoreKit/
      UiKit/
  03_qml_app/
```

![多项目目录结构](assets/screenshots/project-tree.png)

目录规划建议：

- `apps/` 放最终可执行程序，每个 app 自己有 `CMakeLists.txt`。
- `libs/` 放可复用 C++/Qt 库，不直接依赖具体 app。
- `plugins/` 放插件或插件式模块，能独立成 target。
- `resources/` 放 `.qrc`、图片、样式、翻译原始文件。
- `tests/` 放测试 target，避免混在 app 目录里。
- `docs/` 放教程、架构图、发布说明。

## 3. 根 CMakeLists 的职责

根 `CMakeLists.txt` 不应该塞满源文件。它的职责是设定项目、查找公共依赖、引入子目录：

```cmake
cmake_minimum_required(VERSION 3.19)

project(StudyCMake
    VERSION 1.0.0
    LANGUAGES CXX
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup(REQUIRES 6.5 SUPPORTS_UP_TO 6.11)

add_subdirectory(libs/CoreKit)
add_subdirectory(libs/UiKit)
add_subdirectory(apps/DesignerTool)
add_subdirectory(apps/ImageViewer)
```

本仓库根目录多了一层 `option()`，目的是让教程示例按需构建：

```cmake
option(STUDY_BUILD_WIDGETS_BASIC "Build the basic Qt Widgets example" OFF)

if(STUDY_BUILD_WIDGETS_BASIC)
    add_subdirectory(examples/01_widgets_basic)
endif()
```

真实产品仓库通常可以默认构建主 app，把 examples、tools、tests 设为可选。

## 4. 单 Widgets 应用

见 [examples/01_widgets_basic/CMakeLists.txt](../examples/01_widgets_basic/CMakeLists.txt)。

```cmake
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_add_executable(WidgetsBasic
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(WidgetsBasic
    PRIVATE
        Qt::Core
        Qt::Widgets
)
```

`mainwindow.ui` 能自动生成 `ui_mainwindow.h`，原因是 `qt_standard_project_setup()` 启用了 `CMAKE_AUTOUIC`。如果你的 target 在调用 `qt_standard_project_setup()` 之前创建，自动处理可能不会作用到它。

常见文件对应关系：

- `.h` 中有 `Q_OBJECT`：需要 MOC。
- `.ui`：需要 UIC 生成界面头文件。
- `.qrc`：需要 RCC 打包资源。
- `.cpp`：正常 C++ 编译单元。

## 5. 多项目：多个 app + 多个 lib

多项目工程的核心是“库先定义，应用后链接”：

```text
libs/CoreKit  -> Study::CoreKit
libs/UiKit    -> Study::UiKit, links Study::CoreKit
apps/DesignerTool -> links Study::UiKit
apps/ImageViewer  -> links Study::CoreKit
```

![Target 依赖关系](assets/screenshots/target-graph.png)

库 target 示例：

```cmake
qt_add_library(CoreKit STATIC
    include/CoreKit/projectinfo.h
    src/projectinfo.cpp
)

target_include_directories(CoreKit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CoreKit
    PUBLIC
        Qt::Core
)

add_library(Study::CoreKit ALIAS CoreKit)
```

这里 `PUBLIC` 和 `PRIVATE` 很重要：

- `PRIVATE`：只有当前 target 自己需要。
- `PUBLIC`：当前 target 需要，链接它的人也需要。
- `INTERFACE`：当前 target 自己不编译使用，只传递给消费者。

如果 `CoreKit` 的公开头文件包含了 `QString`，那么 `Qt::Core` 应该是 `PUBLIC`，这样 app 链接 `Study::CoreKit` 后能自动拿到 Qt Core 的 include 和 link 信息。

应用 target 示例：

```cmake
qt_add_executable(DesignerTool
    WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(DesignerTool
    PRIVATE
        Qt::Widgets
        Study::UiKit
)
```

建议为内部库增加命名空间别名，例如 `Study::CoreKit`。这样 CMake 报错会更明确，也能和外部包风格保持一致。

## 6. 多项目下面再嵌子项目

有些 app 内部还会有插件、工具面板、协议模块、编辑器扩展等。可以继续嵌套：

```text
apps/DesignerTool/
  CMakeLists.txt
  main.cpp
  plugins/
    ColorPanel/
      CMakeLists.txt
      colorpanel.h
      colorpanel.cpp
```

父级 app：

```cmake
add_subdirectory(plugins/ColorPanel)
```

子项目：

```cmake
qt_add_library(ColorPanelPlugin STATIC
    colorpanel.cpp
    colorpanel.h
)

target_link_libraries(ColorPanelPlugin
    PUBLIC
        Qt::Widgets
)

add_library(Study::ColorPanelPlugin ALIAS ColorPanelPlugin)
```

嵌套子项目适合这几类情况：

- 只服务于某个 app 的复杂模块。
- 将来可能拆成动态插件。
- 模块需要自己的资源、UI、测试或第三方依赖。
- 团队中由不同人维护，目录边界能降低冲突。

如果只是两个 `.cpp`，不要过度拆 target。先用 `target_sources()` 加到 app 里即可。

## 7. 什么时候用 target_sources

当 target 已经在父目录创建，但源文件分散在子目录时，可以用 `target_sources()`：

```cmake
target_sources(DesignerTool
    PRIVATE
        panels/objectinspector.cpp
        panels/objectinspector.h
)
```

适用场景：

- 子目录只是源码分类，不需要独立链接。
- 不希望 IDE 里出现过多 target。
- 模块没有独立复用价值。

不适合：

- 子模块要被多个 app 复用。
- 子模块有独立依赖或测试。
- 子模块需要安装、导出或插件化。

## 8. QML/Qt Quick 工程

见 [examples/03_qml_app/CMakeLists.txt](../examples/03_qml_app/CMakeLists.txt)。

```cmake
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(QmlDashboard
    main.cpp
)

qt_add_qml_module(QmlDashboard
    URI Study.Dashboard
    VERSION 1.0
    QML_FILES
        Main.qml
        components/MetricCard.qml
)
```

`qt_add_qml_module()` 是 Qt 6 推荐的 QML 模块定义方式。它会处理 QML 文件、资源、类型注册、qmldir 以及相关工具 target。

QML 模块命名建议：

- URI 使用反向域名或产品命名空间，例如 `Company.Product.Controls`。
- 文件目录尽量和模块边界一致。
- 公共 QML 控件独立成模块，app 只 import。
- C++ 类型注册和 QML 文件放在同一个 backing target 时最简单。

部署 QML 应用时使用：

```cmake
qt_generate_deploy_qml_app_script(
    TARGET QmlDashboard
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
```

## 9. 资源、图标、翻译

Qt 6 可以直接在 target 中加入 `.qrc`：

```cmake
qt_add_executable(MyApp
    main.cpp
    assets.qrc
)
```

也可以用更显式的资源 API：

```cmake
qt_add_resources(MyApp "app_assets"
    PREFIX "/"
    FILES
        images/logo.png
        styles/app.qss
)
```

翻译建议放在根 setup 或专门模块中统一管理：

```cmake
qt_standard_project_setup(
    REQUIRES 6.5
    I18N_SOURCE_LANGUAGE en
    I18N_TRANSLATED_LANGUAGES zh_CN ja_JP
)
```

如果是多个 app，共享翻译和每个 app 的翻译要分开命名，避免生成文件互相覆盖。

## 10. 安装与部署

Qt 应用不能只复制 `.exe`。Windows 还需要 Qt DLL、platform plugins、imageformats、styles、QML imports 等。标准 Widgets 应用可用：

```cmake
install(TARGETS MyApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MyApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
```

命令行：

```powershell
cmake --install build --prefix package
```

多 app 时可以为每个 app 生成部署脚本，或在顶层集中处理。简单项目按 app 各自写，复杂产品再抽函数。

## 11. VS Code 配置

推荐安装：

- CMake Tools
- C/C++
- Qt tools 扩展按需安装

使用 `CMakePresets.json` 后，VS Code 可以直接选择 preset：

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "multi-project",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/out/build/multi-project",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "STUDY_BUILD_MULTI_PROJECT": "ON"
      }
    }
  ]
}
```

![VS Code 选择 CMake Preset](assets/screenshots/vscode-presets.png)

如果 CMake 找不到 Qt，常见解决方式：

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/Qt/6.7.3/msvc2019_64
```

或写入 `CMakeUserPresets.json`，这个文件不提交到 Git：

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "my-qt",
      "inherits": "multi-project",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "C:/Qt/6.7.3/msvc2019_64"
      }
    }
  ]
}
```

## 12. Qt Creator 配置

Qt Creator 对 Qt CMake 支持很完整。建议：

- 一个 Kit 对应一个 Qt 版本、编译器和架构。
- 使用 shadow build，不要把构建产物放进源码目录。
- Debug/Release 分开构建目录。
- 多 app 工程用 target selector 选择启动目标。
- 改了 `find_package()`、target 名、Qt 模块后重新运行 CMake 配置。

Qt Creator 生成的 `CMakeLists.txt.user` 是本机配置，不应提交。

## 13. 常见工程模式

### 单 app

```text
src/
  main.cpp
  mainwindow.*
  CMakeLists.txt
```

适合工具、小 demo、一次性应用。

### app + 公共库

```text
libs/Core/
apps/MainApp/
```

适合业务开始增长，但最终只有一个应用。

### 多 app + 多库

```text
apps/Admin/
apps/Client/
apps/Launcher/
libs/Domain/
libs/Ui/
libs/Network/
```

适合同一产品线多个入口，共用底层能力。

### app 下嵌 feature/plugin

```text
apps/Editor/plugins/Timeline/
apps/Editor/plugins/Inspector/
```

适合编辑器、IDE、设计器、工业软件、内部平台。

### 超级工程

```text
third_party/
tools/
examples/
tests/
```

适合 SDK、框架、组件库。注意默认构建项要克制，避免读者或 CI 被大量示例拖慢。

## 14. 常见坑位

- `Q_OBJECT` 相关链接错误：确认启用了 AUTOMOC，头文件在 target 源文件列表中。
- `.ui` 找不到 `ui_xxx.h`：确认启用了 AUTOUIC，`.ui` 在 target 源文件列表中。
- `Qt6Config.cmake` 找不到：设置 `CMAKE_PREFIX_PATH` 或使用 Qt Creator Kit。
- `target_link_libraries` 用了裸库路径：优先使用 `Qt::Widgets`、`Study::CoreKit` 这种 imported/alias target。
- include 目录全局污染：少用 `include_directories()`，改用 `target_include_directories()`。
- 多 app 安装互相覆盖：给资源、插件、配置文件安排清晰的安装目标目录。
- Debug/Release 混用：Windows 上尤其要避免不同配置 DLL 和 exe 混在一起。
- QML import 运行时失败：使用 `qt_add_qml_module()`，部署时使用 QML 部署脚本。
- 子目录变量互相影响：少依赖普通变量跨目录传递，优先让 target 传递属性。

## 15. 可复用模板

### 静态库模板

```cmake
qt_add_library(MyLib STATIC
    include/MyLib/foo.h
    src/foo.cpp
)

target_include_directories(MyLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MyLib
    PUBLIC
        Qt::Core
)

add_library(Project::MyLib ALIAS MyLib)
```

### Widgets app 模板

```cmake
qt_add_executable(MyApp
    WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(MyApp
    PRIVATE
        Qt::Widgets
        Project::MyLib
)
```

### QML app 模板

```cmake
qt_add_executable(MyQuickApp main.cpp)

qt_add_qml_module(MyQuickApp
    URI Project.App
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(MyQuickApp
    PRIVATE
        Qt::Quick
)
```

### 可选子工程模板

```cmake
option(BUILD_TOOLS "Build developer tools" OFF)

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()
```

## 16. 发布到 GitHub 的建议

提交内容建议包含：

- `README.md`：入口、环境要求、快速运行命令。
- `docs/qt-cmake-tutorial.md`：完整教程。
- `examples/`：能编译的最小示例。
- `CMakePresets.json`：共享构建入口。
- `.gitignore`：排除构建目录和本机配置。
- 截图：放在 `docs/assets/screenshots/`，Markdown 使用相对路径。

README 不要只写概念，最好给可复制命令。教程中每个复杂结构都配一个小示例，读者更容易迁移到自己的项目。

## 17. 参考资料

- Qt 官方文档：[Build with CMake](https://doc.qt.io/qt-6.11/cmake-manual.html)
- Qt 官方文档：[qt_standard_project_setup](https://doc.qt.io/qt-6/qt-standard-project-setup.html)
- Qt 官方文档：[qt_add_library](https://doc.qt.io/qt-6/qt-add-library.html)
- Qt 官方文档：[qt_add_qml_module](https://doc.qt.io/qt-6/qt-add-qml-module.html)
- Qt 官方文档：[qt_generate_deploy_app_script](https://doc.qt.io/qt-6/qt-generate-deploy-app-script.html)
- CMake 官方文档：[CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
- CMake 官方文档：[install](https://cmake.org/cmake/help/latest/command/install.html)
- CMake 官方文档：[Importing and Exporting Guide](https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html)
