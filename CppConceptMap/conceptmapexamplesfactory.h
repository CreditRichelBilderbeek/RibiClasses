//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPEXAMPLESFACTORY_H
#define CONCEPTMAPEXAMPLESFACTORY_H

#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapcompetency.h"

#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

struct ExamplesFactory
{
  ExamplesFactory() noexcept;

  Examples Create(
    const std::vector<std::pair<std::string,Competency> >& v) const noexcept;

  int GetNumberOfTests() const noexcept { return static_cast<int>(GetTests().size()); }
  Examples GetTest(const int i) const noexcept;
  std::vector<Examples> GetNastyTests() const noexcept;
  std::vector<Examples> GetTests() const noexcept;

  private:

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif // NDEBUG
};

} //~namespace cmap

} //~namespace ribi

#endif // CONCEPTMAPEXAMPLESFACTORY_H
