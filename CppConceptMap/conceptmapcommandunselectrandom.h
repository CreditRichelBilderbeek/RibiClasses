//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2015 Richel Bilderbeek

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
#ifndef CONCEPTMAPCOMMANDUNSELECTRANDOM_H
#define CONCEPTMAPCOMMANDUNSELECTRANDOM_H

#include "conceptmapcommand.h"
#include "conceptmapnode.h"

namespace ribi {
namespace cmap {

///Add another item to the selected pool
class CommandUnselectRandom final : public Command
{
  public:

  using ConstEdges = std::vector<Edge>;
  using ConstNodes = std::vector<Node>;
  using Edges = std::vector<Edge>;
  using Nodes = std::vector<Node>;
  using EdgesAndNodes = std::pair<Edges,Nodes>;
  using ConstEdgesAndNodes = std::pair<ConstEdges,ConstNodes>;

  CommandUnselectRandom(const boost::shared_ptr<ConceptMap> conceptmap);

  CommandUnselectRandom(const CommandUnselectRandom&) = delete;
  CommandUnselectRandom& operator=(const CommandUnselectRandom&) = delete;
  ~CommandUnselectRandom() noexcept {}

  void redo() override;
  void undo() override;

  private:
  const boost::shared_ptr<ConceptMap> m_conceptmap;
  EdgesAndNodes m_new_selected;
  const EdgesAndNodes m_old_selected;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECTRANDOM_H
