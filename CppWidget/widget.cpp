//---------------------------------------------------------------------------
/*
Widget, GUI independent widget class
Copyright (C) 2011-2015 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppWidget.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "widget.h"

#include <boost/geometry/algorithms/equals.hpp>

#include "geometry.h"
#include "testtimer.h"

#pragma GCC diagnostic pop

ribi::Widget::Widget(
  const Point& top_left,
  const Point& bottom_right
)
  : m_geometry{
    CreateRect(top_left,bottom_right)}
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::Widget::Rect ribi::Widget::CreateRect(
  const Point& top_left,
  const Point& bottom_right
) noexcept
{
  return CreateRect(
    top_left.x(),
    top_left.y(),
    bottom_right.x() - top_left.x(),
    bottom_right.y() - top_left.y()
  );
}

ribi::Widget::Rect ribi::Widget::CreateRect(
  const double left, const double top, const double width, const double height) noexcept
{
  return Geometry().CreateRect(left,top,width,height);
}

double ribi::Widget::GetBottom() const noexcept
{
  return Geometry().GetBottom(m_geometry);
}

double ribi::Widget::GetHeight() const noexcept
{
  return Geometry().GetHeight(m_geometry);
}

double ribi::Widget::GetLeft() const noexcept
{
  return Geometry().GetLeft(m_geometry);
}

double ribi::Widget::GetRight() const noexcept
{
  return Geometry().GetRight(m_geometry);
}

double ribi::Widget::GetTop() const noexcept
{
  return Geometry().GetTop(m_geometry);
}

std::string ribi::Widget::GetVersion() noexcept
{
  return "3.0";
}

std::vector<std::string> ribi::Widget::GetVersionHistory() noexcept
{
  return {
    "2011-07-03: version 1.0: initial version",
    "2011-08-07: version 1.1: added signal that is emitted when geometry changes",
    "2014-03-28: version 2.0: replace Rect by Boost.Geometry its box class",
    "2015-11-21: version 3.0: made Widget a regular data type"
  };
}

double ribi::Widget::GetWidth() const noexcept
{
  return Geometry().GetWidth(m_geometry);
}

bool ribi::Widget::IsIn(const double x, const double y) const noexcept
{
  return boost::geometry::within(Point(x,y),m_geometry);
}

void ribi::Widget::SetGeometry(const double left, const double top, const double width, const double height) noexcept
{
  SetGeometry(CreateRect(left,top,width,height));
}

void ribi::Widget::SetGeometry(const Rect& geometry) noexcept
{
  if (!boost::geometry::equals(m_geometry,geometry))
  {
    m_geometry = geometry;
  }
}

#ifndef NDEBUG
void ribi::Widget::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  using Point = boost::geometry::model::d2::point_xy<double>;
  {
    const Widget a;
    const Widget b;
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.0,1.0));
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.5,0.0),Point(1.0,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.5),Point(1.0,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.5,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.0,1.5));
    assert(a != b);
  }

  {
    const Widget a;
    const Widget b(a);
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    Widget b(Point(0.0,0.0),Point(1.0,1.5));
    assert(a != b);
    b = a;
    assert(a == b);
  }
}
#endif


bool ribi::operator==(const Widget& lhs, const Widget& rhs) noexcept
{
  using boost::geometry::equals;
  return equals(lhs.GetGeometry(),rhs.GetGeometry());
}

bool ribi::operator!=(const Widget& lhs, const Widget& rhs) noexcept
{
  return !(lhs == rhs);
}
