/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QTextCodec>
#include <QtDeclarative/QDeclarativeView>
#include <QtOpenGL/QGLWidget>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QApplication>
#else
    #include <QtGui/QApplication>
#endif

#include "qml/QmlVideoPlayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Test QML");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    qmlRegisterType<VlcQmlVideoPlayer>("VLCQt", 0, 9, "VlcVideoPlayer");

    QApplication app(argc, argv);

    QDeclarativeView view;
    view.setViewport(new QGLWidget);
    view.setSource(QUrl("qml/video.qml"));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.show();

    return app.exec();
}

