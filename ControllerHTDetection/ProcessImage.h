#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include "Json.h"
#include "Algorithm.h"

#define TRY_CATCH_CV(FN) try {FN;} catch(const std::exception& e){/*qDebug() << e.what();*/}

/*
* @brief,���Խ��
*/
enum TestResult 
{
	TR_OK,
	TR_NG,
	TR_NO,
	TR_TS
};

/*
* @brief,ͼ������
*/
class ProcessImage : public QObject
{
	Q_OBJECT

public:
	/*
	* @brief,����
	* @param1,������
	*/
	ProcessImage(int channel, QObject* parent = nullptr);

	/*
	* @brief,����
	*/
	~ProcessImage();

	/*
	* @brief,��ȡ���մ���
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @brief,���豸
	* @return,bool
	*/
	bool open();

	/*
	* @brief,�ر��豸
	* @return,bool
	*/
	bool close();

	/*
	* @brief,��ʼ����
	* @return,void
	*/
	void startTest();

	/*
	* @brief,ֹͣ����
	* @return,void
	*/
	void stopTest();

	/*
	* @brief,��ȡ�ɼ���
	* @return,BaseCap*
	*/
	CapBase* getCaptureCard() const;

	/*
	* @brief,���·���ɼ���
	* @return,bool
	*/
	bool reallocCap();

	/*
	* @brief,����ת������
	* @param1,�Ƿ�ת��
	* @return,void
	*/
	void setConvertProc(bool convert);
protected:
	/*
	* @brief,�������մ���
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @brief,��ȡǰ���ο�
	* @param1,��
	* @param2,��
	* @return,cv::Rect
	*/
	cv::Rect getFrontRect(int row, int column) const;

	/*
	* @brief,��ȡ����ο�
	* @param1,��
	* @param2,��
	* @return,cv::Rect
	*/
	cv::Rect getRearRect(int row, int column) const;

	/*
	* @brief,��ȡ����ο�
	* @param1,��
	* @param2,��
	* @return,cv::Rect
	*/
	cv::Rect getLeftRect(int row, int column) const;

	/*
	* @brief,��ȡ�Ҿ��ο�
	* @param1,��
	* @param2,��
	* @return,cv::Rect
	*/
	cv::Rect getRightRect(int row, int column) const;

	/*
	* @brief,����ͼ��
	* @param1,ͼ��
	* @param2,��
	* @param3,��
	* @return,void
	*/
	void processImage(cv::Mat& mat, int row, int column);

	/*
	* @brief,����б���Ŀ
	* @param1,�ı�
	* @return,void
	*/
	void addListItem(const QString& text);

	/*
	* @brief,��ȡ��������
	* @param1,����
	* @return,QString
	*/
	QString getDirectionName(int index) const;

	/*
	* @brief,��ɫ�Ƿ���ȷ
	* @param1,ԭʼ��ɫ
	* @param2,Ŀ����ɫ
	* @return,bool
	*/
	bool colorRight(const QString& srcColor, const QString& dstColor) const;

	/*
	* @brief,�ڰ�ռ���Ƿ���ȷ
	* @param1,��ɫ�ٷֱ�
	* @param2,��ɫ�ٷֱ�
	* @return,bool
	*/
	bool blackWhitePercentRight(double blackPrecent, double whitePrecent) const;

	/*
	* @brief,��ȡ��ɫ
	* @param1,����
	* @return,cv::Scalar
	*/
	cv::Scalar getColor(const QString& keyName) const;
signals:
	/*
	* @brief,����б���Ŀ�ź�
	* @param1,�ı�
	* @return,void
	*/
	void addListItemSignal(int channel, const QString& text);

private:
	//���մ���
	QString m_lastError = "δ֪����";

	//�ɼ���
	CapBase* m_capture = nullptr;

	cv::Ptr<cv::freetype::FreeType2> m_ft2;

	//�����Ϣ
	DeviceInfo* m_deviceInfo = nullptr;
	
	//������Ϣ
	RectInfo* m_rectInfo = nullptr;

	//ͨ����Ϣ
	ChannelInfo* m_channelInfo = nullptr;

	//������Ϣ
	TestInfo* m_testInfo = nullptr;

	//ָ������Ϣ
	SpecifyRowInfo* m_specifyRowInfo = nullptr;

	//ָ������Ϣ
	SpecifyColumnInfo* m_specifyColumnInfo = nullptr;

	//ͨ��
	int m_channel = 0;

	//��ʼ����
	bool m_startTest = false;
};
