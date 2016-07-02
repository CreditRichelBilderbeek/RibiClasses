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
#ifndef QTCONCEPTMAPCONCEPTMAP_H
#define QTCONCEPTMAPCONCEPTMAP_H

#include <QUndoStack>
#include "qtkeyboardfriendlygraphicsview.h"
#include "qtconceptmapfwd.h"
#include "qtconceptmapqtedge.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///QtConceptMap displays a ConceptMap
///It does not offer UI interaction with the user
///QtConceptMap does offer UI interaction
//Cannot mark it as a final class, because that would break Qt4 compatibility
class QtConceptMap : public ribi::QtKeyboardFriendlyGraphicsView
{
  Q_OBJECT

public:
  ///The mode of the concept map:
  /// * edit: the concept map is edited by the student
  /// * rate: the edited concept map is rated by the assessor
  enum class Mode { edit, rate };

  explicit QtConceptMap(QWidget* parent = 0);
  QtConceptMap(const QtConceptMap&) = delete;
  QtConceptMap& operator=(const QtConceptMap&) = delete;
  ~QtConceptMap() noexcept;

  ///Raw pointer, because ConceptMap its QUndoStack will take over ownership of pointer
  void DoCommand(Command * const command) noexcept;

  ///Obtain the concept map
  const ConceptMap& GetConceptMap() const noexcept { return m_conceptmap; }
        ConceptMap& GetConceptMap()       noexcept { return m_conceptmap; }

  ///The square showing the examples
  const QtExamplesItem * GetQtExamplesItem() const noexcept { return m_examples_item; }
  QtExamplesItem * GetQtExamplesItem() noexcept { return m_examples_item; }

  ///The arrow that must be clicked to add a new edge
  const QtTool * GetQtToolItem() const noexcept { return m_tools; }
  QtTool * GetQtToolItem() noexcept { return m_tools; }

  ///Obtain the QGraphicsScene
  QGraphicsScene* GetScene() const noexcept;

  std::vector<QtEdge *> GetSelectedQtEdges() const noexcept;
  std::vector<QtNode *> GetSelectedQtNodes() const noexcept;

  const QUndoStack& GetUndo() const noexcept { return m_undo; }
        QUndoStack& GetUndo()       noexcept { return m_undo; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void HideExamplesItem() noexcept;

  [[deprecated("Replaced by HideExamplesItem, which correctly described the member function")]]
  void RemoveExamplesItem() noexcept = delete;

  void SetConceptMap(const ConceptMap& conceptmap);

  void SetMode(const Mode mode) noexcept;

  void Undo() noexcept;

public slots:

  void keyPressEvent(QKeyEvent* event) noexcept;
  void mouseMoveEvent(QMouseEvent * event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

protected:

  ///Delete a QtEdge
  //void DeleteQtEdge(const QtEdge * const edge);

  ///Obtain the center node, if there is any
  QtNode * GetCenterNode()       noexcept;

  ///Obtain the first QtNode under the cursor
  ///Returns nullptr if none is present
  QtNode* GetItemBelowCursor(const QPointF& pos) const;

  ///Set the rectangle with text showing the examples
  void SetExamplesItem(QtExamplesItem * const item);

private:

  ///The arrow used to create a new arrow
  ///Is hidden when not active
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtNewArrow * const m_arrow;

  ///The concept map to work on, the Model
  ConceptMap m_conceptmap;

  ///The item showing the examples
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtExamplesItem * const m_examples_item;

  ///The item highlighter, used when creating a new relation
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtItemHighlighter * const m_highlighter;

  Mode m_mode;

  ///The item showing the tools
  QtTool * const m_tools;

  QUndoStack m_undo;

  ///Remove all Qt and non-Qt items
  void RemoveConceptMap();

  ///Called when an Edge gets deleted from the ConceptMap
  void DeleteEdge(const Edge& edge);

  ///Called when a Node gets deleted from theConceptMap
  //void DeleteNode(const Node& node);


  ///Called when an item wants to be edited
  void OnEdgeKeyDownPressed(QtEdge * const item, const int key);

  ///Called when an item wants to be edited
  void OnNodeKeyDownPressed(QtNode* const item, const int key);

  ///Called whenever the tools item is clicked
  void OnToolsClicked();

  /// Writes the selecteness of the QtConceptMap
  /// to the ConceptMap
  void UpdateConceptMap();

  ///The QtExamplesItem must be shown on nodes with examples
  void UpdateExamplesItem();

private slots:

  void onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason);
  void onSelectionChanged();
};

///Counts the QtNodes that are Nodes, i.e. are not on an edge
int CountQtNodes(const QGraphicsScene * const scene) noexcept;
int CountQtEdges(const QGraphicsScene * const scene) noexcept;
int CountSelectedQtNodes(const QGraphicsScene * const scene) noexcept;
int CountSelectedQtEdges(const QGraphicsScene * const scene) noexcept;

///Check is QtConceptMap and its ConceptMap have the requested number of edges and nodes
///Will throw if there is an internal inconsistency
bool DoubleCheckEdgesAndNodes(const QtConceptMap& qtconceptmap, const int n_edges, const int n_nodes);

///Check is QtConceptMap and its ConceptMap have the requested number of selected edges and nodes
///Will throw if there is an internal inconsistency
bool DoubleCheckSelectedEdgesAndNodes(const QtConceptMap& qtconceptmap, const int n_edges, const int n_nodes);

QtEdge * FindQtEdge(const int edge_id, const QGraphicsScene * const scene) noexcept;

//Find the Qt edge with the same from and to
QtEdge * FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept;

///Find the edge this QtNode is in the center of
///Returns nullptr if the QtNode is not on a QtEdge
QtEdge * FindQtEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept;

QtNode * FindQtNode(const int node_id, const QGraphicsScene * const scene) noexcept;

std::vector<QtEdge *> GetQtEdges(const QGraphicsScene * const scene) noexcept;

///Get all the edges connected to the concept
std::vector<QtEdge*> GetQtEdges(
  const QtNode * const from,
  const QGraphicsScene * const scene
) noexcept;


std::vector<QtNode *> GetQtNodes(const QGraphicsScene * const scene) noexcept;

///Check if this item is the center node
bool IsQtCenterNode(const QGraphicsItem* const item);

///Is this QtNode in the center on a QtEdge?
bool IsOnEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept;


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTMAP_H
