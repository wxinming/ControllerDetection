#pragma once
#pragma execution_character_set("utf-8")

#include <Windows.h>
#include <QDialog>
#include <QMouseEvent>
#include <QMessageBox>

#include "ui_ScanCodeDlg.h"
#include "Json.h"

#include <Dependent/Lib/invo_mes_plugin/include/invo_mes_plugin.h>
#pragma comment(lib, "../../Dependent/Lib/invo_mes_plugin/lib/x86/invo_mes_plugin.lib")

/*
* @ScanCodeDlg,扫码对话框
*/
class ScanCodeDlg : public QDialog
{
	Q_OBJECT
public:
	/*
	* @ScanCodeDlg,构造
	* @param1,父对象
	*/
	ScanCodeDlg(QWidget* parent = nullptr);

	/*
	* @~ScanCodeDlg,析构
	*/
	~ScanCodeDlg();

	/*
	* @setChannel,设置通道
	* @param1,通道
	* @return,void
	*/
	void setChannel(int channel);

	/*
	* @getLastError,获取最终错误
	* @return,QString
	*/
	QString getLastError() const;

signals:
	void changeTitleSignal(const QString& text);

	void hideWindowSignal();

	void updateMesHintSignal(const char* color, const QString& text);

public slots:
	/*
	* @returnPressedSlot,回车按下槽
	* @return,void
	*/
	void returnPressedSlot();

	void changeTitleSlot(const QString& text);

	void hideWindowSlot();
protected:

	OVERRIDE_MOUSE_EVENT;

	/*
	* @judgeCode,判断条码
	* @param1,条码
	* @return,bool
	*/
	bool judgeCode(const QString& code);

	/*
	* @eventFilter,事件过滤器
	* @param1,主对象
	* @param2,事件
	* @return,bool
	*/
	virtual bool eventFilter(QObject* obj, QEvent* event);

	/*
	* @sendCode,发送条码
	* @param1,条码
	* @return,bool
	*/
	bool sendCode(const QString& code);

	/*
	* @setLastError,设置最终错误
	* @param1,错误信息
	* @return,void
	*/
	void setLastError(const QString& error);
private:
	/*用户界面类*/
	Ui::ScanCodeDlg ui;

	/*通道*/
	int m_channel = -1;

	/*最终错误*/
	QString m_lastError = "未知错误";

	//条形码
	QString m_barcode;

	/*最小化标签*/
	QLabel m_minimize;

	//基本信息
	BaseInfo* m_baseInfo = nullptr;

	//插件
	void* m_plugin = nullptr;
};
