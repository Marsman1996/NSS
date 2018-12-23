#include <windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlwin.h>
#include <opencv.hpp>
#include "resource.h"

void lockscreen(HWND hwnd){
    // system("rundll32.exe user32.dll,LockWorkStation");
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);  //显示窗体的API 传入需要显示的窗体句柄和显示方式
    BlockInput(true);  //This only works under "Run as Admin" mode
}

void unlockscreen(HWND hwnd){
    BlockInput(false);
    ShowWindow(hwnd, SW_HIDE);
}

LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  //声明消息回调函数

HWND initlockwindow(HINSTANCE hInstance){
    HWND hwnd;
    WNDCLASS wndcls;                                                //定义窗体类
    
    wndcls.cbClsExtra = 0;                                          //结构体后附加的字节数，一般总为0
    wndcls.cbWndExtra = 0;                                          //窗体实例附加的字节数，一般总为0
    // wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);     //背景颜色
    wndcls.hbrBackground = CreatePatternBrush(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);                   //光标句柄
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);                 //图标句柄 任务栏显示的图标
    wndcls.hInstance = hInstance;                                   //模块句柄
    wndcls.lpfnWndProc = WinSunProc;                                //函数指针，指向处理窗口消息的函数入口
    wndcls.lpszClassName = _T("windows");                           //自定义类名，不要与其他类名重复
    wndcls.lpszMenuName = NULL;                                     //菜单名的字符串
    wndcls.style = CS_HREDRAW | CS_VREDRAW;                         //指定窗口风格 详见参数注释1
    
    if (!RegisterClass(&wndcls))                                        //注册窗体类，如果失败直接返回0结束程序
        exit(-1);                          
    hwnd = CreateWindow(_T("windows"), _T("Screensaver"), WS_OVERLAPPEDWINDOW, 0, 0, 300, 150, NULL, NULL, hInstance, NULL);//创建窗体
    CWindow myWindow;
    myWindow.Attach(hwnd);
    myWindow.ModifyStyle(WS_CAPTION, 0);
    return hwnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PTSTR szCmdLine, int iCmdShow){  //Win32 Api 主程序函数入口
    HWND hwnd;                                                      //定义句柄用来保存成功创建窗口后返回的句柄
    MSG msg;                                                        //定义消息结构体变量

    hwnd = initlockwindow(hInstance);
    //APIWS_OVERLAPPEDWINDOW为 Window Styles  //详见参数注释2
    // UpdateWindow(hwnd);//刷新窗体的API

    // cv::Mat img = cv::imread("../resource/error.bmp");
    // cv::namedWindow("pic");
    // imshow("pic", img);
    // cv::waitKey(1000);

    lockscreen(hwnd);
    Sleep(10000);
    unlockscreen(hwnd);
    Sleep(5000);
    lockscreen(hwnd);
    Sleep(5000);
    unlockscreen(hwnd);
    
    cv::Mat img;
    cv::VideoCapture cap(0);
    while(1){
        cap >> img;
        
        if(img.empty()){

        }
        else{
            imshow("pic", img);

        }
        cv::waitKey(30);
    }
    
    while (GetMessage(&msg, NULL, 0, 0)){//进入消息循环
        TranslateMessage(&msg); //将虚拟键消息转换为字符消息。字符消息被送到调用线程的消息队列中，在下一次线程调用函数GetMessage或PeekMessage时被读出
        DispatchMessage(&msg); //该函数调度一个消息给窗口程序。通常调度从GetMessage取得的消息。消息被调度到的窗口程序即是MainProc()函数
    }
    return 0;
}
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){//回调函数定义
    switch (uMsg){
    case WM_DESTROY://关闭窗口是系统发送的消息
        PostQuitMessage(0);//发送退出消息GetMessage收到消息后将return 0,主函数退出消息循环
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);//不处理的消息交给系统处理。
    }
    return 0;
}