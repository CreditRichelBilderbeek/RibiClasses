#ifndef QTLABELEDQUADBEZIERARROWITEM_H
#define QTLABELEDQUADBEZIERARROWITEM_H

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
//#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include "qtroundededitrectitem.h"
#pragma GCC diagnostic pop

namespace ribi {

struct QtQuadBezierArrowItem;

///The QtLabeledQuadBezierArrowItem is a QtRoundedEditRectItem that
///draws a bezier curve underneath itself, between head and tail arrowhead
struct QtLabeledQuadBezierArrowItem : public QtRoundedEditRectItem
{

  QtLabeledQuadBezierArrowItem(
    QGraphicsItem *const from,
    const bool tail,
    const std::string& text,
    const bool head,
    QGraphicsItem *const to,
    QGraphicsItem *parent = 0
  );
  QtLabeledQuadBezierArrowItem(const QtLabeledQuadBezierArrowItem&) = delete;
  QtLabeledQuadBezierArrowItem& operator=(const QtLabeledQuadBezierArrowItem&) = delete;
  ~QtLabeledQuadBezierArrowItem() noexcept {}

        boost::shared_ptr<const QtQuadBezierArrowItem>  GetArrow() const noexcept { return m_arrow; }
  const boost::shared_ptr<      QtQuadBezierArrowItem>& GetArrow()       noexcept { return m_arrow; }

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

protected:
  virtual QRectF boundingRect() const noexcept override final;
  virtual void keyPressEvent(QKeyEvent *event) noexcept override final;
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept override final;

private:
  const boost::shared_ptr<QtQuadBezierArrowItem> m_arrow;

};

} //~namespace ribi

#endif // QTLABELEDQUADBEZIERARROWITEM_H
