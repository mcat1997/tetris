#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>

#include "myview.h"

int main(int argc, char* argv[]) {

    QApplication  app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //QTime�ṩ�����ӹ���
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//secsTo()Ϊ���ص�ǰ������
    QPixmap pix(":/images/logo.png");
    QSplashScreen splash(pix);
    splash.resize(pix.size());
    splash.show();
    app.processEvents();//���øú�����Ŀ����Ϊ��ʹ���������������ͬʱ��Ȼ�ܹ���Ӧ����¼�

    MyView view;//��������ֱ�ӵ��õ���ͼ��
    view.show();

    splash.finish(&view);//������view��ɳ�ʼ��������ͽ�����������

    return app.exec();
}
