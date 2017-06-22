MyTinySTL
=========

| linux   |  windows  | release  |
|:-------:|:---------:|:--------:|
|[![Build Status](https://travis-ci.org/Alinshans/MyTinySTL.svg?branch=master)](https://travis-ci.org/Alinshans/MyTinySTL)|[![Build Status](https://ci.appveyor.com/api/projects/status/github/Alinshans/MyTinySTL?branch=master&svg=true)](https://ci.appveyor.com/project/Alinshans/mytinystl)|[![Release](http://i1.buimg.com/594413/09544a84b6c8976e.png)](https://github.com/Alinshans/MyTinySTL/releases)|

## 简介
   基于 `C++11` 的 `tinySTL`，并使用中文文档与中文注释，作为新手练习用途。实现了大部分 STL 中的容器与函数，但仍存在许多不足与 bug 。如发现错误，还请在 [`Issues`](https://github.com/Alinshans/MyTinySTL/issues) 中指出，欢迎 `Fork` 和 [`Pull requests`](https://github.com/Alinshans/MyTinySTL/pulls) 改善代码，谢谢！

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
```bash
$ git clone git@github.com:Alinshans/MyTinySTL.git
$ cd MyTinySTL
```
  2. 构建并运行
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ cd ../bin && ./stltest
```

  * windows
  1. git clone 或 [Download ZIP](https://github.com/Alinshans/MyTinySTL/archive/master.zip)
  2. 使用 `vs2015`（或 `vs2017`）打开 `MyTinySTL.sln`，配置成 `Release` 模式，（Ctrl + F5）开始执行。
  
## 文档
  见 [Wiki](https://github.com/Alinshans/MyTinySTL/wiki)

## 测试
  见 [Test](https://github.com/Alinshans/MyTinySTL/tree/master/Test)

---

## Introduction

This is a `tinySTL` based on `C++11`, which is my first project for practice. I use the Chinese documents and annotations for convenience, maybe there will be an English version later, but now I have no time to do that yet. I have achieved the vast majority of the containers and functions of `STL`, and there may be some deficiencies and bugs. If you find that, please point out that in [`Issues`](https://github.com/Alinshans/MyTinySTL/issues), or make a [`Pull requests`](https://github.com/Alinshans/MyTinySTL/pulls) to improve it, thinks!

## Supported

* os
  * linux
  * windows
  * osx
* complier
  * gcc 4.8 or later
  * clang 3.5 or later
  * msvc 14.0 or later

## Required

* cmake 2.8 (if you build with gcc/clang)

## Run test

* linux/osx

1. git clone
```bash
$ git clone git@github.com:Alinshans/MyTinySTL.git
$ cd MyTinySTL
```
2. build and run
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ cd ../bin && ./stltest
```

* windows

1. git clone or [Download ZIP](https://github.com/Alinshans/MyTinySTL/archive/master.zip)
2. use `vs2015`(or `vs2017`) open the file `MyTinySTL.sln`, configured in `Release`, run this project(Ctrl + F5).

## Documents

See [Wiki](https://github.com/Alinshans/MyTinySTL/wiki)

## Test

See [Test](https://github.com/Alinshans/MyTinySTL/tree/master/Test)