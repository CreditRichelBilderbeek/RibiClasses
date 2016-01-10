//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2015 The Brainweaver Team

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QUndoStack>

#include "qtkeyboardfriendlygraphicsview.h"
#include "qtconceptmapfwd.h"
#include "qtconceptmapqtedge.h"
#include "conceptmap.h"
#pragma GCC diagnostic pop

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

  std::vector<const QtEdge *> GetSelectedQtEdges() const noexcept;
  std::vector<const QtNode *> GetSelectedQtNodes() const noexcept;

  const QUndoStack& GetUndo() const noexcept { return m_undo; }
        QUndoStack& GetUndo()       noexcept { return m_undo; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;


  void RemoveExamplesItem() noexcept { SetExamplesItem(nullptr); }

  void SetConceptMap(const ConceptMap& conceptmap);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  void Undo() noexcept;

  ///Signal emitted when a concept map item requests to be edited
  boost::signals2::signal<void(QtRoundedEditRectItem*)> m_signal_conceptmapitem_requests_edit;

  boost::signals2::signal<void(const ConceptMap sub_conceptmap)> m_signal_request_rate_concept;

public slots:

  void keyPressEvent(QKeyEvent* event) noexcept;
  void mouseMoveEvent(QMouseEvent * event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

protected:

  ///Delete a QtEdge
  void DeleteQtEdge(const QtEdge * const edge);

  ///Delete a Node
  //void DeleteQtNode(const QtNode * const node);

  ///Obtain the center node, if there is any
  QtNode * GetCenterNode()       noexcept;

  ///Obtain the first QtNode under the cursor
  ///Returns nullptr if none is present
  QtNode* GetItemBelowCursor(const QPointF& pos) const;

  ///Check if this item is the center node
  static bool IsQtCenterNode(const QGraphicsItem* const item);

  ///All items from a ConceptMap are put in at the center and need to be repositioned
  void RepositionItems();

  ///Set the rectangle with text showing the examples
  void SetExamplesItem(QtExamplesItem * const item);

private:

  ///The arrow used to create a new arrow
  ///Is nullptr when not active
  QtNewArrow * m_arrow;

  ///The concept map to work on, the Model
  ConceptMap m_conceptmap;

  ///The item showing the examples
  QtExamplesItem * m_examples_item;

  ///The item highlighter, used when creating a new relation
  QtItemHighlighter * const m_highlighter;

  ///The item showing the tools
  QtTool * m_tools;

  QUndoStack m_undo;

  ///Remove all Qt and non-Qt items
  void CleanMe();

  ///Called when an Edge gets deleted from the ConceptMap
  void DeleteEdge(const Edge& edge);

  ///Called when a Node gets deleted from the ConceptMap
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

private slots:

  void onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason);
  void onSelectionChanged();

public slots:

  ///Called whenever a concept is clicked or moved
  ///If item is nullptr, the last item might be deleted
  ///Use QGraphicsItem* due to QtKeyboardFriendlyGraphicsView working on QGraphicsItems
  //void OnItemRequestsUpdate(const QGraphicsItem* const item);

  ///Called when an item requests a scene update
  void OnRequestSceneUpdate();
};

int CountQtNodes(const QGraphicsScene * const scene) noexcept;
int CountQtEdges(const QGraphicsScene * const scene) noexcept;

QtEdge * FindQtEdge(
  const Edge& edge,
  const QGraphicsScene * const scene
) noexcept;

//Find the Qt edge with the same from and to
QtEdge * FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept;


///Find the QtNode containing the Node
QtNode * FindQtNode(const Node& node, const QGraphicsScene * const scene) noexcept;
QtNode * FindQtNode(const int node_id, const QGraphicsScene * const scene) noexcept;

std::vector<QtEdge *> GetQtEdges(const QGraphicsScene * const scene) noexcept;

///Get all the edges connected to the concept
std::vector<QtEdge*> GetQtEdges(
  const QtNode * const from,
  const QGraphicsScene * const scene
) noexcept;

std::vector<QtNode *> GetQtNodes(const QGraphicsScene * const scene) noexcept;


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTMAP_H
