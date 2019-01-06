# 基于面部识别的屏幕保护程序
<!-- Screensaver based on face recognition -->
Homework of Network and System Security.
## 概述
本Repo实现了一个基于面部识别的屏幕保护系统。启动进程后，摄像头会持续拍照，传给Seetaface模型判断是否为机主，如果不是机主或者没有拍到人脸时就会锁定；锁定屏幕后，摄像头仍然持续拍照并记录比对结果，在pgina上给出任意输入并点击登录（此处有待后续修改），pgina会检查比对结果，如果正确就解锁。  

## 实现
### 锁定模块实现
调用[ *LockWorkStation* ]锁定工作站 

### 拍照实现
调用[OpenCV 2.4](https://github.com/opencv/opencv)实现

### 人脸识别实现
调用[SeetaFaceEngine](https://github.com/seetaface/SeetaFaceEngine)实现

### 登录解锁模块插件实现
借助[Pgina](https://github.com/pgina/pgina)编写插件实现

## References
[blog: OpenCV安装教程](https://blog.csdn.net/mars_xiaolei/article/details/78759041)  
[blog: Seetaface安装教程](https://blog.csdn.net/u014349384/article/details/76408114)
[pgina教程](https://http://pgina.org/)