# ESP-SUB-MASTER

## Introduction

The **ESP-SUB-MASTER** project is a versatile underwater robot control program developed using the ESP-IDF framework on the ESP32 series chips. This project works in conjunction with [SUB-NAVI](https://github.com/sfxfs/sub-navi) and [ROV-HOST](https://github.com/bohonghuang/rov-host).

**ESP-SUB-MASTER** offers the following features:

- Serial communication with SUB-NAVI using Protocol Buffers
- Control and read related peripherals through the MCU's peripheral interfaces
- Hardware implementation of Dshot control for thrusters
- Visual project configuration based on `esp-menuconfig`

## Compilation

This project is developed using VSCode. Please ensure that the `esp-idf` plugin is installed beforehand. After cloning the project for the first time, use the plugin to generate the `.vscode` directory to enable proper code error checking. You can compile and upload the project using the plugin.

## Development

The main directories of this project include:

| Directory Name | Description                                            |
| -------------- | ------------------------------------------------------ |
| component      | Contains various third-party libraries and common code |
| main           | Contains the main operational logic code               |

Please add peripheral drivers to the `components` folder according to the existing file structure.