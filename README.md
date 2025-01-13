# ESP-SUB-MASTER

中文 | [English](README_EN.md)

## 介绍

**ESP-SUB-MASTER** 项目是基于 ESP32 系列芯片，使用 ESP-IDF 框架编写的通用型水下机器人控制程序。本项目需要与 [SUB-NAVI](https://github.com/sfxfs/sub-navi) 和 [ROV-HOST](https://github.com/bohonghuang/rov-host) 配合使用。

**ESP-SUB-MASTER** 提供以下功能：

- 基于 Protocol Buffers 与 SUB-NAVI 进行串口通信
- 通过 MCU 的外设接口控制和读取相关外设
- 硬件实现 Dshot、Oneshot 控制推进器
- 基于 `esp-menuconfig` 的可视化项目配置

## 编译

本项目要求的 `esp-idf` 最低版本为 `v5.3.2`，推荐使用 VSCode 进行开发，请提前安装 `esp-idf` 插件。首次克隆项目后，请使用插件生成 `.vscode` 目录，以确保代码查错功能正常。可以直接通过插件进行项目的编译和烧录。

## 开发

本项目的主要文件夹包括：

| 文件夹名  | 描述                       |
| --------- | -------------------------- |
| component | 存放各类第三方库和通用代码 |
| main      | 存放主要运行逻辑代码       |

请按照原有文件规范将外设驱动添加到 `components` 文件夹中。