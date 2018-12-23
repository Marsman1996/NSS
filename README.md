# 基于面部识别的屏幕保护程序
<!-- Screensaver based on face recognition -->
Homework of Network and System Security.
## 概述
本Repo实现了一个基于面部识别的屏幕保护系统, 当摄像头检测到特定人物处于离开状态时会自动锁定电脑, 检测到特定人物存在时会解锁.  

## 实现
### 锁定模块实现
<!-- 通过调用user32.dll中的[ *LockWorkStation()* ](https://docs.microsoft.com/zh-cn/windows/desktop/api/winuser/nf-winuser-lockworkstation)实现锁屏.  -->
调用[ *BlockInput()* ](https://docs.microsoft.com/zh-cn/windows/desktop/api/winuser/nf-winuser-blockinput)使键盘鼠标阻塞, 使其不能正常解锁. 
并创建全屏窗口作为屏保显示界面.
<!-- (锁屏界面属于system用户) -->

### 拍照实现
调用OpenCV库函数实现

### 


## REFERENCE
[blog: OpenCV安装教程](https://blog.csdn.net/mars_xiaolei/article/details/78759041)  
