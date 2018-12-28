#include <windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlwin.h>
#include <opencv.hpp>
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


float check_user_present(cv::Mat img);

void lockscreen(HWND hwnd){
    // system("rundll32.exe user32.dll,LockWorkStation");
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);  //
    BlockInput(true);  //This only works under "Run as Admin" mode
}

void unlockscreen(HWND hwnd){
    BlockInput(false);
    ShowWindow(hwnd, SW_HIDE);
}

LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  //

HWND initlockwindow(HINSTANCE hInstance){
    HWND hwnd;
    WNDCLASS wndcls;                                                //
    
    wndcls.cbClsExtra = 0;                                          //
    wndcls.cbWndExtra = 0;                                          //
    // wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);     //
    wndcls.hbrBackground = CreatePatternBrush(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);                   //
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);                 //
    wndcls.hInstance = hInstance;                                   //
    wndcls.lpfnWndProc = WinSunProc;                                //
    wndcls.lpszClassName = _T("windows");                           //
    wndcls.lpszMenuName = NULL;                                     //
    wndcls.style = CS_HREDRAW | CS_VREDRAW;                         //
    
    if (!RegisterClass(&wndcls))                                        //
        exit(-1);                          
    hwnd = CreateWindow(_T("windows"), _T("Screensaver"), WS_OVERLAPPEDWINDOW, 0, 0, 300, 150, NULL, NULL, hInstance, NULL);//
    CWindow myWindow;
    myWindow.Attach(hwnd);
    myWindow.ModifyStyle(WS_CAPTION, 0);
    return hwnd;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPTSTR szCmdLine, int iCmdShow){  //
    HWND hwnd;                                                      //
    MSG msg;                                                        //
    bool is_lock = false;

	FILE *fp;
	

    hwnd = initlockwindow(hInstance);
    //APIWS_OVERLAPPEDWINDOWΪ Window Styles  //
    // UpdateWindow(hwnd);//

    // cv::Mat img = cv::imread("../resource/error.bmp");
    // cv::namedWindow("pic");
    // imshow("pic", img);
    // cv::waitKey(1000);
    
    cv::Mat img;
    cv::VideoCapture cap(0);
    while(1){
		if (is_lock == true)
			Sleep(100);
		else
			Sleep(10000);
        cap >> img;
        
        if(img.empty()){
            continue;
        }
        
		float res = check_user_present(img);

		if (is_lock == false && res < 0.6){
            lockscreen(hwnd);
            is_lock = true;
			fp = fopen("d:\\myfile.txt", "a+");
			fprintf(fp, "%f\n", res);
			fclose(fp);
        }
		else if (is_lock == true && res > 0.6){
            unlockscreen(hwnd);
            is_lock = false;
			Sleep(10000);
			fp = fopen("d:\\myfile.txt", "a+");
			fprintf(fp, "%f\n", res);
			fclose(fp);
			
        }
    }
    
    while (GetMessage(&msg, NULL, 0, 0)){//
        TranslateMessage(&msg); //
        DispatchMessage(&msg); //
    }
	fclose(fp);
    return 0;
}
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){//
    switch (uMsg){
    case WM_DESTROY://
        PostQuitMessage(0);//
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);//
    }
    return 0;
}
