#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>

#include <Utility/Utility.h>
#pragma comment(lib,"Utility.lib")

#include <libcan/libcan.h>
#ifdef QT_DEBUG
#pragma comment(lib, "libcand.lib")
#else
#pragma comment(lib, "libcan.lib")
#endif

#include <ItechPower/ItechPower.h>
#pragma comment(lib, "ItechPower.lib")

#include <CaptureCard/CaptureCard.h>
#pragma comment(lib, "CaptureCard.lib")

//最大通道数
#define MAX_CHANNEL_COUNT 200

//最大矩形数
#define MAX_RECT_COUNT 4

//最大行数
#define MAX_ROW_COUNT 10

//最大列数
#define MAX_COLUMN_COUNT 10

//定义设备是否使用
#define NOT_USE "不使用"

//通道排列
enum ChannelAlign
{
	//列向右
	CA_COLUMN_RIGHT,

	//行向下
	CA_ROW_DOWN,

	//正方形向右
	CA_SQUARE_RIGHT,
};

/*
* @DeviceInfo,基本信息
* @notice,此结构体大小,顺序必须与键列表一致,否则将会导致错误
*/
struct DeviceInfo
{
	//图像分辨率
	QString imageResolution;

	//采集卡名称
	QString captureName;

	//画面个数
	QString frameCount;

	//电源类型
	QString powerType;

	//电源电压
	QString powerVoltage;

	//电源电流
	QString powerCurrent;

	//电源端口
	QString powerPort;

	//电源波特率
	QString powerBaudrate;

	//CAN卡名称
	QString canName;

	//CAN卡波特率
	QString canBaudrate;

	//CAN卡拓展帧
	QString canExpandFrame;
};

/*
* @RectInfo,矩形信息
* @notice,此结构体大小,顺序必须与键列表一致,否则将会导致错误
*/
struct RectInfo
{
	//行
	int rows;

	//列
	int columns;

	//通道排列
	int channelAlign;

	//整体X轴偏移
	int overallXOffset;

	//整体Y轴偏移
	int overallYOffset;

	//整体前后间隔
	int overallFRInterval;

	//整体左右间隔
	int overallLRInterval;

	//前X坐标
	int frontX;

	//前Y坐标
	int frontY;

	//前宽
	int frontWidth;

	//前高
	int frontHeight;

	//后X坐标
	int rearX;

	//后Y坐标
	int rearY;

	//后宽
	int rearWidth;

	//后高
	int rearHeight;

	//左X坐标
	int leftX;

	//左Y坐标
	int leftY;

	//左宽
	int leftWidth;

	//左高
	int leftHeight;

	//右X坐标
	int rightX;

	//右Y坐标
	int rightY;

	//右宽
	int rightWidth;

	//右高
	int rightHeight;

	//内部前间隔
	int internalFrontInterval;

	//内部后间隔
	int internalRearInterval;

	//内部左间隔
	int internalLeftInterval;

	//内部右间隔
	int internalRightInterval;

	//显示通道编号
	int showChannelId;

	//显示文本
	int showText;

	//显示矩形
	int showRect;

	//显示禁用
	int showDisable;

	//显示二值化
	int showBinaryThresh;
};

//通道信息
struct ChannelInfo
{
	//是否启用
	int enable = false;

	//判定次数
	int determineCount[MAX_RECT_COUNT] = { 0 };

	//动态图像
	cv::Mat* dynamicMat = nullptr;

	//动态总和
	//如果设备卡死,图像还在继续显示,残留着上次的缓存,
	//需要这个参数的总和来判断图像是否丢失.
	uint dynamicSum = 0;

	//动态占比
	double dynamicPercent = 0;

	//动态时间
	quint64 dynamicTime = 0;

	//动态次数
	int dynamicCount = 0;

	//方向结果[前后左右]
	int directionResult[MAX_RECT_COUNT] = { 0 };

	//最终结果
	int finalResult = 0;

	//闪烁开关
	bool flickerSwitch = false;

	//闪烁切换
	bool flickerSwap = false;

	//闪烁时间
	quint64 flickerTime = 0;

	ChannelInfo()
	{
		dynamicMat = new(std::nothrow) cv::Mat;
	}

	~ChannelInfo()
	{
		if (dynamicMat)
			delete dynamicMat;
	}

	ChannelInfo& operator=(const ChannelInfo& other)
	{
		if (this != &other)
		{
			other.dynamicMat->copyTo(*this->dynamicMat);
			memcpy(determineCount, other.determineCount, sizeof(determineCount));
			memcpy(directionResult, other.directionResult, sizeof(directionResult));
			dynamicCount = other.dynamicCount;
			dynamicPercent = other.dynamicPercent;
			dynamicTime = other.dynamicTime;
			dynamicSum = other.dynamicSum;
			enable = other.enable;
			finalResult = other.finalResult;
			flickerTime = other.flickerTime;
			flickerSwap = other.flickerSwap;
			flickerSwitch = other.flickerSwitch;
		}
		return *this;
	}

	static void copyArray(ChannelInfo* a, const ChannelInfo* b, int size = MAX_CHANNEL_COUNT)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i] = b[i];
		}
		return;
	}
};

//CAN报文信息
struct CanMsgInfo
{
	//ID
	QString id;

	//数据
	QString data;

	//周期
	QString period;

	//次数
	QString count;

	//启用
	QString enable;
};

//测试信息
struct TestInfo
{
	//图像算法
	QString imageAlgorithm;

	//检测时间
	QString detectionTime;

	//上电等待
	QString powerOnWait;

	//黑色占比
	QString blackPercent;

	//白色占比
	QString whitePercent;

	//二值化值
	QString binaryThresh;

	//前颜色
	QString frontColor;

	//后颜色
	QString rearColor;

	//左颜色
	QString leftColor;

	//右颜色
	QString rightColor;

	//判定次数
	QString determineCount;

	//动态占比
	QString dynamicPercent;

	//动态阈值
	QString dynamicThresh;

	//动态频率
	QString dynamicFrequency;

	//动态检测
	QString dynamicDetection;

	//跳过检测
	QString skipDetection;
};

//基本信息
struct BaseInfo
{
	//条码判断
	QString codeJudge;

	//条码长度
	QString codeLength;

	//启用判断
	QString enableJudge;

	//启用上传
	QString enableUpload;
};

//指定行信息
struct SpecifyRowInfo
{
	int x;
	int y;
};

//指定列信息
struct SpecifyColumnInfo
{
	int x;
	int y;
};

//账户信息
struct AccountInfo
{
	//用户名称
	QString userName;

	//用户密码
	QString userPassword;
};

//颜色信息
struct ColorInfo
{
	//前矩形框
	QString frontRect;

	//后矩形框
	QString rearRect;

	//左矩形框
	QString leftRect;

	//右矩形框
	QString rightRect;

	//通道字体
	QString channelFont;

	//文本字体
	QString textFont;

	//禁用矩形框
	QString disableRect;

	//禁用线条
	QString disableLine;
};

//三原色信息
struct RgbInfo
{
	int r;
	int g;
	int b;
};

class Json : public QObject
{
	Q_OBJECT
public:
	/*
	* @brief,赋值构造删除
	*/
	Json& operator=(const Json&) = delete;

	/*
	* @brief,拷贝构造删除
	*/
	Json(const Json&) = delete;

	/*
	* @brief,获取实例
	* @return,Json*
	*/
	static Json* getInstance();

	/*
	* @brief,删除实例
	* @return,void
	*/
	static void deleteInstance();

	/*
	* @brief,获取最终错误
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @brief,初始化
	* @param1,机种名
	* @param2,是否更新
	* @return,bool
	*/
	bool initialize(const QString& moduleName, bool update = false);

	/*
	* @brief,获取索引
	* @param1,行
	* @param2,列
	* @return,int
	*/
	int getIndex(int row, int column) const;

	/*
	* @brief,获取设备信息
	* @return,DeviceInfo*
	*/
	DeviceInfo* getDeviceInfo() const;

	/*
	* @brief,获取设备信息值
	* @param1,键
	* @return,QString
	*/
	QString getDeviceInfoValue(const QString& key) const;

	/*
	* @brief,设置设备信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setDeviceInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取矩形信息
	* @return,RectInfo*
	*/
	RectInfo* getRectInfo() const;

	/*
	* @brief,获取矩形信息值
	* @param1,键
	* @return,QString
	*/
	QString getRectInfoValue(const QString& key) const;

	/*
	* @brief,设置矩形信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setRectInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取通道信息
	* @return,ChannelInfo*
	*/
	ChannelInfo* getChannelInfo() const;

	/*
	* @brief,获取通道信息值
	* @param1,键
	* @return,bool
	*/
	int getChannelInfoValue(const QString& key) const;

	/*
	* @brief,设置通道信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setChannelInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取CAN报文信息
	* @return,CanMsgInfo*
	*/
	CanMsgInfo* getCanMsgInfo() const;

	/*
	* @brief,获取CAN报文信息值
	* @param1,键
	* @return,QString
	*/
	QString getCanMsgInfoValue(const QString& key) const;

	/*
	* @brief,设置CAN报文信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setCanMsgInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取测试信息
	* @return,TestInfo*
	*/
	TestInfo* getTestInfo() const;

	/*
	* @brief,获取测试信息值
	* @param1,键
	* @return,QString
	*/
	QString getTestInfoValue(const QString& key) const;

	/*
	* @brief,设置测试信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setTestInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取基本信息
	* @return,BaseInfo*
	*/
	BaseInfo* getBaseInfo() const;

	/*
	* @brief,获取基本信息值
	* @param1,键
	* @return,QString
	*/
	QString getBaseInfoValue(const QString& key) const;

	/*
	* @brief,设置基本信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setBaseInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取电源通道
	* @return,void
	*/
	int getPowerChannel() const;

	/*
	* @brief,获取通道总数
	* @return,void
	*/
	int getChannelTotal() const;

	/*
	* @brief,获取已启用的通道总数
	* @return,int
	*/
	int getEnabledChannelTotal() const;

	/*
	* @brief,获取通道结果
	* @return,bool
	*/
	bool getChannelResult() const;

	/*
	* @brief,获取指定行信息
	* @return,SpecifyRowInfo*
	*/
	SpecifyRowInfo* getSpecifyRowInfo() const;

	/*
	* @brief,获取指定行信息值
	* @param1,父键
	* @param2,子键
	* @return,int
	*/
	int getSpecifyRowInfoValue(const QString& parentKey, const QString& childKey) const;

	/*
	* @brief,设置指定行信息值
	* @param1,父键
	* @param2,子键
	* @param3,值
	* @return,bool
	*/
	bool setSpecifyRowInfoValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*
	* @brief,获取指定列信息
	* @return,SpecifyColumnInfo*
	*/
	SpecifyColumnInfo* getSpecifyColumnInfo() const;

	/*
	* @brief,获取指定列信息值
	* @param1,父键
	* @param2,子键
	* @return,int
	*/
	int getSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey) const;

	/*
	* @brief,获取指定列信息值
	* @param1,父键
	* @param2,子键
	* @return,int
	*/
	bool setSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*
	* @brief,获取账户信息
	* @return,AccountInfo*
	*/
	AccountInfo* getAccountInfo() const;

	/*
	* @brief,获取账户信息值
	* @return,QString
	*/
	QString getAccountInfoValue(const QString& key) const;

	/*
	* @brief,设置账户信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setAccountInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取颜色信息
	* @return,ColorInfo*
	*/
	ColorInfo* getColorInfo() const;

	/*
	* @brief,获取颜色信息值
	* @param1,键
	* @return,QString
	*/
	QString getColorInfoValue(const QString& key) const;

	/*
	* @brief,设置颜色信息值
	* @param1,键
	* @param2,值
	* @return,bool
	*/
	bool setColorInfoValue(const QString& key, const QString& value);

	/*
	* @brief,获取颜色信息键列表
	* @return,const QStringList&
	*/
	const QStringList& getColorInfoKeyList() const;

	/*
	* @brief,获取颜色信息值列表
	* @return,const QStringList&
	*/
	const QStringList& getColorInfoValueList() const;

	/*
	* @brief,获取三原色信息值
	* @param1,颜色信息键
	* @return,const RgbInfo&
	*/
	RgbInfo getRgbInfoValue(const QString& key) const;

	/*
	* @brief,调用解析颜色信息
	* @return,bool
	*/
	bool callParseColorInfo();
public:
	/*
	* @brief,读取机种文件
	* @param1,文件名
	* @return,bool
	*/
	bool readTypeFile(const QString& fileName);

	/*
	* @brief,写入机种文件
	* @param1,文件名
	* @return,bool
	*/
	bool writeTypeFile(const QString& fileName);

	/*
	* @brief,更新机种文件
	* @param1,文件名
	* @return,bool
	*/
	bool updateTypeFile(const QString& fileName);

	/*
	* @brief,读取颜色文件
	* @param1,文件名
	* @return,bool
	*/
	bool readShareFile(const QString& fileName);

	/*
	* @brief,写入颜色文件
	* @param1,文件名
	* @return,bool
	*/
	bool writeShareFile(const QString& fileName);

	/*
	* @brief,更新颜色文件
	* @param1,文件名
	* @return,bool
	*/
	bool updateShareFile(const QString& fileName);

	/*
	* @brief,获取分辨率
	* @param1,大小
	* @return,bool
	*/
	bool getResolution(QSize& size);

	/*
	* @brief,重置通道信息
	* @return,void
	*/
	void resetChannelInfo();
protected:
	/*
	* @brief,设置最终错误
	* @param1,最终错误
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @brief,解析设备信息
	* @return,bool
	*/
	bool parseDeviceInfo();

	/*
	* @brief,解析矩形信息
	* @return,bool
	*/
	bool parseRectInfo();

	/*
	* @brief,解析通道信息
	* @return,bool
	*/
	bool parseChannelInfo();

	/*
	* @brief,解析CAN报文信息
	* @return,bool
	*/
	bool parseCanMsgInfo();

	/*
	* @brief,解析测试信息
	* @return,bool
	*/
	bool parseTestInfo();

	/*
	* @brief,解析基本信息
	* @return,bool
	*/
	bool parseBaseInfo();

	/*
	* @brief,解析指定行信息
	* @return,bool
	*/
	bool parseSpecifyRowInfo();

	/*
	* @brief,解析指定列信息
	* @return,bool
	*/
	bool parseSpecifyColumnInfo();

	/*
	* @brief,解析账户信息
	* @return,bool
	*/
	bool parseAccountInfo();

	/*
	* @brief,解析颜色信息
	* @param1,是否为被调用
	* @return,bool
	*/
	bool parseColorInfo(bool byCall = false);
private:
	/*
	* @brief,构造
	*/
	Json(QObject* parent = nullptr);

	/*
	* @brief,析构
	*/
	~Json();

	/*
	* @brief,单例指针,指向自身
	*/
	static Json* m_self;

	//最终错误
	QString m_lastError = "未知错误";

	//设备信息键列表
	QStringList m_deviceInfoKeyList =
	{
		"图像分辨率",//1
		"采集卡名称",//3
		"画面个数",//4
		"电源类型",//5
		"电源电压",//6
		"电源电流",//7
		"电源串口",//8
		"电源波特率",//9
		"CAN名称",//10
		"CAN波特率",//11
		"CAN拓展帧"//12
	};

	//设备信息值列表
	QStringList m_deviceInfoValueList =
	{
		"640*480",//1
		"TCHD",//3
		"1",//4
		"不使用",//5
		"5V",//6
		"1A",//7
		"COM1",//8
		"9600",//9
		"ZLG",//10
		"500",//1
		"0"//12
	};

	//矩形信息键列表
	QStringList m_rectInfoKeyList =
	{
		"行数",//1
		"列数",//2
		"通道排列",
		"整体X轴偏移",//5
		"整体Y轴偏移",//6
		"整体前后间隔",//7
		"整体左右间隔",//8	

		"前矩形X",
		"前矩形Y",
		"前矩形宽",//3
		"前矩形高",//4

		"后矩形X",
		"后矩形Y",
		"后矩形宽",
		"后矩形高",

		"左矩形X",
		"左矩形Y",
		"左矩形宽",
		"左矩形高",

		"右矩形X",
		"右矩形Y",
		"右矩形宽",
		"右矩形高",

		"内部前间隔",//9
		"内部后间隔",//10
		"内部左间隔",//11
		"内部右间隔",//12

		"显示通道号",
		"显示文本",//13
		"显示矩形框",//14
		"显示禁用框",
		"显示二值化"
	};

	//矩形信息值列表
	QStringList m_rectInfoValueList =
	{
		"4",//1
		"4",//2
		"0",

		//整体间隔
		"0",//5
		"0",//6
		"0",//7
		"0",//8

		//前
		"0",
		"0",
		"10",//3
		"15",//4

		//后
		"0",
		"0",
		"10",
		"15",

		//左
		"0",
		"0",
		"10",
		"15",

		//右
		"0",
		"0",
		"10",
		"15",

		//内部
		"0",//9
		"0",//10
		"0",//11
		"0",//12

		//启用
		"1",
		"0",//13
		"1",//14
		"1",
		"0"
	};

	//CAN报文键列表
	QStringList m_canMsgKeyList =
	{
		"ID",
		"数据",
		"间隔",
		"次数",
		"启用"
	};

	//CAN报文值列表
	QStringList m_canMsgValueList =
	{
		"0x2ab",
		"00 00 00 00 30 00 00 00",
		"50",
		"0",
		"0"
	};

	//测试信息键列表
	QStringList m_testInfoKeyList =
	{
		"图像算法",//1
		"检测时间",//2
		"上电等待",//3
		"黑色占比",//4
		"白色占比",//5
		"二值化值",//6
		"前景颜色",//7
		"后景颜色",//8
		"左景颜色",//9
		"右景颜色",//10
		"判定次数",//11
		"动态占比",//12
		"动态阈值",//13
		"动态频率",//14
		"动态检测",//15
		"跳过检测"//16
	};

	//测试信息值列表
	QStringList m_testInfoValueList =
	{
		"黑白占比",//1
		"30",//2
		"15000",//3
		"20",//4
		"20",//5
		"110",//6
		"!=黑",//7
		"!=黑",//8
		"!=黑",//9
		"!=黑",//10
		"300",//11
		"0.1",//12
		"10.5",//13
		"500",//14
		"0",//15
		"0"//16
	};

	//基本信息键列表
	QStringList m_baseInfoKeyList =
	{
		"条码判断",
		"条码长度",
		"启用判断",
		"启用上传"
	};

	//基本信息值列表
	QStringList m_baseInfoValueList =
	{
		"NULL",
		"0",
		"1",
		"1"
	};

	//账户信息键列表
	QStringList m_accountInfoKeyList =
	{
		"用户名称",
		"用户密码"
	};

	//账户信息值列表
	QStringList m_accountInfoValueList =
	{
		"INVO",
		"1."
	};

	//颜色信息键列表
	QStringList m_colorInfoKeyList =
	{
		"前矩形框",//1
		"后矩形框",//2
		"左矩形框",//3
		"右矩形框",//4
		"通道字体",//5
		"文本字体",//6
		"禁用矩形框",//7
		"禁用线条"//8
	};

	//颜色信息值列表
	QStringList m_colorInfoValueList =
	{
		"255,0,0",
		"255,255,0",
		"155,255,0",
		"255,182,193",
		"255,0,0",
		"255,165,0",
		"128,138,135",
		"0,191,255"
	};

	//设备信息对象
	QJsonObject m_deviceInfoObj;

	//设备信息结构体
	DeviceInfo m_deviceInfo = { 0 };

	//矩形信息对象
	QJsonObject m_rectInfoObj;

	//矩形信息结构体
	RectInfo m_rectInfo = { 0 };

	//通道信息对象
	QJsonObject m_channelInfoObj;

	//通道信息结构体
	ChannelInfo m_channelInfo[MAX_CHANNEL_COUNT];

	//通道信息结构体备份
	ChannelInfo m_channelInfoBackup[MAX_CHANNEL_COUNT];

	//CAN报文信息对象
	QJsonObject m_canMsgInfoObj;

	//CAN报文信息结构体
	CanMsgInfo m_canMsgInfo;

	//测试信息对象
	QJsonObject m_testInfoObj;

	//测试信息结构体
	TestInfo m_testInfo;

	//基本信息对象
	QJsonObject m_baseInfoObj;

	//基本信息结构体
	BaseInfo m_baseInfo;

	//指定行信息对象
	QJsonObject m_specifyRowInfoObj;

	//指定行信息结构体
	SpecifyRowInfo m_specifyRowInfo[MAX_ROW_COUNT] = { 0 };

	//指定列信息对象
	QJsonObject m_specifyColumnInfoObj;

	//指定列信息结构体
	SpecifyColumnInfo m_specifyColumnInfo[MAX_COLUMN_COUNT] = { 0 };

	//账户信息对象
	QJsonObject m_accountInfoObj;

	//账户信息
	AccountInfo m_accountInfo;

	//颜色信息对象
	QJsonObject m_colorInfoObj;

	//颜色信息结构体
	ColorInfo m_colorInfo;

	//三原色信息MAP
	QMap<QString, RgbInfo> m_rgbInfoMap;

	//三原色信息默认值
	RgbInfo m_rgbInfoDefaultValue = { 255, 0, 0 };
};
