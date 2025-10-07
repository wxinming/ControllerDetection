#include "SettingDlg.h"
#include "SettingDlgDefine.h"
#include <QColorDialog>

SettingDlg::SettingDlg(const QString& moduleName, bool connected, bool* exist, QWidget* parent) :
	m_moduleName(moduleName),
	m_selfExist(exist),
	m_connect(connected),
	QWidget(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

	connect(ui.saveButton, &QPushButton::clicked, this, &SettingDlg::saveButtonSlot);

	connect(ui.exitButton, &QPushButton::clicked, this, &SettingDlg::exitButtonSlot);

	if (m_selfExist)
	{
		*m_selfExist = true;
	}
}

SettingDlg::~SettingDlg()
{
	if (!m_saveFile)
	{
		*m_deviceInfo = m_deviceInfoBackup;

		if (ui.imageResolutionCombo->currentText() != m_deviceInfo->imageResolution)
		{
			emit resolutionChangedSignal(m_deviceInfo->imageResolution);
		}

		if (ui.frameCountCombo->currentText() != m_deviceInfo->frameCount)
		{
			emit frameCountChangedSignal(m_deviceInfo->frameCount.toInt());
		}

		if (m_rectInfo->showBinaryThresh != m_rectInfoBackup.showBinaryThresh)
		{
			emit setShowBinaryThreshSignal(m_rectInfoBackup.showBinaryThresh);
		}
		*m_rectInfo = m_rectInfoBackup;

		if (m_canMsgInfo->enable != m_canMsgInfoBackup.enable)
		{
			emit setCanMsgEnableSignal(m_canMsgInfoBackup.enable.toInt());
		}
		*m_canMsgInfo = m_canMsgInfoBackup;

		ChannelInfo::copyArray(m_channelInfo, m_channelInfoBackup);

		memcpy(m_specifyRowInfo, m_specifyRowInfoBackup, sizeof(m_specifyRowInfoBackup));

		memcpy(m_specifyColumnInfo, m_specifyColumnInfoBackup, sizeof(m_specifyColumnInfoBackup));

		*m_colorInfo = m_colorInfoBackup;
		GET_JSON->callParseColorInfo();
	}

	if (m_selfExist)
	{
		*m_selfExist = false;
	}
}

bool SettingDlg::initialize()
{
	m_deviceInfo = GET_JSON->getDeviceInfo();

	m_rectInfo = GET_JSON->getRectInfo();

	m_channelInfo = GET_JSON->getChannelInfo();

	m_canMsgInfo = GET_JSON->getCanMsgInfo();

	m_testInfo = GET_JSON->getTestInfo();

	m_baseInfo = GET_JSON->getBaseInfo();

	m_specifyRowInfo = GET_JSON->getSpecifyRowInfo();

	m_specifyColumnInfo = GET_JSON->getSpecifyColumnInfo();

	m_accountInfo = GET_JSON->getAccountInfo();

	m_colorInfo = GET_JSON->getColorInfo();

	initControl();

	initDeviceInfo();

	initRectInfo();

	initChannelInfo();

	initCanMsgInfo();

	initTestInfo();

	initBaseInfo();

	initSpecifyInfo();

	initShareInfo();

	return true;
}

void SettingDlg::saveButtonSlot()
{
	bool result = GET_JSON->initialize(m_moduleName, true);
	if (!result)
	{
		utility::QMessageBoxEx::warning(this, "错误", "保存失败," + GET_JSON->getLastError());
		return;
	}

	m_saveFile = true;
	utility::QMessageBoxEx::information(this, "提示", "保存成功");
	emit detectionTimeChangedSignal();

	if (m_channelChanged)
	{
		m_channelChanged = false;
		emit resetScanSignal();
	}

	if (m_deviceInfoBackup.captureName != m_deviceInfo->captureName)
	{
		m_deviceInfoBackup.captureName = m_deviceInfo->captureName;
		emit reallocCapSignal();
	}

	if (m_deviceInfoBackup.canName != m_deviceInfo->canName)
	{
		m_deviceInfoBackup.canName = m_deviceInfo->canName;
		emit reallocCanSignal();
	}
	return;
}

void SettingDlg::exitButtonSlot()
{
	this->close();
}

void SettingDlg::rowCountComboChangedSlot(const QString& text)
{
	int rows = text.toInt();
	m_rectInfo->rows = rows;
	setChannelWidget();
}

void SettingDlg::columnCountComboChangedSlot(const QString& text)
{
	int columns = text.toInt();
	m_rectInfo->columns = columns;
	setChannelWidget();
}

void SettingDlg::setFrontRectXSlot(int x)
{
	m_rectInfo->frontX = x;
}

void SettingDlg::setFrontRectYSlot(int y)
{
	m_rectInfo->frontY = y;
}

void SettingDlg::setFrontRectWidthSlot(int width)
{
	m_rectInfo->frontWidth = width;
}

void SettingDlg::setFrontRectHeightSlot(int height)
{
	m_rectInfo->frontHeight = height;
}

void SettingDlg::setRearRectXSlot(int x)
{
	m_rectInfo->rearX = x;
}

void SettingDlg::setRearRectYSlot(int y)
{
	m_rectInfo->rearY = y;
}

void SettingDlg::setRearRectWidthSlot(int width)
{
	m_rectInfo->rearWidth = width;
}

void SettingDlg::setRearRectHeightSlot(int height)
{
	m_rectInfo->rearHeight = height;
}

void SettingDlg::setLeftRectXSlot(int x)
{
	m_rectInfo->leftX = x;
}

void SettingDlg::setLeftRectYSlot(int y)
{
	m_rectInfo->leftY = y;
}

void SettingDlg::setLeftRectWidthSlot(int width)
{
	m_rectInfo->leftWidth = width;
}

void SettingDlg::setLeftRectHeightSlot(int height)
{
	m_rectInfo->leftHeight = height;
}

void SettingDlg::setRightRectXSlot(int x)
{
	m_rectInfo->rightX = x;
}

void SettingDlg::setRightRectYSlot(int y)
{
	m_rectInfo->rightY = y;
}

void SettingDlg::setRightRectWidthSlot(int width)
{
	m_rectInfo->rightWidth = width;
}

void SettingDlg::setRightRectHeightSlot(int height)
{
	m_rectInfo->rightHeight = height;
}

void SettingDlg::channelAlignComboSlot(int index)
{
	m_rectInfo->channelAlign = index;
}

void SettingDlg::setOverallXOffsetSlot(int offset)
{
	m_rectInfo->overallXOffset = offset;
}

void SettingDlg::setOverallYOffsetSlot(int offset)
{
	m_rectInfo->overallYOffset = offset;
}

void SettingDlg::setOverallFRIntervalSlot(int interval)
{
	m_rectInfo->overallFRInterval = interval;
}

void SettingDlg::setOverallLRIntervalSlot(int interval)
{
	m_rectInfo->overallLRInterval = interval;
}

void SettingDlg::setInternalFrontIntervalSlot(int interval)
{
	m_rectInfo->internalFrontInterval = interval;
}

void SettingDlg::setInternalRearIntervalSlot(int interval)
{
	m_rectInfo->internalRearInterval = interval;
}

void SettingDlg::setInternalLeftIntervalSlot(int interval)
{
	m_rectInfo->internalLeftInterval = interval;
}

void SettingDlg::setInternalRightIntervalSlot(int interval)
{
	m_rectInfo->internalRightInterval = interval;
}

void SettingDlg::setShowChannelIdSlot(bool checked)
{
	m_rectInfo->showChannelId = checked;
}

void SettingDlg::setShowTextSlot(bool checked)
{
	m_rectInfo->showText = checked;
}

void SettingDlg::setShowRectSlot(bool checked)
{
	m_rectInfo->showRect = checked;
}

void SettingDlg::setShowDisableSlot(bool checked)
{
	m_rectInfo->showDisable = checked;
}

void SettingDlg::setShowBinaryThreshSlot(bool checked)
{
	m_rectInfo->showBinaryThresh = checked;
	emit setShowBinaryThreshSignal(checked);
}

void SettingDlg::imageResolutionComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("图像分辨率", text);
	m_deviceInfo->imageResolution = text;
	resolutionChangedSignal(text);
}

void SettingDlg::captureNameComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("采集卡名称", text);
	m_deviceInfo->captureName = text;
}

void SettingDlg::frameCountComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("画面个数", text);
	m_deviceInfo->frameCount = text;
	frameCountChangedSignal(text.toInt());
	setChannelWidget();
}

void SettingDlg::powerTypeComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("电源类型", text);
	m_deviceInfo->powerType = text;
}

void SettingDlg::powerVoltageComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("电源电压", text);
	m_deviceInfo->powerVoltage = text;
}

void SettingDlg::powerCurrentComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("电源电流", text);
	m_deviceInfo->powerCurrent = text;
}

void SettingDlg::powerPortComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("电源串口", text);
	m_deviceInfo->powerPort = text;
}

void SettingDlg::powerBuadrateSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("电源波特率", text);
	m_deviceInfo->powerBaudrate = text;
}

void SettingDlg::canNameComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("CAN名称", text);
	m_deviceInfo->canName = text;
}

void SettingDlg::canBaudrateComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("CAN波特率", text);
	m_deviceInfo->canBaudrate = text;
}

void SettingDlg::canExpandFrameComboSlot(const QString& text)
{
	GET_JSON->setDeviceInfoValue("CAN拓展帧", text);
	m_deviceInfo->canExpandFrame = text;
}

void SettingDlg::channelStateChangedSlot(int state)
{
	int index = static_cast<QCheckBox*>(sender())->objectName().split("_")[1].toInt();
	m_channelInfo[index].enable = state;
	GET_JSON->setChannelInfoValue(QString::number(index), QString::number(state));
	m_channelChanged = true;
}

void SettingDlg::chooseAllChannelSlot(int state)
{
	for (int i = 0; i < m_channelList.size(); i++)
		m_channelList[i]->setChecked(state);
	return;
}

void SettingDlg::canMsgIdSlot()
{
	do
	{
		bool convert = false;
		QString id = ui.canIdLine->text();
		id.toInt(&convert, 16);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "CAN报文帧ID必须为16进制整数");
			ui.canIdLine->setText(m_canMsgInfo->id);
			break;
		}
		//GET_JSON->setCanMsgInfoValue("ID", id);
		m_canMsgInfo->id = id;
	} while (false);
	return;
}

void SettingDlg::canMsgDataSlot()
{
	do
	{
		QString data = ui.canDataLine->text();
		QStringList split = data.split(" ");
		if (split.size() != 8)
		{
			utility::QMessageBoxEx::warning(this, "错误", "CAN报文数据必须为8个字节,中间以空格分开");
			ui.canDataLine->setText(m_canMsgInfo->data);
			break;
		}

		bool convert = false, success = true;
		int i = 0;
		for (; i < split.size(); i++)
		{
			split[i].toInt(&convert, 16);
			if (!convert)
			{
				success = false;
				break;
			}
		}

		if (!success)
		{
			utility::QMessageBoxEx::warning(this, "错误", QString("CAN报文数据第%1个字节不为16进制值").arg(i + 1));
			ui.canDataLine->setText(m_canMsgInfo->data);
			break;
		}
		//GET_JSON->setCanMsgInfoValue("数据", data);
		m_canMsgInfo->data = data;
	} while (false);
	return;
}

void SettingDlg::canMsgIntervalSlot()
{
	do
	{
		bool convert = false;
		QString interval = ui.canIntervalLine->text();
		interval.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "CAN报文间隔不为整数");
			ui.canIntervalLine->setText(m_canMsgInfo->period);
			break;
		}
		//GET_JSON->setCanMsgInfoValue("间隔", interval);
		m_canMsgInfo->period = interval;
	} while (false);
	return;
}

void SettingDlg::canMsgTimesSlot()
{
	do
	{
		bool convert = false;
		QString times = ui.canTimesLine->text();
		times.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "CAN报文次数不为整数");
			ui.canIntervalLine->setText(m_canMsgInfo->count);
			break;
		}
		//GET_JSON->setCanMsgInfoValue("次数", times);
		m_canMsgInfo->count = times;
	} while (false);
	return;
}

void SettingDlg::canMsgEnableSlot(int state)
{
	//GET_JSON->setCanMsgInfoValue("启用", QString::number(state));
	m_canMsgInfo->enable = QString::number(state);
	emit setCanMsgEnableSignal(state);
}

void SettingDlg::imageAlgorithmSlot(const QString& text)
{
	GET_JSON->setTestInfoValue("图像算法", text);
}

void SettingDlg::detectionTimeSlot()
{
	bool result = false;
	do
	{
		bool convert = false;
		QString time = ui.detectionTimeLine->text();
		time.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "检测时间不为整数");
			break;
		}
		GET_JSON->setTestInfoValue("检测时间", time);
		result = true;
	} while (false);
	if (!result)
	{
		ui.detectionTimeLine->setText(m_testInfo->detectionTime);
	}
	return;
}

void SettingDlg::powerOnWaitSlot()
{
	bool result = false;
	do
	{
		bool convert = false;
		QString wait = ui.powerOnWaitLine->text();
		wait.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "上电时间不为整数");
			break;
		}
		GET_JSON->setTestInfoValue("上电等待", wait);
		result = true;
	} while (false);
	if (!result)
	{
		ui.powerOnWaitLine->setText(m_testInfo->powerOnWait);
	}
	return;
}

void SettingDlg::frontColorSlot()
{
	QString frontColor = ui.frontColorLine->text();
	if (!isColorInfoInputRight(frontColor))
	{
		ui.frontColorLine->setText(m_testInfo->frontColor);
		return;
	}
	GET_JSON->setTestInfoValue("前景颜色", frontColor);
	return;
}

void SettingDlg::rearColorSlot()
{
	QString rearColor = ui.rearColorLine->text();
	if (!isColorInfoInputRight(rearColor))
	{
		ui.rearColorLine->setText(m_testInfo->rearColor);
		return;
	}
	GET_JSON->setTestInfoValue("后景颜色", rearColor);
	return;
}

void SettingDlg::leftColorSlot()
{
	QString leftColor = ui.leftColorLine->text();
	if (!isColorInfoInputRight(leftColor))
	{
		ui.leftColorLine->setText(m_testInfo->leftColor);
		return;
	}
	GET_JSON->setTestInfoValue("左景颜色", leftColor);
	return;
}

void SettingDlg::rightColorSlot()
{
	QString rightColor = ui.rightColorLine->text();
	if (!isColorInfoInputRight(rightColor))
	{
		ui.rightColorLine->setText(m_testInfo->rightColor);
		return;
	}
	GET_JSON->setTestInfoValue("右景颜色", rightColor);
	return;
}

void SettingDlg::determineCountSlot()
{
	bool result = false;
	do
	{
		bool convert = false;
		QString times = ui.determineCountLine->text();
		times.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "判定次数不为整数");
			break;
		}
		GET_JSON->setTestInfoValue("判定次数", times);
		result = true;
	} while (false);
	if (!result)
	{
		ui.determineCountLine->setText(m_testInfo->determineCount);
	}
	return;
}

void SettingDlg::blackPercentSlot()
{
	bool result = false;
	do 
	{
		bool convert = false;
		QString value = ui.blackPercentLine->text();
		float precent = value.toFloat(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "黑色占比只能为数字");
			break;
		}

		if (precent < 0 || precent >= 100.0000001)
		{
			utility::QMessageBoxEx::warning(this, "错误", "黑色占比范围必须在0.0~100.0之间");
			break;
		}
		GET_JSON->setTestInfoValue("黑色占比", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.blackPercentLine->setText(m_testInfo->blackPercent);
	}
	return;
}

void SettingDlg::whitePercentSlot()
{
	bool result = false;
	do 
	{
		bool convert = false;
		QString value = ui.whitePercentLine->text();
		float precent = value.toFloat(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "白色占比只能为数字");
			break;
		}

		if (precent < 0 || precent >= 100.0000001)
		{
			utility::QMessageBoxEx::warning(this, "错误", "白色占比范围必须在0.0~100.0之间");
			break;
		}
		GET_JSON->setTestInfoValue("白色占比", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.whitePercentLine->setText(m_testInfo->whitePercent);
	}
	return;
}

void SettingDlg::binaryThreshSlot()
{
	bool result = false, convert = false;
	do
	{
		QString value = ui.binaryThreshLine->text();
		float thresh = value.toFloat(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "二值化值只能为数字");
			break;
		}

		if (thresh < 0 || thresh >= 255.0000001)
		{
			utility::QMessageBoxEx::warning(this, "错误", "二值化值范围必须在0.0~255.0之间");
			break;
		}
		GET_JSON->setTestInfoValue("二值化值", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.binaryThreshLine->setText(m_testInfo->binaryThresh);
	}
	return;
}

void SettingDlg::dynamicPercentSlot()
{
	bool result = false, convert = false;
	do
	{
		QString value = ui.dynamicPercentLine->text();
		value.toDouble(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "动态占比只能为数字");
			break;
		}

		GET_JSON->setTestInfoValue("动态占比", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.dynamicPercentLine->setText(m_testInfo->dynamicPercent);
	}
	return;
}

void SettingDlg::dynamicThreshSlot()
{
	bool result = false, convert = false;
	do
	{
		QString value = ui.dynamicThreshLine->text();
		double thresh = value.toDouble(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "动态阈值只能为数字");
			break;
		}

		if (thresh < 0 || thresh >= 255.0000001)
		{
			utility::QMessageBoxEx::warning(this, "错误", "二值化值范围必须在0.0~255.0之间");
			break;
		}

		GET_JSON->setTestInfoValue("动态阈值", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.dynamicThreshLine->setText(m_testInfo->dynamicThresh);
	}
}

void SettingDlg::dynamicFrequencySlot()
{
	bool result = false, convert = false;
	do
	{
		QString value = ui.dynamicFrequencyLine->text();
		value.toInt(&convert);
		if (!convert)
		{
			utility::QMessageBoxEx::warning(this, "错误", "动态频率只能为整数");
			break;
		}

		GET_JSON->setTestInfoValue("动态频率", value);
		result = true;
	} while (false);
	if (!result)
	{
		ui.dynamicFrequencyLine->setText(m_testInfo->dynamicFrequency);
	}
}

void SettingDlg::dynamicDetectionSlot(int state)
{
	GET_JSON->setTestInfoValue("动态检测", QString::number(state));
}

void SettingDlg::skipDetectionSlot(int state)
{
	GET_JSON->setTestInfoValue("跳过检测", QString::number(state));
}

void SettingDlg::codeJudgeSlot()
{
	QString judge = ui.codeJudgeLine->text();
	GET_JSON->setBaseInfoValue("条码判断", judge);
}

void SettingDlg::codeLengthSlot()
{
	bool convert = false;
	QString length = ui.codeLengthLine->text();
	length.toInt(&convert);
	if (!convert)
	{
		length = QString::number(length.size());
		ui.codeLengthLine->setText(length);
	}
	GET_JSON->setBaseInfoValue("条码长度", length);
}

void SettingDlg::enableJudgeSlot(int state)
{
	GET_JSON->setBaseInfoValue("启用判断", QString::number(state));
}

void SettingDlg::enableUploadSlot(int state)
{
	GET_JSON->setBaseInfoValue("启用上传", QString::number(state));
}

void SettingDlg::userNameSlot()
{
	QString userName = ui.userNameLine->text();
	GET_JSON->setAccountInfoValue("用户名称", userName);
}

void SettingDlg::userPasswordSlot()
{
	QString userPassword = ui.userPasswordLine->text();
	GET_JSON->setAccountInfoValue("用户密码", userPassword);
}

void SettingDlg::specifyRowRectComboSlot(const QString& text)
{
	int index = text.toInt() - 1;
	ui.specifyRowXOffsetHSlider->setValue(m_specifyRowInfo[index].x);
	ui.specifyRowYOffsetHSlider->setValue(m_specifyRowInfo[index].y);
}

void SettingDlg::specifyRowXOffsetHSliderSlot(int x)
{
	int index = ui.specifyRowRectCombo->currentIndex();
	m_specifyRowInfo[index].x = x;
	GET_JSON->setSpecifyRowInfoValue(QString::number(index), "X", QString::number(x));
}

void SettingDlg::specifyRowYOffsetHSliderSlot(int y)
{
	int index = ui.specifyRowRectCombo->currentIndex();
	m_specifyRowInfo[index].y = y;
	GET_JSON->setSpecifyRowInfoValue(QString::number(index), "Y", QString::number(y));
}

void SettingDlg::specifyColumnRectComboSlot(const QString& text)
{
	int index = text.toInt() - 1;
	ui.specifyColumnXOffsetHSlider->setValue(m_specifyColumnInfo[index].x);
	ui.specifyColumnYOffsetHSlider->setValue(m_specifyColumnInfo[index].y);
}

void SettingDlg::specifyColumnXOffsetHSliderSlot(int x)
{
	int index = ui.specifyColumnRectCombo->currentIndex();
	m_specifyColumnInfo[index].x = x;
	GET_JSON->setSpecifyColumnInfoValue(QString::number(index), "X", QString::number(x));
}

void SettingDlg::specifyColumnYOffsetHSliderSlot(int y)
{
	int index = ui.specifyColumnRectCombo->currentIndex();
	m_specifyColumnInfo[index].y = y;
	GET_JSON->setSpecifyColumnInfoValue(QString::number(index), "Y", QString::number(y));
}

void SettingDlg::chooseColorSlot()
{
	QPushButton* button = dynamic_cast<QPushButton*>(sender());
	if (!button)
	{
		utility::QMessageBoxEx::warning(this, "错误", "dynamic_cast触发了一个nullptr异常");
		return;
	}

	QString objectName = button->objectName();
	QString* valuePtr = nullptr;
	QLineEdit* lineEdit = nullptr;
	if (objectName == ui.frontRectButton->objectName())
	{
		valuePtr = &m_colorInfo->frontRect;
		lineEdit = ui.frontRectLine;
	}
	else if (objectName == ui.rearRectButton->objectName())
	{
		valuePtr = &m_colorInfo->rearRect;
		lineEdit = ui.rearRectLine;
	}
	else if (objectName == ui.leftRectButton->objectName())
	{
		valuePtr = &m_colorInfo->leftRect;
		lineEdit = ui.leftRectLine;
	}
	else if (objectName == ui.rightRectButton->objectName())
	{
		valuePtr = &m_colorInfo->rightRect;
		lineEdit = ui.rightRectLine;
	}
	else if (objectName == ui.channelFontButton->objectName())
	{
		valuePtr = &m_colorInfo->channelFont;
		lineEdit = ui.channelFontLine;
	}
	else if (objectName == ui.textFontButton->objectName())
	{
		valuePtr = &m_colorInfo->textFont;
		lineEdit = ui.textFontLine;
	}
	else if (objectName == ui.disableRectColorButton->objectName())
	{
		valuePtr = &m_colorInfo->disableRect;
		lineEdit = ui.disableRectColorLine;
	}
	else if (objectName == ui.disableLineColorButton->objectName())
	{
		valuePtr = &m_colorInfo->disableLine;
		lineEdit = ui.disableLineColorLine;
	}
	else
	{
		utility::QMessageBoxEx::warning(this, "错误", QString("未知的对象名[%1]").arg(objectName));
		return;
	}

	QStringList split = valuePtr->split(",", QString::SkipEmptyParts);
	if (split.size() != 3)
	{
		utility::QMessageBoxEx::warning(this, "错误", "颜色数据解析错误");
		return;
	}

	QColor rgb = { split[0].toInt(),split[1].toInt(),split[2].toInt() };
	QColor color = QColorDialog::getColor(rgb, this, "颜色对话框", QColorDialog::ShowAlphaChannel);
	if (color.isValid())
	{
		*valuePtr = Q_SPRINTF("%d,%d,%d", color.red(), color.green(), color.blue());
		lineEdit->setStyleSheet(QString("background-color:rgb(%1)").arg(*valuePtr));
	}

	GET_JSON->callParseColorInfo();

	return;
}

void SettingDlg::restoreDefaultColorSlot()
{
	QList<QLineEdit*> lineList = 
	{
		ui.frontRectLine,
		ui.rearRectLine,
		ui.leftRectLine,
		ui.rightRectLine,
		ui.channelFontLine,
		ui.textFontLine,
		ui.disableRectColorLine,
		ui.disableLineColorLine
	};

	QStringList colorList = GET_JSON->getColorInfoValueList();
	if (lineList.size() != colorList.size())
	{
		utility::QMessageBoxEx::warning(this, "错误", "控件列表与颜色列表数量不一致");
		return;
	}

	QString* valuePtr = reinterpret_cast<QString*>(m_colorInfo);
	for (int i = 0; i < lineList.size(); i++, valuePtr++)
	{
		lineList[i]->setStyleSheet(QString("background-color:rgb(%1)").arg(colorList[i]));
		*valuePtr = colorList[i];
	}
	GET_JSON->callParseColorInfo();
	return;
}

void SettingDlg::initDeviceInfo()
{
	do
	{
		m_deviceInfoBackup = *m_deviceInfo;

		QList<QComboBox*> comboList = 
		{
			ui.imageResolutionCombo,
			ui.captureNameCombo,
			ui.frameCountCombo,
			ui.powerTypeCombo,
			ui.powerVoltageCombo,
			ui.powerCurrentCombo,
			ui.powerPortCombo,
			ui.powerBuadrateCombo,
			ui.canNameCombo,
			ui.canBaudrateCombo,
			ui.canExpandFrame
		};

		auto portList = comm::Serial::getPortList();
		for (int i = 0; i < portList.size(); i++) {
			ui.powerPortCombo->addItem(QIcon(":/Images/Resources/Images/key.ico"), portList[i].name.c_str());
		}

		if (comboList.size() != sizeof(DeviceInfo) / sizeof(QString))
		{
			utility::QMessageBoxEx::warning(this, "错误", "加载设备数据失败,组合框控件数量与结构体数量不匹配");
			break;
		}

		void (SettingDlg:: * comboFnc[])(const QString&) =
		{
			&SettingDlg::imageResolutionComboSlot,
			&SettingDlg::captureNameComboSlot,
			&SettingDlg::frameCountComboSlot,
			&SettingDlg::powerTypeComboSlot,
			&SettingDlg::powerVoltageComboSlot,
			&SettingDlg::powerCurrentComboSlot,
			&SettingDlg::powerPortComboSlot,
			&SettingDlg::powerBuadrateSlot,
			&SettingDlg::canNameComboSlot,
			&SettingDlg::canBaudrateComboSlot,
			&SettingDlg::canExpandFrameComboSlot
		};

		if (comboList.size() != sizeof(comboFnc) / sizeof(*comboFnc))
		{
			utility::QMessageBoxEx::warning(this, "错误", "关联信号槽失败,组合框控件数量与组合框函数指针数字不匹配");
			break;
		}

		QString* valuePtr = reinterpret_cast<QString*>(m_deviceInfo);
		for (int i = 0; i < comboList.size(); i++, valuePtr++)
		{
			comboList[i]->setCurrentText(*valuePtr);
			connect(comboList[i], &QComboBox::currentTextChanged, this, comboFnc[i]);
		}

	} while (false);
	return;
}

void SettingDlg::initRectInfo()
{

	memcpy(&m_rectInfoBackup, m_rectInfo, sizeof(m_rectInfoBackup));

	connect(ui.rowCountCombo, &QComboBox::currentTextChanged, this, &SettingDlg::rowCountComboChangedSlot);

	connect(ui.columnCountCombo, &QComboBox::currentTextChanged, this, &SettingDlg::columnCountComboChangedSlot);

	connect(ui.channelAlignCombo, CAST_QCOMBOBOX_CHANGED_INT, this, &SettingDlg::channelAlignComboSlot);

	ui.rowCountCombo->setCurrentText(N_TO_Q_STR(m_rectInfo->rows));

	ui.columnCountCombo->setCurrentText(N_TO_Q_STR(m_rectInfo->columns));

	ui.channelAlignCombo->setCurrentIndex(m_rectInfo->channelAlign);

	QList<QSlider*> senderList = 
	{
		ui.xOffsetHSlider,
		ui.yOffsetHSlider,
		ui.overallFRIntervalHSlider,
		ui.overallLRIntervalHSlider,

		ui.frontRectXOffsetHSlider,
		ui.frontRectYOffsetHSlider,
		ui.frontRectWidthHSlider,
		ui.frontRectHeightHSlider,

		ui.rearRectXOffsetHSlider,
		ui.rearRectYOffsetHSlider,
		ui.rearRectWidthHSlider,
		ui.rearRectHeightHSlider,

		ui.leftRectXOffsetHSlider,
		ui.leftRectYOffsetHSlider,
		ui.leftRectWidthHSlider,
		ui.leftRectHeightHSlider,

		ui.rightRectXOffsetHSlider,
		ui.rightRectYOffsetHSlider,
		ui.rightRectWidthHSlider,
		ui.rightRectHeightHSlider,

		
		ui.internalFrontIntervalHSlider,
		ui.internalRearIntervalHSlider,
		ui.internalLeftIntervalHSlider,
		ui.internalRightIntervalHSlider
	};

	void (SettingDlg:: * slotFnc[])(int) = 
	{
		&SettingDlg::setOverallXOffsetSlot,
		&SettingDlg::setOverallYOffsetSlot,
		&SettingDlg::setOverallFRIntervalSlot,
		&SettingDlg::setOverallLRIntervalSlot,

		&SettingDlg::setFrontRectXSlot,
		&SettingDlg::setFrontRectYSlot,
		&SettingDlg::setFrontRectWidthSlot,
		&SettingDlg::setFrontRectHeightSlot,

		&SettingDlg::setRearRectXSlot,
		&SettingDlg::setRearRectYSlot,
		&SettingDlg::setRearRectWidthSlot,
		&SettingDlg::setRearRectHeightSlot,

		&SettingDlg::setLeftRectXSlot,
		&SettingDlg::setLeftRectYSlot,
		&SettingDlg::setLeftRectWidthSlot,
		&SettingDlg::setLeftRectHeightSlot,

		&SettingDlg::setRightRectXSlot,
		&SettingDlg::setRightRectYSlot,
		&SettingDlg::setRightRectWidthSlot,
		&SettingDlg::setRightRectHeightSlot,

	
		&SettingDlg::setInternalFrontIntervalSlot,
		&SettingDlg::setInternalRearIntervalSlot,
		&SettingDlg::setInternalLeftIntervalSlot,
		&SettingDlg::setInternalRightIntervalSlot
	};

	if (senderList.size() != sizeof(slotFnc) / sizeof(*slotFnc))
	{
		utility::QMessageBoxEx::warning(this, "错误", "信号槽数量不一致绑定失败");
		return;
	}

	int* valuePtr = &m_rectInfo->overallXOffset;
	for (int i = 0; i < senderList.size(); i++, valuePtr++)
	{
		connect(senderList[i], &QSlider::valueChanged, this, slotFnc[i]);
		senderList[i]->setValue(*valuePtr);
	}

	connect(ui.showChannelIdCheck, &QCheckBox::clicked, this, &SettingDlg::setShowChannelIdSlot);

	connect(ui.showTextCheck, &QCheckBox::clicked, this, &SettingDlg::setShowTextSlot);

	connect(ui.showRectCheck, &QCheckBox::clicked, this, &SettingDlg::setShowRectSlot);

	connect(ui.showDisableCheck, &QCheckBox::clicked, this, &SettingDlg::setShowDisableSlot);

	connect(ui.showBinaryThreshCheck, &QCheckBox::clicked, this, &SettingDlg::setShowBinaryThreshSlot);

	ui.showChannelIdCheck->setChecked(m_rectInfo->showChannelId);

	ui.showTextCheck->setChecked(m_rectInfo->showText);

	ui.showRectCheck->setChecked(m_rectInfo->showRect);

	ui.showDisableCheck->setChecked(m_rectInfo->showDisable);
}

void SettingDlg::initChannelInfo()
{
	ChannelInfo::copyArray(m_channelInfoBackup, m_channelInfo);
	setChannelWidget();
}

void SettingDlg::setChannelWidget()
{
	m_channelList.clear();

	QLayout* layout = ui.channelGroup->layout();
	if (layout)
	{
		QLayoutItem* item = nullptr;
		while (item = layout->takeAt(0))
		{
			if (item->widget())
				delete item->widget();
			delete item;
		}
	}
	else
	{
		layout = new QGridLayout;
	}

	int frameCount = m_deviceInfo->frameCount.toInt();

	int rows = m_rectInfo->rows * frameCount;

	int columns = m_rectInfo->columns;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int index = (j + 1) + i * columns;
			QCheckBox* check = new QCheckBox(N_TO_Q_STR(index), ui.channelGroup);
			check->setObjectName(Q_SPRINTF("channelCheckBox_%d", index - 1));
			check->setChecked(m_channelInfo[index - 1].enable);
			connect(check, &QCheckBox::stateChanged, this, &SettingDlg::channelStateChangedSlot);
			static_cast<QGridLayout*>(layout)->addWidget(check, i, j);
			m_channelList.append(check);
		}
	}

	QCheckBox* chooseAll = new QCheckBox("全选", ui.channelGroup);
	connect(chooseAll, &QCheckBox::stateChanged, this, &SettingDlg::chooseAllChannelSlot);
	static_cast<QGridLayout*>(layout)->addWidget(chooseAll, rows, columns - 1);
	ui.channelGroup->setLayout(layout);
}

void SettingDlg::initCanMsgInfo()
{
	m_canMsgInfoBackup = *m_canMsgInfo;

	ui.canIdLine->setText(m_canMsgInfo->id);

	ui.canDataLine->setText(m_canMsgInfo->data);

	ui.canIntervalLine->setText(m_canMsgInfo->period);

	ui.canTimesLine->setText(m_canMsgInfo->count);

	ui.canEnableCheck->setChecked(m_canMsgInfo->enable.toInt());

	connect(ui.canIdLine, &QLineEdit::editingFinished, this, &SettingDlg::canMsgIdSlot);

	connect(ui.canDataLine, &QLineEdit::editingFinished, this, &SettingDlg::canMsgDataSlot);

	connect(ui.canIntervalLine, &QLineEdit::editingFinished, this, &SettingDlg::canMsgIntervalSlot);

	connect(ui.canTimesLine, &QLineEdit::editingFinished, this, &SettingDlg::canMsgTimesSlot);

	connect(ui.canEnableCheck, &QCheckBox::stateChanged, this, &SettingDlg::canMsgEnableSlot);
}

void SettingDlg::initTestInfo()
{
	ui.imageAlgorithmCombo->setCurrentText(m_testInfo->imageAlgorithm);
	connect(ui.imageAlgorithmCombo, &QComboBox::currentTextChanged, this, &SettingDlg::imageAlgorithmSlot);

	ui.dynamicDetectionCheck->setChecked(m_testInfo->dynamicDetection.toInt());
	connect(ui.dynamicDetectionCheck, &QCheckBox::stateChanged, this, &SettingDlg::dynamicDetectionSlot);

	ui.skipDetectionCheck->setChecked(m_testInfo->skipDetection.toInt());
	connect(ui.skipDetectionCheck, &QCheckBox::stateChanged, this, &SettingDlg::skipDetectionSlot);

	QList<QLineEdit*> lineList = 
	{
		ui.detectionTimeLine,//5
		ui.powerOnWaitLine,//1
		ui.blackPercentLine,//2
		ui.whitePercentLine,//3
		ui.binaryThreshLine,//4
		ui.frontColorLine,//6
		ui.rearColorLine,//7
		ui.leftColorLine,//8
		ui.rightColorLine,//9
		ui.determineCountLine,//10
		ui.dynamicPercentLine,//11
		ui.dynamicThreshLine,//12
		ui.dynamicFrequencyLine//13
	};

	void (SettingDlg:: * slotFnc[])() =
	{
		&SettingDlg::detectionTimeSlot,//5
		&SettingDlg::powerOnWaitSlot,//1
		&SettingDlg::blackPercentSlot,//2
		&SettingDlg::whitePercentSlot,//3
		&SettingDlg::binaryThreshSlot,//4
		&SettingDlg::frontColorSlot,//6
		&SettingDlg::rearColorSlot,//7
		&SettingDlg::leftColorSlot,//8
		&SettingDlg::rightColorSlot,//9
		&SettingDlg::determineCountSlot,//10
		&SettingDlg::dynamicPercentSlot,//11
		&SettingDlg::dynamicThreshSlot,//12
		&SettingDlg::dynamicFrequencySlot//13
	};
	
	if (lineList.size() != sizeof(slotFnc) / sizeof(*slotFnc))
	{
		utility::QMessageBoxEx::warning(this, "错误", "文本框列表与槽函数指针数组不相等");
		return;
	}

	QString* valuePtr = &m_testInfo->detectionTime;
	for (int i = 0; i < lineList.size(); i++, valuePtr++)
	{
		lineList[i]->setText(*valuePtr);
		connect(lineList[i], &QLineEdit::editingFinished, this, slotFnc[i]);
	}
	return;
}

void SettingDlg::initBaseInfo()
{
	ui.codeJudgeLine->setText(m_baseInfo->codeJudge);

	ui.codeLengthLine->setText(m_baseInfo->codeLength);

	ui.enableJudgeCheck->setChecked(m_baseInfo->enableJudge.toInt());

	ui.enableUploadCheck->setChecked(m_baseInfo->enableUpload.toInt());

	connect(ui.codeJudgeLine, &QLineEdit::editingFinished, this, &SettingDlg::codeJudgeSlot);

	connect(ui.codeLengthLine, &QLineEdit::editingFinished, this, &SettingDlg::codeLengthSlot);

	connect(ui.enableJudgeCheck, &QCheckBox::stateChanged, this, &SettingDlg::enableJudgeSlot);

	connect(ui.enableUploadCheck, &QCheckBox::stateChanged, this, &SettingDlg::enableUploadSlot);
}

bool SettingDlg::isColorInfoInputRight(const QString& color)
{
	bool result = false;
	do
	{
		QString suffix = color.mid(0, 2);
		if (suffix != "!=" && suffix != "==")
		{
			utility::QMessageBoxEx::warning(this, "错误", "颜色条件前2个字符必须为!=或==");
			break;
		}

		if (color.contains("，"))
		{
			utility::QMessageBoxEx::warning(this, "错误", "颜色条件不可包含中文字符逗号");
			break;
		}
		result = true;
	} while (false);
	return result;
}

void SettingDlg::initSpecifyInfo()
{
	memcpy(m_specifyRowInfoBackup, m_specifyRowInfo, sizeof(m_specifyRowInfoBackup));

	memcpy(m_specifyColumnInfoBackup, m_specifyColumnInfo, sizeof(m_specifyColumnInfoBackup));

	connect(ui.specifyRowRectCombo, &QComboBox::currentTextChanged, this, &SettingDlg::specifyRowRectComboSlot);

	connect(ui.specifyRowXOffsetHSlider, &QSlider::valueChanged, this, &SettingDlg::specifyRowXOffsetHSliderSlot);
	ui.specifyRowXOffsetHSlider->setValue(m_specifyRowInfo[0].x);

	connect(ui.specifyRowYOffsetHSlider, &QSlider::valueChanged, this, &SettingDlg::specifyRowYOffsetHSliderSlot);
	ui.specifyRowYOffsetHSlider->setValue(m_specifyRowInfo[0].y);

	connect(ui.specifyColumnRectCombo, &QComboBox::currentTextChanged, this, &SettingDlg::specifyColumnRectComboSlot);

	connect(ui.specifyColumnXOffsetHSlider, &QSlider::valueChanged, this, &SettingDlg::specifyColumnXOffsetHSliderSlot);
	ui.specifyColumnXOffsetHSlider->setValue(m_specifyColumnInfo[0].x);

	connect(ui.specifyColumnYOffsetHSlider, &QSlider::valueChanged, this, &SettingDlg::specifyColumnYOffsetHSliderSlot);
	ui.specifyColumnYOffsetHSlider->setValue(m_specifyColumnInfo[0].y);
}

void SettingDlg::initShareInfo()
{
	m_colorInfoBackup = *m_colorInfo;

	ui.userNameLine->setText(m_accountInfo->userName);

	ui.userPasswordLine->setText(m_accountInfo->userPassword);

	connect(ui.userNameLine, &QLineEdit::editingFinished, this, &SettingDlg::userNameSlot);

	connect(ui.userPasswordLine, &QLineEdit::editingFinished, this, &SettingDlg::userPasswordSlot);

	connect(ui.restoreDefaultColorButton, &QPushButton::clicked, this, &SettingDlg::restoreDefaultColorSlot);

	QList<QLineEdit*> lineList =
	{
		ui.frontRectLine,
		ui.rearRectLine,
		ui.leftRectLine,
		ui.rightRectLine,
		ui.channelFontLine,
		ui.textFontLine,
		ui.disableRectColorLine,
		ui.disableLineColorLine
	};

	QString* valuePtr = reinterpret_cast<QString*>(m_colorInfo);
	for (int i = 0; i < lineList.size(); i++, valuePtr++)
	{
		lineList[i]->setStyleSheet(QString("background-color:rgb(%1)").arg(*valuePtr));
	}

	QList<QPushButton*> buttonList = 
	{
		ui.frontRectButton,
		ui.rearRectButton,
		ui.leftRectButton,
		ui.rightRectButton,
		ui.channelFontButton,
		ui.textFontButton,
		ui.disableRectColorButton,
		ui.disableLineColorButton
	};

	for (int i = 0; i < buttonList.size(); i++)
	{
		connect(buttonList[i], &QPushButton::clicked, this, &SettingDlg::chooseColorSlot);
	}
}

void SettingDlg::initControl()
{
	if (!m_connect)
	{
		return;
	}

	QList<QWidget*> widgetList = 
	{
		ui.frameCountCombo,
		ui.captureNameCombo,
		ui.powerTypeCombo,
		ui.powerVoltageCombo,
		ui.powerCurrentCombo,
		ui.powerPortCombo,
		ui.powerBuadrateCombo,
		ui.canNameCombo,
		ui.canBaudrateCombo,
		ui.canExpandFrame,
		ui.rowCountCombo,
		ui.columnCountCombo
	};

	for (auto& x : widgetList)
	{
		x->setEnabled(false);
	}
}

