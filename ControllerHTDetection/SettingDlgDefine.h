#pragma once
#pragma execution_character_set("utf-8")

/*
* 这个文件后期用来处理多个画面配置,为拓展使用
*/

/*
* @CONNECT_RECT_SIGNAL_SLOT,连接矩形信号槽
* @param1,索引[通道0或通道1]
*/
#define CONNECT_RECT_SIGNAL_SLOT(INDEX)\
\
connect(ui.rowCountCombo_##INDEX, &QComboBox::currentTextChanged, this, &SettingDlg::rowCountComboChangedSlot);\
\
connect(ui.columnCountCombo_##INDEX, &QComboBox::currentTextChanged, this, &SettingDlg::columnCountComboChangedSlot);\
\
connect(ui.channelAlignCombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&SettingDlg::channelAlignComboSlot);\
\
connect(ui.rectWidthHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setRectWidth);\
\
connect(ui.rectHeightHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setRectHeight);\
\
connect(ui.xOffsetHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setOverallXOffset);\
\
connect(ui.yOffsetHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setOverallYOffset);\
\
connect(ui.overallFRIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setOverallFRInterval);\
\
connect(ui.overallLRIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setOverallLRInterval);\
\
connect(ui.internalFrontIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setInternalFrontInterval);\
\
connect(ui.internalRearIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setInternalRearInterval);\
\
connect(ui.internalLeftIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setInternalLeftInterval);\
\
connect(ui.internalRightIntervalHSlider_##INDEX, &QSlider::valueChanged, this, &SettingDlg::setInternalRightInterval);\
\
connect(ui.showTextCheck_##INDEX, &QCheckBox::clicked, this, &SettingDlg::setShowText);\
\
connect(ui.showRectCheck_##INDEX, &QCheckBox::clicked, this, &SettingDlg::setShowRect);

/*
* @SET_RECT_DATA,设置通道数据
* @param1,索引[通道0或通道1]
*/
#define SET_RECT_DATA(INDEX)\
\
ui.rowCountCombo_##INDEX->setCurrentText(N_TO_Q_STR(m_rectInfo[INDEX].rows));\
\
ui.columnCountCombo_##INDEX->setCurrentText(N_TO_Q_STR(m_rectInfo[INDEX].columns));\
\
ui.channelAlignCombo->setCurrentIndex(m_rectInfo[INDEX].channelAlign);\
\
ui.rectWidthHSlider_##INDEX->setValue(m_rectInfo[INDEX].width);\
\
ui.rectHeightHSlider_##INDEX->setValue(m_rectInfo[INDEX].height);\
\
ui.xOffsetHSlider_##INDEX->setValue(m_rectInfo[INDEX].xOffset);\
\
ui.yOffsetHSlider_##INDEX->setValue(m_rectInfo[INDEX].yOffset);\
\
ui.overallFRIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].overallFRInterval);\
\
ui.overallLRIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].overallLRInterval);\
\
ui.internalFrontIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].internalFrontInterval);\
\
ui.internalRearIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].internalRearInterval);\
\
ui.internalLeftIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].internalLeftInterval);\
\
ui.internalRightIntervalHSlider_##INDEX->setValue(m_rectInfo[INDEX].internalRightInterval);\
\
ui.showTextCheck_##INDEX->setChecked(m_rectInfo[INDEX].showText);\
\
ui.showRectCheck_##INDEX->setChecked(m_rectInfo[INDEX].showRect);

/*
* @SET_CHANNEL_WIDGET,设置通道控件
* @param1,索引[通道0或通道1]
* @return,void
*/
#define SET_CHANNEL_WIDGET(INDEX)\
[&]()->void{\
QLayout* layout = ui.channelGroup->layout();\
if (layout)\
{\
	QLayoutItem* item = nullptr;\
	while(item = layout->takeAt(0))\
	{\
		if (item->widget())\
			delete item->widget();\
		delete item;\
	}\
}\
else\
{\
	layout = new QGridLayout;\
}\
\
for (int i = 0; i < m_rectInfo->rows; i++)\
{\
	for (int j = 0; j < m_rectInfo->columns; j++)\
	{\
		int index = (j + 1) + i * m_rectInfo->columns;\
qDebug() << index;\
		QCheckBox* check = new QCheckBox(N_TO_Q_STR(index), ui.channelGroup);\
		check->setObjectName(Q_SPRINTF("channelCheckBox_%d",index - 1));\
		connect(check,&QCheckBox::stateChanged,this,&SettingDlg::channelStateChangedSlot);\
		check->setChecked(m_channelInfo[index - 1].checked);\
		static_cast<QGridLayout*>(layout)->addWidget(check, i, j);\
	}\
}\
QCheckBox* all = new QCheckBox("全选" , ui.channelGroup);\
static_cast<QGridLayout*>(layout)->addWidget(all,m_rectInfo->rows,m_rectInfo->columns - 1);\
ui.channelGroup->setLayout(layout);\
}()

/*
* @USE_SAME_RECT,使用相同的矩形
* @notice,这个为以后用作拓展选项
*/
#define USE_SAME_RECT 1

/*
* @GET_RECT_OBJECT_INDEX,获取矩形对象索引
* @notice,控件对象名不可随意更改,否则将会崩溃
* @param1,控件数据类型
* @return,int
*/

#if USE_SAME_RECT
#define GET_RECT_OBJECT_INDEX(TYPE) 0
#else
#define GET_RECT_OBJECT_INDEX(TYPE)\
dynamic_cast<TYPE>(sender())->objectName().split("_")[1].toInt();
#endif
