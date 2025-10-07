#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include "Json.h"
#include "Algorithm.h"

#define TRY_CATCH_CV(FN) try {FN;} catch(const std::exception& e){/*qDebug() << e.what();*/}

/*
* @brief,测试结果
*/
enum TestResult 
{
	TR_OK,
	TR_NG,
	TR_NO,
	TR_TS
};

/*
* @brief,图像处理类
*/
class ProcessImage : public QObject
{
	Q_OBJECT

public:
	/*
	* @brief,构造
	* @param1,父对象
	*/
	ProcessImage(int channel, QObject* parent = nullptr);

	/*
	* @brief,析构
	*/
	~ProcessImage();

	/*
	* @brief,获取最终错误
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @brief,打开设备
	* @return,bool
	*/
	bool open();

	/*
	* @brief,关闭设备
	* @return,bool
	*/
	bool close();

	/*
	* @brief,开始测试
	* @return,void
	*/
	void startTest();

	/*
	* @brief,停止测试
	* @return,void
	*/
	void stopTest();

	/*
	* @brief,获取采集卡
	* @return,BaseCap*
	*/
	CapBase* getCaptureCard() const;

	/*
	* @brief,重新分配采集卡
	* @return,bool
	*/
	bool reallocCap();

	/*
	* @brief,设置转换处理
	* @param1,是否转换
	* @return,void
	*/
	void setConvertProc(bool convert);
protected:
	/*
	* @brief,设置最终错误
	* @param1,错误信息
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @brief,获取前矩形框
	* @param1,行
	* @param2,列
	* @return,cv::Rect
	*/
	cv::Rect getFrontRect(int row, int column) const;

	/*
	* @brief,获取后矩形框
	* @param1,行
	* @param2,列
	* @return,cv::Rect
	*/
	cv::Rect getRearRect(int row, int column) const;

	/*
	* @brief,获取左矩形框
	* @param1,行
	* @param2,列
	* @return,cv::Rect
	*/
	cv::Rect getLeftRect(int row, int column) const;

	/*
	* @brief,获取右矩形框
	* @param1,行
	* @param2,列
	* @return,cv::Rect
	*/
	cv::Rect getRightRect(int row, int column) const;

	/*
	* @brief,处理图像
	* @param1,图像
	* @param2,行
	* @param3,列
	* @return,void
	*/
	void processImage(cv::Mat& mat, int row, int column);

	/*
	* @brief,添加列表项目
	* @param1,文本
	* @return,void
	*/
	void addListItem(const QString& text);

	/*
	* @brief,获取方向名称
	* @param1,索引
	* @return,QString
	*/
	QString getDirectionName(int index) const;

	/*
	* @brief,颜色是否正确
	* @param1,原始颜色
	* @param2,目标颜色
	* @return,bool
	*/
	bool colorRight(const QString& srcColor, const QString& dstColor) const;

	/*
	* @brief,黑白占比是否正确
	* @param1,黑色百分比
	* @param2,白色百分比
	* @return,bool
	*/
	bool blackWhitePercentRight(double blackPrecent, double whitePrecent) const;

	/*
	* @brief,获取颜色
	* @param1,键名
	* @return,cv::Scalar
	*/
	cv::Scalar getColor(const QString& keyName) const;
signals:
	/*
	* @brief,添加列表项目信号
	* @param1,文本
	* @return,void
	*/
	void addListItemSignal(int channel, const QString& text);

private:
	//最终错误
	QString m_lastError = "未知错误";

	//采集卡
	CapBase* m_capture = nullptr;

	cv::Ptr<cv::freetype::FreeType2> m_ft2;

	//设别信息
	DeviceInfo* m_deviceInfo = nullptr;
	
	//矩形信息
	RectInfo* m_rectInfo = nullptr;

	//通道信息
	ChannelInfo* m_channelInfo = nullptr;

	//测试信息
	TestInfo* m_testInfo = nullptr;

	//指定行信息
	SpecifyRowInfo* m_specifyRowInfo = nullptr;

	//指定列信息
	SpecifyColumnInfo* m_specifyColumnInfo = nullptr;

	//通道
	int m_channel = 0;

	//开始测试
	bool m_startTest = false;
};
