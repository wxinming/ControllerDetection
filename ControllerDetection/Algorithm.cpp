#include "Algorithm.h"
#include <QVector>

void Algorithm::hsvGetColor(const cv::Mat& mat, const cv::Rect& rect, QString& colorName)
{
	if (mat.empty() /*|| rect.empty()*/)
	{
		return;
	}

	std::vector<size_t> vec;
	cv::Mat roi = mat(rect);
	cv::Mat hsv;
	cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);

	size_t size = hsv.cols * hsv.rows * hsv.elemSize();
	size_t red = 0, green = 0, blue = 0;
	size_t count = 0;

	for (size_t i = 0; i < size; i += hsv.elemSize())
	{
		blue += hsv.data[i];
		green += hsv.data[i + 1];
		red += hsv.data[i + 2];
		count++;
	}

	red /= count;
	green /= count;
	blue /= count;

	vec.push_back(blue);
	vec.push_back(green);
	vec.push_back(red);

	char color[32] = { 0 };

	if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 255) && (vec[2] >= 0 && vec[2] <= 46))
	{
		strcpy(color, "黑");
	}
	else if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 43) && (vec[2] >= 46 && vec[2] <= 220))
	{
		strcpy(color, "灰");
	}
	else if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 35) && (vec[2] >= 221 && vec[2] <= 255))
	{
		strcpy(color, "白");
	}
	else if (((vec[0] >= 0 && vec[0] <= 10) || (vec[0] >= 156 && vec[0] <= 180)) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "红");
	}
	else if ((vec[0] >= 11 && vec[0] <= 25) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "橙");
	}
	else if ((vec[0] >= 26 && vec[0] <= 34) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "黄");
	}
	else if ((vec[0] >= 35 && vec[0] <= 77) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "绿");
	}
	else if ((vec[0] >= 78 && vec[0] <= 99) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "青");
	}
	else if ((vec[0] >= 100 && vec[0] <= 124) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "蓝");
	}
	else if ((vec[0] >= 125 && vec[0] <= 155) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
	{
		strcpy(color, "紫");
	}
	else
	{
		strcpy(color, "未");
	}

	colorName = color;
}

void Algorithm::getBlackWhitePrecent(const cv::Mat& mat, const cv::Rect& rect, double thresh, double* blackPrecent, double* whitePrecent)
{
	cv::Mat roi = mat(rect), gray, dst;
	cv::cvtColor(roi, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, dst, thresh, 255, 0);

	ulong black = 0, white = 0, total = dst.total();
	for (int i = 0; i < dst.rows; ++i)
	{
		for (int j = 0; j < dst.cols; ++j)
		{
			dst.at<uchar>(i, j) ? ++white : ++black;
		}
	}

	*blackPrecent = (double(black) / double(total)) * 100;
	*whitePrecent = (double(white) / double(total)) * 100;
}

int Algorithm::getImageDataSum(const cv::Mat& mat, const cv::Rect& rect)
{
	int sum = 0;
	cv::Mat img = mat(rect);
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
			sum += (img.at<cv::Vec3b>(i, j)[0] +
				img.at<cv::Vec3b>(i, j)[1] +
				img.at<cv::Vec3b>(i, j)[2]);
	return sum;
}

void Algorithm::getDynamicValue(cv::Mat& a, cv::Mat& b, double thresh, uint& sum, double& percent)
{
	cv::Mat diff;
	cv::Mat ele = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(8, 8));
	//非线程安全,需要判断,否则会抛出异常
	if (a.size() != b.size())
		return;
	cv::absdiff(a, b, diff);
	cv::dilate(diff, diff, ele);
	cv::erode(diff, diff, ele);
	cv::threshold(diff, diff, thresh, 255, 0);

	double pixel = diff.total() * 255;

	sum = 0;
	for (int i = 0; i < diff.rows; ++i)
		for (int j = 0; j < diff.cols; ++j)
			sum += diff.at<uchar>(i, j);
	percent = (static_cast<double>(sum) / pixel) * 100.0f;
}

void Algorithm::colorEnhancement(const cv::Mat& src, cv::Mat& dst, int filter)
{
	cv::Mat orig_img = src.clone();
	cv::Mat simg;

	if (orig_img.channels() != 1)
	{
		cvtColor(orig_img, simg, CV_BGR2GRAY);
	}
	else
	{
		return;
	}

	long int N = simg.rows * simg.cols;

	int histo_b[256]{};
	int histo_g[256]{};
	int histo_r[256]{};

	for (int i = 0; i < 256; i++)
	{
		histo_b[i] = 0;
		histo_g[i] = 0;
		histo_r[i] = 0;
	}
	cv::Vec3b intensity;

	for (int i = 0; i < simg.rows; i++)
	{
		for (int j = 0; j < simg.cols; j++)
		{
			intensity = orig_img.at<cv::Vec3b>(i, j);

			histo_b[intensity.val[0]] = histo_b[intensity.val[0]] + 1;
			histo_g[intensity.val[1]] = histo_g[intensity.val[1]] + 1;
			histo_r[intensity.val[2]] = histo_r[intensity.val[2]] + 1;
		}
	}

	for (int i = 1; i < 256; i++)
	{
		histo_b[i] = histo_b[i] + filter * histo_b[i - 1];
		histo_g[i] = histo_g[i] + filter * histo_g[i - 1];
		histo_r[i] = histo_r[i] + filter * histo_r[i - 1];
	}

	int vmin_b = 0;
	int vmin_g = 0;
	int vmin_r = 0;
	int s1 = 3;
	int s2 = 3;

	while (histo_b[vmin_b + 1] <= N * s1 / 100)
	{
		vmin_b = vmin_b + 1;
	}
	while (histo_g[vmin_g + 1] <= N * s1 / 100)
	{
		vmin_g = vmin_g + 1;
	}
	while (histo_r[vmin_r + 1] <= N * s1 / 100)
	{
		vmin_r = vmin_r + 1;
	}

	int vmax_b = 255 - 1;
	int vmax_g = 255 - 1;
	int vmax_r = 255 - 1;

	while (histo_b[vmax_b - 1] > (N - ((N / 100) * s2)))
	{
		vmax_b = vmax_b - 1;
	}
	if (vmax_b < 255 - 1)
	{
		vmax_b = vmax_b + 1;
	}
	while (histo_g[vmax_g - 1] > (N - ((N / 100) * s2)))
	{
		vmax_g = vmax_g - 1;
	}
	if (vmax_g < 255 - 1)
	{
		vmax_g = vmax_g + 1;
	}
	while (histo_r[vmax_r - 1] > (N - ((N / 100) * s2)))
	{
		vmax_r = vmax_r - 1;
	}
	if (vmax_r < 255 - 1)
	{
		vmax_r = vmax_r + 1;
	}

	for (int i = 0; i < simg.rows; i++)
	{
		for (int j = 0; j < simg.cols; j++)
		{

			intensity = orig_img.at<cv::Vec3b>(i, j);

			if (intensity.val[0] < vmin_b)
			{
				intensity.val[0] = vmin_b;
			}
			if (intensity.val[0] > vmax_b)
			{
				intensity.val[0] = vmax_b;
			}


			if (intensity.val[1] < vmin_g)
			{
				intensity.val[1] = vmin_g;
			}
			if (intensity.val[1] > vmax_g)
			{
				intensity.val[1] = vmax_g;
			}

			if (intensity.val[2] < vmin_r)
			{
				intensity.val[2] = vmin_r;
			}
			if (intensity.val[2] > vmax_r)
			{
				intensity.val[2] = vmax_r;
			}

			orig_img.at<cv::Vec3b>(i, j) = intensity;
		}
	}

	for (int i = 0; i < simg.rows; i++)
	{
		for (int j = 0; j < simg.cols; j++)
		{

			intensity = orig_img.at<cv::Vec3b>(i, j);
			intensity.val[0] = (intensity.val[0] - vmin_b) * 255 / (vmax_b - vmin_b);
			intensity.val[1] = (intensity.val[1] - vmin_g) * 255 / (vmax_g - vmin_g);
			intensity.val[2] = (intensity.val[2] - vmin_r) * 255 / (vmax_r - vmin_r);
			orig_img.at<cv::Vec3b>(i, j) = intensity;
		}
	}


	cv::Mat blurred;
	double sigma = 1;
	double threshold = 50;
	double amount = 1;
	GaussianBlur(orig_img, blurred, cv::Size(), sigma, sigma);
	cv::Mat lowContrastMask = abs(orig_img - blurred) < threshold;
	cv::Mat sharpened = orig_img * (1 + amount) + blurred * (-amount);
	orig_img.copyTo(sharpened, lowContrastMask);
	dst = sharpened.clone();
}
