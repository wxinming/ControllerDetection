#include "ScanCodeDlg.h"

extern bool g_threadWait;

/*条码列表*/
extern QStringList g_codeList;

/*重复条码列表*/
QStringList g_repeatCodeList;

bool ScanCodeDlg::sendCode(const QString& code)
{
	bool result = false;
	do
	{
		if (!m_baseInfo->enableUpload.toInt())
		{
			g_codeList.push_back(ui.codeLine->text());
			this->hide();
			g_threadWait = false;
			result = true;
			break;
		}

		auto byte = code.toLocal8Bit();
		if (imp_mes_plugin_send_message(m_plugin, byte.data(), byte.length()) < 0)
		{
			setLastError("MES客户端未开启");
			break;
		}
		result = true;
	} while (false);
	return result;
}

void ScanCodeDlg::setLastError(const QString& error)
{
	m_lastError = error;
}

void ScanCodeDlg::setChannel(int channel)
{
	m_channel = channel;
	ui.titleLabel->setText(QString("请扫通道%1条码").arg(m_channel));
}

QString ScanCodeDlg::getLastError() const
{
	return m_lastError;
}

bool ScanCodeDlg::judgeCode(const QString& code)
{
	bool result = false, success = true;
	do
	{
		int codeLength = m_baseInfo->codeLength.toInt();
		int enableJudge = m_baseInfo->enableJudge.toInt();
		QString codeJudge = m_baseInfo->codeJudge;

		if (!enableJudge || (!codeLength && codeJudge == "NULL"))
		{
			result = true;
			break;
		}

		RUN_BREAK(code.length() != codeLength || code.mid(0, codeJudge.length()) != codeJudge,
			Q_SPRINTF("通道%d条码格式错误", m_channel));

		g_repeatCodeList.push_back(code);

		for (int i = 0; i < g_repeatCodeList.count(); i++)
		{
			for (int j = i + 1; j < g_repeatCodeList.count(); j++)
			{
				if (g_repeatCodeList.value(i) == g_repeatCodeList.value(j))
				{
					success = false;
					g_repeatCodeList.pop_back();
					break;
				}
			}

			if (!success)
			{
				break;
			}
		}

		RUN_BREAK(!success, Q_SPRINTF("通道%d条码重复", m_channel));

		result = true;
	} while (false);
	return result;
}

bool ScanCodeDlg::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == &m_minimize)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent = reinterpret_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				this->showMinimized();
				return true;
			}
		}
	}

	/*通过事件过滤器,屏蔽ESC键,来连接对话框退出*/
	if (event->type() == QEvent::KeyPress)
	{
		if (reinterpret_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape)
		{
			//this->close();
			QMessageBox::information(this, "友情提示", "好好工作,不要调皮");
			return true;
		}
	}
	return QObject::eventFilter(obj, event);
}

ScanCodeDlg::ScanCodeDlg(QWidget* parent)
	: QDialog(parent)
{
	this->ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	installEventFilter(this);
	ui.titleLabel->setStyleSheet(ui.titleLabel->styleSheet() + ";color:black;");
	connect(ui.codeLine, &QLineEdit::returnPressed, this, &ScanCodeDlg::returnPressedSlot);
	connect(this, &ScanCodeDlg::changeTitleSignal, this, &ScanCodeDlg::changeTitleSlot);
	connect(this, &ScanCodeDlg::hideWindowSignal, this, &ScanCodeDlg::hideWindowSlot);

	m_minimize.setParent(this);
	m_minimize.setText("-");
	QFont font;
	font.setBold(true);
	font.setPixelSize(20);
	m_minimize.setFont(font);
	m_minimize.setAlignment(Qt::AlignCenter);
	m_minimize.setFixedSize(QSize(25, 25));
	m_minimize.setStyleSheet("color:rgb(0,0,0);background-color:transparent;");
	m_minimize.move(this->size().width() - m_minimize.size().width(), 0);
	m_minimize.installEventFilter(this);

	m_baseInfo = GET_JSON->getBaseInfo();

	m_plugin = imp_mes_plugin_new();
	imp_mes_plugin_initialize(m_plugin);
	imp_mes_plugin_set_connection_cb(m_plugin, [](bool connected, void* user) {
		auto dlg = static_cast<ScanCodeDlg*>(user);
		if (connected) {
			dlg->updateMesHintSignal("rgb(0,255,0)", "MES已连接");
		}
		else {
			dlg->updateMesHintSignal("rgb(255,0,0)", "MES未连接");
		}
	}, this);

	imp_mes_plugin_set_recv_message_cb(m_plugin, [](const imp_mes_plugin_message_t* msg, void* user) {
		auto dlg = static_cast<ScanCodeDlg*>(user);
		if (msg->command_code == imp_mes_plugin_command::IMP_MES_PLUGIN_COMMAND_QUERY_SN_RESPONSE) {
			if (msg->error_code == 0) {
				g_codeList.push_back(dlg->m_barcode);
				dlg->hideWindowSignal();
				g_threadWait = false;
			}
			else {
				dlg->changeTitleSignal(QString::fromLocal8Bit(msg->error_string));
				if (!g_repeatCodeList.isEmpty()) {
					g_repeatCodeList.pop_back();
				}
			}
		}
	}, this);

	QTimer::singleShot(1000, [this]() {imp_mes_plugin_start(m_plugin); });
}

ScanCodeDlg::~ScanCodeDlg()
{
	imp_mes_plugin_stop(m_plugin);
	imp_mes_plugin_set_recv_message_cb(m_plugin, nullptr, nullptr);
	imp_mes_plugin_set_connection_cb(m_plugin, nullptr, nullptr);
	imp_mes_plugin_free(m_plugin);
}

void ScanCodeDlg::returnPressedSlot()
{
	m_barcode = ui.codeLine->text();

	if (!judgeCode(m_barcode))
	{
		ui.titleLabel->setText(m_lastError);
		goto clear;
	}
	
	if (!sendCode(m_barcode))
	{
		if (!g_repeatCodeList.isEmpty())
		{
			g_repeatCodeList.pop_back();
		}
		ui.titleLabel->setText(m_lastError);
		goto clear;
	}
clear:
	ui.codeLine->clear();
}

void ScanCodeDlg::changeTitleSlot(const QString& text)
{
	ui.titleLabel->setText(text);
}

void ScanCodeDlg::hideWindowSlot()
{
	hide();
}
