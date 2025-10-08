#include "ChooseDlg.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	autoReleaseNewWindow<ChooseDlg>()->show();
	return a.exec();
}
