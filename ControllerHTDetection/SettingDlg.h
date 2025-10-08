#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_SettingDlg.h"
#include "Json.h"

#include <Serial/Serial.h>
#pragma comment(lib, "Serial.lib")

class SettingDlg : public QWidget
{
	Q_OBJECT
public:
	/*
	* @brief,����
	* @param1,������
	* @param2,�Ƿ�����
	* @param3,������
	*/
	SettingDlg(const QString& moduleName, bool connected, bool* exist, QWidget* parent = Q_NULLPTR);

	/*
	* @brief,����
	*/
	~SettingDlg();

	/*
	* @brief,��ʼ��
	* @return,bool
	*/
	bool initialize();

	//��ť�ۺ���
public slots:
	/*
	* @brief,���水ť�ۺ���
	* @return,void
	*/
	void saveButtonSlot();

	/*
	* @brief,�˳���ť�ۺ���
	* @return,void
	*/
	void exitButtonSlot();

	//���βۺ���
protected slots:
	/*
	* @brief,������Ͽ�ۺ���
	* @param1,��������ı������ֵ
	* @return,void
	*/
	void rowCountComboChangedSlot(const QString& text);

	/*
	* @brief,������Ͽ�ۺ���
	* @param1,��������ı������ֵ
	* @return,void
	*/
	void columnCountComboChangedSlot(const QString& text);

	/*
	* @brief,����ǰ����X���
	* @param1,xֵ
	* @return,void
	*/
	void setFrontRectXSlot(int x);

	/*
	* @brief,����ǰ����Y���
	* @param1,yֵ
	* @return,void
	*/
	void setFrontRectYSlot(int y);

	/*
	* @brief,����ǰ���ο���
	* @param1,���
	* @retrun,void
	*/
	void setFrontRectWidthSlot(int width);

	/*
	* @brief,����ǰ���ο�߶�
	* @param1,�߶�
	* @return,vodi
	*/
	void setFrontRectHeightSlot(int height);

	/*
	* @brief,���ú����X���
	* @param1,xֵ
	* @return,void
	*/
	void setRearRectXSlot(int x);

	/*
	* @brief,���ú����Y���
	* @param1,yֵ
	* @return,void
	*/
	void setRearRectYSlot(int y);

	/*
	* @brief,���ú���ο�Ȳ�
	* @param1,���
	* @return,void
	*/
	void setRearRectWidthSlot(int width);

	/*
	* @brief,���ú���θ߶Ȳ�
	* @param1,�߶�
	* @return,void
	*/
	void setRearRectHeightSlot(int height);

	/*
	* @brief,���������x���
	* @param1,xֵ
	* @return,void
	*/
	void setLeftRectXSlot(int x);

	/*
	* @brief,���������y���
	* @param1,yֵ
	* @return,void
	*/
	void setLeftRectYSlot(int y);

	/*
	* @brief,��������ο�Ȳ�
	* @param1,���
	* @return,void
	*/
	void setLeftRectWidthSlot(int width);

	/*
	* @brief,��������θ߶Ȳ�
	* @param1,�߶�
	* @return,void
	*/
	void setLeftRectHeightSlot(int height);

	/*
	* @brief,�����Ҿ���x���
	* @param1,xֵ
	* @return,void
	*/
	void setRightRectXSlot(int x);

	/*
	* @brief,�����Ҿ���y���
	* @param1,yֵ
	* @return,void
	*/
	void setRightRectYSlot(int y);

	/*
	* @brief,�����Ҿ��ο�Ȳ�
	* @param1,���
	* @return,void
	*/
	void setRightRectWidthSlot(int width);

	/*
	* @brief,�����Ҿ��θ߶Ȳ�
	* @param1,�߶�
	* @return,void
	*/
	void setRightRectHeightSlot(int height);

	/*
	* @brief,ͨ������
	* @param1,����
	* @return,void
	*/
	void channelAlignComboSlot(int index);

	/*
	* @brief,��������Xƫ��
	* @param1,xƫ����
	* @return,void
	*/
	void setOverallXOffsetSlot(int offset);

	/*
	* @brief,��������Yƫ��
	* @param1,yƫ����
	* @return,void
	*/
	void setOverallYOffsetSlot(int offset);

	/*
	* @brief,��������ǰ����
	* @param1,���
	* @return,void
	*/
	void setOverallFRIntervalSlot(int interval);

	/*
	* @brief,�����������Ҽ��
	* @param1,���
	* @return,void
	*/
	void setOverallLRIntervalSlot(int interval);

	/*
	* @brief,�����ڲ�ǰ���
	* @param1,���
	* @return,void
	*/
	void setInternalFrontIntervalSlot(int interval);

	/*
	* @brief,�����ڲ�����
	* @param1,���
	* @return,void
	*/
	void setInternalRearIntervalSlot(int interval);

	/*
	* @brief,�����ڲ�����
	* @param1,���
	* @return,void
	*/
	void setInternalLeftIntervalSlot(int interval);

	/*
	* @brief,�����ڲ��Ҽ��
	* @param1,���
	* @return,void
	*/
	void setInternalRightIntervalSlot(int interval);

	/*
	* @brief,����ͨ����Ų�
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowChannelIdSlot(bool checked);

	/*
	* @brief,������ʾ�ı�
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowTextSlot(bool checked);

	/*
	* @brief,������ʾ����
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowRectSlot(bool checked);

	/*
	* @brief,������ʾ���ò�
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowDisableSlot(bool checked);

	/*
	* @brief,������ʾ��ֵ����ֵ
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowBinaryThreshSlot(bool checked);

	//�豸�ۺ���
protected slots:
	/*
	* @brief,ͼ��ֱ�����Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void imageResolutionComboSlot(const QString& text);

	/*
	* @brief,�ɼ���������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void captureNameComboSlot(const QString& text);

	/*
	* @brief,���������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void frameCountComboSlot(const QString& text);

	/*
	* @brief,��Դ������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void powerTypeComboSlot(const QString& text);

	/*
	* @brief,��Դ��ѹ��Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void powerVoltageComboSlot(const QString& text);

	/*
	* @brief,��Դ������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void powerCurrentComboSlot(const QString& text);

	/*
	* @brief,��Դ�Ͽ���Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void powerPortComboSlot(const QString& text);

	/*
	* @brief,��Դ��������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void powerBuadrateSlot(const QString& text);

	/*
	* @brief,CAN������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void canNameComboSlot(const QString& text);

	/*
	* @brief,CAN��������Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void canBaudrateComboSlot(const QString& text);

	/*
	* @brief,CAN��չ֡��Ͽ��
	* @param1,��ǰ�ı�
	* @return,void
	*/
	void canExpandFrameComboSlot(const QString& text);

	//ͨ���ۺ���
protected slots:
	/*
	* @brief,ͨ��״̬�ı��
	* @param1,״̬
	* @return,void
	*/
	void channelStateChangedSlot(int state);

	/*
	* @brief,ѡ������ͨ����
	* @param1,״̬
	* @return,void
	*/
	void chooseAllChannelSlot(int state);

	//CAN���Ĳۺ���
protected slots:
	/*
	* @brief,CAN����ID��
	* @return,void
	*/
	void canMsgIdSlot();

	/*
	* @brief,CAN�������ݲ�
	* @return,void
	*/
	void canMsgDataSlot();

	/*
	* @brief,CAN���ļ����
	* @return,void
	*/
	void canMsgIntervalSlot();

	/*
	* @brief,CAN���Ĵ�����
	* @return,void
	*/
	void canMsgTimesSlot();

	/*
	* @brief,CAN�������ò�
	* @return,void
	*/
	void canMsgEnableSlot(int state);

protected slots:
	/*
	* @brief,ͼ���㷨��
	* @param1,�ı�����
	* @return,void
	*/
	void imageAlgorithmSlot(const QString& text);

	/*
	* @brief,���ʱ���
	* @return,void
	*/
	void detectionTimeSlot();

	/*
	* @brief,�ϵ�ȴ���
	* @return,void
	*/
	void powerOnWaitSlot();

	/*
	* @brief,ǰ����ɫ��
	* @return,void
	*/
	void frontColorSlot();

	/*
	* @brief,����ɫ��
	* @return,void
	*/
	void rearColorSlot();

	/*
	* @brief,����ɫ��
	* @return,void
	*/
	void leftColorSlot();

	/*
	* @brief,�Ҿ���ɫ��
	* @return,void
	*/
	void rightColorSlot();

	/*
	* @brief,�ж�������
	* @reutrn,void
	*/
	void determineCountSlot();

	/*
	* @brief,��ɫ�ٷֱȲ�
	* @return,void
	*/
	void blackPercentSlot();

	/*
	* @brief,��ɫ�ٷֱȲ�
	* @return,void
	*/
	void whitePercentSlot();

	/*
	* @brief,��������ֵ��
	* @return,void
	*/
	void binaryThreshSlot();

	/*
	* @brief,��̬ռ�Ȳ�
	* @return,void
	*/
	void dynamicPercentSlot();

	/*
	* @brief,��̬��ֵ��
	* @return,void
	*/
	void dynamicThreshSlot();

	/*
	* @brief,��̬Ƶ�ʲ�
	* @return,void
	*/
	void dynamicFrequencySlot();

	/*
	* @brief,��̬����
	* @return,void
	*/
	void dynamicDetectionSlot(int state);

	/*
	* @brief,��������
	* @return,void
	*/
	void skipDetectionSlot(int state);

protected slots:
	/*
	* @brief,�����жϲ�
	* @return,void
	*/
	void codeJudgeSlot();

	/*
	* @brief,���볤�Ȳ�
	* @return,void
	*/
	void codeLengthSlot();

	/*
	* @brief,�����жϲ�
	* @param1,״̬
	* @return,void
	*/
	void enableJudgeSlot(int state);

	/*
	* @brief,�����ϴ���
	* @param1,״̬
	* @return,void
	*/
	void enableUploadSlot(int state);

	/*
	* @brief,�û�����
	* @return,void
	*/
	void userNameSlot();

	/*
	* @brief,�û������
	* @return,void
	*/
	void userPasswordSlot();

protected slots:
	/*
	* @brief,ָ���о�����Ͽ��
	* @param1,�ı�
	* @return,void
	*/
	void specifyRowRectComboSlot(const QString& text);

	/*
	* @brief,ָ����Xƫ�Ʋ�
	* @param1,x
	* @return,void
	*/
	void specifyRowXOffsetHSliderSlot(int x);

	/*
	* @brief,ָ����Yƫ�Ʋ�
	* @param1,y
	* @return,void
	*/
	void specifyRowYOffsetHSliderSlot(int y);

	/*
	* @brief,ָ���о�����Ͽ��
	* @param1,�ı�
	* @return,void
	*/
	void specifyColumnRectComboSlot(const QString& text);

	/*
	* @brief,ָ����Xƫ�Ʋ�
	* @param1,x
	* @return,void
	*/
	void specifyColumnXOffsetHSliderSlot(int x);

	/*
	* @brief,ָ����Yƫ�Ʋ�
	* @param1,x
	* @return,void
	*/
	void specifyColumnYOffsetHSliderSlot(int y);

protected slots:
	/*
	* @brief,ѡ����ɫ��
	* @return,void
	*/
	void chooseColorSlot();

	/*
	* @brief,��ԭĬ����ɫ��
	* @return,void
	*/
	void restoreDefaultColorSlot();

signals:
	/*
	* @brief,��������ı��ź�
	* @param1,����ʵ�ʸ���
	* @return,void
	*/
	void frameCountChangedSignal(int count);

	/*
	* @brief,�ֱ��ʸı��ź�
	* @param1,�ֱ���(640*480)
	* @return,void
	*/
	void resolutionChangedSignal(const QString& resolution);

	/*
	* @brief,���ʱ��ı��ź�
	* @return,void
	*/
	void detectionTimeChangedSignal();

	/*
	* @brief,����ɨ���ź�
	* @return,void
	*/
	void resetScanSignal();

	/*
	* @brief,���·���ɼ����ź�
	* @return,void
	*/
	void reallocCapSignal();

	/*
	* @brief,���·���CAN����Ϣ
	* @return,void
	*/
	void reallocCanSignal();

	/*
	* @brief,��ʾ��ֵ����ֵ�ź�
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowBinaryThreshSignal(bool show);

	/*
	* @brief,����CAN���������ź�
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setCanMsgEnableSignal(bool enable);
protected:
	//��ʼ���ؼ�
	void initControl();

	//��ʼ���豸��Ϣ
	void initDeviceInfo();

	//��ʼ��������Ϣ
	void initRectInfo();

	//��ʼ��ͨ����Ϣ
	void initChannelInfo();

	//����ͨ������
	void setChannelWidget();

	//��ʼ��CAN������Ϣ
	void initCanMsgInfo();

	//��ʼ��������Ϣ
	void initTestInfo();

	//��ʼ��������Ϣ
	void initBaseInfo();

	//��ɫ��Ϣ�����Ƿ���ȷ
	bool isColorInfoInputRight(const QString& color);

	//��ʼ��ָ����Ϣ
	void initSpecifyInfo();

	//��ʼ��������Ϣ
	void initShareInfo();
private:
	Ui::SettingDlg ui;

	//�豸��Ϣָ��
	DeviceInfo* m_deviceInfo = nullptr;

	//�豸��Ϣ����
	DeviceInfo m_deviceInfoBackup;

	//������Ϣָ��
	RectInfo* m_rectInfo = nullptr;

	//������Ϣ����
	RectInfo m_rectInfoBackup = {0};

	//ͨ����Ϣָ��
	ChannelInfo* m_channelInfo = nullptr;

	//ͨ����Ϣ����
	ChannelInfo m_channelInfoBackup[MAX_CHANNEL_COUNT];

	//CAN������Ϣָ��
	CanMsgInfo* m_canMsgInfo = nullptr;

	//CAN������Ϣ����
	CanMsgInfo m_canMsgInfoBackup;

	//������Ϣָ��
	TestInfo* m_testInfo = nullptr;

	//������Ϣ
	BaseInfo* m_baseInfo = nullptr;

	//�Ƿ񱣴��ļ�
	bool m_saveFile = false;

	//������
	QString m_moduleName = "Unknown";

	//�Ƿ�����
	bool m_connect = false;

	//��ѡ���б�,���ڴ���ȫѡ/��ѡ
	QList<QCheckBox*> m_channelList;

	//ͨ���Ѹı�
	bool m_channelChanged = false;

	//ָ������Ϣ
	SpecifyRowInfo* m_specifyRowInfo = nullptr;

	//ָ������Ϣ����
	SpecifyRowInfo m_specifyRowInfoBackup[MAX_ROW_COUNT] = { 0 };

	//ָ������Ϣ
	SpecifyColumnInfo* m_specifyColumnInfo = nullptr;

	//ָ������Ϣ����
	SpecifyColumnInfo m_specifyColumnInfoBackup[MAX_COLUMN_COUNT] = { 0 };

	//�˻���Ϣ
	AccountInfo* m_accountInfo = nullptr;

	//��ɫ��Ϣ
	ColorInfo* m_colorInfo = nullptr;

	//��ɫ��Ϣ����
	ColorInfo m_colorInfoBackup;

	//�������
	bool* m_selfExist = nullptr;
};
