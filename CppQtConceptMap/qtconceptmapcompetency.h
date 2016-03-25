//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#ifndef QTCONCEPTMAPCOMPETENCY_H
#define QTCONCEPTMAPCOMPETENCY_H

#include <map>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QColor>
#include <QIcon>
#include "conceptmapcompetency.h"

#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Class to convert cmap::Competency to QColor and vice versa
struct QtCompetency
{
  QtCompetency();

  ///Convert a QColor to a cmap::Competency
  cmap::Competency ColorToCompetency(const QColor& color) const;

  ///Convert a cmap::Competency to a QColor
  QColor CompetencyToColor(const cmap::Competency competency) const;

  ///Convert a cmap::Competency to a QIcon
  QIcon CompetencyToIcon(const cmap::Competency competency) const;

  ///Convert a QIcon to a cmap::Competency
  cmap::Competency IconToCompetency(const QIcon& icon) const;

  private:
  ///The map between cmap::Competency and QColor
  static const std::map<cmap::Competency,QColor> m_color_map;

  ///The map between cmap::Competency and QIcon
  static std::map<cmap::Competency,QIcon> m_icon_map;

  ///Create map between cmap::Competency and QColor
  static const std::map<cmap::Competency,QColor> CreateColorMap();

  ///Create map between cmap::Competency and QIcon
  static const std::map<cmap::Competency,QIcon> CreateIconMap();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCOMPETENCY_H
