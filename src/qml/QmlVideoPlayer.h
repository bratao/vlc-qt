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

#ifndef VLCQT_QMLVIDEOPLAYER_H_
#define VLCQT_QMLVIDEOPLAYER_H_

#include <QtDeclarative/QDeclarativeItem>

#include "SharedExportQml.h"

class VlcAudio;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VideoGraphicsObject;

class VLCQT_QML_EXPORT VlcQmlVideoPlayer : public QDeclarativeItem
{
Q_OBJECT
public:
    explicit VlcQmlVideoPlayer(QDeclarativeItem *parent = 0);
    ~VlcQmlVideoPlayer();

	Q_INVOKABLE void close();
	Q_INVOKABLE void openFile(const QString &file);
	Q_INVOKABLE void openStream(const QString &stream);
	Q_INVOKABLE void pause();
	Q_INVOKABLE void play();
	Q_INVOKABLE void stop();

    /// \reimp
    void classBegin();


protected:
    /// Forwards geometry changes to the internal VideoGraphicsObject.
    /// \reimp
    virtual void geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry);

private:
    /// The contained VideoGraphicsObject (it is a childitem of this item actually).
    VideoGraphicsObject *_graphicsObject;

	void openInternal();

	VlcInstance *_instance;
	VlcMediaPlayer *_player;
	VlcMedia *_media;

	VlcAudio *_audioManager;

	bool _hasMedia;
};

#endif // VLCQT_QMLVIDEOPLAYER_H_
