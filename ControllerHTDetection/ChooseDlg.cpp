#include "ChooseDlg.h"

ChooseDlg::ChooseDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	utility::window::setTheme();

	utility::window::setBorderRadius(this);

	utility::renameByVersion(this);

	autoReleaseNewWindow<utility::UpdateDialog>(this);

	if (!initialize())
	{
		utility::QMessageBoxEx::warning(this, "����", getLastError());
	}
}

ChooseDlg::~ChooseDlg()
{
	destroyAllWindow();
}

QString ChooseDlg::getLastError() const
{
	return m_lastError;
}

bool ChooseDlg::initialize()
{
	bool result = false;
	do 
	{
		addFileList();

		connect(ui.addButton, &QPushButton::clicked, this, &ChooseDlg::addButtonSlot);

		connect(ui.deleteButton, &QPushButton::clicked, this, &ChooseDlg::deleteButtonSlot);

		connect(ui.copyButton, &QPushButton::clicked, this, &ChooseDlg::copyButtonSlot);

		connect(ui.renameButton, &QPushButton::clicked, this, &ChooseDlg::renameButtonSlot);

		connect(ui.confirmButton, &QPushButton::clicked, this, &ChooseDlg::confirmButtonSlot);

		connect(ui.exitButton, &QPushButton::clicked, this, &ChooseDlg::exitButtonSlot);

		QPushButton* version = NO_THROW_NEW QPushButton(ui.label);
		RUN_BREAK(!version, "�汾��ť�����ڴ�ʧ��");
		QFont font;
		font.setPixelSize(12);
		font.setFamily("����");
		font.setItalic(false);
		version->setFont(font);
		version->setText("�汾:" + utility::getVersion());
		version->setStyleSheet("background:transparent;");
		version->setFlat(true);
		connect(version, &QPushButton::clicked, this, &ChooseDlg::versionButtonSlot);

		RUN_BREAK(!GET_JSON, "Json��ʼ��ʧ��");

		result = true;
	} while (false);
	return result;
}

void ChooseDlg::versionButtonSlot()
{
	QString text;
	utility::file::read(":/Images/change_logs.txt", text);
	auto dlg = autoReleaseNewWindow<utility::LogWidget>(text);
	dlg->show();
}

void ChooseDlg::setLastError(const QString& error)
{
	m_lastError = error;
}

void ChooseDlg::addFileList()
{
	ui.nameListCombo->clear();

	auto list = utility::getFileListBySuffixName(m_dirName, { ".json" });

	QIcon icon = QIcon(":/Images/Resources/Images/chrome.ico");
	for (auto& x : list)
	{
		ui.nameListCombo->addItem(icon, QFileInfo(x).baseName());
	}
}

bool ChooseDlg::checkInput()
{
	return false;
}

void ChooseDlg::addButtonSlot()
{
	do
	{
		const QString moduleName = ui.destNameLine->text();

		if (moduleName.isEmpty())
		{
			utility::QMessageBoxEx::warning(this, "��ʾ", "����������Ϊ��");
			break;
		}

		if (!utility::existDir(m_dirName))
		{
			utility::makeDir(m_dirName);
			SetFileAttributes(Q_TO_WC_STR(m_dirName), FILE_ATTRIBUTE_HIDDEN);
		}

		const QString fileName = QString("%1\\%2.json").arg(m_dirName, moduleName);
		if (!GET_JSON->writeTypeFile(fileName))
		{
			utility::QMessageBoxEx::warning(this, "����", "��ӻ���ʧ��," + GET_JSON->getLastError());
			break;
		}

		addFileList();

		utility::QMessageBoxEx::information(this, "��ʾ", "��ӻ��ֳɹ�");
	} while (false);
	return;
}

void ChooseDlg::deleteButtonSlot()
{
	do
	{
		utility::QMessageBoxEx::information(this, "��ʾ", "��·��ͨ,����ϵ����Աɾ��");
		return;

		if (!ui.nameListCombo->count())
		{
			utility::QMessageBoxEx::warning(this, "��ʾ", "����Ϊ��,�޷�����ɾ��");
			break;
		}

		const QString moduleName = ui.nameListCombo->currentText();
		const QString fileName = QString("%1\\%2.json").arg(m_dirName, moduleName);

		const auto result = utility::QMessageBoxEx::question(this, "��ʾ", QString("��ȷ��Ҫɾ������[%1]��?").arg(moduleName));
		if (result != utility::QMessageBoxEx::Yes)
		{
			break;
		}

		utility::file::remove(fileName);

		addFileList();
	} while (false);
	return;
}

void ChooseDlg::copyButtonSlot()
{
	do
	{
		const QString srcName = ui.nameListCombo->currentText();
		const QString destName = ui.destNameLine->text();

		if (srcName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "��ʾ", "�����б�Ϊ��,�޷����и���");
			break;
		}

		if (destName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "��ʾ", "Ŀ�����Ϊ��,�޷����и���");
			break;
		}

		const auto button = utility::QMessageBoxEx::question(this, "��ʾ", QString("���ƻ���[%1]��[%2]?").arg(srcName, destName));
		if (button != QMessageBox::Yes)
		{
			break;
		}

		const QString srcPath = QString("%1\\%2.json").arg(m_dirName, srcName);
		const QString destPath = QString("%1\\%2.json").arg(m_dirName, destName);

		const auto list = utility::getFileListBySuffixName(m_dirName, { ".json" });
		if (list.contains(destPath))
		{
			utility::QMessageBoxEx::information(this, "��ʾ", QString("�Ѵ��ڻ���[%1],�޷����и���").arg(destName));
			break;
		}

		if (!CopyFileW(Q_TO_WC_STR(srcPath), Q_TO_WC_STR(destPath), FALSE))
		{
			utility::QMessageBoxEx::warning(this, "����",QString("���ƻ���[%1]��[%2]ʧ��").arg(srcName, destName));
			break;
		}
		ui.destNameLine->clear();
		addFileList();
		utility::QMessageBoxEx::information(this, "��ʾ", "���ƻ��ֳɹ�");
	} while (false);
	return;
}

void ChooseDlg::renameButtonSlot()
{
	do
	{
		const QString srcName = ui.nameListCombo->currentText();
		const QString destName = ui.destNameLine->text();

		if (srcName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "��ʾ", "�����б�Ϊ��,�޷�����������");
			break;
		}

		if (destName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "��ʾ", "Ŀ�����Ϊ��,�޷�����������");
			break;
		}

		const auto button = utility::QMessageBoxEx::question(this, "��ʾ", QString("����������[%1]��[%2]?").arg(srcName, destName));
		if (button != QMessageBox::Yes)
		{
			break;
		}

		const QString srcPath = QString("%1\\%2.json").arg(m_dirName, srcName);
		const QString destPath = QString("%1\\%2.json").arg(m_dirName, destName);

		const auto list = utility::getFileListBySuffixName(m_dirName, { ".json" });
		if (list.contains(destPath))
		{
			utility::QMessageBoxEx::information(this, "��ʾ", QString("�Ѵ��ڻ���[%1],�޷�����������").arg(destName));
			break;
		}

		if (!utility::file::rename(srcPath, destPath))
		{
			utility::QMessageBoxEx::warning(this, "����", QString("����������[%1]��[%2]ʧ��").arg(srcName, destName));
			break;
		}

		ui.destNameLine->clear();
		addFileList();
		utility::QMessageBoxEx::information(this, "��ʾ", "���������ֳɹ�");
	} while (false);
	return;
}

void ChooseDlg::confirmButtonSlot()
{
	do 
	{
		if (!ui.nameListCombo->count())
		{
			utility::QMessageBoxEx::warning(this, "��ʾ", "����Ϊ��,������ӻ���");
			break;
		}

		if (!MainDlg::m_dlgExist)
		{
			MainDlg* dlg = NO_THROW_NEW MainDlg;
			if (!dlg)
			{
				utility::QMessageBoxEx::warning(this, "����", "���Ի�������ڴ�ʧ��");
				break;
			}

			if (!dlg->initialize(ui.nameListCombo->currentText()))
			{
				utility::QMessageBoxEx::warning(this, "����", dlg->getLastError());
				dlg->close();
				break;
			}
			dlg->show();
			hide();
			connect(dlg, &MainDlg::applicationExitSignal, this, [&]() {show(); });
		}
	} while (false);
	return;
}

void ChooseDlg::exitButtonSlot()
{
	this->close();
}
