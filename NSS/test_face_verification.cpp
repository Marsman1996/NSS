/*
*
* This file is part of the open-source SeetaFace engine, which includes three modules:
* SeetaFace Detection, SeetaFace Alignment, and SeetaFace Identification.
*
* This file is part of the SeetaFace Identification module, containing codes implementing the
* face identification method described in the following paper:
*
*
*   VIPLFaceNet: An Open Source Deep Face Recognition SDK,
*   Xin Liu, Meina Kan, Wanglong Wu, Shiguang Shan, Xilin Chen.
*   In Frontiers of Computer Science.
*
*
* Copyright (C) 2016, Visual Information Processing and Learning (VIPL) group,
* Institute of Computing Technology, Chinese Academy of Sciences, Beijing, China.
*
* The codes are mainly developed by Jie Zhang(a Ph.D supervised by Prof. Shiguang Shan)
*
* As an open-source face recognition engine: you can redistribute SeetaFace source codes
* and/or modify it under the terms of the BSD 2-Clause License.
*
* You should have received a copy of the BSD 2-Clause License along with the software.
* If not, see < https://opensource.org/licenses/BSD-2-Clause>.
*
* Contact Info: you can send an email to SeetaFace@vipl.ict.ac.cn for any problems.
*
* Note: the above information must be kept whenever or wherever the codes are used.
*
*/

#include<iostream>
using namespace std;

#ifdef _WIN32
#pragma once
#include <opencv2/core/version.hpp>

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) \
  CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif //_DEBUG

#pragma comment( lib, cvLIB("core") )
#pragma comment( lib, cvLIB("imgproc") )
#pragma comment( lib, cvLIB("highgui") )

#endif //_WIN32

#if defined(__unix__) || defined(__APPLE__)

#ifndef fopen_s

#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL

#endif //fopen_s

#endif //__unix

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "face_identification.h"
#include "recognizer.h"
#include "face_detection.h"
#include "face_alignment.h"

#include "math_functions.h"

#include <vector>
#include <string>
#include <iostream>
#include<fstream>
#include <algorithm>


using namespace seeta;

#define TEST(major, minor) major##_##minor##_Tester()
#define EXPECT_NE(a, b) if ((a) == (b)) std::cout << "ERROR: "
#define EXPECT_EQ(a, b) if ((a) != (b)) std::cout << "ERROR: "

#ifdef _WIN32
std::string DATA_DIR = "C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/data/";
std::string MODEL_DIR = "C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/FaceIdentification/model/";
#else
std::string DATA_DIR = "./data/";
std::string MODEL_DIR = "./model/";
#endif

int Compare(const void *a, const void *b){
	float da = *(float *)a;
	float db = *(float *)b;
	return (da > db) ? 1 : -1;
}

float check_user_present(cv::Mat img) {
  // Initialize face detection model
  seeta::FaceDetection detector("C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/FaceDetection/model/seeta_fd_frontal_v1.0.bin");
  detector.SetMinFaceSize(40);
  detector.SetScoreThresh(2.f);
  detector.SetImagePyramidScaleFactor(0.8f);
  detector.SetWindowStep(4, 4);
  float sim[5];

  // Initialize face alignment model 
  seeta::FaceAlignment point_detector("C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/FaceAlignment/model/seeta_fa_v1.1.bin");

  // Initialize face Identification model 
  FaceIdentification face_recognizer((MODEL_DIR + "seeta_fr_v1.0.bin").c_str());
  std::string test_dir = DATA_DIR + "test_face_recognizer/";

  //load image
  cv::Mat target_img_color = img;
  cv::Mat target_img_gray;
  cv::cvtColor(target_img_color, target_img_gray, CV_BGR2GRAY);

  cv::Mat test1_img_color = cv::imread(test_dir + "images/compare_im/TEST_1.jpg", 1);
  cv::Mat test1_img_gray;
  cv::cvtColor(test1_img_color, test1_img_gray, CV_BGR2GRAY);

  cv::Mat test2_img_color = cv::imread(test_dir + "images/compare_im/TEST_2.jpg", 1);
  cv::Mat test2_img_gray;
  cv::cvtColor(test2_img_color, test2_img_gray, CV_BGR2GRAY);

  cv::Mat test3_img_color = cv::imread(test_dir + "images/compare_im/TEST_3.jpg", 1);
  cv::Mat test3_img_gray;
  cv::cvtColor(test3_img_color, test3_img_gray, CV_BGR2GRAY);

  cv::Mat test4_img_color = cv::imread(test_dir + "images/compare_im/TEST_4.jpg", 1);
  cv::Mat test4_img_gray;
  cv::cvtColor(test4_img_color, test4_img_gray, CV_BGR2GRAY);

  cv::Mat test5_img_color = cv::imread(test_dir + "images/compare_im/TEST_5.jpg", 1);
  cv::Mat test5_img_gray;
  cv::cvtColor(test5_img_color, test5_img_gray, CV_BGR2GRAY);

  ImageData target_img_data_color(target_img_color.cols, target_img_color.rows, target_img_color.channels());
  target_img_data_color.data = target_img_color.data;

  ImageData target_img_data_gray(target_img_gray.cols, target_img_gray.rows, target_img_gray.channels());
  target_img_data_gray.data = target_img_gray.data;

  ImageData test1_img_data_color(test1_img_color.cols, test1_img_color.rows, test1_img_color.channels());
  test1_img_data_color.data = test1_img_color.data;

  ImageData test1_img_data_gray(test1_img_gray.cols, test1_img_gray.rows, test1_img_gray.channels());
  test1_img_data_gray.data = test1_img_gray.data;

  ImageData test2_img_data_color(test2_img_color.cols, test2_img_color.rows, test2_img_color.channels());
  test2_img_data_color.data = test2_img_color.data;

  ImageData test2_img_data_gray(test2_img_gray.cols, test2_img_gray.rows, test2_img_gray.channels());
  test2_img_data_gray.data = test2_img_gray.data;

  ImageData test3_img_data_color(test3_img_color.cols, test3_img_color.rows, test3_img_color.channels());
  test3_img_data_color.data = test3_img_color.data;

  ImageData test3_img_data_gray(test3_img_gray.cols, test3_img_gray.rows, test3_img_gray.channels());
  test3_img_data_gray.data = test3_img_gray.data;

  ImageData test4_img_data_color(test4_img_color.cols, test4_img_color.rows, test4_img_color.channels());
  test4_img_data_color.data = test4_img_color.data;

  ImageData test4_img_data_gray(test4_img_gray.cols, test4_img_gray.rows, test4_img_gray.channels());
  test4_img_data_gray.data = test4_img_gray.data;

  ImageData test5_img_data_color(test5_img_color.cols, test5_img_color.rows, test5_img_color.channels());
  test5_img_data_color.data = test5_img_color.data;

  ImageData test5_img_data_gray(test5_img_gray.cols, test5_img_gray.rows, test5_img_gray.channels());
  test5_img_data_gray.data = test5_img_gray.data;

  // Detect faces
  std::vector<seeta::FaceInfo> target_faces = detector.Detect(target_img_data_gray);
  int32_t target_face_num = static_cast<int32_t>(target_faces.size());

  std::vector<seeta::FaceInfo> test1_faces = detector.Detect(test1_img_data_gray);
  int32_t test1_face_num = static_cast<int32_t>(test1_faces.size());

  std::vector<seeta::FaceInfo> test2_faces = detector.Detect(test2_img_data_gray);
  int32_t test2_face_num = static_cast<int32_t>(test2_faces.size());

  std::vector<seeta::FaceInfo> test3_faces = detector.Detect(test3_img_data_gray);
  int32_t test3_face_num = static_cast<int32_t>(test3_faces.size());

  std::vector<seeta::FaceInfo> test4_faces = detector.Detect(test4_img_data_gray);
  int32_t test4_face_num = static_cast<int32_t>(test4_faces.size());

  std::vector<seeta::FaceInfo> test5_faces = detector.Detect(test5_img_data_gray);
  int32_t test5_face_num = static_cast<int32_t>(test5_faces.size());

  if (target_face_num == 0)
  {
    return 0;
  }
  else{
	  // Detect 5 facial landmarks
	  seeta::FacialLandmark target_points[5];
	  point_detector.PointDetectLandmarks(target_img_data_gray, target_faces[0], target_points);

	  seeta::FacialLandmark test1_points[5];
	  point_detector.PointDetectLandmarks(test1_img_data_gray, test1_faces[0], test1_points);

	  seeta::FacialLandmark test2_points[5];
	  point_detector.PointDetectLandmarks(test2_img_data_gray, test2_faces[0], test2_points);

	  seeta::FacialLandmark test3_points[5];
	  point_detector.PointDetectLandmarks(test3_img_data_gray, test3_faces[0], test3_points);

	  seeta::FacialLandmark test4_points[5];
	  point_detector.PointDetectLandmarks(test4_img_data_gray, test4_faces[0], test4_points);

	  seeta::FacialLandmark test5_points[5];
	  point_detector.PointDetectLandmarks(test5_img_data_gray, test5_faces[0], test5_points);

	  for (int i = 0; i < 5; i++)
	  {
		  cv::circle(target_img_color, cv::Point(target_points[i].x, target_points[i].y), 2,
			  CV_RGB(0, 255, 0));
		  cv::circle(test1_img_color, cv::Point(test1_points[i].x, test1_points[i].y), 2,
			  CV_RGB(0, 255, 0));
		  cv::circle(test2_img_color, cv::Point(test2_points[i].x, test2_points[i].y), 2,
			  CV_RGB(0, 255, 0));
		  cv::circle(test3_img_color, cv::Point(test3_points[i].x, test3_points[i].y), 2,
			  CV_RGB(0, 255, 0));
		  cv::circle(test4_img_color, cv::Point(test4_points[i].x, test4_points[i].y), 2,
			  CV_RGB(0, 255, 0));
		  cv::circle(test5_img_color, cv::Point(test5_points[i].x, test5_points[i].y), 2,
			  CV_RGB(0, 255, 0));
	  }
	  //cv::imwrite("gallery_point_result.jpg", target_img_color);
	  //cv::imwrite("probe_point_result.jpg", test1_img_color);

	  // Extract face identity feature
	  float target_fea[2048];
	  float test1_fea[2048];
	  float test2_fea[2048];
	  float test3_fea[2048];
	  float test4_fea[2048];
	  float test5_fea[2048];
	  face_recognizer.ExtractFeatureWithCrop(target_img_data_color, target_points, target_fea);
	  face_recognizer.ExtractFeatureWithCrop(test1_img_data_color, test1_points, test1_fea);
	  face_recognizer.ExtractFeatureWithCrop(test2_img_data_color, test2_points, test2_fea);
	  face_recognizer.ExtractFeatureWithCrop(test3_img_data_color, test3_points, test3_fea);
	  face_recognizer.ExtractFeatureWithCrop(test4_img_data_color, test4_points, test4_fea);
	  face_recognizer.ExtractFeatureWithCrop(test5_img_data_color, test5_points, test5_fea);

	  // Caculate similarity of two faces
	  sim[0] = face_recognizer.CalcSimilarity(target_fea, test1_fea);
	  sim[1] = face_recognizer.CalcSimilarity(target_fea, test2_fea);
	  sim[2] = face_recognizer.CalcSimilarity(target_fea, test3_fea);
	  sim[3] = face_recognizer.CalcSimilarity(target_fea, test4_fea);
	  sim[4] = face_recognizer.CalcSimilarity(target_fea, test5_fea);
	  qsort(sim, sizeof(sim) / sizeof(sim[0]), sizeof(sim[0]), Compare);
	  //return ((sim[3] + sim[2] + sim[1]) / 3);
	  return sim[4];
  }
}
/*

int main() {
	// Initialize face detection model
	seeta::FaceDetection detector("C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/FaceDetection/model/seeta_fd_frontal_v1.0.bin");
	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
	float sim[5];

	// Initialize face alignment model 
	seeta::FaceAlignment point_detector("C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/FaceAlignment/model/seeta_fa_v1.1.bin");

	// Initialize face Identification model 
	FaceIdentification face_recognizer((MODEL_DIR + "seeta_fr_v1.0.bin").c_str());
	std::string test_dir = DATA_DIR + "test_face_recognizer/";

	//load image
	cv::Mat target_img_color = cv::imread(test_dir + "images/compare_im/4.jpg", 1);;
	cv::Mat target_img_gray;
	cv::cvtColor(target_img_color, target_img_gray, CV_BGR2GRAY);

	cv::Mat test1_img_color = cv::imread(test_dir + "images/compare_im/TEST_1.jpg", 1);
	cv::Mat test1_img_gray;
	cv::cvtColor(test1_img_color, test1_img_gray, CV_BGR2GRAY);

	cv::Mat test2_img_color = cv::imread(test_dir + "images/compare_im/TEST_2.jpg", 1);
	cv::Mat test2_img_gray;
	cv::cvtColor(test2_img_color, test2_img_gray, CV_BGR2GRAY);

	cv::Mat test3_img_color = cv::imread(test_dir + "images/compare_im/TEST_3.jpg", 1);
	cv::Mat test3_img_gray;
	cv::cvtColor(test3_img_color, test3_img_gray, CV_BGR2GRAY);

	cv::Mat test4_img_color = cv::imread(test_dir + "images/compare_im/TEST_4.jpg", 1);
	cv::Mat test4_img_gray;
	cv::cvtColor(test4_img_color, test4_img_gray, CV_BGR2GRAY);

	cv::Mat test5_img_color = cv::imread(test_dir + "images/compare_im/TEST_5.jpg", 1);
	cv::Mat test5_img_gray;
	cv::cvtColor(test5_img_color, test5_img_gray, CV_BGR2GRAY);

	ImageData target_img_data_color(target_img_color.cols, target_img_color.rows, target_img_color.channels());
	target_img_data_color.data = target_img_color.data;

	ImageData target_img_data_gray(target_img_gray.cols, target_img_gray.rows, target_img_gray.channels());
	target_img_data_gray.data = target_img_gray.data;

	ImageData test1_img_data_color(test1_img_color.cols, test1_img_color.rows, test1_img_color.channels());
	test1_img_data_color.data = test1_img_color.data;

	ImageData test1_img_data_gray(test1_img_gray.cols, test1_img_gray.rows, test1_img_gray.channels());
	test1_img_data_gray.data = test1_img_gray.data;

	ImageData test2_img_data_color(test2_img_color.cols, test2_img_color.rows, test2_img_color.channels());
	test2_img_data_color.data = test2_img_color.data;

	ImageData test2_img_data_gray(test2_img_gray.cols, test2_img_gray.rows, test2_img_gray.channels());
	test2_img_data_gray.data = test2_img_gray.data;

	ImageData test3_img_data_color(test3_img_color.cols, test3_img_color.rows, test3_img_color.channels());
	test3_img_data_color.data = test3_img_color.data;

	ImageData test3_img_data_gray(test3_img_gray.cols, test3_img_gray.rows, test3_img_gray.channels());
	test3_img_data_gray.data = test3_img_gray.data;

	ImageData test4_img_data_color(test4_img_color.cols, test4_img_color.rows, test4_img_color.channels());
	test4_img_data_color.data = test4_img_color.data;

	ImageData test4_img_data_gray(test4_img_gray.cols, test4_img_gray.rows, test4_img_gray.channels());
	test4_img_data_gray.data = test4_img_gray.data;

	ImageData test5_img_data_color(test5_img_color.cols, test5_img_color.rows, test5_img_color.channels());
	test5_img_data_color.data = test5_img_color.data;

	ImageData test5_img_data_gray(test5_img_gray.cols, test5_img_gray.rows, test5_img_gray.channels());
	test5_img_data_gray.data = test5_img_gray.data;

	// Detect faces
	std::vector<seeta::FaceInfo> target_faces = detector.Detect(target_img_data_gray);
	int32_t target_face_num = static_cast<int32_t>(target_faces.size());

	std::vector<seeta::FaceInfo> test1_faces = detector.Detect(test1_img_data_gray);
	int32_t test1_face_num = static_cast<int32_t>(test1_faces.size());

	std::vector<seeta::FaceInfo> test2_faces = detector.Detect(test2_img_data_gray);
	int32_t test2_face_num = static_cast<int32_t>(test2_faces.size());

	std::vector<seeta::FaceInfo> test3_faces = detector.Detect(test3_img_data_gray);
	int32_t test3_face_num = static_cast<int32_t>(test3_faces.size());

	std::vector<seeta::FaceInfo> test4_faces = detector.Detect(test4_img_data_gray);
	int32_t test4_face_num = static_cast<int32_t>(test4_faces.size());

	std::vector<seeta::FaceInfo> test5_faces = detector.Detect(test5_img_data_gray);
	int32_t test5_face_num = static_cast<int32_t>(test5_faces.size());

	if (target_face_num == 0)
	{
		return 0;
	}
	else{
		// Detect 5 facial landmarks
		seeta::FacialLandmark target_points[5];
		point_detector.PointDetectLandmarks(target_img_data_gray, target_faces[0], target_points);

		seeta::FacialLandmark test1_points[5];
		point_detector.PointDetectLandmarks(test1_img_data_gray, test1_faces[0], test1_points);

		seeta::FacialLandmark test2_points[5];
		point_detector.PointDetectLandmarks(test2_img_data_gray, test2_faces[0], test2_points);

		seeta::FacialLandmark test3_points[5];
		point_detector.PointDetectLandmarks(test3_img_data_gray, test3_faces[0], test3_points);

		seeta::FacialLandmark test4_points[5];
		point_detector.PointDetectLandmarks(test4_img_data_gray, test4_faces[0], test4_points);

		seeta::FacialLandmark test5_points[5];
		point_detector.PointDetectLandmarks(test5_img_data_gray, test5_faces[0], test5_points);

		for (int i = 0; i < 5; i++)
		{
			cv::circle(target_img_color, cv::Point(target_points[i].x, target_points[i].y), 2,
				CV_RGB(0, 255, 0));
			cv::circle(test1_img_color, cv::Point(test1_points[i].x, test1_points[i].y), 2,
				CV_RGB(0, 255, 0));
			cv::circle(test2_img_color, cv::Point(test2_points[i].x, test2_points[i].y), 2,
				CV_RGB(0, 255, 0));
			cv::circle(test3_img_color, cv::Point(test3_points[i].x, test3_points[i].y), 2,
				CV_RGB(0, 255, 0));
			cv::circle(test4_img_color, cv::Point(test4_points[i].x, test4_points[i].y), 2,
				CV_RGB(0, 255, 0));
			cv::circle(test5_img_color, cv::Point(test5_points[i].x, test5_points[i].y), 2,
				CV_RGB(0, 255, 0));
		}
		//cv::imwrite("gallery_point_result.jpg", target_img_color);
		//cv::imwrite("probe_point_result.jpg", test1_img_color);

		// Extract face identity feature
		float target_fea[2048];
		float test1_fea[2048];
		float test2_fea[2048];
		float test3_fea[2048];
		float test4_fea[2048];
		float test5_fea[2048];
		face_recognizer.ExtractFeatureWithCrop(target_img_data_color, target_points, target_fea);
		face_recognizer.ExtractFeatureWithCrop(test1_img_data_color, test1_points, test1_fea);
		face_recognizer.ExtractFeatureWithCrop(test2_img_data_color, test2_points, test2_fea);
		face_recognizer.ExtractFeatureWithCrop(test3_img_data_color, test3_points, test3_fea);
		face_recognizer.ExtractFeatureWithCrop(test4_img_data_color, test4_points, test4_fea);
		face_recognizer.ExtractFeatureWithCrop(test5_img_data_color, test5_points, test5_fea);

		// Caculate similarity of two faces
		sim[0] = face_recognizer.CalcSimilarity(target_fea, test1_fea);
		sim[1] = face_recognizer.CalcSimilarity(target_fea, test2_fea);
		sim[2] = face_recognizer.CalcSimilarity(target_fea, test3_fea);
		sim[3] = face_recognizer.CalcSimilarity(target_fea, test4_fea);
		sim[4] = face_recognizer.CalcSimilarity(target_fea, test5_fea);
		qsort(sim, sizeof(sim) / sizeof(sim[0]), sizeof(sim[0]), Compare);
		ofstream ofile;
		ofile.open("d:\\test.txt");
		ofile << ((sim[3] + sim[2] + sim[1]) / 3) << endl;
		return 0;
	}
}
*/

