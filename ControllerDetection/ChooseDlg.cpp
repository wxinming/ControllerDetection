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
		utility::QMessageBoxEx::warning(this, "错误", getLastError());
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
		RUN_BREAK(!version, "版本按钮分配内存失败");
		QFont font;
		font.setPixelSize(12);
		font.setFamily("宋体");
		font.setItalic(false);
		version->setFont(font);
		version->setText("版本:" + utility::getVersion());
		version->setStyleSheet("background:transparent;");
		version->setFlat(true);
		connect(version, &QPushButton::clicked, this, &ChooseDlg::versionButtonSlot);

		RUN_BREAK(!GET_JSON, "Json初始化失败");

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
			utility::QMessageBoxEx::warning(this, "提示", "机种名不可为空");
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
			utility::QMessageBoxEx::warning(this, "错误", "添加机种失败," + GET_JSON->getLastError());
			break;
		}

		addFileList();

		utility::QMessageBoxEx::information(this, "提示", "添加机种成功");
	} while (false);
	return;
}

void ChooseDlg::deleteButtonSlot()
{
	do
	{
		utility::QMessageBoxEx::information(this, "提示", "此路不通,请联系管理员删除");
		return;

		if (!ui.nameListCombo->count())
		{
			utility::QMessageBoxEx::warning(this, "提示", "机种为空,无法进行删除");
			break;
		}

		const QString moduleName = ui.nameListCombo->currentText();
		const QString fileName = QString("%1\\%2.json").arg(m_dirName, moduleName);

		const auto result = utility::QMessageBoxEx::question(this, "提示", QString("你确定要删除机种[%1]吗?").arg(moduleName));
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
			utility::QMessageBoxEx::information(this, "提示", "机种列表为空,无法进行复制");
			break;
		}

		if (destName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "提示", "目标机种为空,无法进行复制");
			break;
		}

		const auto button = utility::QMessageBoxEx::question(this, "提示", QString("复制机种[%1]到[%2]?").arg(srcName, destName));
		if (button != QMessageBox::Yes)
		{
			break;
		}

		const QString srcPath = QString("%1\\%2.json").arg(m_dirName, srcName);
		const QString destPath = QString("%1\\%2.json").arg(m_dirName, destName);

		const auto list = utility::getFileListBySuffixName(m_dirName, { ".json" });
		if (list.contains(destPath))
		{
			utility::QMessageBoxEx::information(this, "提示", QString("已存在机种[%1],无法进行复制").arg(destName));
			break;
		}

		if (!CopyFileW(Q_TO_WC_STR(srcPath), Q_TO_WC_STR(destPath), FALSE))
		{
			utility::QMessageBoxEx::warning(this, "错误",QString("复制机种[%1]到[%2]失败").arg(srcName, destName));
			break;
		}
		ui.destNameLine->clear();
		addFileList();
		utility::QMessageBoxEx::information(this, "提示", "复制机种成功");
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
			utility::QMessageBoxEx::information(this, "提示", "机种列表为空,无法进行重命名");
			break;
		}

		if (destName.isEmpty())
		{
			utility::QMessageBoxEx::information(this, "提示", "目标机种为空,无法进行重命名");
			break;
		}

		const auto button = utility::QMessageBoxEx::question(this, "提示", QString("重命名机种[%1]到[%2]?").arg(srcName, destName));
		if (button != QMessageBox::Yes)
		{
			break;
		}

		const QString srcPath = QString("%1\\%2.json").arg(m_dirName, srcName);
		const QString destPath = QString("%1\\%2.json").arg(m_dirName, destName);

		const auto list = utility::getFileListBySuffixName(m_dirName, { ".json" });
		if (list.contains(destPath))
		{
			utility::QMessageBoxEx::information(this, "提示", QString("已存在机种[%1],无法进行重命名").arg(destName));
			break;
		}

		if (!utility::file::rename(srcPath, destPath))
		{
			utility::QMessageBoxEx::warning(this, "错误", QString("重命名机种[%1]到[%2]失败").arg(srcName, destName));
			break;
		}

		ui.destNameLine->clear();
		addFileList();
		utility::QMessageBoxEx::information(this, "提示", "重命名机种成功");
	} while (false);
	return;
}

void ChooseDlg::confirmButtonSlot()
{
	do 
	{
		if (!ui.nameListCombo->count())
		{
			utility::QMessageBoxEx::warning(this, "提示", "机种为空,请先添加机种");
			break;
		}

		if (!MainDlg::m_dlgExist)
		{
			MainDlg* dlg = NO_THROW_NEW MainDlg;
			if (!dlg)
			{
				utility::QMessageBoxEx::warning(this, "错误", "主对话框分配内存失败");
				break;
			}

			if (!dlg->initialize(ui.nameListCombo->currentText()))
			{
				utility::QMessageBoxEx::warning(this, "错误", dlg->getLastError());
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
