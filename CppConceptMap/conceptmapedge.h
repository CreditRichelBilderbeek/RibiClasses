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
#ifndef CONCEPTMAPEDGE_H
#define CONCEPTMAPEDGE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "conceptmapfwd.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop


namespace ribi {
namespace cmap {

struct EdgeFactory;

///An Edge is the GUI-independent part of the edges used in QtConceptMap.
///An Edge goes from one Node to another, which must a different Node,
/// at the center of the Edge is a Node
struct Edge
{
  ~Edge() noexcept;

  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }

  ///Get the Node this edge originates from
  const Node * GetFrom() const noexcept { return m_from; }

  ///Get the Node index this edge goes to
  const Node * GetTo() const noexcept { return m_to; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Does the edge have an arrow at the head?
  bool HasHeadArrow() const noexcept { return m_head_arrow; }

  ///Does the edge have an arrow at the tail?
  bool HasTailArrow() const noexcept { return m_tail_arrow; }

  ///Set the Node index this edge originates from
  //void SetFrom(const NodePtr& from) noexcept;

  ///Set if the head has an arrow
  void SetHeadArrow(const bool has_head_arrow) noexcept;

  ///Set the center Node
  void SetNode(const Node& node) noexcept;

  ///Set if the tail has an arrow
  void SetTailArrow(const bool has_tail_arrow) noexcept;

  ///Set the Node index this edge goes to
  //void SetTo(const NodePtr& to) noexcept;

  std::string ToStr() const noexcept;

  ///Convert an Edge from an XML std::string
  ///The container of nodes is needed to convert the 'to' and 'from'
  ///field to indices
  static std::string ToXml(
    const Edge& c,
    const std::vector<Node>& nodes
  ) noexcept;

  private:
  ///The Node this edge originates from
  const Node * m_from;

  ///Is there an arrowhead at the 'to' node?
  bool m_head_arrow;

  ///The Node on the Edge
  Node m_node;

  ///Is there an arrowhead at the 'from' node?
  bool m_tail_arrow;

  ///The Node this edge goes to
  const Node * m_to;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  ///Block constructor, except for EdgeFactory
  friend class EdgeFactory;
  explicit Edge(
    const Node& node,
    const Node& from,
    const bool tail_arrow,
    const Node& to,
    const bool head_arrow
  );

  ///Bundles Node its signals into emitting a signal that the node has changed
  void OnConceptChanged(Node * const node) noexcept;
  void OnFromChanged(Node * const node) noexcept;
  void OnToChanged(Node * const node) noexcept;
};

bool IsConnectedToCenterNode(const Edge& edge) noexcept;

std::ostream& operator<<(std::ostream& os, const Edge& edge) noexcept;

bool operator==(const Edge& lhs, const Edge& rhs);
bool operator!=(const Edge& lhs, const Edge& rhs);
bool operator<(const Edge& lhs, const Edge& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGE_H
