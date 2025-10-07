#pragma once
#pragma execution_character_set("utf-8")

#include <QString>

#include <ImageProcess/ImageProcess.h>
#pragma comment(lib, "ImageProcess.lib")

namespace Algorithm
{
	void hsvGetColor(const cv::Mat& mat, const cv::Rect& rect, QString& colorName);

	void getBlackWhitePrecent(const cv::Mat& mat, const cv::Rect& rect, double thresh, double* blackPrecent, double* whitePrecent);

	int getImageDataSum(const cv::Mat& mat, const cv::Rect& rect);

	void getDynamicValue(cv::Mat& a, cv::Mat& b, double thresh, uint& sum, double& percent);

	void colorEnhancement(const cv::Mat& src, cv::Mat& dst, int filter);
}
