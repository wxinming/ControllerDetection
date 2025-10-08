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

//���ͨ����
#define MAX_CHANNEL_COUNT 200

//��������
#define MAX_RECT_COUNT 4

//�������
#define MAX_ROW_COUNT 10

//�������
#define MAX_COLUMN_COUNT 10

//�����豸�Ƿ�ʹ��
#define NOT_USE "��ʹ��"

//ͨ������
enum ChannelAlign
{
	//������
	CA_COLUMN_RIGHT,

	//������
	CA_ROW_DOWN,

	//����������
	CA_SQUARE_RIGHT,
};

/*
* @DeviceInfo,������Ϣ
* @notice,�˽ṹ���С,˳���������б�һ��,���򽫻ᵼ�´���
*/
struct DeviceInfo
{
	//ͼ��ֱ���
	QString imageResolution;

	//�ɼ�������
	QString captureName;

	//�������
	QString frameCount;

	//��Դ����
	QString powerType;

	//��Դ��ѹ
	QString powerVoltage;

	//��Դ����
	QString powerCurrent;

	//��Դ�˿�
	QString powerPort;

	//��Դ������
	QString powerBaudrate;

	//CAN������
	QString canName;

	//CAN��������
	QString canBaudrate;

	//CAN����չ֡
	QString canExpandFrame;
};

/*
* @RectInfo,������Ϣ
* @notice,�˽ṹ���С,˳���������б�һ��,���򽫻ᵼ�´���
*/
struct RectInfo
{
	//��
	int rows;

	//��
	int columns;

	//ͨ������
	int channelAlign;

	//����X��ƫ��
	int overallXOffset;

	//����Y��ƫ��
	int overallYOffset;

	//����ǰ����
	int overallFRInterval;

	//�������Ҽ��
	int overallLRInterval;

	//ǰX����
	int frontX;

	//ǰY����
	int frontY;

	//ǰ��
	int frontWidth;

	//ǰ��
	int frontHeight;

	//��X����
	int rearX;

	//��Y����
	int rearY;

	//���
	int rearWidth;

	//���
	int rearHeight;

	//��X����
	int leftX;

	//��Y����
	int leftY;

	//���
	int leftWidth;

	//���
	int leftHeight;

	//��X����
	int rightX;

	//��Y����
	int rightY;

	//�ҿ�
	int rightWidth;

	//�Ҹ�
	int rightHeight;

	//�ڲ�ǰ���
	int internalFrontInterval;

	//�ڲ�����
	int internalRearInterval;

	//�ڲ�����
	int internalLeftInterval;

	//�ڲ��Ҽ��
	int internalRightInterval;

	//��ʾͨ�����
	int showChannelId;

	//��ʾ�ı�
	int showText;

	//��ʾ����
	int showRect;

	//��ʾ����
	int showDisable;

	//��ʾ��ֵ��
	int showBinaryThresh;
};

//ͨ����Ϣ
struct ChannelInfo
{
	//�Ƿ�����
	int enable = false;

	//�ж�����
	int determineCount[MAX_RECT_COUNT] = { 0 };

	//��̬ͼ��
	cv::Mat* dynamicMat = nullptr;

	//��̬�ܺ�
	//����豸����,ͼ���ڼ�����ʾ,�������ϴεĻ���,
	//��Ҫ����������ܺ����ж�ͼ���Ƿ�ʧ.
	uint dynamicSum = 0;

	//��̬ռ��
	double dynamicPercent = 0;

	//��̬ʱ��
	quint64 dynamicTime = 0;

	//��̬����
	int dynamicCount = 0;

	//������[ǰ������]
	int directionResult[MAX_RECT_COUNT] = { 0 };

	//���ս��
	int finalResult = 0;

	//��˸����
	bool flickerSwitch = false;

	//��˸�л�
	bool flickerSwap = false;

	//��˸ʱ��
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

//CAN������Ϣ
struct CanMsgInfo
{
	//ID
	QString id;

	//����
	QString data;

	//����
	QString period;

	//����
	QString count;

	//����
	QString enable;
};

//������Ϣ
struct TestInfo
{
	//ͼ���㷨
	QString imageAlgorithm;

	//���ʱ��
	QString detectionTime;

	//�ϵ�ȴ�
	QString powerOnWait;

	//��ɫռ��
	QString blackPercent;

	//��ɫռ��
	QString whitePercent;

	//��ֵ��ֵ
	QString binaryThresh;

	//ǰ��ɫ
	QString frontColor;

	//����ɫ
	QString rearColor;

	//����ɫ
	QString leftColor;

	//����ɫ
	QString rightColor;

	//�ж�����
	QString determineCount;

	//��̬ռ��
	QString dynamicPercent;

	//��̬��ֵ
	QString dynamicThresh;

	//��̬Ƶ��
	QString dynamicFrequency;

	//��̬���
	QString dynamicDetection;

	//�������
	QString skipDetection;
};

//������Ϣ
struct BaseInfo
{
	//�����ж�
	QString codeJudge;

	//���볤��
	QString codeLength;

	//�����ж�
	QString enableJudge;

	//�����ϴ�
	QString enableUpload;
};

//ָ������Ϣ
struct SpecifyRowInfo
{
	int x;
	int y;
};

//ָ������Ϣ
struct SpecifyColumnInfo
{
	int x;
	int y;
};

//�˻���Ϣ
struct AccountInfo
{
	//�û�����
	QString userName;

	//�û�����
	QString userPassword;
};

//��ɫ��Ϣ
struct ColorInfo
{
	//ǰ���ο�
	QString frontRect;

	//����ο�
	QString rearRect;

	//����ο�
	QString leftRect;

	//�Ҿ��ο�
	QString rightRect;

	//ͨ������
	QString channelFont;

	//�ı�����
	QString textFont;

	//���þ��ο�
	QString disableRect;

	//��������
	QString disableLine;
};

//��ԭɫ��Ϣ
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
	* @brief,��ֵ����ɾ��
	*/
	Json& operator=(const Json&) = delete;

	/*
	* @brief,��������ɾ��
	*/
	Json(const Json&) = delete;

	/*
	* @brief,��ȡʵ��
	* @return,Json*
	*/
	static Json* getInstance();

	/*
	* @brief,ɾ��ʵ��
	* @return,void
	*/
	static void deleteInstance();

	/*
	* @brief,��ȡ���մ���
	* @return,QString
	*/
	QString getLastError() const;

	/*
	* @brief,��ʼ��
	* @param1,������
	* @param2,�Ƿ����
	* @return,bool
	*/
	bool initialize(const QString& moduleName, bool update = false);

	/*
	* @brief,��ȡ����
	* @param1,��
	* @param2,��
	* @return,int
	*/
	int getIndex(int row, int column) const;

	/*
	* @brief,��ȡ�豸��Ϣ
	* @return,DeviceInfo*
	*/
	DeviceInfo* getDeviceInfo() const;

	/*
	* @brief,��ȡ�豸��Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getDeviceInfoValue(const QString& key) const;

	/*
	* @brief,�����豸��Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setDeviceInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ������Ϣ
	* @return,RectInfo*
	*/
	RectInfo* getRectInfo() const;

	/*
	* @brief,��ȡ������Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getRectInfoValue(const QString& key) const;

	/*
	* @brief,���þ�����Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setRectInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡͨ����Ϣ
	* @return,ChannelInfo*
	*/
	ChannelInfo* getChannelInfo() const;

	/*
	* @brief,��ȡͨ����Ϣֵ
	* @param1,��
	* @return,bool
	*/
	int getChannelInfoValue(const QString& key) const;

	/*
	* @brief,����ͨ����Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setChannelInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡCAN������Ϣ
	* @return,CanMsgInfo*
	*/
	CanMsgInfo* getCanMsgInfo() const;

	/*
	* @brief,��ȡCAN������Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getCanMsgInfoValue(const QString& key) const;

	/*
	* @brief,����CAN������Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setCanMsgInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ������Ϣ
	* @return,TestInfo*
	*/
	TestInfo* getTestInfo() const;

	/*
	* @brief,��ȡ������Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getTestInfoValue(const QString& key) const;

	/*
	* @brief,���ò�����Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setTestInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ������Ϣ
	* @return,BaseInfo*
	*/
	BaseInfo* getBaseInfo() const;

	/*
	* @brief,��ȡ������Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getBaseInfoValue(const QString& key) const;

	/*
	* @brief,���û�����Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setBaseInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ��Դͨ��
	* @return,void
	*/
	int getPowerChannel() const;

	/*
	* @brief,��ȡͨ������
	* @return,void
	*/
	int getChannelTotal() const;

	/*
	* @brief,��ȡ�����õ�ͨ������
	* @return,int
	*/
	int getEnabledChannelTotal() const;

	/*
	* @brief,��ȡͨ�����
	* @return,bool
	*/
	bool getChannelResult() const;

	/*
	* @brief,��ȡָ������Ϣ
	* @return,SpecifyRowInfo*
	*/
	SpecifyRowInfo* getSpecifyRowInfo() const;

	/*
	* @brief,��ȡָ������Ϣֵ
	* @param1,����
	* @param2,�Ӽ�
	* @return,int
	*/
	int getSpecifyRowInfoValue(const QString& parentKey, const QString& childKey) const;

	/*
	* @brief,����ָ������Ϣֵ
	* @param1,����
	* @param2,�Ӽ�
	* @param3,ֵ
	* @return,bool
	*/
	bool setSpecifyRowInfoValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*
	* @brief,��ȡָ������Ϣ
	* @return,SpecifyColumnInfo*
	*/
	SpecifyColumnInfo* getSpecifyColumnInfo() const;

	/*
	* @brief,��ȡָ������Ϣֵ
	* @param1,����
	* @param2,�Ӽ�
	* @return,int
	*/
	int getSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey) const;

	/*
	* @brief,��ȡָ������Ϣֵ
	* @param1,����
	* @param2,�Ӽ�
	* @return,int
	*/
	bool setSpecifyColumnInfoValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*
	* @brief,��ȡ�˻���Ϣ
	* @return,AccountInfo*
	*/
	AccountInfo* getAccountInfo() const;

	/*
	* @brief,��ȡ�˻���Ϣֵ
	* @return,QString
	*/
	QString getAccountInfoValue(const QString& key) const;

	/*
	* @brief,�����˻���Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setAccountInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ��ɫ��Ϣ
	* @return,ColorInfo*
	*/
	ColorInfo* getColorInfo() const;

	/*
	* @brief,��ȡ��ɫ��Ϣֵ
	* @param1,��
	* @return,QString
	*/
	QString getColorInfoValue(const QString& key) const;

	/*
	* @brief,������ɫ��Ϣֵ
	* @param1,��
	* @param2,ֵ
	* @return,bool
	*/
	bool setColorInfoValue(const QString& key, const QString& value);

	/*
	* @brief,��ȡ��ɫ��Ϣ���б�
	* @return,const QStringList&
	*/
	const QStringList& getColorInfoKeyList() const;

	/*
	* @brief,��ȡ��ɫ��Ϣֵ�б�
	* @return,const QStringList&
	*/
	const QStringList& getColorInfoValueList() const;

	/*
	* @brief,��ȡ��ԭɫ��Ϣֵ
	* @param1,��ɫ��Ϣ��
	* @return,const RgbInfo&
	*/
	RgbInfo getRgbInfoValue(const QString& key) const;

	/*
	* @brief,���ý�����ɫ��Ϣ
	* @return,bool
	*/
	bool callParseColorInfo();
public:
	/*
	* @brief,��ȡ�����ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool readTypeFile(const QString& fileName);

	/*
	* @brief,д������ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool writeTypeFile(const QString& fileName);

	/*
	* @brief,���»����ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool updateTypeFile(const QString& fileName);

	/*
	* @brief,��ȡ��ɫ�ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool readShareFile(const QString& fileName);

	/*
	* @brief,д����ɫ�ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool writeShareFile(const QString& fileName);

	/*
	* @brief,������ɫ�ļ�
	* @param1,�ļ���
	* @return,bool
	*/
	bool updateShareFile(const QString& fileName);

	/*
	* @brief,��ȡ�ֱ���
	* @param1,��С
	* @return,bool
	*/
	bool getResolution(QSize& size);

	/*
	* @brief,����ͨ����Ϣ
	* @return,void
	*/
	void resetChannelInfo();
protected:
	/*
	* @brief,�������մ���
	* @param1,���մ���
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @brief,�����豸��Ϣ
	* @return,bool
	*/
	bool parseDeviceInfo();

	/*
	* @brief,����������Ϣ
	* @return,bool
	*/
	bool parseRectInfo();

	/*
	* @brief,����ͨ����Ϣ
	* @return,bool
	*/
	bool parseChannelInfo();

	/*
	* @brief,����CAN������Ϣ
	* @return,bool
	*/
	bool parseCanMsgInfo();

	/*
	* @brief,����������Ϣ
	* @return,bool
	*/
	bool parseTestInfo();

	/*
	* @brief,����������Ϣ
	* @return,bool
	*/
	bool parseBaseInfo();

	/*
	* @brief,����ָ������Ϣ
	* @return,bool
	*/
	bool parseSpecifyRowInfo();

	/*
	* @brief,����ָ������Ϣ
	* @return,bool
	*/
	bool parseSpecifyColumnInfo();

	/*
	* @brief,�����˻���Ϣ
	* @return,bool
	*/
	bool parseAccountInfo();

	/*
	* @brief,������ɫ��Ϣ
	* @param1,�Ƿ�Ϊ������
	* @return,bool
	*/
	bool parseColorInfo(bool byCall = false);
private:
	/*
	* @brief,����
	*/
	Json(QObject* parent = nullptr);

	/*
	* @brief,����
	*/
	~Json();

	/*
	* @brief,����ָ��,ָ������
	*/
	static Json* m_self;

	//���մ���
	QString m_lastError = "δ֪����";

	//�豸��Ϣ���б�
	QStringList m_deviceInfoKeyList =
	{
		"ͼ��ֱ���",//1
		"�ɼ�������",//3
		"�������",//4
		"��Դ����",//5
		"��Դ��ѹ",//6
		"��Դ����",//7
		"��Դ����",//8
		"��Դ������",//9
		"CAN����",//10
		"CAN������",//11
		"CAN��չ֡"//12
	};

	//�豸��Ϣֵ�б�
	QStringList m_deviceInfoValueList =
	{
		"640*480",//1
		"TCHD",//3
		"1",//4
		"��ʹ��",//5
		"5V",//6
		"1A",//7
		"COM1",//8
		"9600",//9
		"ZLG",//10
		"500",//1
		"0"//12
	};

	//������Ϣ���б�
	QStringList m_rectInfoKeyList =
	{
		"����",//1
		"����",//2
		"ͨ������",
		"����X��ƫ��",//5
		"����Y��ƫ��",//6
		"����ǰ����",//7
		"�������Ҽ��",//8	

		"ǰ����X",
		"ǰ����Y",
		"ǰ���ο�",//3
		"ǰ���θ�",//4

		"�����X",
		"�����Y",
		"����ο�",
		"����θ�",

		"�����X",
		"�����Y",
		"����ο�",
		"����θ�",

		"�Ҿ���X",
		"�Ҿ���Y",
		"�Ҿ��ο�",
		"�Ҿ��θ�",

		"�ڲ�ǰ���",//9
		"�ڲ�����",//10
		"�ڲ�����",//11
		"�ڲ��Ҽ��",//12

		"��ʾͨ����",
		"��ʾ�ı�",//13
		"��ʾ���ο�",//14
		"��ʾ���ÿ�",
		"��ʾ��ֵ��"
	};

	//������Ϣֵ�б�
	QStringList m_rectInfoValueList =
	{
		"4",//1
		"4",//2
		"0",

		//������
		"0",//5
		"0",//6
		"0",//7
		"0",//8

		//ǰ
		"0",
		"0",
		"10",//3
		"15",//4

		//��
		"0",
		"0",
		"10",
		"15",

		//��
		"0",
		"0",
		"10",
		"15",

		//��
		"0",
		"0",
		"10",
		"15",

		//�ڲ�
		"0",//9
		"0",//10
		"0",//11
		"0",//12

		//����
		"1",
		"0",//13
		"1",//14
		"1",
		"0"
	};

	//CAN���ļ��б�
	QStringList m_canMsgKeyList =
	{
		"ID",
		"����",
		"���",
		"����",
		"����"
	};

	//CAN����ֵ�б�
	QStringList m_canMsgValueList =
	{
		"0x2ab",
		"00 00 00 00 30 00 00 00",
		"50",
		"0",
		"0"
	};

	//������Ϣ���б�
	QStringList m_testInfoKeyList =
	{
		"ͼ���㷨",//1
		"���ʱ��",//2
		"�ϵ�ȴ�",//3
		"��ɫռ��",//4
		"��ɫռ��",//5
		"��ֵ��ֵ",//6
		"ǰ����ɫ",//7
		"����ɫ",//8
		"����ɫ",//9
		"�Ҿ���ɫ",//10
		"�ж�����",//11
		"��̬ռ��",//12
		"��̬��ֵ",//13
		"��̬Ƶ��",//14
		"��̬���",//15
		"�������"//16
	};

	//������Ϣֵ�б�
	QStringList m_testInfoValueList =
	{
		"�ڰ�ռ��",//1
		"30",//2
		"15000",//3
		"20",//4
		"20",//5
		"110",//6
		"!=��",//7
		"!=��",//8
		"!=��",//9
		"!=��",//10
		"300",//11
		"0.1",//12
		"10.5",//13
		"500",//14
		"0",//15
		"0"//16
	};

	//������Ϣ���б�
	QStringList m_baseInfoKeyList =
	{
		"�����ж�",
		"���볤��",
		"�����ж�",
		"�����ϴ�"
	};

	//������Ϣֵ�б�
	QStringList m_baseInfoValueList =
	{
		"NULL",
		"0",
		"1",
		"1"
	};

	//�˻���Ϣ���б�
	QStringList m_accountInfoKeyList =
	{
		"�û�����",
		"�û�����"
	};

	//�˻���Ϣֵ�б�
	QStringList m_accountInfoValueList =
	{
		"INVO",
		"1."
	};

	//��ɫ��Ϣ���б�
	QStringList m_colorInfoKeyList =
	{
		"ǰ���ο�",//1
		"����ο�",//2
		"����ο�",//3
		"�Ҿ��ο�",//4
		"ͨ������",//5
		"�ı�����",//6
		"���þ��ο�",//7
		"��������"//8
	};

	//��ɫ��Ϣֵ�б�
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

	//�豸��Ϣ����
	QJsonObject m_deviceInfoObj;

	//�豸��Ϣ�ṹ��
	DeviceInfo m_deviceInfo = { 0 };

	//������Ϣ����
	QJsonObject m_rectInfoObj;

	//������Ϣ�ṹ��
	RectInfo m_rectInfo = { 0 };

	//ͨ����Ϣ����
	QJsonObject m_channelInfoObj;

	//ͨ����Ϣ�ṹ��
	ChannelInfo m_channelInfo[MAX_CHANNEL_COUNT];

	//ͨ����Ϣ�ṹ�屸��
	ChannelInfo m_channelInfoBackup[MAX_CHANNEL_COUNT];

	//CAN������Ϣ����
	QJsonObject m_canMsgInfoObj;

	//CAN������Ϣ�ṹ��
	CanMsgInfo m_canMsgInfo;

	//������Ϣ����
	QJsonObject m_testInfoObj;

	//������Ϣ�ṹ��
	TestInfo m_testInfo;

	//������Ϣ����
	QJsonObject m_baseInfoObj;

	//������Ϣ�ṹ��
	BaseInfo m_baseInfo;

	//ָ������Ϣ����
	QJsonObject m_specifyRowInfoObj;

	//ָ������Ϣ�ṹ��
	SpecifyRowInfo m_specifyRowInfo[MAX_ROW_COUNT] = { 0 };

	//ָ������Ϣ����
	QJsonObject m_specifyColumnInfoObj;

	//ָ������Ϣ�ṹ��
	SpecifyColumnInfo m_specifyColumnInfo[MAX_COLUMN_COUNT] = { 0 };

	//�˻���Ϣ����
	QJsonObject m_accountInfoObj;

	//�˻���Ϣ
	AccountInfo m_accountInfo;

	//��ɫ��Ϣ����
	QJsonObject m_colorInfoObj;

	//��ɫ��Ϣ�ṹ��
	ColorInfo m_colorInfo;

	//��ԭɫ��ϢMAP
	QMap<QString, RgbInfo> m_rgbInfoMap;

	//��ԭɫ��ϢĬ��ֵ
	RgbInfo m_rgbInfoDefaultValue = { 255, 0, 0 };
};
