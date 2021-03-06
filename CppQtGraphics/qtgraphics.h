//---------------------------------------------------------------------------
/*
QtGraphics, Qt graphics code snippets
Copyright (C) 2015-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtArrowItem.htm
//---------------------------------------------------------------------------
#ifndef QTGRAPHICS_H
#define QTGRAPHICS_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QImage>
#pragma GCC diagnostic pop

namespace ribi {

struct QtGraphics
{
  QtGraphics();

  ///Create a pretty QImage for testing purposes
  QImage CreateImage(const int width, const int height, const int z = 0) const noexcept;

  ///Draw a QImage on another QImage
  ///Scanline with std::copy
  void DrawImage(QImage& target, const QImage& source,  const int left, const int top) const noexcept;

  ///Draw a QImage on another QImage
  ///Scanline with indexed copy
  void DrawImageSlow(QImage& target, const QImage& source,  const int left, const int top) const noexcept;

  ///Draw a QImage on another QImage
  ///Pixel-by-pixel
  void DrawImageSlowest(QImage& target, const QImage& source,  const int left, const int top) const noexcept;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;
};

} //~namespace ribi

#endif // QTGRAPHICS_H
