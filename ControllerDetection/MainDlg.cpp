#include "MainDlg.h"
#include <process.h>

bool g_threadWait = false;

QStringList g_codeList;

QStringList g_codeListBackup;

extern QStringList g_repeatCodeList;

bool MainDlg::m_dlgExist = false;

MainDlg::MainDlg(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

	REGISTER_META_TYPE(QItemSelection);
}

MainDlg::~MainDlg()
{
	m_dlgExist = false;

	m_quit = true;

	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(m_image[i]);
	}

	if (m_existSettingDlg)
	{
		m_settingDlg->close();
	}
}

QString MainDlg::getLastError() const
{
	return m_lastError;
}

bool MainDlg::initialize(const QString& moduleName)
{
	bool result = false, success = true;
	do
	{
		m_dlgExist = true;

		getOriginSizeExceptImage();

		m_moduleName = moduleName;

		setTitleModuleName();

		connect(&m_countDownTimer, &QTimer::timeout, this, &MainDlg::countDownTimerSlot);

		connect(ui.startButton, &QPushButton::clicked, this, &MainDlg::startButtonSlot);

		connect(ui.connectButton, &QPushButton::clicked, this, &MainDlg::connectButtonSlot);

		connect(ui.settingButton, &QPushButton::clicked, this, &MainDlg::settingButtonSlot);

		connect(ui.exitButton, &QPushButton::clicked, this, [&]() {this->close(); });

		connect(this, &MainDlg::setScanCodeDlgSignal, this, &MainDlg::setScanCodeDlgSlot);

		connect(this, &MainDlg::setStartButtonEnableSignal, this, &MainDlg::setStartButtonEnableSlot);

		connect(this, &MainDlg::addListItemSignal, this, &MainDlg::addListItemSlot);

		connect(&m_scanCodeDlg, &ScanCodeDlg::updateMesHintSignal, this, &MainDlg::updateMesHintSlot);

		RUN_BREAK(!GET_JSON->initialize(m_moduleName), GET_JSON->getLastError());

		m_deviceInfo = GET_JSON->getDeviceInfo();

		m_rectInfo = GET_JSON->getRectInfo();

		m_channelInfo = GET_JSON->getChannelInfo();

		m_testInfo = GET_JSON->getTestInfo();

		m_canMsgInfo = GET_JSON->getCanMsgInfo();

		m_baseInfo = GET_JSON->getBaseInfo();

		m_accountInfo = GET_JSON->getAccountInfo();

		m_authDlg.setUserName(&m_accountInfo->userName);
		m_authDlg.setPassword(&m_accountInfo->userPassword);
		m_authDlg.setEnterButtonIcon(QIcon(":/Images/Resources/Images/save.ico"));
		m_authDlg.setExitButtonIcon(QIcon(":/Images/Resources/Images/del.ico"));

		setTitleCountDown(m_testInfo->detectionTime.toInt(), 0);

		QList<QLabel*> imageList = { ui.imageLabel0,ui.imageLabel1 };
		for (int i = 0; i < 2; i++)
		{
			m_image[i] = NO_THROW_NEW ProcessImage(i);

			if (!m_image[i])
			{
				success = false;
				setLastError("图像处理分配内存失败");
				break;
			}

			connect(m_image[i], &ProcessImage::addListItemSignal, this, &MainDlg::addListItemSlot);

			connect(m_image[i]->getCaptureCard(), QPIX_SIGNAL, imageList[i], &QLabel::setPixmap);
		}

		if (!success)
		{
			break;
		}

		if (m_deviceInfo->canName != NOT_USE) {
			auto names = can::getSupportDeviceNames();
			auto iter = std::find(names.begin(), names.end(), m_deviceInfo->canName.toStdString());
			auto index = 0;
			if (iter != names.end()) {
				index = iter - names.begin();
			}
			m_can = can::autoReleaseNew(static_cast<can::DeviceType>(index));
		}

		setWidgetFixedSize();

		changeFrameCount();

		_beginthread(scanCodeThread, 0, this);

		result = true;
	} while (false);
	return result;
}

bool MainDlg::changeFrameCount(int count)
{
	bool result = false;
	do
	{
		count = !count ? m_deviceInfo->frameCount.toInt() : count;

		ui.imageLabel1->setHidden(count == 1);
		ui.listWidget1->setHidden(count == 1);

		int width = m_originWidth + ui.imageLabel0->width() * count + (count - 1) * m_spacing;
		int height = m_originHeight + ui.imageLabel0->height() + m_spacing * 2;

		setFixedSize(width, height);

		QRect rect = utility::window::getScreenSize();
		int x = rect.width() / 2;
		QPoint point;
		point.setX(x - this->width() / 2);
		move(point);

		utility::window::setBorderRadius(this);

		result = true;
	} while (false);
	return result;
}

void MainDlg::getOriginSizeExceptImage()
{
	m_originWidth = width() - ui.imageLabel0->width() * 2 - m_spacing;
	m_originHeight = height() - ui.imageLabel0->height() - m_spacing * 2;
}

bool MainDlg::setWidgetFixedSize(const QString& resolution)
{
	bool result = false;
	do
	{
		QString size = resolution.isEmpty() ?
			m_deviceInfo->imageResolution : resolution;

		QStringList split = size.split("*", QString::SkipEmptyParts);
		RUN_BREAK(split.size() != 2, "分辨率解析失败");

		int width = split[0].toInt();
		int height = split[1].toInt();
		ui.imageLabel0->setFixedSize(width, height);
		ui.imageLabel1->setFixedSize(width, height);

		m_image[0]->getCaptureCard()->setImageScaleSize(width, height);
		m_image[1]->getCaptureCard()->setImageScaleSize(width, height);
		result = true;
	} while (false);
	return result;
}

void MainDlg::setTitleModuleName()
{
	QString title(Q_SPRINTF("机种 %s  时间 00:00", Q_TO_C_STR(m_moduleName)));
	ui.titleLabel->setText(title);
}

void MainDlg::setTitleCountDown(int minute, int second)
{
	QString title(Q_SPRINTF("机种 %s  时间 %02d:%02d", Q_TO_C_STR(m_moduleName),
		minute, second));
	ui.titleLabel->setText(title);
}

void MainDlg::scanCodeThread(void* args)
{
	bool resetValue = false;
	MainDlg* dlg = static_cast<MainDlg*>(args);
	ChannelInfo* channelInfo = dlg->m_channelInfo;
	while (!dlg->m_quit)
	{
	RESET_SCAN:

		if (dlg->m_connect && dlg->m_scan && GET_JSON->getEnabledChannelTotal())
		{
			resetValue = true;
			int frameCount = dlg->m_deviceInfo->frameCount.toInt();
			int total = GET_JSON->getChannelTotal();
			for (int index = g_codeList.size(); index < total; index++)
			{
				if (!channelInfo[index].enable)
				{
					g_codeList.append(QString::number(index));
					continue;
				}

				g_threadWait = true;
				emit dlg->setScanCodeDlgSignal(index + 1);
				while (g_threadWait)
				{
					if (!dlg->m_connect || dlg->m_resetScan)
					{
						g_repeatCodeList.clear();
						emit dlg->setScanCodeDlgSignal(index + 1, false);
						if (dlg->m_resetScan)
						{
							for (int i = 0; i < g_codeList.size(); i++)
							{
								if (!channelInfo[i].enable)
									g_codeList[i] = QString::number(i);
							}
							g_repeatCodeList = g_codeList;
							dlg->m_resetScan = false;
							goto RESET_SCAN;
						}
						break;
					}

					Sleep(200);
				}

				if (resetValue)
				{
					GET_JSON->resetChannelInfo();
					resetValue = false;
				}

				if (!dlg->m_connect)
				{
					break;
				}

				int channel = (index + 1) > total / frameCount;
				emit dlg->addListItemSignal(channel, Q_SPRINTF("通道%02d,输入条码  %s",
					index + 1, Q_TO_C_STR(g_codeList[index])));
			}

			if (dlg->m_connect)
			{
				emit dlg->addListItemSignal(-1, "请点击开始进行检测");
				emit dlg->setStartButtonEnableSignal(true);
			}

			dlg->m_scan = false;
			g_codeListBackup = g_codeList;
			g_codeList.clear();
			g_repeatCodeList.clear();
		}
		Sleep(200);
	}
}

bool MainDlg::isCountDownFinished() const
{
	int time = m_testInfo->detectionTime.toInt();
	return m_countDownSecond >= time * 60;
}

void MainDlg::addCanMsg()
{
	if (m_deviceInfo->canName == NOT_USE)
	{
		return;
	}
	int period = m_canMsgInfo->period.toInt(nullptr, 16);
	int count = m_canMsgInfo->count.toInt();

	can::Msg msg = { 0 };
	msg.id = m_canMsgInfo->id.toInt(nullptr, 16);
	msg.dlc = 8;
	msg.sendCycle = period;
	msg.sendCount = count;
	QStringList split = m_canMsgInfo->data.split(" ");
	for (int i = 0; i < split.size(); i++)
	{
		msg.data[i] = split[i].toInt(nullptr, 16);
	}

	msg.sendType = count ? can::SendType::EVENT : can::SendType::CYCLE;
	m_can->addMsg(msg);

	if (m_canMsgInfo->enable.toInt()) {
		m_can->startAsyncSendMsg();
	}
}

void MainDlg::deleteCanMsg()
{
	if (m_deviceInfo->canName == NOT_USE) {
		return;
	}
	m_can->deleteAllMsgs();
}

void MainDlg::closeEvent(QCloseEvent* event)
{
	if (utility::QMessageBoxEx::question(this, "友情提示", "你确定要退出吗?") == QMessageBox::Yes)
	{
		emit applicationExitSignal();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainDlg::countDownTimerSlot()
{
	int minute = 0, second = 0;
	int time = m_testInfo->detectionTime.toInt();
	utility::minuteCountdown(time, ++m_countDownSecond, minute, second);
	setTitleCountDown(minute, second);

	if (isCountDownFinished())
	{
		startButtonSlot();
	}
	return;
}

void MainDlg::startButtonSlot()
{
	int frameCount = m_deviceInfo->frameCount.toInt();
	if (!m_start)
	{
		setTestResultSlot(TR_TS);
		GET_JSON->resetChannelInfo();
		for (int i = 0; i < frameCount; i++)
		{
			m_image[i]->startTest();
			addListItem(i, Q_SPRINTF("通道%d,测试开始", i));
		}
		m_countDownTimer.start(1000);
		m_countDownSecond = 0;
		addCanMsg();
	}
	else
	{
		bool finished = isCountDownFinished();
		if (!finished)
		{
			auto button = utility::QMessageBoxEx::question(this, "提示", "你确定要停止吗?");
			if (button != QMessageBox::Yes)
			{
				return;
			}
		}

		for (int i = 0; i < frameCount; i++)
		{
			m_image[i]->stopTest();
			addListItem(i, Q_SPRINTF("通道%d,测试%s", i, finished ? "结束" : "终止"));
		}
		m_countDownTimer.stop();
		ui.startButton->setEnabled(false);
		bool result = finished && GET_JSON->getChannelResult();
		setTestResultSlot(result ? TR_OK : TR_NG);
		saveLog(result);
		m_scan = true;
	}
	ui.startButton->setText(m_start ? "开始" : "停止");
	ui.connectButton->setEnabled(m_start);
	m_start = !m_start;
}

void MainDlg::connectButtonSlot()
{
	do
	{
		int frameCount = m_deviceInfo->frameCount.toInt();
		int powerChannel = GET_JSON->getPowerChannel();
		QString canName = m_deviceInfo->canName;
		if (!m_connect)
		{
			if (powerChannel != -1)
			{
				if (!m_power.open(m_deviceInfo->powerPort.remove("COM").toInt(),
					m_deviceInfo->powerBaudrate.toInt(),
					m_deviceInfo->powerVoltage.remove("V").toFloat(),
					m_deviceInfo->powerCurrent.remove("A").toFloat(), powerChannel))
				{
					addListItem(-1, "打开电源失败,请检查串口是否正确");
					break;
				}

				m_power.setOutput(true);
			}

			if (canName != NOT_USE)
			{
				can::Device dev;
				dev.arbiBaud[0] = m_deviceInfo->canBaudrate.toInt();
				dev.isExpandFrame = m_deviceInfo->canExpandFrame.toInt();
				if (!m_can->open(dev)) {
					m_power.close();
					addListItem(-1, LS_TO_Q_STR(m_can->getLastError()));
					break;
				}
				addCanMsg();
			}

			addListItem(-1, "打开采集卡可能存在暂时卡死状态,请耐心等待...");

			bool success = true;
			for (int i = 0; i < frameCount; i++) {
				bool opened = m_image[i]->open();
				if (opened) {
					addListItem(i, Q_SPRINTF("打开采集卡通道%d%s", i, SU_FA(opened)));
				}
				else {
					addListItem(i, Q_SPRINTF("打开采集卡通道%d%s,%s", i, SU_FA(opened),
						Q_TO_C_STR(m_image[i]->getCaptureCard()->getLastError())));
				}

				if (!opened) {
					if (i > 0) {
						addListItem(-1, "采集卡设备未完全打开,请检查硬件设备");
						m_image[i - 1]->close();
					}

					if (canName != NOT_USE) {
						m_can->deleteAllMsgs();
						m_can->stopAsyncSendMsg();
						m_can->close();
					}

					if (powerChannel != -1)
					{
						m_power.close();
					}
					success = false;
					break;
				}
			}

			if (!success) {
				break;
			}
		}
		else {
			if (canName != NOT_USE) {
				m_can->deleteAllMsgs();
				m_can->stopAsyncSendMsg();
				m_can->close();
			}

			if (powerChannel != -1) {
				m_power.close();
			}

			addListItem(-1, "关闭采集卡可能存在暂时卡死状态,请耐心等待...");
			for (int i = 0; i < frameCount; i++) {
				m_image[i]->close();
				addListItem(i, Q_SPRINTF("关闭通道%d成功", i));
			}

			ui.startButton->setEnabled(false);
		}

		GET_JSON->resetChannelInfo();
		ui.connectButton->setText(m_connect ? "连接" : "断开");
		ui.exitButton->setEnabled(m_connect);
		m_connect = !m_connect;
		m_scan = m_connect;
	} while (false);
	return;
}

void MainDlg::settingButtonSlot()
{
	if (m_existSettingDlg)
	{
		if (!m_settingDlg->isActiveWindow())
		{
			m_settingDlg->activateWindow();
		}
		m_settingDlg->isMaximized() ? m_settingDlg->showMaximized() : m_settingDlg->showNormal();
	}
	else
	{
		if (!m_authDlg.exec())
		{
			return;
		}

		m_settingDlg = NO_THROW_NEW SettingDlg(m_moduleName, m_connect, &m_existSettingDlg);
		if (!m_settingDlg)
		{
			utility::QMessageBoxEx::warning(this, "错误", "设置界面分配内存失败");
			return;
		}
		connect(m_settingDlg, &SettingDlg::frameCountChangedSignal, this, &MainDlg::frameCountChangedSlot);
		connect(m_settingDlg, &SettingDlg::resolutionChangedSignal, this, &MainDlg::resolutionChangedSlot);
		connect(m_settingDlg, &SettingDlg::detectionTimeChangedSignal, this, &MainDlg::detectionTimeChangedSlot);
		connect(m_settingDlg, &SettingDlg::resetScanSignal, this, &MainDlg::resetScanSlot);
		connect(m_settingDlg, &SettingDlg::reallocCapSignal, this, &MainDlg::reallocCapSlot);
		connect(m_settingDlg, &SettingDlg::reallocCanSignal, this, &MainDlg::reallocCanSlot);
		connect(m_settingDlg, &SettingDlg::setShowBinaryThreshSignal, this, &MainDlg::setShowBinaryThreshSlot);
		connect(m_settingDlg, &SettingDlg::setCanMsgEnableSignal, this, &MainDlg::setCanMsgEnableSlot);
		m_settingDlg->initialize();
		m_settingDlg->show();
	}
}

void MainDlg::frameCountChangedSlot(int count)
{
	changeFrameCount(count);
}

void MainDlg::resolutionChangedSlot(const QString& resolution)
{
	setWidgetFixedSize(resolution);
	changeFrameCount(ui.imageLabel1->isHidden() ? 1 : 2);
}

void MainDlg::addListItemSlot(int channel, const QString& text)
{
	if (channel == 0)
	{
		addList0Item(text);
	}
	else if (channel == 1)
	{
		addList1Item(text);
	}
	else
	{
		addList0Item(text);
		addList1Item(text);
	}
	return;
}

void MainDlg::updateMesHintSlot(const char* color, const QString& text)
{
	ui.mesHintLabel->setStyleSheet(Q_SPRINTF("color:%s;", color));
	ui.mesHintLabel->setText(text);
}

void MainDlg::setScanCodeDlgSlot(int channel, bool show)
{
	m_scanCodeDlg.setChannel(channel);
	show ? m_scanCodeDlg.show() : m_scanCodeDlg.hide();
}

void MainDlg::setStartButtonEnableSlot(bool enable)
{
	ui.startButton->setEnabled(enable);
}

void MainDlg::detectionTimeChangedSlot()
{
	setTitleCountDown(m_testInfo->detectionTime.toInt(), 0);
}

void MainDlg::setTestResultSlot(int testResult)
{
	QString result = "NO", color = "black";
	switch (testResult)
	{
	case TR_TS:result = "TS"; color = "rgb(255,255,0)"; break;
	case TR_OK:result = "OK"; color = "rgb(0,255,0)"; break;
	case TR_NG:result = "NG"; color = "rgb(255,0,0)"; break;
	case TR_NO:result = "NO"; color = "rgb(0,0,0)"; break;
	default:break;
	}
	ui.resultLabel->setText(result);
	ui.resultLabel->setStyleSheet(QString("background-color:%1;color:%2").arg(color).arg("rgb(0,0,0)"));
}

void MainDlg::resetScanSlot()
{
	m_resetScan = true;
}

void MainDlg::reallocCapSlot()
{
	QList<QLabel*> imageList = { ui.imageLabel0,ui.imageLabel1 };
	for (int i = 0; i < 2; i++)
	{
		m_image[i]->reallocCap();
		connect(m_image[i], &ProcessImage::addListItemSignal, this, &MainDlg::addListItemSlot);
		connect(m_image[i]->getCaptureCard(), QPIX_SIGNAL, imageList[i], &QLabel::setPixmap);
	}
}

void MainDlg::reallocCanSlot()
{
	auto names = can::getSupportDeviceNames();
	auto iter = std::find(names.begin(), names.end(), m_deviceInfo->canName.toStdString());
	auto index = 0;
	if (iter != names.end()) {
		index = iter - names.begin();
	}
	m_can = can::autoReleaseNew(static_cast<can::DeviceType>(index));
}

void MainDlg::setShowBinaryThreshSlot(bool show)
{
	for (int i = 0; i < 2; i++)
	{
		m_image[i]->setConvertProc(show);
	}
}

void MainDlg::setCanMsgEnableSlot(bool enable)
{
	enable ? addCanMsg() : deleteCanMsg();
}

void MainDlg::setLastError(const QString& error)
{
	m_lastError = error;
}

void MainDlg::addListItem(int channel, const QString& text)
{
	addListItemSlot(channel, text);
}

void MainDlg::addList0Item(const QString& text)
{
	QString icon = ":/Images/Resources/Images/";

	if (text.contains("OK") || text.contains("成功"))
	{
		icon.append("ok.ico");
	}
	else if (text.contains("NG") || text.contains("失败"))
	{
		icon.append("ng.ico");
	}
	else
	{
		icon.append("star.ico");
	}

	QString temp = utility::getCurrentTime().append("  ").append(text);
	if (ui.listWidget0->count() > 500)
	{
		ui.listWidget0->clear();
	}
	ui.listWidget0->addItem(new QListWidgetItem(QIcon(icon), temp));
	ui.listWidget0->setCurrentRow(ui.listWidget0->count() - 1);
}

void MainDlg::addList1Item(const QString& text)
{
	QString icon = ":/Images/Resources/Images/";

	if (text.contains("OK") || text.contains("成功"))
	{
		icon.append("ok.ico");
	}
	else if (text.contains("NG") || text.contains("失败"))
	{
		icon.append("ng.ico");
	}
	else
	{
		icon.append("star.ico");
	}

	QString temp = utility::getCurrentTime().append("  ").append(text);
	if (ui.listWidget1->count() > 500)
	{
		ui.listWidget1->clear();
	}
	ui.listWidget1->addItem(new QListWidgetItem(QIcon(icon), temp));
	ui.listWidget1->setCurrentRow(ui.listWidget1->count() - 1);
}

bool MainDlg::saveLog(bool success)
{
	bool result = false, saveNgLog = false;
	do
	{
		auto filePath = QString("Log\\%1\\%2").arg(m_moduleName, utility::getCurrentDate(true));

		utility::makePath(filePath);

		if (!success)
		{
			auto button = utility::QMessageBoxEx::question(this, "提示", "检测NG,是否要保存日志?");
			if (button == utility::QMessageBoxEx::Yes)
			{
				saveNgLog = true;
			}
		}

		auto fileName = QString("%1\\%2.csv").arg(filePath, utility::getCurrentTimeEx(true));
		QFile file(fileName);
		if (!file.open(QFile::Append))
		{
			setLastError("创建日志文件失败," + file.errorString());
			break;
		}

		QTextStream stream(&file);
		stream.setCodec("utf-8");
		stream << QString("条形码,检测结果") << endl;
		for (int i = 0; i < GET_JSON->getChannelTotal(); i++)
		{
			if ((m_channelInfo[i].finalResult || saveNgLog) && m_channelInfo[i].enable)
			{
				stream
					<< g_codeListBackup[i] << ","
					<< (OK_NG(m_channelInfo[i].finalResult)) << endl;
			}
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

