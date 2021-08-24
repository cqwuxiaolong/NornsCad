/*------------------------------------------------------------------------------------------*\
  
  
  
   by Haiyang Xu, 2013.
   
   
   
   This program is free software; permission is hereby granted to use, copy, modify,
   and distribute this source code, or portions thereof, for any purpose, without fee,
   subject to the restriction that the copyright notice may not be removed
   or altered from any source or altered source distribution.
   
   The software is released on an as-is basis and without any warranties of any kind.
   In particular, the software is not guaranteed to be fault-tolerant or free from failure.
   The author disclaims all warranties with regard to this software, any use,
   and any consequent failure, is purely the responsibility of the user.
   
   Copyright (C) 2010-2011 Haiyang Xu         findway.xu@gmail.com
\*------------------------------------------------------------------------------------------*/
#include "QDirectStream.h"
#include <streambuf>

QDirectStream::QDirectStream(std::ostream &stream/*, QTextEdit* text_edit*/) : m_stream(stream)
{
     //log_window = text_edit;
     m_old_buf = stream.rdbuf();
     stream.rdbuf(this);
}
QDirectStream::~QDirectStream()
{
     // output anything that is left
     if (!m_string.empty())

         emit textReady(m_string.c_str());
         //log_window->append(m_string.c_str());

     m_stream.rdbuf(m_old_buf);
}

QDirectStream::int_type QDirectStream::overflow(int_type v)
{
     if (v == '\n')
     {

         emit textReady(m_string.c_str());
         //log_window->append(QString(m_string.c_str()));
         m_string.erase(m_string.begin(), m_string.end());
     }
     else
     m_string += v;

     return v;
}

std::streamsize QDirectStream::xsputn(const char *p, std::streamsize n)
{

     m_string.append(p, p + n);
     int pos = 0;
     while (pos != std::string::npos)
     {

         pos = m_string.find('\n');
         if (pos != std::string::npos)
          {
           std::string tmp(m_string.begin(), m_string.begin() + pos);
           emit textReady(tmp.c_str());
           //log_window->append(QString(tmp.c_str()));
           m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
          }
     }
     return n;
}