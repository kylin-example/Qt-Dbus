/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>

#include <QDebug>
#include <QStandardPaths>
#include "sysdbusregister.h"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QString txt;
          switch (type) {
          //调试信息提示
          case QtDebugMsg:
                  txt = QString("Debug: %1").arg(msg);
                  break;

          //一般的warning提示
          case QtWarningMsg:
                  txt = QString("Warning: %1").arg(msg);
          break;
          //严重错误提示
          case QtCriticalMsg:
                  txt = QString("Critical: %1").arg(msg);
          break;
          //致命错误提示
          case QtFatalMsg:
                  txt = QString("Fatal: %1").arg(msg);
                  abort();
          }
    QFile outFile(qgetenv("HOME") +"/tmp/ukui-test.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);  //
    ts << txt << endl;
}


int main(int argc, char *argv[]){

    QCoreApplication app(argc, argv);
    app.setOrganizationName("Kylin Team");
    app.setApplicationName("uktest-service");

    qInstallMessageHandler(outputMessage);


    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService("com.ukui.test.qt.systemdbus")){
        qCritical() << "QDbus register service failed reason:" << systemBus.lastError();
        exit(1);
    }

    if (!systemBus.registerObject("/", new SysdbusRegister(), QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)){
        qCritical() << "QDbus register object failed reason:" << systemBus.lastError();
        exit(2);
    }

    return app.exec();
}
