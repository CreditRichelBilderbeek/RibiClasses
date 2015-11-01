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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapnodedialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "ui_qtconceptmapnodedialog.h"
#include "testtimer.h"
#include "trace.h"

#pragma GCC diagnostic pop

ribi::cmap::QtNodeDialog::QtNodeDialog(QWidget *parent)
  : ribi::QtHideAndShowDialog(parent),
    ui(new Ui::QtNodeDialog),
    m_node{},
    m_qtconceptdialog{new QtConceptDialog}
{
  ui->setupUi(this);
  #ifndef NDEBUG
  Test();
  #endif

  {
    assert(layout());
    layout()->addWidget(m_qtconceptdialog.get());

  }
}

ribi::cmap::QtNodeDialog::~QtNodeDialog()
{
  delete ui;
}

int ribi::cmap::QtNodeDialog::GetMinimumHeight(const Node& node) noexcept
{
  return QtConceptDialog::GetMinimumHeight(node.GetConcept()) + 82;
}


double ribi::cmap::QtNodeDialog::GetUiX() const noexcept
{
  return ui->box_x->value();
}

double ribi::cmap::QtNodeDialog::GetUiY() const noexcept
{
  return ui->box_y->value();
}

std::string ribi::cmap::QtNodeDialog::GetUiName() const noexcept
{
  return m_qtconceptdialog->GetUiName();
}

void ribi::cmap::QtNodeDialog::SetNode(const boost::shared_ptr<Node>& node) noexcept
{
  const bool verbose{false};

  assert(node);
  if (m_node == node)
  {

    return;
  }

  if (verbose)
  {
    std::stringstream s;
    s << "Setting node '" << node->ToStr() << "'\n";
  }
  const auto concept_after = node->GetConcept();
  const auto x_after = node->GetX();
  const auto y_after = node->GetY();

  bool concept_changed  = true;
  bool x_changed  = true;
  bool y_changed = true;

  if (m_node)
  {
    const auto concept_before = m_node->GetConcept();
    const auto x_before = m_node->GetX();
    const auto y_before = m_node->GetY();

    concept_changed = concept_before != concept_after;
    x_changed = x_before != x_after;
    y_changed = y_before != y_after;


    if (verbose)
    {
      if (concept_changed)
      {
        std::stringstream s;
        s
          << "Concept will change from "
          << concept_before.ToStr()
          << " to "
          << concept_after.ToStr()
          << '\n'
        ;
        TRACE(s.str());
      }
      if (x_changed)
      {
        std::stringstream s;
        s << "X will change from " << x_before
          << " to " << x_after << '\n';
        TRACE(s.str());
      }
      if (y_changed)
      {
        std::stringstream s;
        s << "Y will change from " << y_before
          << " to " << y_after << '\n';
        TRACE(s.str());
      }
    }
    //Disconnect m_concept
    m_node->m_signal_concept_changed.disconnect(
      boost::bind(&ribi::cmap::QtNodeDialog::OnConceptChanged,this,boost::lambda::_1)
    );
    m_node->m_signal_x_changed.disconnect(
      boost::bind(&ribi::cmap::QtNodeDialog::OnXchanged,this,boost::lambda::_1)
    );
    m_node->m_signal_y_changed.disconnect(
      boost::bind(&ribi::cmap::QtNodeDialog::OnYchanged,this,boost::lambda::_1)
    );
  }

  //Replace m_example by the new one
  m_node = node;


  assert(m_node->GetConcept() == concept_after );
  assert(m_node->GetX() == x_after );
  assert(m_node->GetY() == y_after);

  m_node->m_signal_concept_changed.connect(
    boost::bind(&ribi::cmap::QtNodeDialog::OnConceptChanged,this,boost::lambda::_1)
  );
  m_node->m_signal_x_changed.connect(
    boost::bind(&ribi::cmap::QtNodeDialog::OnXchanged,this,boost::lambda::_1)
  );
  m_node->m_signal_y_changed.connect(
    boost::bind(&ribi::cmap::QtNodeDialog::OnYchanged,this,boost::lambda::_1)
  );

  //Emit everything that has changed
  if (concept_changed)
  {
    m_node->m_signal_concept_changed(m_node.get());
  }
  if (x_changed)
  {
    m_node->m_signal_x_changed(m_node.get());
  }
  if (y_changed)
  {
    m_node->m_signal_y_changed(m_node.get());
  }

  this->setMinimumHeight(
    this->GetMinimumHeight(
      *m_node
    )
  );

  assert( node ==  m_node);
  assert(*node == *m_node);
}

void ribi::cmap::QtNodeDialog::OnConceptChanged(Node * const node)
{
  const bool verbose{false};
  assert(node);

  const auto concept_before = m_qtconceptdialog->GetConcept();
  const Concept concept_after = node->GetConcept();

  if (verbose)
  {
    std::stringstream s;
    s << "Change concept from "
    << concept_before.ToStr()
    << " to " << concept_after.ToStr();
    TRACE(s.str());
  }

  m_qtconceptdialog->SetConcept(concept_after);

  assert(m_qtconceptdialog->GetConcept() == concept_after);
}

void ribi::cmap::QtNodeDialog::OnXchanged(Node * const node)
{
  assert(node);
  ui->box_x->setValue(node->GetX());
}

void ribi::cmap::QtNodeDialog::OnYchanged(Node * const node)
{
  assert(node);
  ui->box_y->setValue(node->GetY());
}

void ribi::cmap::QtNodeDialog::SetUiName(const std::string& name) noexcept
{
  this->m_qtconceptdialog->SetUiName(name);
  assert(GetUiName() == name);
}


void ribi::cmap::QtNodeDialog::SetUiX(const double x) noexcept
{
  ui->box_x->setValue(x);
  //Calls 'on_box_x_valueChanged' automatically
}

void ribi::cmap::QtNodeDialog::SetUiY(const double y) noexcept
{
  ui->box_y->setValue(y);
  //Calls 'on_box_y_valueChanged' automatically
  //ui->box_y does not immediatly update
  //m_node->SetY(y);
  //on_box_y_valueChanged(y);
}


#ifndef NDEBUG
void ribi::cmap::QtNodeDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  NodeFactory().GetTest(1);
  QtConceptDialog();

  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  QtNodeDialog dialog;
  const boost::shared_ptr<Node> node{NodeFactory().GetTest(1)};
  dialog.SetNode(node);
  if (verbose) { TRACE("X of QtNode and QtNodeDialog must match at start"); }
  {
    assert(std::abs(dialog.GetUiX() - node->GetX()) < 2.0);
  }
  if (verbose) { TRACE("If X is set via QtNode, QtNodeDialog must sync"); }
  {
    const double old_x{node->GetX()};
    const double new_x{old_x + 10.0};
    node->SetX(new_x);
    assert(std::abs(new_x - dialog.GetUiX()) < 2.0);
  }
  if (verbose) { TRACE("If X is set via QtNodeDialog, QtNode must sync"); }
  {
    const double old_x{dialog.GetUiX()};
    const double new_x{old_x + 10.0};
    dialog.SetUiX(new_x);
    assert(std::abs(new_x - node->GetX()) < 2.0);
  }
}
#endif

void ribi::cmap::QtNodeDialog::on_box_x_valueChanged(double arg1)
{
  assert(m_node);
  m_node->SetX(arg1);
}

void ribi::cmap::QtNodeDialog::on_box_y_valueChanged(double arg1)
{
  m_node->SetY(arg1);
}
