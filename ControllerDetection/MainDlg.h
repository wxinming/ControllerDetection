#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include <QScreen>

#include "ui_MainDlg.h"
#include "ProcessImage.h"
#include "SettingDlg.h"
#include "ScanCodeDlg.h"

class MainDlg : public QWidget
{
	Q_OBJECT
public:
	/*
	* @MainDlg,����
	*/
	MainDlg(QWidget* parent = Q_NULLPTR);

	/*
	* @~MainDlg,����
	*/
	~MainDlg();

	/*
	* @getLastError,��ȡ���մ���
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @initInstance,��ʼ��
	* @param1,������
	* @return,bool
	*/
	bool initialize(const QString& moduleName);

	/*
	* @changeChannelCount,�ı仭����
	* @param1,����
	* @return,bool
	*/
	bool changeFrameCount(int count = 0);

	/*
	* @getOriginSizeExceptImage,��ȡԭʼ��С����ͼ��
	* @return,void
	*/
	void getOriginSizeExceptImage();

	/*
	* @setWidgetFixedSize,���ò����̶���С
	* @param1,�ֱ��ʿ�ѡ,�����д��ʹ�������ļ��е�
	* @return,bool
	*/
	bool setWidgetFixedSize(const QString& resolution = QString());
public:
	//�Ի����Ƿ����,���ڴ������һֱ�����Ի���
	static bool m_dlgExist;

protected:
	/*
	* @setTitleModuleName,���ñ��������
	* @return,void
	*/
	void setTitleModuleName();

	/*
	* @setTitleCountDown,���ñ��⵹��ʱ
	* @param1,����
	* @param2,��
	* @return,void
	*/
	void setTitleCountDown(int minute, int second);

	/*
	* @scanCodeThread,ɨ���߳�
	* @param1,����
	* @return,void
	*/
	static void scanCodeThread(void* args);

	/*
	* @isCountDownFinished,����ʱ�Ƿ����
	* @return,bool
	*/
	bool isCountDownFinished() const;

	/*
	* @addCanMsg,���CAN����
	* @return,void
	*/
	void addCanMsg();

	/*
	* @deleteCanMsg,ɾ��CAN����
	* @return,void
	*/
	void deleteCanMsg();

	/*
	* @closeEvent,�ر��¼�
	* @param1,�¼�
	* @return,void
	*/
	void closeEvent(QCloseEvent* event);
public slots:
	/*
	* @countDownTimerSlot,����ʱ��ʱ���ۺ���
	* @return,void
	*/
	void countDownTimerSlot();

	/*
	* @startButtonSlot,��ʼ��ť��
	* @return,void
	*/
	void startButtonSlot();

	/*
	* @connectButtonSlot,���Ӱ�ť�ۺ���
	* @return,void
	*/
	void connectButtonSlot();

	/*
	* @settingButtonSlot,���ð�ť�ۺ���
	* @return,void
	*/
	void settingButtonSlot();

	/*
	* @changeChannelCountSlot,�ı�ͨ�����ۺ���
	* @param1,����
	* @return,void
	*/
	void frameCountChangedSlot(int count);

	/*
	* @changeImageSizeSlot,�ı�ͼ���С�ۺ���
	* @param1,�ֱ���
	* @return,void
	*/
	void resolutionChangedSlot(const QString& resolution);

	/*
	* @addListItemSlot ����б�0��Ŀ��
	* @param[in] channel ͨ��
	* @param[in] text �ı�
	* @return void
	*/
	void addListItemSlot(int channel, const QString& text);

	/*
	* @brief ����MES��ʾ��
	* @param[in] color ��ɫ
	* @param[in] text �ı�
	* @return void
	*/
	void updateMesHintSlot(const char* color, const QString& text);

signals:
	/*
	* @addListItemSignal,����б�0��Ŀ�ź�
	* @param1,ͨ��
	* @param2,�ı�
	* @return,void
	*/
	void addListItemSignal(int channel, const QString& text);

	/*
	* @setScanCodeDlgSignal,����ɨ��Ի����ź�
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setScanCodeDlgSignal(int channel, bool show = true);

	/*
	* @setStartButtonEnableSignal,���ÿ�ʼ��ť�����ź�
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setStartButtonEnableSignal(bool enable);

	/*
	* @applicationExitSignal,Ӧ�ó����˳��ź�
	* @return,void
	*/
	void applicationExitSignal();
private slots:
	/*
	* @setScanCodeDlgSlot,����ɨ��Ի����
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setScanCodeDlgSlot(int channel, bool show);

	/*
	* @setStartButtonEnableSlot,���ÿ�ʼ��ť���ò�
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setStartButtonEnableSlot(bool enable);

	/*
	* @detectionTimeChangedSlot,���ʱ��ı��
	* @return,void
	*/
	void detectionTimeChangedSlot();

	/*
	* @setTestResultSlot,���ò��Խ����
	* @param1,���
	* @return,void
	*/
	void setTestResultSlot(int result);

	/*
	* @resetScanSlot,����ɨ���
	* @return,void
	*/
	void resetScanSlot();

	/*
	* @reallocCapSlot,���·���ɼ�����
	* @return,void
	*/
	void reallocCapSlot();

	/*
	* @reallocCanSlot,���·���CAN����
	* @return,void
	*/
	void reallocCanSlot();

	/*
	* @setShowBinaryThreshSlot,��ʾ��ֵ����ֵ��
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setShowBinaryThreshSlot(bool show);

	/*
	* @setCanMsgEnableSlot,����CAN�������ò�
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setCanMsgEnableSlot(bool enable);
protected:
	OVERRIDE_MOUSE_EVENT;

	/*
	* @setLastError,�������մ���
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @addListItem,����б�0��Ŀ
	* @param1,ͨ��
	* @param2,�ı�
	* @return,void
	*/
	void addListItem(int channel, const QString& text);

	/*
	* @addList0Item,�����б�0��Ŀ
	* @param1,�ı�
	* @return,void
	*/
	void addList0Item(const QString& text);

	/*
	* @addList1Item,�����б�1��Ŀ
	* @param1,�ı�
	* @return,void
	*/
	void addList1Item(const QString& text);

	/*
	* @saveLog,������־
	* @param1,�Ƿ�ɹ�
	* @return,void
	*/
	bool saveLog(bool success);
private:
	Ui::MainDlgClass ui;

	//ͼ������
	ProcessImage* m_image[2] = { 0 };

	//���մ���
	QString m_lastError = "No Error";

	//������
	QString m_moduleName = "δ֪";

	//����ʱ��ʱ��
	QTimer m_countDownTimer;

	//����ʱ��
	int m_countDownSecond = 0;

	//�Ƿ��˳�
	bool m_quit = false;

	//�Ƿ�����
	bool m_connect = false;

	//�Ƿ�ɨ��
	bool m_scan = false;

	//�Ƿ�ʼ
	bool m_start = false;

	//CAN��
	std::shared_ptr<can::Base> m_can;

	//��Դ
	ItechPower m_power;

	//�豸��Ϣ
	DeviceInfo* m_deviceInfo = nullptr;

	//������Ϣ
	RectInfo* m_rectInfo = nullptr;

	//ͨ����Ϣ
	ChannelInfo* m_channelInfo = nullptr;

	//������Ϣ
	TestInfo* m_testInfo = nullptr;

	//CAN������Ϣ
	CanMsgInfo* m_canMsgInfo = nullptr;

	//������Ϣ
	BaseInfo* m_baseInfo = nullptr;

	//�˻���Ϣ
	AccountInfo* m_accountInfo = nullptr;

	//ԭʼ���
	int m_originWidth = 0;

	//ԭʼ�߶�
	int m_originHeight = 0;

	//����ͼ��֮�䲼�ֵļ��
	int m_spacing = 6;

	//ɨ��Ի���
	ScanCodeDlg m_scanCodeDlg;

	//���öԻ���
	SettingDlg* m_settingDlg = nullptr;

	//��֤�Ի���
	utility::AuthDialog m_authDlg;

	//����ɨ��
	bool m_resetScan = false;

	//�������öԻ���
	bool m_existSettingDlg = false;
};

