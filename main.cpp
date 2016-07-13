#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>

#include "myview.h"

int main(int argc, char* argv[]) {

    QApplication  app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //QTime提供了闹钟功能
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//secsTo()为返回当前的秒数
    QPixmap pix(":/images/logo.png");
    QSplashScreen splash(pix);
    splash.resize(pix.size());
    splash.show();
    app.processEvents();//调用该函数的目的是为了使程序在启动画面的同时仍然能够响应鼠标事件

    MyView view;//主函数是直接调用的视图类
    view.show();

    splash.finish(&view);//当窗口view完成初始化工作后就结束启动画面

    return app.exec();
}
