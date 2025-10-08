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
	* @brief,构造
	* @param1,机种名
	* @param2,是否连接
	* @param3,父对象
	*/
	SettingDlg(const QString& moduleName, bool connected, bool* exist, QWidget* parent = Q_NULLPTR);

	/*
	* @brief,析构
	*/
	~SettingDlg();

	/*
	* @brief,初始化
	* @return,bool
	*/
	bool initialize();

	//按钮槽函数
public slots:
	/*
	* @brief,保存按钮槽函数
	* @return,void
	*/
	void saveButtonSlot();

	/*
	* @brief,退出按钮槽函数
	* @return,void
	*/
	void exitButtonSlot();

	//矩形槽函数
protected slots:
	/*
	* @brief,行数组合框槽函数
	* @param1,点击触发改变得索引值
	* @return,void
	*/
	void rowCountComboChangedSlot(const QString& text);

	/*
	* @brief,列数组合框槽函数
	* @param1,点击触发改变得索引值
	* @return,void
	*/
	void columnCountComboChangedSlot(const QString& text);

	/*
	* @brief,设置前矩形X轴槽
	* @param1,x值
	* @return,void
	*/
	void setFrontRectXSlot(int x);

	/*
	* @brief,设置前矩形Y轴槽
	* @param1,y值
	* @return,void
	*/
	void setFrontRectYSlot(int y);

	/*
	* @brief,设置前矩形框宽度
	* @param1,宽度
	* @retrun,void
	*/
	void setFrontRectWidthSlot(int width);

	/*
	* @brief,设置前矩形框高度
	* @param1,高度
	* @return,vodi
	*/
	void setFrontRectHeightSlot(int height);

	/*
	* @brief,设置后矩形X轴槽
	* @param1,x值
	* @return,void
	*/
	void setRearRectXSlot(int x);

	/*
	* @brief,设置后矩形Y轴槽
	* @param1,y值
	* @return,void
	*/
	void setRearRectYSlot(int y);

	/*
	* @brief,设置后矩形宽度槽
	* @param1,宽度
	* @return,void
	*/
	void setRearRectWidthSlot(int width);

	/*
	* @brief,设置后矩形高度槽
	* @param1,高度
	* @return,void
	*/
	void setRearRectHeightSlot(int height);

	/*
	* @brief,设置左矩形x轴槽
	* @param1,x值
	* @return,void
	*/
	void setLeftRectXSlot(int x);

	/*
	* @brief,设置左矩形y轴槽
	* @param1,y值
	* @return,void
	*/
	void setLeftRectYSlot(int y);

	/*
	* @brief,设置左矩形宽度槽
	* @param1,宽度
	* @return,void
	*/
	void setLeftRectWidthSlot(int width);

	/*
	* @brief,设置左矩形高度槽
	* @param1,高度
	* @return,void
	*/
	void setLeftRectHeightSlot(int height);

	/*
	* @brief,设置右矩形x轴槽
	* @param1,x值
	* @return,void
	*/
	void setRightRectXSlot(int x);

	/*
	* @brief,设置右矩形y轴槽
	* @param1,y值
	* @return,void
	*/
	void setRightRectYSlot(int y);

	/*
	* @brief,设置右矩形宽度槽
	* @param1,宽度
	* @return,void
	*/
	void setRightRectWidthSlot(int width);

	/*
	* @brief,设置右矩形高度槽
	* @param1,高度
	* @return,void
	*/
	void setRightRectHeightSlot(int height);

	/*
	* @brief,通道排列
	* @param1,类型
	* @return,void
	*/
	void channelAlignComboSlot(int index);

	/*
	* @brief,设置整体X偏移
	* @param1,x偏移量
	* @return,void
	*/
	void setOverallXOffsetSlot(int offset);

	/*
	* @brief,设置整体Y偏移
	* @param1,y偏移量
	* @return,void
	*/
	void setOverallYOffsetSlot(int offset);

	/*
	* @brief,设置整体前后间隔
	* @param1,间隔
	* @return,void
	*/
	void setOverallFRIntervalSlot(int interval);

	/*
	* @brief,设置整体左右间隔
	* @param1,间隔
	* @return,void
	*/
	void setOverallLRIntervalSlot(int interval);

	/*
	* @brief,设置内部前间隔
	* @param1,间隔
	* @return,void
	*/
	void setInternalFrontIntervalSlot(int interval);

	/*
	* @brief,设置内部后间隔
	* @param1,间隔
	* @return,void
	*/
	void setInternalRearIntervalSlot(int interval);

	/*
	* @brief,设置内部左间隔
	* @param1,间隔
	* @return,void
	*/
	void setInternalLeftIntervalSlot(int interval);

	/*
	* @brief,设置内部右间隔
	* @param1,间隔
	* @return,void
	*/
	void setInternalRightIntervalSlot(int interval);

	/*
	* @brief,设置通道编号槽
	* @param1,是否显示
	* @return,void
	*/
	void setShowChannelIdSlot(bool checked);

	/*
	* @brief,设置显示文本
	* @param1,是否显示
	* @return,void
	*/
	void setShowTextSlot(bool checked);

	/*
	* @brief,设置显示矩形
	* @param1,是否显示
	* @return,void
	*/
	void setShowRectSlot(bool checked);

	/*
	* @brief,设置显示禁用槽
	* @param1,是否显示
	* @return,void
	*/
	void setShowDisableSlot(bool checked);

	/*
	* @brief,设置显示二值化阈值
	* @param1,是否显示
	* @return,void
	*/
	void setShowBinaryThreshSlot(bool checked);

	//设备槽函数
protected slots:
	/*
	* @brief,图像分辨率组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void imageResolutionComboSlot(const QString& text);

	/*
	* @brief,采集卡名称组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void captureNameComboSlot(const QString& text);

	/*
	* @brief,画面个数组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void frameCountComboSlot(const QString& text);

	/*
	* @brief,电源类型组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void powerTypeComboSlot(const QString& text);

	/*
	* @brief,电源电压组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void powerVoltageComboSlot(const QString& text);

	/*
	* @brief,电源电流组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void powerCurrentComboSlot(const QString& text);

	/*
	* @brief,电源断开组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void powerPortComboSlot(const QString& text);

	/*
	* @brief,电源波特率组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void powerBuadrateSlot(const QString& text);

	/*
	* @brief,CAN名称组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void canNameComboSlot(const QString& text);

	/*
	* @brief,CAN波特率组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void canBaudrateComboSlot(const QString& text);

	/*
	* @brief,CAN拓展帧组合框槽
	* @param1,当前文本
	* @return,void
	*/
	void canExpandFrameComboSlot(const QString& text);

	//通道槽函数
protected slots:
	/*
	* @brief,通道状态改变槽
	* @param1,状态
	* @return,void
	*/
	void channelStateChangedSlot(int state);

	/*
	* @brief,选择所有通道槽
	* @param1,状态
	* @return,void
	*/
	void chooseAllChannelSlot(int state);

	//CAN报文槽函数
protected slots:
	/*
	* @brief,CAN报文ID槽
	* @return,void
	*/
	void canMsgIdSlot();

	/*
	* @brief,CAN报文数据槽
	* @return,void
	*/
	void canMsgDataSlot();

	/*
	* @brief,CAN报文间隔槽
	* @return,void
	*/
	void canMsgIntervalSlot();

	/*
	* @brief,CAN报文次数槽
	* @return,void
	*/
	void canMsgTimesSlot();

	/*
	* @brief,CAN报文启用槽
	* @return,void
	*/
	void canMsgEnableSlot(int state);

protected slots:
	/*
	* @brief,图像算法槽
	* @param1,文本内容
	* @return,void
	*/
	void imageAlgorithmSlot(const QString& text);

	/*
	* @brief,检测时间槽
	* @return,void
	*/
	void detectionTimeSlot();

	/*
	* @brief,上电等待槽
	* @return,void
	*/
	void powerOnWaitSlot();

	/*
	* @brief,前景颜色槽
	* @return,void
	*/
	void frontColorSlot();

	/*
	* @brief,后景颜色槽
	* @return,void
	*/
	void rearColorSlot();

	/*
	* @brief,左景颜色槽
	* @return,void
	*/
	void leftColorSlot();

	/*
	* @brief,右景颜色槽
	* @return,void
	*/
	void rightColorSlot();

	/*
	* @brief,判定次数槽
	* @reutrn,void
	*/
	void determineCountSlot();

	/*
	* @brief,黑色百分比槽
	* @return,void
	*/
	void blackPercentSlot();

	/*
	* @brief,白色百分比槽
	* @return,void
	*/
	void whitePercentSlot();

	/*
	* @brief,二进制阈值槽
	* @return,void
	*/
	void binaryThreshSlot();

	/*
	* @brief,动态占比槽
	* @return,void
	*/
	void dynamicPercentSlot();

	/*
	* @brief,动态阈值槽
	* @return,void
	*/
	void dynamicThreshSlot();

	/*
	* @brief,动态频率槽
	* @return,void
	*/
	void dynamicFrequencySlot();

	/*
	* @brief,动态检测槽
	* @return,void
	*/
	void dynamicDetectionSlot(int state);

	/*
	* @brief,跳过检测槽
	* @return,void
	*/
	void skipDetectionSlot(int state);

protected slots:
	/*
	* @brief,条码判断槽
	* @return,void
	*/
	void codeJudgeSlot();

	/*
	* @brief,条码长度槽
	* @return,void
	*/
	void codeLengthSlot();

	/*
	* @brief,启用判断槽
	* @param1,状态
	* @return,void
	*/
	void enableJudgeSlot(int state);

	/*
	* @brief,启用上传槽
	* @param1,状态
	* @return,void
	*/
	void enableUploadSlot(int state);

	/*
	* @brief,用户名槽
	* @return,void
	*/
	void userNameSlot();

	/*
	* @brief,用户密码槽
	* @return,void
	*/
	void userPasswordSlot();

protected slots:
	/*
	* @brief,指定行矩形组合框槽
	* @param1,文本
	* @return,void
	*/
	void specifyRowRectComboSlot(const QString& text);

	/*
	* @brief,指定行X偏移槽
	* @param1,x
	* @return,void
	*/
	void specifyRowXOffsetHSliderSlot(int x);

	/*
	* @brief,指定行Y偏移槽
	* @param1,y
	* @return,void
	*/
	void specifyRowYOffsetHSliderSlot(int y);

	/*
	* @brief,指定列矩形组合框槽
	* @param1,文本
	* @return,void
	*/
	void specifyColumnRectComboSlot(const QString& text);

	/*
	* @brief,指定列X偏移槽
	* @param1,x
	* @return,void
	*/
	void specifyColumnXOffsetHSliderSlot(int x);

	/*
	* @brief,指定列Y偏移槽
	* @param1,x
	* @return,void
	*/
	void specifyColumnYOffsetHSliderSlot(int y);

protected slots:
	/*
	* @brief,选择颜色槽
	* @return,void
	*/
	void chooseColorSlot();

	/*
	* @brief,还原默认颜色槽
	* @return,void
	*/
	void restoreDefaultColorSlot();

signals:
	/*
	* @brief,画面个数改变信号
	* @param1,画面实际个数
	* @return,void
	*/
	void frameCountChangedSignal(int count);

	/*
	* @brief,分辨率改变信号
	* @param1,分辨率(640*480)
	* @return,void
	*/
	void resolutionChangedSignal(const QString& resolution);

	/*
	* @brief,检测时间改变信号
	* @return,void
	*/
	void detectionTimeChangedSignal();

	/*
	* @brief,重置扫描信号
	* @return,void
	*/
	void resetScanSignal();

	/*
	* @brief,重新分配采集卡信号
	* @return,void
	*/
	void reallocCapSignal();

	/*
	* @brief,重新分配CAN卡信息
	* @return,void
	*/
	void reallocCanSignal();

	/*
	* @brief,显示二值化阈值信号
	* @param1,是否显示
	* @return,void
	*/
	void setShowBinaryThreshSignal(bool show);

	/*
	* @brief,设置CAN报文启用信号
	* @param1,是否启用
	* @return,void
	*/
	void setCanMsgEnableSignal(bool enable);
protected:
	//初始化控件
	void initControl();

	//初始化设备信息
	void initDeviceInfo();

	//初始化矩形信息
	void initRectInfo();

	//初始化通道信息
	void initChannelInfo();

	//设置通道部件
	void setChannelWidget();

	//初始化CAN报文信息
	void initCanMsgInfo();

	//初始化测试信息
	void initTestInfo();

	//初始化基本信息
	void initBaseInfo();

	//颜色信息输入是否正确
	bool isColorInfoInputRight(const QString& color);

	//初始化指定信息
	void initSpecifyInfo();

	//初始化共享信息
	void initShareInfo();
private:
	Ui::SettingDlg ui;

	//设备信息指针
	DeviceInfo* m_deviceInfo = nullptr;

	//设备信息备份
	DeviceInfo m_deviceInfoBackup;

	//矩形信息指针
	RectInfo* m_rectInfo = nullptr;

	//矩形信息备份
	RectInfo m_rectInfoBackup = {0};

	//通道信息指针
	ChannelInfo* m_channelInfo = nullptr;

	//通道信息备份
	ChannelInfo m_channelInfoBackup[MAX_CHANNEL_COUNT];

	//CAN报文信息指针
	CanMsgInfo* m_canMsgInfo = nullptr;

	//CAN报文信息备份
	CanMsgInfo m_canMsgInfoBackup;

	//测试信息指针
	TestInfo* m_testInfo = nullptr;

	//基本信息
	BaseInfo* m_baseInfo = nullptr;

	//是否保存文件
	bool m_saveFile = false;

	//机种名
	QString m_moduleName = "Unknown";

	//是否连接
	bool m_connect = false;

	//复选框列表,用于处理全选/反选
	QList<QCheckBox*> m_channelList;

	//通道已改变
	bool m_channelChanged = false;

	//指定行信息
	SpecifyRowInfo* m_specifyRowInfo = nullptr;

	//指定行信息备份
	SpecifyRowInfo m_specifyRowInfoBackup[MAX_ROW_COUNT] = { 0 };

	//指定列信息
	SpecifyColumnInfo* m_specifyColumnInfo = nullptr;

	//指定列信息备份
	SpecifyColumnInfo m_specifyColumnInfoBackup[MAX_COLUMN_COUNT] = { 0 };

	//账户信息
	AccountInfo* m_accountInfo = nullptr;

	//颜色信息
	ColorInfo* m_colorInfo = nullptr;

	//颜色信息备份
	ColorInfo m_colorInfoBackup;

	//自身存在
	bool* m_selfExist = nullptr;
};
