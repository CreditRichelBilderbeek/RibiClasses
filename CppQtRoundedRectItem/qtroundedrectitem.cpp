//---------------------------------------------------------------------------
/*
QtRoundedRectItem, rectangular-shaped QGraphicsItem
Copyright (C) 2012-2016 Richel Bilderbeek and Claudio Tiecher

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
//From http://www.richelbilderbeek.nl/CppQtRoundedRectItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtroundedrectitem.h"

#include <cassert>
#include <cmath>
#include <QPainter>
#include <QGraphicsScene>
#include <QKeyEvent>

#pragma GCC diagnostic pop

ribi::QtRoundedRectItem::QtRoundedRectItem(QGraphicsItem *parent)
 : QGraphicsRectItem(parent),
   m_contour_pen(QPen(QColor(0,0,0),0.0)),
   m_focus_pen(QPen(QColor(0,0,0),0.0,Qt::DashLine)),
   m_radius_x(4.0),
   m_radius_y(4.0)
{
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    );
  this->SetContourPen(QPen(QColor(0,0,0),2.0));
  this->SetFocusPen(QPen(QColor(0,0,0),3.0,Qt::DashLine));
  const double height = 32.0;
  const double width = 64.0;
  this->SetInnerWidth(width);
  this->SetInnerHeight(height);
  this->setBrush(Qt::white); //Makes the rectangle opaque; otherwise it would be transparent
}

ribi::QtRoundedRectItem::~QtRoundedRectItem() noexcept
{
  //OK
}

double ribi::QtRoundedRectItem::GetInnerHeight() const noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  return GetOuterHeight() - (2.0 * pen_width);
}

QRectF ribi::QtRoundedRectItem::GetInnerRect() const noexcept
{
  return QRectF(
    GetCenterX(),
    GetCenterY(),
    GetInnerWidth(),
    GetInnerHeight()
  );
}

double ribi::QtRoundedRectItem::GetInnerWidth() const noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  return GetOuterWidth() - (2.0 * pen_width);
}


QRectF ribi::QtRoundedRectItem::GetOuterRect() const noexcept
{
  const QRectF r{QGraphicsRectItem::rect()};
  return r.translated(GetCenterX(),GetCenterY());
}


std::string ribi::QtRoundedRectItem::GetVersion() noexcept
{
  return "1.11";
}

std::vector<std::string> ribi::QtRoundedRectItem::GetVersionHistory() noexcept
{
  return {
    "2012-12-13: version 1.0: initial version",
    "2012-12-19: version 1.1: added use of pen, brush and focus-indicating pen",
    "2012-12-22: version 1.2: correctly uses the focus and regular pen, added contour pen",
    "2014-06-14: version 1.3: removed superfluous signal",
    "2014-06-14: version 1.4: fixed issue #219",
    "2014-06-16: version 1.5: disallow setRect and setPos (use SetRoundedRect and SetPos instead), cooperation with QtRoundedRectItemDialog",
    "2014-06-22: version 1.6: allow setting the inner and outer rectangle",
    "2014-08-07: version 1.7: renamed IncludingPen member functions to Outer",
    "2014-08-08: version 1.8: removed using with rectangles from interface, as it led to incorrectness and confusion",
    "2014-08-09: version 1.9: increased use of TDD, fixed bug",
    "2015-02-08: version 1.10: gaining/losing focus for different pen widths work correctly",
    "2015-08-21: version 1.11: rectangles are opaque and white by default"
  };
}

void ribi::QtRoundedRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) noexcept
{
  QGraphicsRectItem::mouseMoveEvent(event);
  this->update();
  this->scene()->update();
}

void ribi::QtRoundedRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) noexcept
{
  painter->setBrush(brush());

  const QPen thickest_pen = GetContourPen().widthF() > GetFocusPen().widthF()
                         ? GetContourPen()
                         : GetFocusPen();

  if (this->isSelected() || this->hasFocus())
  {
    painter->setPen(m_focus_pen);
    assert(painter->pen() == m_focus_pen);
  }
  else
  {
    painter->setPen(m_contour_pen);
    assert(painter->pen() == m_contour_pen);
  }

  const double w{GetOuterWidth() - (2 * thickest_pen.widthF()) + this->GetCurrentPen().widthF()};
  const double h{GetOuterHeight() - (2 * thickest_pen.widthF()) + this->GetCurrentPen().widthF()};

  painter->drawRoundedRect(
    QRectF(
      -0.5 * w,
      -0.5 * h,
       1.0 * w, //Width
       1.0 * h //Height
    ),
    m_radius_x,m_radius_y
  );
}

void ribi::QtRoundedRectItem::SetCenterX(const double x) noexcept
{
  const auto current_pos = this->GetCenterPos();
  QGraphicsRectItem::setPos(
    x,
    current_pos.y()
  );
  this->update();
}

void ribi::QtRoundedRectItem::SetCenterY(const double y) noexcept
{
  const double current_y = this->GetCenterY();
  if (current_y != y)
  {
    const auto current_pos = this->GetCenterPos();
    QGraphicsRectItem::setPos(
      current_pos.x(),
      y
    );
    this->update();
  }
}

void ribi::QtRoundedRectItem::SetContourPen(const QPen& pen) noexcept
{
  #ifndef NDEBUG
  const double inner_height_before = GetInnerHeight();
  const double inner_width_before = GetInnerWidth();
  #endif
  //if (m_contour_pen.widthF() != pen.widthF())
  if (m_contour_pen != pen)
  {
    const double height = GetInnerHeight();
    const double width = GetInnerWidth();
    m_contour_pen = pen;
    this->SetInnerHeight(height);
    this->SetInnerWidth(width);

    this->update();
  }
  #ifndef NDEBUG
  else
  {
    const double inner_height_after = GetInnerHeight();
    const double inner_width_after = GetInnerWidth();
    assert(std::abs(inner_height_before - inner_height_after) < 0.001);
    assert(std::abs(inner_width_before  - inner_width_after ) < 0.001);
  }
  #endif
  assert(std::abs(pen.widthF() - GetContourPen().widthF()) < 0.01);
}

void ribi::QtRoundedRectItem::SetFocusPen(const QPen& pen) noexcept
{
  #ifndef NDEBUG
  const double inner_height_before = GetInnerHeight();
  const double inner_width_before = GetInnerWidth();
  #endif
  if (m_focus_pen != pen)
  {
    const double height = GetInnerHeight();
    const double width = GetInnerWidth();
    m_focus_pen = pen;
    this->SetInnerHeight(height);
    this->SetInnerWidth(width);

    this->update();
  }
  #ifndef NDEBUG
  else
  {
    const double inner_height_after = GetInnerHeight();
    const double inner_width_after = GetInnerWidth();
    assert(std::abs(inner_height_before - inner_height_after) < 0.001);
    assert(std::abs(inner_width_before  - inner_width_after ) < 0.001);
  }
  #endif

  assert(std::abs(pen.widthF() - GetFocusPen().widthF()) < 0.01);
}

void ribi::QtRoundedRectItem::SetInnerHeight(const double height) noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  SetOuterHeight(
    height + (2.0 * pen_width)
  );
}

void ribi::QtRoundedRectItem::SetInnerWidth(const double width) noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  SetOuterWidth(
    width + (2.0 * pen_width)
  );
}


void ribi::QtRoundedRectItem::SetOuterHeight(const double height) noexcept
{
  if (height != GetOuterHeight())
  {
    const auto w = GetOuterWidth();
    const auto h = height;
    QGraphicsRectItem::setRect(
      QRectF(
        QPointF(-0.5 * w,-0.5 * h),
        QPointF( 0.5 * w, 0.5 * h)
      )
    );
    this->update();
  }
}

void ribi::QtRoundedRectItem::SetOuterWidth(const double width) noexcept
{
  if (width != GetOuterWidth())
  {
    const auto w = width;
    const auto h = GetOuterHeight();
    QGraphicsRectItem::setRect(
      QRectF(
        QPointF(-0.5 * w,-0.5 * h),
        QPointF( 0.5 * w, 0.5 * h)
      )
    );
    this->update();
  }
}

void ribi::QtRoundedRectItem::SetRadiusX(const double radius_x) noexcept
{
  m_radius_x = radius_x;
  this->update();
  assert(radius_x == GetRadiusX());
}

void ribi::QtRoundedRectItem::SetRadiusY(const double radius_y) noexcept
{
  m_radius_y = radius_y;
  assert(radius_y == GetRadiusY());
}

bool ribi::operator==(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept
{
  return
       lhs.GetContourPen() == rhs.GetContourPen()
    && lhs.GetFocusPen() == rhs.GetFocusPen()
    && lhs.GetRadiusX() == rhs.GetRadiusX()
    && lhs.GetRadiusY() == rhs.GetRadiusY()
    && lhs.GetInnerWidth() == rhs.GetInnerWidth()
    && lhs.GetInnerHeight() == rhs.GetInnerHeight()
  ;
}

bool ribi::operator!=(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept
{
  return !(lhs == rhs);
}
