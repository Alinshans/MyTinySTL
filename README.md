MyTinySTL
=========

| linux   |  windows  | release  |
|:-------:|:---------:|:--------:|
|[![Build Status](https://travis-ci.org/Alinshans/MyTinySTL.svg?branch=master)](https://travis-ci.org/Alinshans/MyTinySTL)|[![Build Status](https://ci.appveyor.com/api/projects/status/github/Alinshans/MyTinySTL?branch=master&svg=true)](https://ci.appveyor.com/project/Alinshans/mytinystl)|[![Release](http://i1.buimg.com/594413/09544a84b6c8976e.png))](https://github.com/Alinshans/MyTinySTL/releases)|

## 简介
   基于 `C++11` 的 `tinySTL`，并使用中文文档与中文注释，作为新手练习用途。实现了大部分 STL 中的容器与函数，但仍存在许多不足与 bug 。如发现错误，还请在 `Issues` 中指出，欢迎 `Fork` 和 `Pull requests` 改善代码，谢谢！<br>

## 支持

* 操作系统
  * linux
  * windows
  * osx
* 编译器
  * GCC 4.8
  * Clang 3.5 
  * MSVC 14.0 

## 需要
  * cmake 2.8 (使用g++/clang编译)

## 运行

  * linux/osx
  1. git clone
```shell
$ git clone git@github.com:Alinshans/MyTinySTL.git
```
  2. 构建并运行
```shell
$ mkdir build && cd ./build
$ cmake ..
$ make
$ cd ../bin && ./stltest
```

  * windows
  使用 `vs2015`（或 `vs2017`）打开 `MyTinySTL.sln`，配置 `Relase` 模式，（Ctrl + F5）开始执行。
  
## 文档
  见 [Wiki](https://github.com/Alinshans/MyTinySTL/wiki)

## 测试
  见 [Test](https://github.com/Alinshans/MyTinySTL/tree/master/Test)

  
