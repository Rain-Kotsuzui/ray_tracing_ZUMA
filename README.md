# 基于QT的3DZUMA游戏

在课程作业的基础上3D化，并对轨迹进行了改进。

## 项目结构
- `core`：游戏逻辑实现
- `mySTL`：自定义的STL容器实现，还有一些自定义数据类型
- `main.cpp`：程序入口
- `res`：地板的贴图资源
- `README.md`：项目说明文档

## 编译

1. 在项目根目录下创建一个build文件夹：
   1. 打开命令面板（Windows/Linux快捷键：`Ctrl+Shift+P` ，MacOS快捷键：`Cmd+Shift+P`）；
   2. 输入 `tasks: run task` 并运行此命令；
   3. 选择 `mkdir` 任务并运行。

2. 运行qmake生成Makefile:
   1. 打开命令面板（Windows/Linux快捷键：`Ctrl+Shift+P` ，MacOS快捷键：`Cmd+Shift+P`）；
   2. 输入 `tasks: run task` 并运行此命令；
   3. 选择 `qmake-release` 任务并运行。

3. 使用Makefile编译项目:
   1. 打开命令面板（Windows/Linux快捷键：`Ctrl+Shift+P` ，MacOS快捷键：`Cmd+Shift+P`）；
   2. 输入 `tasks: run task` 并运行此命令；
   3. 选择 `make-release` 任务并运行。

4. 运行项目:
   1. 打开命令面板（Windows/Linux快捷键：`Ctrl+Shift+P` ，MacOS快捷键：`Cmd+Shift+P`）；
   2. 输入 `tasks: run task` 并运行此命令；
   3. 选择 `run-release` 任务并运行。

编译完成后，可执行文件位于`build/release` 目录下。

#### 注意事项

- 步骤1只需要运行一次，也就是只需要创建一次build文件夹。
- 步骤2-4可以直接通过运行步骤4来实现，也就是直接运行 `run-release` 任务。
  - 因为配置了 `run-release` 依赖于 `make-release` ， `make-release` 依赖于 `qmake-release` 
  - 所以 `run-release` 会自动执行 `qmake-release` 和 `make-release` 任务
- 如出现编译运行的问题且长时间未解决，可尝试将build文件夹删除后重新执行上述步骤1-4。

## 操作方法

### 基本移动
- `W`：向上移动
- `A`：向左移动
- `S`：向下移动
- `D`：向右移动
- `space`：向上移动
- `shift`：向下移动
- `滚轮`：缩放视角
- `鼠标左键`：发射

### 功能键
- `L`：打开泛光效果
- `J`：减弱泛光效果
- `K`：增强泛光效果
- `P`：控制路径显示

## 更新内容
- 增加了光线追踪
- 增加了轨迹改进
- 增加了画面后处理
- 更新了MySTL内容

## 设置
- 可以在`src/core/paramter.h`中设置游戏参数，包括但不限于：
  - `GROUND`：地面贴图
  - `SET_GROUND`：显示地面
  - `SAMPLE_COUNT`：采样次数
  - `SCREEN_RESOLUTION`：分辨率更改

## 更新日志
- 版本1.0 - 初始版本

## 联系方式

邮箱rainkotsuzui@gmail.com

## 许可证

本项目使用 MIT 许可证。有关详细信息，请参阅 LICENSE 文件。
