#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_ChooseDlg.h"

#include "MainDlg.h"

class ChooseDlg : public QWidget
{
	Q_OBJECT
public:
	ChooseDlg(QWidget *parent = Q_NULLPTR);

	~ChooseDlg();

	QString getLastError() const;

	bool initialize();

public slots:
	void versionButtonSlot();

protected:
	OVERRIDE_MOUSE_EVENT;

	void setLastError(const QString& error);

	void addFileList();

	bool checkInput();
protected slots:
	void addButtonSlot();

	void deleteButtonSlot();

	void copyButtonSlot();

	void renameButtonSlot();

	void confirmButtonSlot();

	void exitButtonSlot();
private:
	Ui::ChooseDlg ui;

	QString m_lastError = "Î´Öª´íÎó";

	QString m_dirName = "Config";
};
