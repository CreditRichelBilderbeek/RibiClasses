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
#ifndef CONCEPTMAPNODE_H
#define CONCEPTMAPNODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include "conceptmapfwd.h"
#include "conceptmapconcept.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct NodeFactory;

///A Node is a Concept with coordinats, that is used as an Element in a ConceptMap
///A Node is the GUI independent part of a node. It is displayed as:
/// - QtNode (as QtConceptMapElement: a QGraphicsItem, to be used in a QGraphicsView)
/// - QtNodeDialog (as a QDialog, to be used in a QDialog)
///Node is used as a base class by:
/// - CenterNode
struct Node
{
  ///Use NodeFactory as an unused argument to enforce using it
  explicit Node(
    const Concept& concept = Concept(),
    const bool is_center_node = false,
    const double x = 0.0,
    const double y = 0.0
  ) noexcept;

  ///Get the Concept
  const Concept& GetConcept() const noexcept { return m_concept; }
        Concept& GetConcept()       noexcept { return m_concept; }

  ///Get some test nodes
  static std::vector<Node> GetTests() noexcept;

  int GetId() const noexcept { return m_id; }

  ///Get the x coordinat
  double GetX() const noexcept { return m_x; }

  ///Get the y coordinat
  double GetY() const noexcept { return m_y; }

  bool IsCenterNode() const noexcept { return m_is_center_node; }

  ///Set the concept
  void SetConcept(const Concept& concept) noexcept;

  void SetIsCenterNode(const bool is_center_node) noexcept { m_is_center_node = is_center_node; }

  ///Set the position
  void SetPos(const double x, const double y) noexcept { SetX(x); SetY(y); }

  ///Set the x coordinat
  void SetX(const double x) noexcept;

  ///Set the y coordinat
  void SetY(const double y) noexcept;

  std::string ToStr() const noexcept;

  protected:
  friend class NodeFactory;
  friend class CenterNodeFactory;


  private:

  ///The Concept
  Concept m_concept;

  int m_id; //Unique ID

  bool m_is_center_node;

  ///The x-coordinat
  double m_x;

  ///The y-coordinat
  double m_y;

  static int sm_ids; //ID to assign

  ///Called whenever something on Concept changes
  ///Re-emits m_concept_changed with 'this'
  void OnConceptChanged(Concept * const this_concept) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  friend bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;

};

///Count the number of CenterNodes
int CountCenterNodes(const std::vector<Node>& nodes) noexcept;

std::vector<Node>::const_iterator FindCenterNode(const std::vector<Node>& nodes) noexcept;

///Similar to operator==, except that GUI elements are not tested for equality
bool HasSameContent(const Node& lhs, const Node& rhs) noexcept;

///Returns true if Node is of derived class type CenterNode
///Returns true if Node is Node
bool IsCenterNode(const Node& node) noexcept;

///To uniquely identify all Nodes
bool HaveSameIds(const Node& lhs, const Node& rhs) noexcept;

std::string ToXml(const Node& node) noexcept;

bool operator==(const Node& lhs, const Node& rhs) noexcept;
bool operator!=(const Node& lhs, const Node& rhs) noexcept;
bool operator<(const Node& lhs, const Node& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Node& node) noexcept;
std::istream& operator>>(std::istream& is, Node& node) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPNODE_H
