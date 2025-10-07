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
	* @MainDlg,构造
	*/
	MainDlg(QWidget* parent = Q_NULLPTR);

	/*
	* @~MainDlg,析构
	*/
	~MainDlg();

	/*
	* @getLastError,获取最终错误
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @initInstance,初始化
	* @param1,机种名
	* @return,bool
	*/
	bool initialize(const QString& moduleName);

	/*
	* @changeChannelCount,改变画面数
	* @param1,数量
	* @return,bool
	*/
	bool changeFrameCount(int count = 0);

	/*
	* @getOriginSizeExceptImage,获取原始大小除了图像
	* @return,void
	*/
	void getOriginSizeExceptImage();

	/*
	* @setWidgetFixedSize,设置部件固定大小
	* @param1,分辨率可选,如果不写则使用配置文件中的
	* @return,bool
	*/
	bool setWidgetFixedSize(const QString& resolution = QString());
public:
	//对话框是否存在,用于处理避免一直开启对话框
	static bool m_dlgExist;

protected:
	/*
	* @setTitleModuleName,设置标题机种名
	* @return,void
	*/
	void setTitleModuleName();

	/*
	* @setTitleCountDown,设置标题倒计时
	* @param1,分钟
	* @param2,秒
	* @return,void
	*/
	void setTitleCountDown(int minute, int second);

	/*
	* @scanCodeThread,扫码线程
	* @param1,参数
	* @return,void
	*/
	static void scanCodeThread(void* args);

	/*
	* @isCountDownFinished,倒计时是否结束
	* @return,bool
	*/
	bool isCountDownFinished() const;

	/*
	* @addCanMsg,添加CAN报文
	* @return,void
	*/
	void addCanMsg();

	/*
	* @deleteCanMsg,删除CAN报文
	* @return,void
	*/
	void deleteCanMsg();

	/*
	* @closeEvent,关闭事件
	* @param1,事件
	* @return,void
	*/
	void closeEvent(QCloseEvent* event);
public slots:
	/*
	* @countDownTimerSlot,倒计时计时器槽函数
	* @return,void
	*/
	void countDownTimerSlot();

	/*
	* @startButtonSlot,开始按钮槽
	* @return,void
	*/
	void startButtonSlot();

	/*
	* @connectButtonSlot,连接按钮槽函数
	* @return,void
	*/
	void connectButtonSlot();

	/*
	* @settingButtonSlot,设置按钮槽函数
	* @return,void
	*/
	void settingButtonSlot();

	/*
	* @changeChannelCountSlot,改变通道数槽函数
	* @param1,数量
	* @return,void
	*/
	void frameCountChangedSlot(int count);

	/*
	* @changeImageSizeSlot,改变图像大小槽函数
	* @param1,分辨率
	* @return,void
	*/
	void resolutionChangedSlot(const QString& resolution);

	/*
	* @addListItemSlot 添加列表0项目槽
	* @param[in] channel 通道
	* @param[in] text 文本
	* @return void
	*/
	void addListItemSlot(int channel, const QString& text);

	/*
	* @brief 更新MES提示槽
	* @param[in] color 颜色
	* @param[in] text 文本
	* @return void
	*/
	void updateMesHintSlot(const char* color, const QString& text);

signals:
	/*
	* @addListItemSignal,添加列表0项目信号
	* @param1,通道
	* @param2,文本
	* @return,void
	*/
	void addListItemSignal(int channel, const QString& text);

	/*
	* @setScanCodeDlgSignal,设置扫码对话框信号
	* @param1,是否显示
	* @return,void
	*/
	void setScanCodeDlgSignal(int channel, bool show = true);

	/*
	* @setStartButtonEnableSignal,设置开始按钮启用信号
	* @param1,是否启用
	* @return,void
	*/
	void setStartButtonEnableSignal(bool enable);

	/*
	* @applicationExitSignal,应用程序退出信号
	* @return,void
	*/
	void applicationExitSignal();
private slots:
	/*
	* @setScanCodeDlgSlot,设置扫码对话框槽
	* @param1,是否显示
	* @return,void
	*/
	void setScanCodeDlgSlot(int channel, bool show);

	/*
	* @setStartButtonEnableSlot,设置开始按钮启用槽
	* @param1,是否启用
	* @return,void
	*/
	void setStartButtonEnableSlot(bool enable);

	/*
	* @detectionTimeChangedSlot,检测时间改变槽
	* @return,void
	*/
	void detectionTimeChangedSlot();

	/*
	* @setTestResultSlot,设置测试结果槽
	* @param1,结果
	* @return,void
	*/
	void setTestResultSlot(int result);

	/*
	* @resetScanSlot,重置扫描槽
	* @return,void
	*/
	void resetScanSlot();

	/*
	* @reallocCapSlot,重新分配采集卡槽
	* @return,void
	*/
	void reallocCapSlot();

	/*
	* @reallocCanSlot,重新分配CAN卡槽
	* @return,void
	*/
	void reallocCanSlot();

	/*
	* @setShowBinaryThreshSlot,显示二值化阈值槽
	* @param1,是否显示
	* @return,void
	*/
	void setShowBinaryThreshSlot(bool show);

	/*
	* @setCanMsgEnableSlot,设置CAN报文启用槽
	* @param1,是否启用
	* @return,void
	*/
	void setCanMsgEnableSlot(bool enable);
protected:
	OVERRIDE_MOUSE_EVENT;

	/*
	* @setLastError,设置最终错误
	* @param1,错误信息
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @addListItem,添加列表0项目
	* @param1,通道
	* @param2,文本
	* @return,void
	*/
	void addListItem(int channel, const QString& text);

	/*
	* @addList0Item,增加列表0项目
	* @param1,文本
	* @return,void
	*/
	void addList0Item(const QString& text);

	/*
	* @addList1Item,增加列表1项目
	* @param1,文本
	* @return,void
	*/
	void addList1Item(const QString& text);

	/*
	* @saveLog,保存日志
	* @param1,是否成功
	* @return,void
	*/
	bool saveLog(bool success);
private:
	Ui::MainDlgClass ui;

	//图像处理类
	ProcessImage* m_image[2] = { 0 };

	//最终错误
	QString m_lastError = "No Error";

	//机种名
	QString m_moduleName = "未知";

	//倒计时定时器
	QTimer m_countDownTimer;

	//倒计时秒
	int m_countDownSecond = 0;

	//是否退出
	bool m_quit = false;

	//是否连接
	bool m_connect = false;

	//是否扫码
	bool m_scan = false;

	//是否开始
	bool m_start = false;

	//CAN卡
	std::shared_ptr<can::Base> m_can;

	//电源
	ItechPower m_power;

	//设备信息
	DeviceInfo* m_deviceInfo = nullptr;

	//矩形信息
	RectInfo* m_rectInfo = nullptr;

	//通道信息
	ChannelInfo* m_channelInfo = nullptr;

	//测试信息
	TestInfo* m_testInfo = nullptr;

	//CAN报文信息
	CanMsgInfo* m_canMsgInfo = nullptr;

	//基本信息
	BaseInfo* m_baseInfo = nullptr;

	//账户信息
	AccountInfo* m_accountInfo = nullptr;

	//原始宽度
	int m_originWidth = 0;

	//原始高度
	int m_originHeight = 0;

	//两个图像之间布局的间隔
	int m_spacing = 6;

	//扫码对话框
	ScanCodeDlg m_scanCodeDlg;

	//设置对话框
	SettingDlg* m_settingDlg = nullptr;

	//认证对话框
	utility::AuthDialog m_authDlg;

	//重置扫描
	bool m_resetScan = false;

	//存在设置对话框
	bool m_existSettingDlg = false;
};

