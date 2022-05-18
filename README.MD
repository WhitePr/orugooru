# 目录
- [目录](#目录)
- [为什么要写这个项目](#为什么要写这个项目)
- [如何安装](#如何安装)
    - [Arch Linux](#arch-linux)
    - [Termux](#termux)
    - [构建](#构建)
- [如何使用](#如何使用)
    - [显示帮助](#显示帮助)
- [注意事项](#注意事项)
- [如何贡献](#如何贡献)
- [开源协议](#开源协议)

# 为什么要写这个项目
由于在用[uEmuera](https://github.com/xerysherry/uEmuera)玩eraTW时每次更新都要去电脑上转换文本格式 而且在termux上没有什么好的转换方式 所以创建了这个项目

项目名称为日语"オルゴール"的罗马音

# 如何安装

本项目使用了以下库
* libmagic
* boost
* ICU
  
以及您的C++编译器需要最低支持C++20

### Arch Linux

```sh 
pacman -S file boost cmake icu
```

### Termux
```sh
pkg install file boost-headers cmake libicu
```


### 构建

本项目使用cmake

在Linux上构建
```sh
mkdir builid
cd build
cmake .. && cmake --build . 
```
在Termux上构建
```sh
mkdir builid
cd build
cmake .. -DTERMUX=on && cmake --build . 
```
您可以在bin文件夹找到编译好的可执行文件

# 如何使用

### 显示帮助

```sh
orugooru -h
```

# 注意事项
请不要读取过大的文本文件 转换速度先不谈 由于会将文本读入内存 可能会导致内存无法分配导致应用异常退出

目前没有写转换编码的格式正确性 如果输入了错误的转换格式将会导致文件被截断比如:

orugooru -i text.txt -f shift -w 

text.txt将会被截断为0

# 如何贡献
出现了Bug或者有什么好意见请提交issus或者直接PR也行


# 开源协议
本项目使用[MIT](./license)许可证 