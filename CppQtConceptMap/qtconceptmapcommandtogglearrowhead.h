#ifndef QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H
#define QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "qtconceptmapcommand.h"

struct QGraphicsScene;

namespace ribi {
namespace cmap {

struct QtEdge;
struct QtNode;

class CommandToggleArrowHead final : public Command
{
  public:

  CommandToggleArrowHead(
    ConceptMap& conceptmap,
    QGraphicsScene * const scene
  );
  CommandToggleArrowHead(const CommandToggleArrowHead&) = delete;
  CommandToggleArrowHead& operator=(const CommandToggleArrowHead&) = delete;

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap; //The concept map to modify
  ConceptMap m_cmap_after;
  const ConceptMap m_cmap_before;
  Edge m_edge_after;
  const Edge m_edge_before;
  QGraphicsScene * const m_scene;
  QtEdge * const m_qtedge; //The QtEdge to modify


  static Edge ExtractEdge(const ConceptMap& conceptmap, const QGraphicsScene& scene);
  static QtEdge * ExtractQtEdge(const QGraphicsScene& scene);
};

} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H
