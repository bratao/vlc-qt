/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* Based on Phonon multimedia library
* Copyright (C) 2011 Harald Sitter <sitter@kde.org>
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

#include "GlPainter.h"

GlPainter::GlPainter()
    : _context(0),
      _texturesInited(false) { }

GlPainter::~GlPainter() { }

void GlPainter::setContext(QGLContext *context)
{
    _context = context;
}

void GlPainter::initRgb32()
{
    Q_ASSERT(_frame->planeCount == 1);
    _textureCount = _frame->planeCount;

    _texDescriptor.target = GL_TEXTURE_2D;
    _texDescriptor.internalFormat = GL_RGBA;
    _texDescriptor.format = GL_RGBA;
    _texDescriptor.type = GL_UNSIGNED_BYTE;
}

void GlPainter::initYv12()
{
    Q_ASSERT(_frame->planeCount == 3);
    _textureCount = _frame->planeCount;

    _texDescriptor.target = GL_TEXTURE_2D;
    _texDescriptor.internalFormat = GL_LUMINANCE;
    _texDescriptor.format = GL_LUMINANCE;
    _texDescriptor.type = GL_UNSIGNED_BYTE;
}

void GlPainter::initColorMatrix()
{
    _colorMatrix = QMatrix4x4(1.0, 0.0, 0.0, 0.0,
                               0.0, 1.0, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0);
    // If the fame has YUV format, apply color correction:
    if (_frame->format == VideoFrame::Format_YV12) {
        QMatrix4x4 colorSpaceMatrix;
        if (_frame->height > 576)
            colorSpaceMatrix =
                    QMatrix4x4(
                        1.164383561643836,  0.0000,             1.792741071428571, -0.972945075016308,
                        1.164383561643836, -0.21324861427373,  -0.532909328559444,  0.301482665475862,
                        1.164383561643836,  2.112401785714286,  0.0000,            -1.133402217873451,
                        0.0000,             0.0000,             0.0000,             1.0000           );
        else
            colorSpaceMatrix =
                    QMatrix4x4(
                        1.164383561643836,  0.0000,             1.596026785714286, -0.874202217873451,
                        1.164383561643836, -0.391762290094914, -0.812967647237771,  0.531667823499146,
                        1.164383561643836,  2.017232142857142,  0.0000,            -1.085630789302022,
                        0.0000,             0.0000,             0.0000,             1.0000           );
        _colorMatrix = _colorMatrix * colorSpaceMatrix;
    }
}

void GlPainter::initTextures()
{
    if (!_texturesInited) {
        for (unsigned i = 0; i < _frame->planeCount; ++i) {
            glBindTexture(_texDescriptor.target, _textureIds[i]);
            glTexImage2D(_texDescriptor.target,
                         0,
                         _texDescriptor.internalFormat,
                         _frame->visiblePitch[i],
                         _frame->visibleLines[i],
                         0,
                         _texDescriptor.format,
                         _texDescriptor.type,
                         0);
            // Scale appropriately so we can change to target geometry without
            // much hassle.
            glTexParameterf(_texDescriptor.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(_texDescriptor.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(_texDescriptor.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(_texDescriptor.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameterf(_texDescriptor.target, GL_TEXTURE_PRIORITY, 1.0);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            _texturesInited = true;
        }
    }

    for (unsigned i = 0; i < _frame->planeCount; ++i) {
        glBindTexture(_texDescriptor.target, _textureIds[i]);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, _frame->pitch[i]);
        glTexSubImage2D(_texDescriptor.target, 0,
                        0, 0,
                        _frame->visiblePitch[i],
                        _frame->visibleLines[i],
                        _texDescriptor.format,
                        _texDescriptor.type,
                        _frame->plane[i].data());
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // reset to default
    }
}
