#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(TIMER_TIMEOUT);



    port    =8888;
    ui->textEdit_port->setText(QString::number(port));


    Get_IP_Local();

}

MainWindow::~MainWindow()
{
        closethread();

        delete      thread_mysql;

        delete      m_pTimer;
        delete      ui;

}
    void MainWindow::threadFinished()
    {


    }


//停止线程
void MainWindow::on_pushButton_stop_clicked()
{
     //   closethread();
}

//启动线程
 void MainWindow::starthread()
 {

     #if 1
         //开启数据库线程
         thread_mysql    =new Thread_MySQL();
         thread_mysql->moveToThread(&m_thread_sql);
         connect(&m_thread_sql,&QThread::started,thread_mysql,&Thread_MySQL::start);
         connect(&m_thread_sql,&QThread::finished,thread_mysql,&Thread_MySQL::deleteLater);
  //       connect(this,SIGNAL(signalsendtoMysql(QString)),thread_mysql,SLOT(dealmesfrommain(QString)),Qt::QueuedConnection);

     #endif

             m_thread_sql.start();
          //   m_thread_fileread.start();
          //   m_thread_client.start();
 }

//关闭线程
 void MainWindow::closethread()
 {


     if(thread_mysql)
     {
         qDebug() << "线程有效，关闭线程thread_mysql " ;
         thread_mysql->stop();
         m_thread_sql.quit();
         m_thread_sql.wait();

         thread_mysql= NULL;
     }



 }


 void MainWindow::on_pushButton_start_clicked()
{
        starthread();
        slotCreateServer();//start server

        ui->pushButton_start->setEnabled(false);

}


    void MainWindow::handleTimeout()
    {
//        QString msg = QString("%1 -> %2 threadid:[%3]")
//                .arg(__FILE__)
//                .arg(__FUNCTION__)
//                .arg((uintptr_t)QThread::currentThreadId());
//        qDebug() << msg;

        if(m_pTimer->isActive()){
            m_pTimer->start();
        }
    }




void MainWindow::on_pushButton_connect_clicked()
{
    QStringList s;
    s<<QString("%1").arg(PRO_MAIN_INFO);

   s<<ui->textEdit_port->toPlainText();//get port



}

void MainWindow::on_pushButton_send_clicked()
{
    QStringList s;
    s<<QString("%1").arg(PRO_MAIN_SEND);
    s<<ui->textEdit_send->toPlainText();//get send msg


}

void MainWindow::Get_IP_Local(void)//get ip local
{
    QString localHostName = QHostInfo::localHostName();

    QHostInfo hostInfo =QHostInfo::fromName(localHostName);
 //   qDebug()<<hostInfo.addresses().at(2).toString();

    //获得IP
//    QList<QHostAddress>listAddress =    hostInfo.addresses();
//    if(!listAddress.isEmpty())
//    {
           ui->label_IP->setText(hostInfo.addresses().at(5).toString());
//    }
}

void    MainWindow::slotCreateServer()
{
        server  =   new Server(this,port);
        connect(server,SIGNAL(updataServer(QString,int) ),this,SLOT(updataServer(QString,int) ));

}



void    MainWindow::updataServer(QString msg,int length)
{

    //server 收到信息
    ui->textBrowser_server->setText(msg);
    qDebug()<<"length === "<<length;

}

