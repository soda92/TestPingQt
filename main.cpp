#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <locale>

#include <boost/locale.hpp>
#include <boost/locale/info.hpp>
#include <QRegularExpression>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "TestPingQt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QProcess process;
#if defined (WIN32)
    QString name{"ping 192.168.104.72"};
#else
    QString name{"ping -c 4 192.168.104.72"};
#endif
    process.start(name);
    process.waitForFinished(-1); // will wait forever until finished

    QTextCodec* c = QTextCodec::codecForName("gbk");

    auto std_out = c->toUnicode(process.readAllStandardOutput());
//    auto std_err = c->toUnicode(process.readAllStandardError());
//    qDebug() << std_out << std_err;
    QRegularExpression re("[0-9]+%");
    QRegularExpressionMatch match = re.match(std_out);
    bool hasMatch = match.hasMatch();
    if(hasMatch){
        std::cout << match.captured(0).toStdString() << "\n";
    }

//    std::cout << "System Locale: " << boost::locale::util::get_system_locale() << "\n";
//    std::cout << std_out.toStdString() << "\n";

//    boost::locale::generator gen;
//     std::locale loc(gen(""));
//     std::locale::global(loc);
//     std::cout.imbue(loc);

//     std::cout << "LOCALE NAME: " << std::use_facet<boost::locale::info>(loc).name() << std::endl;
//     std::cout << "LOCALE LANG: " << std::use_facet<boost::locale::info>(loc).language() << std::endl;
//     std::cout << "LOCALE COUNTRY: " << std::use_facet<boost::locale::info>(loc).country()
//               << std::endl;
//     std::cout << "LOCALE ENCODING: " << std::use_facet<boost::locale::info>(loc).encoding()
//               << std::endl;
//     std::cout << "LOCALE UTF8: " << std::use_facet<boost::locale::info>(loc).utf8() << std::endl;
    return 0;
//    return a.exec();
}
