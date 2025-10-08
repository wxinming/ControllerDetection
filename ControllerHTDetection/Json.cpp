#include "Json.h"

Json* Json::m_self = nullptr;

Json* Json::getInstance()
{
	if (!m_self)
		m_self = NO_THROW_NEW Json;
	return m_self;
}

void Json::deleteInstance()
{
	SAFE_DELETE(m_self);
}

QString Json::getLastError() const
{
	return m_lastError;
}

bool Json::initialize(const QString& moduleName, bool update)
{
	bool result = false, success = true;
	do 
	{
		QString dirName = "Config";

		if (!utility::existDir(dirName))
		{
			utility::makeDir(dirName);
		}
	
		QString fileName = QString("%1\\Share.ht").arg(dirName);

		if (!utility::file::exist(fileName) || update)
		{
			if (update ? !updateShareFile(fileName) : !writeShareFile(fileName))
			{
				success = false;
				break;
			}
		}

		if (!success || !readShareFile(fileName))
		{
			break;
		}

		fileName = QString("%1\\%2.json").arg(dirName, moduleName);

		if (!utility::file::exist(fileName) || update)
		{
			if (update ? !updateTypeFile(fileName) : !writeTypeFile(fileName))
			{
				success = false;
				break;
			}
		}

		if (!success || !readTypeFile(fileName))
		{
			break;
		}

		result = true;
	} while (false);
	return result;
}

int Json::getIndex(int row, int column) const
{
	int number = 0;
	switch (m_rectInfo.channelAlign)
	{
	case ChannelAlign::CA_COLUMN_RIGHT:
		//编号通过每列向右数,样例
		//1  2  3  4
		//5  6  7  8
		//9  10 11 12
		//13 14 15 16
		number = (column + 1) + row * m_rectInfo.columns;
		break;
	case ChannelAlign::CA_ROW_DOWN:
		//编号通过每行向下数,样例
		//1  5  9   13
		//2  6  10  14
		//3  7  11  15
		//4  8  12  16
		number = (row + 1) + column * m_rectInfo.rows;
		break;
	case ChannelAlign::CA_SQUARE_RIGHT:
		//编号通过正方形向右数,样例
		//1  2  5  6
		//3  4  7  8
		//9  10 13 14
		//11 12 15 16
	{
		const int step = m_rectInfo.columns / (m_rectInfo.columns / 2);
		const int evenStep = m_rectInfo.columns / (m_rectInfo.columns / 2);
		const int oddStep = ((m_rectInfo.columns - 1) * 2 + step) * (row / 2);
		if ((row + 1) % 2)//奇数行
			if ((column + 1) > step)
			{
				int value = !(column % 2) ? 1 : 0;
				//每行增加 = (n - 1) * 2 +系数(value)
				int rowStep = column * 2 + value;
				number = rowStep + (row ? oddStep : 0);
			}
			else
				number = column + 1 + row * m_rectInfo.columns;
		else
			if ((column + 1) > m_rectInfo.columns - step)
				number = column + 1 + row * m_rectInfo.columns;
			else
			{
				int value = !(column % 2) ? 1 : 0;
				//每行增加 = (n - 1) * 2 +系数(value)
				int rowStep = column * 2 + value;
				number = rowStep + (row ? oddStep : 0) + evenStep;
			}
	}
	break;
	default:
		break;
	}
	return number;
}

DeviceInfo* Json::getDeviceInfo() const
{
	return const_cast<DeviceInfo*>(&m_deviceInfo);
}

QString Json::getDeviceInfoValue(const QString& key) const
{
	return m_deviceInfoObj.value(key).toString();
}

bool Json::setDeviceInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_deviceInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_deviceInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

RectInfo* Json::getRectInfo() const
{
	return const_cast<RectInfo*>(&m_rectInfo);
}

QString Json::getRectInfoValue(const QString& key) const
{
	return m_rectInfoObj.value(key).toString();
}

bool Json::setRectInfoValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do
	{
		value.toInt(&convert);
		RUN_BREAK(!convert, QString("键[%1]必须位整数").arg(key));
		RUN_BREAK(!m_rectInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_rectInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

ChannelInfo* Json::getChannelInfo() const
{
	return const_cast<ChannelInfo*>(m_channelInfo);
}

int Json::getChannelInfoValue(const QString& key) const
{
	return m_channelInfoObj.value(key).toString().toInt();
}

bool Json::setChannelInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_channelInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_channelInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

CanMsgInfo* Json::getCanMsgInfo() const
{
	return const_cast<CanMsgInfo*>(&m_canMsgInfo);
}

QString Json::getCanMsgInfoValue(const QString& key) const
{
	return m_canMsgInfoObj.value(key).toString();
}

bool Json::setCanMsgInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_canMsgInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_canMsgInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

TestInfo* Json::getTestInfo() const
{
	return const_cast<TestInfo*>(&m_testInfo);
}

QString Json::getTestInfoValue(const QString& key) const
{
	return m_testInfoObj.value(key).toString();
}

bool Json::setTestInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_testInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_testInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

BaseInfo* Json::getBaseInfo() const
{
	return const_cast<BaseInfo*>(&m_baseInfo);
}

QString Json::getBaseInfoValue(const QString& key) const
{
	return m_baseInfoObj.value(key).toString();
}

bool Json::setBaseInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_baseInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_baseInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

int Json::getPowerChannel() const
{
	int channel = -1;
	QString type = m_deviceInfo.powerType;
	if (type == "单通道")
		channel = 0;
	else if (type == "多通道")
		channel = 2;
	return channel;
}

int Json::getChannelTotal() const
{
	int frameCount = m_deviceInfo.frameCount.toInt();
	int rows = m_rectInfo.rows * frameCount;
	int columns = m_rectInfo.columns;
	int total = rows * columns;
	return total;
}

int Json::getEnabledChannelTotal() const
{
	int sum = 0, total = getChannelTotal();
	for (int i = 0; i < total; ++i)
	{
		if (m_channelInfoBackup[i].enable)
		{
			++sum;
		}
	}
	return sum;
}

bool Json::getChannelResult() const
{
	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		if (!m_channelInfo[i].finalResult)
		{
			return false;
		}
	}
	return true;
}

SpecifyRowInfo* Json::getSpecifyRowInfo() const
{
	return const_cast<SpecifyRowInfo*>(m_specifyRowInfo);
}

int Json::getSpecifyRowInfoValue(const QString& parentKey, const QString& childKey) const
{
	return m_specifyRowInfoObj.value(parentKey).toObject().value(childKey).toString().toInt();
}

bool Json::setSpecifyRowInfoValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_specifyRowInfoObj.contains(parentKey),
			QString("非法的键[%1]").arg(parentKey));
		QJsonObject object = m_specifyRowInfoObj[parentKey].toObject();
		RUN_BREAK(!object.contains(childKey), QString("非法的键[%1]").arg(childKey));
		object[childKey] = value;
		m_specifyRowInfoObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

SpecifyColumnInfo* Json::getSpecifyColumnInfo() const
{
	return const_cast<SpecifyColumnInfo*>(m_specifyColumnInfo);
}

int Json::getSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey) const
{
	return m_specifyColumnInfoObj.value(parentKey).toObject().value(childKey).toString().toInt(); 
}

bool Json::setSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false;
	do
	{
		RUN_BREAK(!m_specifyColumnInfoObj.contains(parentKey),
			QString("非法的键[%1]").arg(parentKey));
		QJsonObject object = m_specifyColumnInfoObj[parentKey].toObject();
		RUN_BREAK(!object.contains(childKey), QString("非法的键[%1]").arg(childKey));
		object[childKey] = value;
		m_specifyColumnInfoObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

AccountInfo* Json::getAccountInfo() const
{
	return const_cast<AccountInfo*>(&m_accountInfo);
}

QString Json::getAccountInfoValue(const QString& key) const
{
	return m_accountInfoObj.value(key).toString();
}

bool Json::setAccountInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_accountInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_accountInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

ColorInfo* Json::getColorInfo() const
{
	return const_cast<ColorInfo*>(&m_colorInfo);
}

QString Json::getColorInfoValue(const QString& key) const
{
	return m_colorInfoObj.value(key).toString();
}

bool Json::setColorInfoValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_colorInfoObj.contains(key), QString("非法的键[%1]").arg(key));
		m_colorInfoObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& Json::getColorInfoKeyList() const
{
	return m_colorInfoKeyList;
}

const QStringList& Json::getColorInfoValueList() const
{
	return m_colorInfoValueList;
}

RgbInfo Json::getRgbInfoValue(const QString& key) const
{
	if (m_rgbInfoMap.contains(key))
	{
		return m_rgbInfoMap[key];
	}
	else
	{
		return m_rgbInfoDefaultValue;
	}
}

bool Json::callParseColorInfo()
{
	return parseColorInfo(true);
}

bool Json::readTypeFile(const QString& fileName)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!utility::file::repairJson1LevelNode(fileName,
			{
				"基本信息",
				"报文信息",
				"测试信息",
				"矩形信息",
				"设备信息"
			},
			{
				m_baseInfoKeyList,
				m_canMsgKeyList,
				m_testInfoKeyList,
				m_rectInfoKeyList,
				m_deviceInfoKeyList
			},
			{
				m_baseInfoValueList,
				m_canMsgValueList,
				m_testInfoValueList,
				m_rectInfoValueList,
				m_deviceInfoValueList
			}), utility::getLastError());

		QJsonObject rootObj;

		RUN_BREAK(!utility::file::readJson(fileName, rootObj), utility::getLastError());

		RUN_BREAK(!rootObj.contains("设备信息"), "丢失对象名矩形信息");
		m_deviceInfoObj = rootObj.value("设备信息").toObject();
		if (!parseDeviceInfo())
			break;

		RUN_BREAK(!rootObj.contains("矩形信息"), "丢失对象名矩形信息");
		m_rectInfoObj = rootObj.value("矩形信息").toObject();
		if (!parseRectInfo())
			break;

		RUN_BREAK(!rootObj.contains("通道信息"), "丢失对象名通道信息");
		m_channelInfoObj = rootObj.value("通道信息").toObject();
		if (!parseChannelInfo())
			break;

		RUN_BREAK(!rootObj.contains("报文信息"), "丢失对象名报文信息");
		m_canMsgInfoObj = rootObj.value("报文信息").toObject();
		if (!parseCanMsgInfo())
			break;

		RUN_BREAK(!rootObj.contains("测试信息"), "丢失对象测试信息");
		m_testInfoObj = rootObj.value("测试信息").toObject();
		if (!parseTestInfo())
			break;

		RUN_BREAK(!rootObj.contains("基本信息"), "丢失对象基本信息");
		m_baseInfoObj = rootObj.value("基本信息").toObject();
		if (!parseBaseInfo())
			break;

		RUN_BREAK(!rootObj.contains("指定行信息"), "丢失对象指定行信息");
		m_specifyRowInfoObj = rootObj.value("指定行信息").toObject();
		if (!parseSpecifyRowInfo())
			break;

		RUN_BREAK(!rootObj.contains("指定列信息"), "丢失对象指定列信息");
		m_specifyColumnInfoObj = rootObj.value("指定列信息").toObject();
		if (!parseSpecifyColumnInfo())
			break;

		result = true;
	} while (false);
	return result;
}

bool Json::writeTypeFile(const QString& fileName)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj, deviceInfoObj, rectInfoObj,
			channelInfoObj, canMsgInfoObj, testInfoObj,
			baseInfoObj, specifyRowObj, specifyColumnObj;

		RUN_BREAK(m_deviceInfoKeyList.size() != m_deviceInfoValueList.size(),
			"设备信息键列表与值列表大小不一致");
		//添加设备信息
		for (int i = 0; i < m_deviceInfoKeyList.size(); i++)
			deviceInfoObj.insert(m_deviceInfoKeyList[i], m_deviceInfoValueList[i]);
		rootObj.insert("设备信息", deviceInfoObj);

		RUN_BREAK(m_rectInfoKeyList.size() != m_rectInfoValueList.size(), 
			"矩形信息键列表与值列表大小不一致");
		//添加矩形信息
		for (int i = 0; i < m_rectInfoKeyList.size(); i++)
			rectInfoObj.insert(m_rectInfoKeyList[i], m_rectInfoValueList[i]);
		rootObj.insert("矩形信息", rectInfoObj);

		//添加通道信息
		for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
			channelInfoObj.insert(Q_SPRINTF("%d", i), "0");
		rootObj.insert("通道信息", channelInfoObj);

		RUN_BREAK(m_canMsgKeyList.size() != m_canMsgValueList.size(), 
			"报文信息键列表与值列表大小不一致");
		//添加报文信息
		for (int i = 0; i < m_canMsgKeyList.size(); i++)
			canMsgInfoObj.insert(m_canMsgKeyList[i], m_canMsgValueList[i]);
		rootObj.insert("报文信息", canMsgInfoObj);

		RUN_BREAK(m_testInfoKeyList.size() != m_testInfoValueList.size(),
			"测试信息键列表与值列表大小不一致");
		//添加测试信息
		for (int i = 0; i < m_testInfoKeyList.size(); i++)
			testInfoObj.insert(m_testInfoKeyList[i], m_testInfoValueList[i]);
		rootObj.insert("测试信息", testInfoObj);

		RUN_BREAK(m_baseInfoKeyList.size() != m_baseInfoValueList.size(), 
			"基本信息键列表与值列表大小不一致");
		//添加基本信息
		for (int i = 0; i < m_baseInfoKeyList.size(); i++)
			baseInfoObj.insert(m_baseInfoKeyList[i], m_baseInfoValueList[i]);
		rootObj.insert("基本信息", baseInfoObj);

		//添加指定行信息
		QJsonObject specifyRowChildObj;
		for (int i = 0; i < MAX_ROW_COUNT; i++)
		{
			specifyRowChildObj.insert("X", "0");
			specifyRowChildObj.insert("Y", "0");
			specifyRowObj.insert(QString::number(i), specifyRowChildObj);
		}
		rootObj.insert("指定行信息", specifyRowObj);

		//添加指定列信息
		QJsonObject specifyColumnChildObj;
		for (int i = 0; i < MAX_COLUMN_COUNT; i++)
		{
			specifyColumnChildObj.insert("X", "0");
			specifyColumnChildObj.insert("Y", "0");
			specifyColumnObj.insert(QString::number(i), specifyColumnChildObj);
		}
		rootObj.insert("指定列信息", specifyColumnObj);

		RUN_BREAK(!utility::file::writeJson(fileName, rootObj), utility::getLastError());
		result = true;
	} while (false);
	return result;
}

bool Json::updateTypeFile(const QString& fileName)
{
	bool result = false;
	do 
	{

		int* intPtr = nullptr;
		intPtr = reinterpret_cast<int*>(&m_rectInfo);
		for (int i = 0; i < m_rectInfoKeyList.size(); i++, intPtr++)
			setRectInfoValue(m_rectInfoKeyList[i], N_TO_Q_STR(*intPtr));

		QJsonObject rootObj;
		rootObj.insert("设备信息", m_deviceInfoObj);
		rootObj.insert("矩形信息", m_rectInfoObj);
		rootObj.insert("通道信息", m_channelInfoObj);

		QString* strPtr = reinterpret_cast<QString*>(&m_canMsgInfo);
		for (int i = 0; i < m_canMsgKeyList.size(); i++, strPtr++)
			setCanMsgInfoValue(m_canMsgKeyList[i], *strPtr);
		rootObj.insert("报文信息", m_canMsgInfoObj);

		rootObj.insert("测试信息", m_testInfoObj);
		rootObj.insert("基本信息", m_baseInfoObj);
		rootObj.insert("指定行信息", m_specifyRowInfoObj);
		rootObj.insert("指定列信息", m_specifyColumnInfoObj);

		RUN_BREAK(!utility::file::writeJson(fileName, rootObj), utility::getLastError());
		result = true;
	} while (false);
	return result;
}

bool Json::readShareFile(const QString& fileName)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!utility::file::repairJson1LevelNode(fileName,
			{
				"账户信息",
				"颜色信息"
			},
			{
				m_accountInfoKeyList,
				m_colorInfoKeyList
			},
			{
				m_accountInfoValueList,
				m_colorInfoValueList
			}), utility::getLastError());

		QJsonObject rootObj;

		RUN_BREAK(!utility::file::readJson(fileName, rootObj), utility::getLastError());

		RUN_BREAK(!rootObj.contains("账户信息"), "丢失账户信息对象");
		m_accountInfoObj = rootObj.value("账户信息").toObject();
		if (!parseAccountInfo())
		{
			break;
		}

		RUN_BREAK(!rootObj.contains("颜色信息"), "丢失颜色信息对象");
		m_colorInfoObj = rootObj.value("颜色信息").toObject();
		if (!parseColorInfo())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Json::writeShareFile(const QString& fileName)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj, accountInfoObj, colorInfoObj;
		for (int i = 0; i < m_accountInfoKeyList.size(); i++)
			accountInfoObj.insert(m_accountInfoKeyList[i], m_accountInfoValueList[i]);
		rootObj.insert("账户信息", accountInfoObj);

		for (int i = 0; i < m_colorInfoKeyList.size(); i++)
			colorInfoObj.insert(m_colorInfoKeyList[i], m_colorInfoValueList[i]);
		rootObj.insert("颜色信息", colorInfoObj);

		RUN_BREAK(!utility::file::writeJson(fileName, rootObj), utility::getLastError());
		result = true;
	} while (false);
	return result;
}

bool Json::updateShareFile(const QString& fileName)
{
	bool result = false;
	do
	{
		QJsonObject rootObj;
		rootObj.insert("账户信息", m_accountInfoObj);

		QString* valuePtr = reinterpret_cast<QString*>(&m_colorInfo);
		for (int i = 0; i < m_colorInfoKeyList.size(); i++, valuePtr++)
		{
			setColorInfoValue(m_colorInfoKeyList[i],*valuePtr);
		}
		rootObj.insert("颜色信息", m_colorInfoObj);

		RUN_BREAK(!utility::file::writeJson(fileName, rootObj), utility::getLastError());
		result = true;
	} while (false);
	return result;
}

bool Json::getResolution(QSize& size)
{
	bool result = false;
	do
	{
		const QStringList split = m_deviceInfo.imageResolution.split("*");
		RUN_BREAK(split.size() != 2, "设备分辨率解析错误");
		const int width = split[0].toInt();
		const int height = split[1].toInt();
		size.setWidth(width);
		size.setHeight(height);
		result = true;
	} while (false);
	return result;
}

void Json::resetChannelInfo()
{
	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		memset(m_channelInfo[i].determineCount, 0, sizeof(int) * MAX_RECT_COUNT);
		memset(m_channelInfo[i].directionResult, 1, sizeof(int) * MAX_RECT_COUNT);
		m_channelInfo[i].dynamicMat->release();
		m_channelInfo[i].dynamicSum = 0;
		m_channelInfo[i].dynamicPercent = 0;
		m_channelInfo[i].dynamicTime = 0;
		m_channelInfo[i].dynamicCount = 0;
		m_channelInfo[i].finalResult = true;
		m_channelInfo[i].flickerSwitch = true;
		m_channelInfo[i].flickerSwap = false;
		m_channelInfo[i].flickerTime = 0;
	}
}

void Json::setLastError(const QString& error)
{
#ifdef QT_DEBUG
	qDebug() << error;
#endif
	m_lastError = error;
}

bool Json::parseDeviceInfo()
{
	bool result = false, success = true;
	do 
	{
		RUN_BREAK(m_deviceInfoKeyList.size() != sizeof(DeviceInfo) / sizeof(QString) ,
			"设备信息结构体与键列表大小不一致");

		RUN_BREAK(m_deviceInfoKeyList.size() != m_deviceInfoValueList.size(), 
			"设备信息键列表与值列表大小不一致");

		QString* valuePtr = reinterpret_cast<QString*>(&m_deviceInfo);
		for (int i = 0; i < m_deviceInfoKeyList.size(); i++, valuePtr++)
		{
			*valuePtr = getDeviceInfoValue(m_deviceInfoKeyList[i]);
			if ((*valuePtr).isEmpty())
			{
				success = false;
				setLastError(QString("设备配置,键[%1]值为空,可能为非法键").arg(m_deviceInfoKeyList[i]));
				break;
			}
		}

		if (!success)
			break;

		result = true;
	} while (false);
	return result;
}

bool Json::parseRectInfo()
{
	bool result = false, success = true, convert = false;
	do 
	{
		RUN_BREAK(m_rectInfoKeyList.size() != sizeof(RectInfo) / sizeof(int),
			"矩形信息结构体与键列表大小不一致");

		RUN_BREAK(m_rectInfoKeyList.size() != m_rectInfoValueList.size(),
			"矩形信息键列表与值列表大小不一致");

		int* valuePtr = reinterpret_cast<int*>(&m_rectInfo);
		for (int i = 0; i < m_rectInfoKeyList.size(); i++, valuePtr++)
		{
			*valuePtr = getRectInfoValue(m_rectInfoKeyList[i]).toInt(&convert);
			if (!convert)
			{
				setLastError(QString("[%1]不为整数").arg(m_rectInfoKeyList[i]));
				success = false;
				break;
			}
		}

		if (!success)
		{
			break;
		}

		result = true;
	} while (false);
	return result;
}

bool Json::parseChannelInfo()
{
	bool result = false;
	do 
	{
		for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
		{
			m_channelInfo[i].enable = getChannelInfoValue(QString::number(i));
		}
		ChannelInfo::copyArray(m_channelInfoBackup, m_channelInfo);
		result = true;
	} while (false);
	return result;
}

bool Json::parseCanMsgInfo()
{
	bool result = false, success = true;
	do
	{
		RUN_BREAK(m_canMsgKeyList.size() != sizeof(CanMsgInfo) / sizeof(QString), 
			"CAN信息结构体与键列表大小不一致");

		RUN_BREAK(m_canMsgKeyList.size() != m_canMsgValueList.size(), 
			"CAN信息键列表与值列表大小不一致");

		QString* valuePtr = reinterpret_cast<QString*>(&m_canMsgInfo);
		for (int i = 0; i < m_canMsgKeyList.size(); i++, valuePtr++)
		{
			*valuePtr = getCanMsgInfoValue(m_canMsgKeyList[i]);
			if (valuePtr->isEmpty())
			{
				success = false;
				setLastError(QString("解析[%1]失败").arg(m_canMsgKeyList[i]));
				break;
			}
		}

		if (!success)
			break;

		result = true;
	} while (false);
	return result;
}

bool Json::parseTestInfo()
{
	bool result = false, success = true;
	do
	{
		RUN_BREAK(m_testInfoKeyList.size() != sizeof(TestInfo) / sizeof(QString), 
			"测试信息结构体与键列表大小不一致");

		RUN_BREAK(m_testInfoKeyList.size() != m_testInfoValueList.size(),
			"测试信息键列表与值列表大小不一致");

		QString* valuePtr = reinterpret_cast<QString*>(&m_testInfo);
		for (int i = 0; i < m_testInfoKeyList.size(); i++, valuePtr++)
		{
			*valuePtr = getTestInfoValue(m_testInfoKeyList[i]);
			if (valuePtr->isEmpty())
			{
				success = false;
				setLastError(QString("解析[%1]失败").arg(m_testInfoKeyList[i]));
				break;
			}
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Json::parseBaseInfo()
{
	bool result = false, success = true;
	do
	{
		RUN_BREAK(m_baseInfoKeyList.size() != sizeof(BaseInfo) / sizeof(QString),
			"基本信息结构体与键列表大小不一致");

		RUN_BREAK(m_baseInfoKeyList.size() != m_baseInfoValueList.size(),
			"基本信息键列表与值列表大小不一致");

		QString* valuePtr = reinterpret_cast<QString*>(&m_baseInfo);
		for (int i = 0; i < m_baseInfoKeyList.size(); i++, valuePtr++)
		{
			*valuePtr = getBaseInfoValue(m_baseInfoKeyList[i]);
			if (valuePtr->isEmpty())
			{
				success = false;
				setLastError(QString("解析[%1]失败").arg(m_baseInfoKeyList[i]));
				break;
			}
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Json::parseSpecifyRowInfo()
{
	bool result = false;
	do 
	{
		for (int i = 0; i < MAX_ROW_COUNT; i++)
		{
			m_specifyRowInfo[i].x = getSpecifyRowInfoValue(QString::number(i), "X");
			m_specifyRowInfo[i].y = getSpecifyRowInfoValue(QString::number(i), "Y");
		}
		result = true;
	} while (false);
	return result;
}

bool Json::parseSpecifyColumnInfo()
{
	bool result = false;
	do 
	{
		for (int i = 0; i < MAX_COLUMN_COUNT; i++)
		{
			m_specifyColumnInfo[i].x = getSpecifyColumnInfoValue(QString::number(i), "X");
			m_specifyColumnInfo[i].y = getSpecifyColumnInfoValue(QString::number(i), "Y");
		}
		result = true;
	} while (false);
	return result;
}

bool Json::parseAccountInfo()
{
	bool result = false;
	do 
	{
		m_accountInfo.userName = getAccountInfoValue("用户名称");
		m_accountInfo.userPassword = getAccountInfoValue("用户密码");

		RUN_BREAK(m_accountInfo.userName.isEmpty() ||
			m_accountInfo.userPassword.isEmpty(), "解析账户信息失败");

		result = true;
	} while (false);
	return result;
}

bool Json::parseColorInfo(bool byCall)
{
	bool result = false, success = true;
	do 
	{
		RUN_BREAK(m_colorInfoKeyList.size() != sizeof(ColorInfo) / sizeof(QString),
			"颜色信息结构体与键列表大小不一致");

		RUN_BREAK(m_colorInfoKeyList.size() != m_colorInfoValueList.size(),
			"颜色信息键列表与值列表大小不一致");

		QString* valuePtr = reinterpret_cast<QString*>(&m_colorInfo);
		for (int i = 0; i < m_colorInfoKeyList.size(); i++, valuePtr++)
		{
			if (!byCall)
				*valuePtr = getColorInfoValue(m_colorInfoKeyList[i]);

			QStringList split = valuePtr->split(",", QString::SkipEmptyParts);
			if (split.size() != 3)
			{
				success = false;
				setLastError(QString("解析[%1]失败").arg(m_colorInfoKeyList[i]));
				break;
			}

			RgbInfo rgbInfo = { 0 };
			rgbInfo.r = split[0].toInt();
			rgbInfo.g = split[1].toInt();
			rgbInfo.b = split[2].toInt();
			if (m_rgbInfoMap.contains(m_colorInfoKeyList[i]))
			{
				m_rgbInfoMap[m_colorInfoKeyList[i]] = rgbInfo;
			}
			else
			{
				m_rgbInfoMap.insert(m_colorInfoKeyList[i], rgbInfo);
			}
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

Json::Json(QObject *parent)
	: QObject(parent)
{
}

Json::~Json()
{
}
