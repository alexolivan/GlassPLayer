// conn_hls.h
//
// Server connector for HTTP live streams (HLS).
//
//   (C) Copyright 2014-2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef CONN_HLS_H
#define CONN_HLS_H

#include <QProcess>
#include <QTcpSocket>
#include <QTimer>

#include "connector.h"
#include "m3uplaylist.h"

class Hls : public Connector
{
  Q_OBJECT;
 public:
  Hls(QObject *parent=0);
  ~Hls();
  Connector::ServerType serverType() const;
  void reset();

 protected:
  void connectToHostConnector(const QString &hostname,uint16_t port);
  void disconnectFromHostConnector();

 private slots:
  void indexProcessFinishedData(int exit_code,QProcess::ExitStatus status);
  void indexProcessErrorData(QProcess::ProcessError err);

 private:
  void LoadIndex(const QUrl &url);
  QProcess *hls_index_process;
  M3uPlaylist *hls_index_playlist;
};


#endif  // CONN_HLS_H