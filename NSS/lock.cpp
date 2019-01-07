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
#include <string>
#include <sstream>
#include <atlstr.h>
using namespace std;
float check_user_present(cv::Mat img);
bool is_lock = false;

void lockscreen(){
	FILE *fp;
	ostringstream buffer;
	
	cv::Mat img;
	cv::VideoCapture cap(0);
	cap >> img;
	if (img.empty()){
		return;
	}
	float res = check_user_present(img);
	/*buffer << res;
	string str = buffer.str() + ".jpg";
	buffer.str("");
	cv::imwrite(str, img);*/
	if ( res < 0.6){
		is_lock = true;
		fp = fopen("d:\\myfile.txt", "w");

		fprintf(fp, "%f\n", res);
		fclose(fp);
		system("rundll32.exe user32.dll,LockWorkStation");
		//ShowWindow(hwnd, SW_SHOWMAXIMIZED);
		//BlockInput(true);  //This only works under "Run as Admin" mode
	}
    
}

void unlockscreen(){
	FILE *fp;
	ostringstream buffer;
	cv::Mat img;
	cv::VideoCapture cap(0);
	while (is_lock == true){
		cap >> img;
		if (img.empty()){
			return;
		}
		float res = check_user_present(img);
		/*buffer << res;
		string str = buffer.str() + ".jpg";
		buffer.str("");
		cv::imwrite(str, img);*/
		if (res > 0.6){
			is_lock = false;

			fp = fopen("d:\\myfile.txt", "w");

			fprintf(fp, "%f\n", res);
			fclose(fp);
			
			//BlockInput(false);
			//ShowWindow(hwnd, SW_HIDE);
		}
	}
    
	
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPTSTR szCmdLine, int iCmdShow){
    while(1){
		if (is_lock == true){
			//Sleep(100);
			unlockscreen();
			Sleep(20000);
		}
		else{
			Sleep(20000);
			lockscreen();
		}
    }

    return 0;
}