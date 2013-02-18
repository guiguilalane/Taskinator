#include <QApplication>
#include "UI/mainwindow.h"
#include <QDateTime>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

#if QT_VERSION < 0x050000
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    // Traduction des boutons des messages en français
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    MainWindow w;


    int width = w.frameGeometry().width();
    int height = w.frameGeometry().height();

    QDesktopWidget wid;
    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();

    w.setGeometry((screenWidth/2) - (width/2), (screenHeight/2) - (height/2), width, height);

    w.show();
    w.AfterCreatedWindow();
    return a.exec();
}
