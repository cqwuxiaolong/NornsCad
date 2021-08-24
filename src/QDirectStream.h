//################
//# QDirectStream.h  #
//# QDirectStream was modified from an early version of QDebugStream from Qt mail lists at http://lists.trolltech.com/qt-interest/2005-06/thread00166-0.html
//# This version of QDirectStream can send data across threads.
//# QDirectStream can direct the standard output stream 'cout' and emit a signal textReady(QString) to  send data.
//#
//# USAGE:
//#         QDirectStream qout(cout); //redirect cout to qout
//#         QObject::connect(&qout,SIGNAL(textReady(QString)),yourReceiverObject,SLOT(youSlot(QString)));
//#
//################

#ifndef Q_DIRECT_STREAM_H
#define Q_DIRECT_STREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>
class QDirectStream : public QObject, std::basic_streambuf<char>
{
    Q_OBJECT

public:
    QDirectStream(std::ostream &stream/*, QTextEdit* text_edit*/);
    ~QDirectStream();


protected:
    virtual int_type overflow(int_type v);
    virtual std::streamsize xsputn(const char *p, std::streamsize n);

private:
     std::ostream &m_stream;
     std::streambuf *m_old_buf;
     std::string m_string;
     //QTextEdit* log_window;

signals:
    void textReady(QString text); //add a signal to send data across threads.
};

#endif