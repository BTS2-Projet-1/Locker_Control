#include "thread_linphonec.h"
#include <QDebug>

Thread_linphonec::Thread_linphonec(Assistance *assist, QThread *parent):QThread(parent)
{
    m_assistance = assist;
    m_linphonec = new QProcess();
    if (m_assistance != NULL)
    {
        connect(m_assistance,SIGNAL(startThreadLinphonec()),this,SLOT(startThread()));
        connect(m_assistance,SIGNAL(EndThreadLinphonec()),this,SLOT(EndThread()));
        m_loopevent = true;
    }
}

Thread_linphonec::~Thread_linphonec()
{
    wait(1000);
    delete m_linphonec;
}

void Thread_linphonec::run()
{
    QString readout;

    while(m_loopevent)
    {
        m_linphonec->waitForReadyRead(300);
        m_ByteArray = m_linphonec->readAll();
        readout.append(m_ByteArray);
        qDebug()<<readout;
        m_ByteArray.clear();
        readout.clear();
    }

}

void Thread_linphonec::startThread()
{
   m_loopevent = true;

    QStringList arg;
        arg << "-V";

    //Démarrage du programme Linphonec en mode vidéo
    m_linphonec->start("/usr/bin/./linphonec",arg);

    if (!m_linphonec->waitForStarted())
        qDebug()<<"help";

    if(m_linphonec->write("call 601\n")==-1)
     {
        qDebug()<<"error ecriture";
     }
     m_linphonec->waitForBytesWritten(1000);

    start();
}

void Thread_linphonec::EndThread()
{
   int result;
   m_loopevent = false;//quitte la lecture

   if(m_linphonec->isOpen())
   {

        m_linphonec->write("terminate\n");
        m_linphonec->waitForBytesWritten(1000);

        m_linphonec->write("quit\n");
        m_linphonec->waitForBytesWritten(1000);

        if(m_linphonec->waitForFinished())
        {
            qDebug()<<"finished process success";
        }

        result = m_linphonec->exitCode();
        if(result == 0)
        {
            qDebug()<<"exit process success";
        }

        m_linphonec->close();//ferme et tue le processus

   }
   exit();
}
