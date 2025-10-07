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
* @ScanCodeDlg,ɨ��Ի���
*/
class ScanCodeDlg : public QDialog
{
	Q_OBJECT
public:
	/*
	* @ScanCodeDlg,����
	* @param1,������
	*/
	ScanCodeDlg(QWidget* parent = nullptr);

	/*
	* @~ScanCodeDlg,����
	*/
	~ScanCodeDlg();

	/*
	* @setChannel,����ͨ��
	* @param1,ͨ��
	* @return,void
	*/
	void setChannel(int channel);

	/*
	* @getLastError,��ȡ���մ���
	* @return,QString
	*/
	QString getLastError() const;

signals:
	void changeTitleSignal(const QString& text);

	void hideWindowSignal();

	void updateMesHintSignal(const char* color, const QString& text);

public slots:
	/*
	* @returnPressedSlot,�س����²�
	* @return,void
	*/
	void returnPressedSlot();

	void changeTitleSlot(const QString& text);

	void hideWindowSlot();
protected:

	OVERRIDE_MOUSE_EVENT;

	/*
	* @judgeCode,�ж�����
	* @param1,����
	* @return,bool
	*/
	bool judgeCode(const QString& code);

	/*
	* @eventFilter,�¼�������
	* @param1,������
	* @param2,�¼�
	* @return,bool
	*/
	virtual bool eventFilter(QObject* obj, QEvent* event);

	/*
	* @sendCode,��������
	* @param1,����
	* @return,bool
	*/
	bool sendCode(const QString& code);

	/*
	* @setLastError,�������մ���
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);
private:
	/*�û�������*/
	Ui::ScanCodeDlg ui;

	/*ͨ��*/
	int m_channel = -1;

	/*���մ���*/
	QString m_lastError = "δ֪����";

	//������
	QString m_barcode;

	/*��С����ǩ*/
	QLabel m_minimize;

	//������Ϣ
	BaseInfo* m_baseInfo = nullptr;

	//���
	void* m_plugin = nullptr;
};
