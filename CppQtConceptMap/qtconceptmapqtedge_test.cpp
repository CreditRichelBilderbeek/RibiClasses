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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapqtedge.h"

#include <boost/lambda/lambda.hpp>

#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "counter.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtedgefactory.h"
#include "qtquadbezierarrowitem.h"
#include "qtroundededitrectitem.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_CASE(ribi_cmap_qtedge_test)
{
  using namespace ribi;
  using namespace ribi::cmap;
  const bool verbose{false};
  const int node_test_index{0};
  const int edge_test_index{0};
  const Node node_from{NodeFactory().GetTest(node_test_index)};
  const Node node_to{NodeFactory().GetTest(node_test_index)};
  const boost::shared_ptr<QtNode> qtnode_from{new QtNode(node_from)};
  const boost::shared_ptr<QtNode> qtnode_to{new QtNode(node_to)};
  const Edge edge{EdgeFactory().GetTest(edge_test_index)};
  const boost::shared_ptr<QtEdge> qtedge{new QtEdge(edge,qtnode_from.get(),qtnode_to.get())};
  //const boost::shared_ptr<QtEdge> qtedge{new QtEdge(edge,qtnode_from,qtnode_to)};
  QtRoundedEditRectItem * qtitem{dynamic_cast<QtRoundedEditRectItem*>(qtedge->GetQtNode())};
  if (verbose) { TRACE("QtEdge must accept hover events"); } //NOT SURE
  {
    //BOOST_CHECK(qtedge->acceptHoverEvents()); //Must remove the 's' in Qt5?
  }
  if (verbose) { TRACE("QtEdge its arrow must accept hover events"); }
  {
    BOOST_CHECK(qtedge->GetArrow()->acceptHoverEvents()); //Must remove the 's' in Qt5?
  }
  //Text
  if (verbose) { TRACE("Text of QtEdge must be one line"); }
  {
    BOOST_CHECK(qtitem->GetText().size() == 1);
  }
  if (verbose) { TRACE("Text of QtEdge and QtRoundedEditRectItem must match at creation"); }
  {
    const std::string qtitem_name{qtitem->GetText()[0]};
    const std::string qtedge_name{qtedge->GetEdge().GetNode().GetConcept().GetName()};
    BOOST_CHECK(qtitem_name == qtedge_name);
  }
  //X
  if (verbose) { TRACE("X of QtEdge and QtRoundedEditRectItem must match at creation"); }
  {
    const double edge_x{edge.GetNode().GetX()};
    const double qtedge_x{qtitem->GetCenterX()};
    BOOST_CHECK(edge_x == qtedge_x);
  }
  if (verbose) { TRACE("If X is set via QtEdge, QtRoundedEditRectItem must sync"); }
  {
    const double old_x{qtedge->GetQtNode()->GetCenterX()};
    const double new_x{old_x + 10.0};
    qtedge->GetQtNode()->SetCenterX(new_x);
    BOOST_CHECK(std::abs(qtitem->GetCenterX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("If X is set via QtRoundedEditRectItem, QtEdge must sync"); }
  {
    const double old_x{qtitem->GetCenterX()};
    const double new_x{old_x + 10.0};
    qtitem->SetCenterX(new_x);
    BOOST_CHECK(std::abs(qtedge->GetQtNode()->GetCenterX() - new_x) < 2.0);
  }
  //Y
//  if (verbose) { TRACE("Y of QtEdge and QtRoundedEditRectItem must match at creation"); }
//  {
//    const double edge_y{edge.GetNode().GetY()};
//    const double qtedge_y{qtitem->GetCenterY()};
//    BOOST_CHECK(edge_y == qtedge_y);
//  }
  if (verbose) { TRACE("If Y is set via QtEdge, QtRoundedEditRectItem must sync"); }
  {
    const double old_y{qtedge->GetQtNode()->GetCenterY()};
    const double new_y{old_y + 10.0};
    qtedge->GetQtNode()->SetCenterY(new_y);
    BOOST_CHECK(std::abs(qtitem->GetCenterY() - new_y) < 2.0);
  }
  if (verbose) { TRACE("If Y is set via QtRoundedEditRectItem, QtEdge must sync"); }
  {
    const double old_y{qtitem->GetCenterY()};
    const double new_y{old_y + 10.0};
    qtitem->SetCenterY(new_y);
    BOOST_CHECK(std::abs(qtedge->GetQtNode()->GetCenterY() - new_y) < 2.0);
  }
  //Center
  {
    const double new_x{qtedge->GetQtNode()->GetCenterX() + 123.45};
    qtedge->GetQtNode()->SetCenterX(new_x);
    BOOST_CHECK(qtedge->GetQtNode()->GetCenterX() == new_x);
  }
  {
    const double new_y{qtedge->GetQtNode()->GetCenterY() + 123.45};
    qtedge->GetQtNode()->SetCenterY(new_y);
    BOOST_CHECK(qtedge->GetQtNode()->GetCenterY() == new_y);
  }
  if (verbose) { TRACE("If a QtEdge its Edge's tail arrow is changed, a signal must be emitted by Edge"); }
  {
    Counter c{0}; //For receiving the signal
    qtedge->m_signal_selected_changed.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
    qtedge->SetSelected(true);
    qtedge->SetSelected(false);
    BOOST_CHECK(c.Get() > 0);
  }
}
