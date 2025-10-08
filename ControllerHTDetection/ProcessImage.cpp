#include "ProcessImage.h"

ProcessImage::ProcessImage(int channel, QObject* parent)
	:m_channel(channel), QObject(parent)
{
	m_deviceInfo = GET_JSON->getDeviceInfo();

	m_rectInfo = GET_JSON->getRectInfo();

	m_channelInfo = GET_JSON->getChannelInfo();

	m_testInfo = GET_JSON->getTestInfo();

	m_specifyRowInfo = GET_JSON->getSpecifyRowInfo();

	m_specifyColumnInfo = GET_JSON->getSpecifyColumnInfo();

	m_capture = CaptureCard::allocCap(m_deviceInfo->captureName);

	m_ft2 = cv::freetype::createFreeType2();

	m_ft2->loadFontData("C:\\Windows\\Fonts\\simhei.ttf", 0);
}

ProcessImage::~ProcessImage()
{
	CaptureCard::freeCap(m_capture);
}

QString ProcessImage::getLastError() const
{
	return m_lastError;
}

bool ProcessImage::open()
{
	bool result = false;
	do
	{
		QSize size;

		RUN_BREAK(!GET_JSON->getResolution(size), GET_JSON->getLastError());

		RUN_BREAK(!m_capture->open(m_channel), m_capture->getLastError());

#ifdef QT_DEBUG
		m_capture->setImageOriginalSize(640, 480);
#endif

		m_capture->setImageScaleSize(size.width(), size.height());

		m_capture->setImageProc([&](cv::Mat& mat)->void
			{
				for (int row = 0; row < m_rectInfo->rows; ++row)
					for (int column = 0; column < m_rectInfo->columns; ++column)
						TRY_CATCH_CV(processImage(mat, row, column));
			});

		m_capture->startCapture();
		result = true;
	} while (false);
	return result;
}

bool ProcessImage::close()
{
	m_capture->stopCapture();
	m_capture->close();
	m_capture->setImageProc(nullptr);
	return true;
}

void ProcessImage::startTest()
{
	m_startTest = true;
}

void ProcessImage::stopTest()
{
	m_startTest = false;
}

CapBase* ProcessImage::getCaptureCard() const
{
	return m_capture;
}

bool ProcessImage::reallocCap()
{
	CaptureCard::freeCap(m_capture);
	m_capture = CaptureCard::allocCap(m_deviceInfo->captureName);
	return m_capture != nullptr;
}

void ProcessImage::setConvertProc(bool convert)
{
	m_capture->setConvertProc(convert ? [&](const cv::Mat& mat)->QImage {
		cv::Mat gray, thresh;
		cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
		cv::threshold(gray, thresh, m_testInfo->binaryThresh.toFloat(), 0xff, 0);
		return QImage(thresh.data, thresh.cols, thresh.rows, QImage::Format_Indexed8).
			copy(0, 0, thresh.cols, thresh.rows);
		}:static_cast<std::function<QImage(const cv::Mat& mat)>>(nullptr));
}

void ProcessImage::setLastError(const QString& error)
{
	m_lastError = error;
}

//系数宽高
static const int coew = 10;
static const int coeh = 15;

cv::Rect ProcessImage::getFrontRect(int row, int column) const
{
	const int x = column
		* m_rectInfo->overallLRInterval
		+ m_rectInfo->overallXOffset
		+ coew
		+ m_rectInfo->frontX
		+ m_specifyRowInfo[row].x
		+ m_specifyColumnInfo[column].x;

	const int y = row
		* m_rectInfo->overallFRInterval
		- m_rectInfo->internalFrontInterval
		+ m_rectInfo->overallYOffset
		+ m_rectInfo->frontY
		+ m_specifyRowInfo[row].y
		+ m_specifyColumnInfo[column].y;

	return cv::Rect(x, y, m_rectInfo->frontWidth, m_rectInfo->frontHeight);
}

cv::Rect ProcessImage::getRearRect(int row, int column) const
{
	const int x = column
		* m_rectInfo->overallLRInterval
		+ m_rectInfo->overallXOffset
		+ coew
		+ m_rectInfo->rearX
		+ m_specifyRowInfo[row].x
		+ m_specifyColumnInfo[column].x;

	const int y = row
		* m_rectInfo->overallFRInterval
		+ m_rectInfo->internalRearInterval
		+ m_rectInfo->overallYOffset
		+ coeh
		* 2
		+ m_rectInfo->rearY
		+ m_specifyRowInfo[row].y
		+ m_specifyColumnInfo[column].y;

	return cv::Rect(x, y, m_rectInfo->rearWidth, m_rectInfo->rearHeight);
}

cv::Rect ProcessImage::getLeftRect(int row, int column) const
{
	const int x = column
		* m_rectInfo->overallLRInterval
		+ m_rectInfo->overallXOffset
		- m_rectInfo->internalLeftInterval
		+ m_rectInfo->leftX
		+ m_specifyRowInfo[row].x
		+ m_specifyColumnInfo[column].x;

	const int y = row
		* m_rectInfo->overallFRInterval
		+ m_rectInfo->overallYOffset
		+ coeh
		+ m_rectInfo->leftY
		+ m_specifyRowInfo[row].y
		+ m_specifyColumnInfo[column].y;

	return cv::Rect(x, y, m_rectInfo->leftWidth, m_rectInfo->leftHeight);
}

cv::Rect ProcessImage::getRightRect(int row, int column) const
{
	const int x = column
		* m_rectInfo->overallLRInterval
		+ m_rectInfo->overallXOffset
		+ coew
		* 2
		+ m_rectInfo->internalRightInterval
		+ m_rectInfo->rightX
		+ m_specifyRowInfo[row].x
		+ m_specifyColumnInfo[column].x;

	const int y = row
		* m_rectInfo->overallFRInterval
		+ m_rectInfo->overallYOffset
		+ coeh
		+ m_rectInfo->rightY
		+ m_specifyRowInfo[row].y
		+ m_specifyColumnInfo[column].y;

	return cv::Rect(x, y, m_rectInfo->rightWidth, m_rectInfo->rightHeight);
}

void ProcessImage::processImage(cv::Mat& mat, int row, int column)
{
	const QList<cv::Rect> rectList =
	{
		getFrontRect(row,column),
		getRearRect(row,column),
		getLeftRect(row,column),
		getRightRect(row,column)
	};

	const QList<cv::Scalar> scalarList =
	{
		getColor("前矩形框"),
		getColor("后矩形框"),
		getColor("左矩形框"),
		getColor("右矩形框")
	};

	const int x = (rectList[0].x + rectList[1].x) / 2;
	const int y = (rectList[2].y + rectList[3].y) / 2 + (rectList[2].height + rectList[3].height) / 4;
	const int index = GET_JSON->getIndex(row + (m_channel * m_rectInfo->rows), column) - 1;

	auto& channel = m_channelInfo[index];
	if (channel.enable)
	{
		const QStringList directionList =
		{
			m_testInfo->frontColor,
			m_testInfo->rearColor,
			m_testInfo->leftColor,
			m_testInfo->rightColor
		};

		const double binThresh = m_testInfo->binaryThresh.toDouble();
		const double dynThresh = m_testInfo->dynamicThresh.toDouble();
		const double dynPercent = m_testInfo->dynamicPercent.toDouble();
		const int dynDetect = m_testInfo->dynamicDetection.toInt();
		const QString& algorithm = m_testInfo->imageAlgorithm;
		const int determine = m_testInfo->determineCount.toInt();
		const int dynFrequency = m_testInfo->dynamicFrequency.toInt();

		if (dynDetect && (channel.directionResult[0] ||
			channel.directionResult[1] ||
			channel.directionResult[2] ||
			channel.directionResult[3]))
		{
			if (GetTickCount64() - channel.dynamicTime > dynFrequency)
			{
				QList<cv::Rect> sortList = rectList;
				std::sort(sortList.begin(), sortList.end(), [](cv::Rect& a, cv::Rect& b) {return a.x < b.x; });
				const int x1 = sortList[0].x;
				std::sort(sortList.begin(), sortList.end(), [](cv::Rect& a, cv::Rect& b) {return a.y < b.y; });
				const int y1 = sortList[0].y;

				std::sort(sortList.begin(), sortList.end(), [](cv::Rect& a, cv::Rect& b) {return a.x > b.x; });
				const int x2 = sortList[0].x + sortList[0].width;
				std::sort(sortList.begin(), sortList.end(), [](cv::Rect& a, cv::Rect& b) {return a.y > b.y; });
				const int y2 = sortList[0].y + sortList[0].height;

				const cv::Rect rect(x1, y1, x2 - x1, y2 - y1);

				cv::Mat cur;
				mat(rect).copyTo(cur);
				cv::Mat* dyn = channel.dynamicMat;
				cv::cvtColor(cur, cur, CV_BGR2GRAY);

				if (dyn->empty())
				{
					cur.copyTo(*dyn);
					return;
				}

				Algorithm::getDynamicValue(cur, *dyn, dynThresh, channel.dynamicSum, channel.dynamicPercent);
				cur.copyTo(*dyn);

				if (m_startTest && channel.dynamicPercent <= dynPercent)
				{
					if (++channel.dynamicCount >= determine)
					{
						memset(channel.directionResult, 0, sizeof(channel.directionResult));
						addListItem(Q_SPRINTF("通道%02d,图像动态NG,动态占比[%.2lf%%/%.2lf%%]", index + 1,
							channel.dynamicPercent, dynPercent));
						channel.finalResult = false;
					}
				}

				//const int sum = Algorithm::getImageDataSum(mat, rect);
				//int& previous = channel.dynamicSum;

				//const int max = std::max<int>(sum, previous);
				//const int min = std::min<int>(sum, previous);

				////防止除0异常,同时相加100
				//channel.dynamicPercent = (float(min + 100) / float(max + 100)) * 100;

				//if (m_startTest && channel.dynamicPercent >= m_testInfo->dynamicPercent.toFloat())
				//{
				//	if (++channel.dynamicCount >= determine)
				//	{
				//		for (int x = 0; x < MAX_RECT_COUNT; ++x)
				//		{
				//			channel.directionResult[x] = false;
				//		}
				//		addListItem(Q_SPRINTF("通道%02d,图像动态NG,动态占比[%.2lf%%/%.2lf%%]", index + 1,
				//			channel.dynamicPercent,
				//			m_testInfo->dynamicPercent.toFloat()));
				//		channel.finalResult = false;
				//	}
				//}
				//previous = sum;
				channel.dynamicTime = GetTickCount64();
			}
		}

		for (int i = 0; i < rectList.size(); i++)
		{
			QString colorName;
			bool right = false;
			double black = 0, white = 0;

			if (algorithm == "黑白占比")
			{
				Algorithm::getBlackWhitePrecent(mat, rectList[i], binThresh, &black, &white);
				right = blackWhitePercentRight(black, white);
			}
			else if (algorithm == "色彩模型")
			{
				Algorithm::hsvGetColor(mat, rectList[i], colorName);
				right = colorRight(colorName, directionList[i]);
			}

			if (m_startTest && !right && channel.directionResult[i])
			{
				if (++channel.determineCount[i] >= determine)
				{
					channel.directionResult[i] = false;
					channel.finalResult = false;
					if (algorithm == "黑白占比")
					{
						addListItem(Q_SPRINTF("通道%02d,%s景检测NG,黑白占比[%.2lf%%/%.2lf%%]",
							index + 1, Q_TO_C_STR(getDirectionName(i)), black, white));
					}
					else if (algorithm == "色彩模型")
					{
						addListItem(Q_SPRINTF("通道%02d,%s景检测NG,色彩模型[%s色]",
							index + 1, Q_TO_C_STR(getDirectionName(i)), Q_TO_C_STR(colorName)));
					}
				}
			}

			if (m_rectInfo->showRect)
			{
				cv::rectangle(mat, rectList[i], scalarList[i]);
			}

			bool success = channel.directionResult[i];

			cv::Size&& size = cv::getTextSize(OK_NG(success), 0, 0.3, 1, nullptr);
			cv::Point origin;
			origin.x = rectList[i].x;
			origin.y = rectList[i].y + size.height * 2;

			if (success)
			{
				if (m_rectInfo->showText)
				{
					if (algorithm == "黑白占比")
					{
						std::string&& text0 = Q_SPRINTF("%.0lf", black).toStdString();
						cv::Size&& size = cv::getTextSize(text0, 0, 0.3, 1, nullptr);
						origin.y = rectList[i].y + size.height + 1;
						cv::putText(mat, text0, origin, 0, 0.3, getColor("文本字体"));
						origin.y += (size.height + 3);
						std::string&& text1 = Q_SPRINTF("%.0lf", white).toStdString();
						cv::putText(mat, text1, origin, 0, 0.3, getColor("文本字体"));
					}
					else if (algorithm == "色彩模型")
					{
						m_ft2->putText(mat, colorName.toStdString(), origin, 15, getColor("文本字体"), -1, 8, 1);
					}

					if (dynDetect)
					{
						cv::Point origin(rectList[0].x, rectList[0].y + rectList[0].height + 10);
						std::string&& text = S_SPRINTF("%.2lf", channel.dynamicPercent);
						cv::putText(mat, text, origin, 0, 0.3, getColor("文本字体"));
					}
				}
			}
			else
			{
				cv::putText(mat, "NG", origin, 0, 0.4, CV_RGB(255, 0, 0));
			}
		}
	}
	else
	{
		if (m_rectInfo->showDisable)
		{
			const int x = rectList[2].x;
			const int y = rectList[0].y;
			const int width = rectList[3].x + rectList[3].width - rectList[2].x;
			const int height = rectList[1].y + rectList[1].height - rectList[0].y;
			const cv::Rect rect = { x, y, width, height };
			cv::rectangle(mat, rect, getColor("禁用矩形框"), -1);

			const int leftX = rectList[2].x;
			const int leftY = rectList[0].y;

			const int rightX = rectList[3].x + rectList[3].width;
			const int rightY = rectList[1].y + rectList[1].height;
			cv::line(mat, { leftX, leftY }, { rightX, rightY }, getColor("禁用线条"), 2);
			cv::line(mat, { rightX, leftY }, { leftX, rightY }, getColor("禁用线条"), 2);
		}
	}

	if (m_rectInfo->showChannelId)
	{
		const double widthAvg = (double(m_rectInfo->frontWidth) + double(m_rectInfo->rearWidth)) / 2;
		const double fontScale = widthAvg / 20;
		const int thickness = 2;
		std::string&& text = std::to_string(index + 1);
		cv::Size&& size = cv::getTextSize(text, 0, fontScale, thickness, nullptr);

		cv::Scalar scalar;
		if (!channel.finalResult && channel.flickerSwitch && !m_startTest)
		{
			if (channel.flickerTime)
			{
				if (GetTickCount64() - channel.flickerTime > ULONGLONG(60000 * 30))
				{
					channel.flickerSwitch = false;
				}
			}
			else
			{
				channel.flickerTime = GetTickCount64();
			}
			scalar = channel.flickerSwap ? CV_RGB(255, 0, 0) : CV_RGB(255, 255, 0);
			channel.flickerSwap = !channel.flickerSwap;
		}
		else
		{
			scalar = getColor("通道字体");
		}
		cv::putText(mat, text, cv::Point(x, y + size.height / 2), 0, fontScale, scalar, thickness);
	}
	return;
}

void ProcessImage::addListItem(const QString& text)
{
	emit addListItemSignal(m_channel, text);
	return;
}

QString ProcessImage::getDirectionName(int index) const
{
	QString result;
	switch (index)
	{
	case 0:result = "前"; break;
	case 1:result = "后"; break;
	case 2:result = "左"; break;
	case 3:result = "右"; break;
	default:result = "未"; break;
	}
	return result;
}

bool ProcessImage::colorRight(const QString& srcColor, const QString& dstColor) const
{
	bool result = false;

	QStringList split;
	QString color = dstColor;
	bool equal = false;
	if (dstColor.contains("!="))
	{
		color.remove("!=");
		equal = false;
	}
	else
	{
		color.remove("==");
		equal = true;
	}

	split = color.split(",", QString::SkipEmptyParts);
	if (split.size())
	{
		for (auto& x : split)
		{
			if (equal)
			{
				if (x == srcColor)
				{
					result = true;
				}
			}
			else
			{
				if (x != srcColor)
				{
					result = true;
				}
			}
		}
	}
	else
	{
		if (equal)
		{
			if (color == srcColor)
			{
				result = true;
			}
		}
		else
		{
			if (color != srcColor)
			{
				result = true;
			}
		}
	}

	return result;
}

bool ProcessImage::blackWhitePercentRight(double blackPrecent, double whitePrecent) const
{
	bool result = false, success = false;
	do
	{
		const float value1 = m_testInfo->blackPercent.toFloat();
		const float value2 = m_testInfo->whitePercent.toFloat();

		if (blackPrecent >= value1 && whitePrecent >= value2)
		{
			success = true;
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

cv::Scalar ProcessImage::getColor(const QString& keyName) const
{
	RgbInfo&& rgbInfo = GET_JSON->getRgbInfoValue(keyName);
	return CV_RGB(rgbInfo.r, rgbInfo.g, rgbInfo.b);
}
