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
#include "qtconceptmaprateconcepttallydialognewname.h"

#include <cassert>
#include <sstream>
#include <numeric>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"

#include "conceptmapcenternodefactory.h"
#include "conceptmap.h"
#include "conceptmapfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapexample.h"
#include "conceptmapedge.h"
#include "conceptmapexamples.h"
#include "testtimer.h"
#include "qtconceptmaprating.h"
#include "trace.h"
#include "ui_qtconceptmaprateconcepttallydialognewname.h"
#pragma GCC diagnostic pop

ribi::cmap::QtRateConceptTallyDialog::QtRateConceptTallyDialog(
  const ConceptMap& conceptmap,
  QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRateConceptTallyDialog),
    m_data(CreateData(conceptmap)),
    m_focus_name(GetFocusName(conceptmap))
{
  ui->setupUi(this);

  const int n_rows = static_cast<int>(m_data.size());
  const int n_cols = 4;
  ui->table->setRowCount(n_rows);
  ui->table->setWordWrap(true);

  //From https://stackoverflow.com/questions/9544122/how-to-word-wrap-text-in-the-rows-and-columns-of-a-qtablewidget
  connect(
    ui->table->horizontalHeader(),
    SIGNAL(sectionResized(int, int, int)),
    ui->table,
    SLOT(resizeRowsToContents()));

  for (int row_index=0; row_index!=n_rows; ++row_index)
  {
    const Row& row = m_data[row_index];
    const Concept concept = std::get<1>(row);
    const int example_index = std::get<2>(row);

    if (example_index == -1)
    {
      //Display concept text
      //Put X checkbox in the relation's name
      //Keep C and S columns empty
      {
        //Put X checkbox in the relation's name in column[0]
        const int column = 0;
        QTableWidgetItem * const i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        i->setCheckState(concept.GetIsComplex() ? Qt::Checked : Qt::Unchecked);
        ui->table->setItem(row_index, column, i);
      }
      {
        //Put uneditable nothing column[1]
        const int column = 1;
        QTableWidgetItem * const i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsEnabled);
        ui->table->setItem(row_index, column, i);
      }
      {
        //Put uneditable nothing column[2]
        const int column = 2;
        QTableWidgetItem * const i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsEnabled);
        ui->table->setItem(row_index, column, i);
      }
      {
        //Put the relation's name in place
        QTableWidgetItem * const i = new QTableWidgetItem;
        i->setFlags(
            Qt::ItemIsSelectable
          | Qt::ItemIsEnabled
        );
        const EdgeDescriptor edge { std::get<0>(row) };
        const bool center_is_from {
          ribi::cmap::GetFrom(edge,conceptmap) == ribi::cmap::GetFocalNode(conceptmap)
        };
        const Node other {
          center_is_from ? ribi::cmap::GetTo(edge,conceptmap) : ribi::cmap::GetFrom(edge, conceptmap)
        };
        const std::string s {
            "via '"
          + concept.GetName() + "' verbonden met '"
          + other.GetConcept().GetName()
          + "'"
        };
        i->setText(s.c_str());

        const int column = 3;
        ui->table->setItem(row_index, column, i);
      }
    }
    else
    {
      assert(example_index < static_cast<int>(concept.GetExamples().Get().size()));
      const Example& example = concept.GetExamples().Get()[example_index];
      //Display index'th example
      for (int col_index=0; col_index!=n_cols; ++col_index)
      {
        if (col_index != 3)
        {
          QTableWidgetItem * const item = new QTableWidgetItem;
          item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
          switch (col_index)
          {
            case 0: item->setCheckState(example.GetIsComplex() ? Qt::Checked : Qt::Unchecked); break;
            case 1: item->setCheckState(example.GetIsConcrete() ? Qt::Checked : Qt::Unchecked); break;
            case 2: item->setCheckState(example.GetIsSpecific() ? Qt::Checked : Qt::Unchecked); break;
            default: assert(!"Should not get here");
          }
          ui->table->setItem(row_index, col_index, item);
        }
        else
        {
          //Text
          QTableWidgetItem * const item = new QTableWidgetItem;
          item->setFlags(
              Qt::ItemIsSelectable
            | Qt::ItemIsEnabled);
          const std::string s = example.GetText();
          item->setText(s.c_str());
          ui->table->setItem(row_index, col_index, item);
        }
      }
    }

  }

  //Set text on top
  ui->label_concept_name->setText(
    ("Voorbeelden/toelichting bij concept: " + m_focus_name).c_str()
  );

  ui->table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  ui->label_debug->setVisible(true);
  QObject::connect(ui->table,SIGNAL(cellChanged(int,int)),this,SLOT(OnCellChanged(int,int)));

  {
    const int x = GetSuggestedComplexity();
    const int c = GetSuggestedConcreteness();
    const int s = GetSuggestedSpecificity();
    std::stringstream m;
    m << "Complexiteit: " << x << ", concreetheid: " << c << ", specificiteit: " << s;
    ui->label_debug->setText(m.str().c_str());
  }
}

ribi::cmap::QtRateConceptTallyDialog::~QtRateConceptTallyDialog() noexcept
{
  delete ui;
}

std::vector<ribi::cmap::QtRateConceptTallyDialog::Row>
  ribi::cmap::QtRateConceptTallyDialog::CreateData(
  const ConceptMap& /* map */
)
{
  std::vector<Row> data;

  #ifdef FIX_ISSUE_10
  assert(map);
  assert(map->GetFocalNode());

  //Add the focal concept its examples (not its name: this cannot be judged)
  {
    const Concept focal_concept = map->GetFocalNode()->GetConcept();
    const int n_examples = boost::numeric_cast<int>(focal_concept.GetExamples().Get().size());
    for (int i=0; i!=n_examples; ++i)
    {
      Edge empty_edge;
      data.push_back(std::make_tuple(empty_edge,focal_concept,i));
    }
  }

  //Collect all relations of the focal node of this sub concept map
  for(const Edge edge: map->GetEdges())
  {
    //But skip the connections to the focal question
    if (IsCenterNode(*edge.GetTo())
      || IsCenterNode(*edge.GetFrom()))
    {
      continue;
    }


    const Concept concept = edge.GetNode().GetConcept();
    data.push_back(std::make_tuple(edge,concept,-1));
    const int n_examples = boost::numeric_cast<int>(concept.GetExamples().Get().size());
    for (int i=0; i!=n_examples; ++i)
    {
      Edge empty_edge;
      data.push_back(std::make_tuple(empty_edge,concept,i));
    }
  }
  #endif // FIX_ISSUE_10
  return data;
}

const ribi::cmap::ConceptMap ribi::cmap::QtRateConceptTallyDialog::CreateTestConceptMap()
{
  //Create a subconcept map for testing:
  // - node with a concept with (1) text 'TextNode' (2) one example with text 'TextExampleNode'
  // - edge with a concept with (1) text 'TextEdge' (2) one example with text 'TextExampleEdge'
  // - node with a concept with (1) text 'TextDontCare'
  ConceptMap sub_conceptmap = ribi::cmap::ConceptMapFactory().GetTest(6);
  return sub_conceptmap;
}

std::string ribi::cmap::QtRateConceptTallyDialog::GetFocusName(
  const ConceptMap& sub_conceptmap) noexcept
{
  assert(boost::num_vertices(sub_conceptmap));
  const Concept focal_concept(ribi::cmap::GetFocalNode(sub_conceptmap).GetConcept());
  return focal_concept.GetName();
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedComplexity() const
{
  //Tally the edges that contribute to complexity
  const int n_edges = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        return init + (std::get<2>(row) == -1 && std::get<1>(row).GetIsComplex() ? 1 : 0);
      }
    );

  //Tally the examples that contribute to complexity
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsComplex() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples + n_edges;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedConcreteness() const
{
  //Tally the examples that contribute to concreteness
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsConcrete() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedSpecificity() const
{
  //Tally the examples that contribute to specificity
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsSpecific() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

void ribi::cmap::QtRateConceptTallyDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
  if ( (event->modifiers() & Qt::ControlModifier)
    && (event->modifiers() & Qt::ShiftModifier)
    && event->key() == Qt::Key_T)
  {
    //Translate
    this->setWindowTitle("Relevance of illustrations");
    {
      ui->label_concept_name->setText(
        ("Illustrations and relations of the cluster: " + m_focus_name).c_str()
      );
    }
    {
      QTableWidgetItem * const item = new QTableWidgetItem;
      item->setText("Illustrations and relations of the cluster:");
      ui->table->setHorizontalHeaderItem(3,item);
    }
    {
      const int x = GetSuggestedComplexity();
      const int c = GetSuggestedConcreteness();
      const int s = GetSuggestedSpecificity();
      std::stringstream m;
      m << "Complexity: " << x << ", concreteness: " << c << ", specificity: " << s;
      ui->label_debug->setText(m.str().c_str());
    }
    return;
  }
}

void ribi::cmap::QtRateConceptTallyDialog::OnCellChanged(int row_index, int col)
{
  assert(row_index >= 0);
  assert(row_index < static_cast<int>(m_data.size()));
  assert(col >= 0);
  assert(col < 4);
  const QTableWidgetItem * const item = ui->table->item(row_index,col);
  assert(item);
  const Row& row = m_data[row_index];
  Concept concept = std::get<1>(row);
  const int index = std::get<2>(row);

  if (index == -1)
  {
    //Concept name
    switch (col)
    {
      case 0: concept.SetIsComplex( item->checkState() == Qt::Checked );
      case 1: break; //Empty cell
      case 2: break; //Empty cell
      case 3: break; //It's read-only! //concept.SetName( item->text().toStdString() ); break;
    }
  }
  else
  {
    //Concept example
    assert(index < static_cast<int>(concept.GetExamples().Get().size()));
    Example& example = concept.GetExamples().Get()[index];
    switch (col)
    {
      case 0: example.SetIsComplex( item->checkState() == Qt::Checked ); break;
      case 1: example.SetIsConcrete( item->checkState() == Qt::Checked ); break;
      case 2: example.SetIsSpecific( item->checkState() == Qt::Checked ); break;
      case 3: break; //It's read-only! //example->SetText( item->text().toStdString() ); break;
    }
  }

  {
    const int x = GetSuggestedComplexity();
    const int c = GetSuggestedConcreteness();
    const int s = GetSuggestedSpecificity();
    std::stringstream m;
    m << "Complexiteit: " << x << ", concreetheid: " << c << ", specificiteit: " << s;
    ui->label_debug->setText(m.str().c_str());
  }
}

void ribi::cmap::QtRateConceptTallyDialog::resizeEvent(QResizeEvent *)
{
  const int small_col_width = 28;
  ui->table->setColumnWidth(0, small_col_width);
  ui->table->setColumnWidth(1, small_col_width);
  ui->table->setColumnWidth(2, small_col_width);
  const int extra_space = 8;
  ui->table->setColumnWidth(3,ui->table->width() - (3 * small_col_width) - (3 * extra_space));
}

void ribi::cmap::QtRateConceptTallyDialog::on_button_ok_clicked()
{
  close();
}

#ifndef NDEBUG
void ribi::cmap::QtRateConceptTallyDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer{__func__,__FILE__,0.1};
  //Empty table
  {
    const ConceptMap conceptmap;
    QtRateConceptTallyDialog d(conceptmap);
  }

  const ConceptMap conceptmap = CreateTestConceptMap();
  


  QtRateConceptTallyDialog d(conceptmap);

  #ifndef NDEBUG
  {
    if(d.ui->table->columnCount() != 4) TRACE(d.ui->table->columnCount());
    if(d.ui->table->rowCount() != 3) TRACE(d.ui->table->rowCount());
  }
  #endif

  assert(d.ui->table->columnCount() == 4);
  assert(d.ui->table->rowCount() == 3);
  assert(boost::num_vertices(conceptmap) == 2);
  assert(boost::num_edges(conceptmap) == 1);
  const Node focal_node = GetFocalNode(conceptmap);
  //const Node other_node = conceptmap.GetNodes()[1]; //Don't care
  const Edge edge = ribi::cmap::GetFirstEdge(conceptmap);

  assert(d.ui->table->item(0,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(0,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(0,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(0,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  assert(d.ui->table->item(1,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(1,1)->flags() == Qt::ItemIsEnabled); //Empty
  assert(d.ui->table->item(1,2)->flags() == Qt::ItemIsEnabled); //Empty
  assert(d.ui->table->item(1,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  assert(d.ui->table->item(2,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(2,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(2,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  assert(d.ui->table->item(2,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  //Check current state, before modification

  assert(d.ui->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  assert(d.ui->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(1,1)->text() == "");
  assert(d.ui->table->item(1,2)->text() == "");
  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  assert(d.ui->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  assert(d.ui->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD assert(d.ui->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  assert(d.ui->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));

  //Modify table
  d.ui->table->item(0,0)->setCheckState(d.ui->table->item(0,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(0,1)->setCheckState(d.ui->table->item(0,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(0,2)->setCheckState(d.ui->table->item(0,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.ui->table->item(0,3)->setText("MODIFIED"); //User should not be able to modify this

  d.ui->table->item(1,0)->setCheckState(d.ui->table->item(1,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.ui->table->item(1,3)->setText("MODIFIED TOO"); //User should not be able to modify this

  d.ui->table->item(2,0)->setCheckState(d.ui->table->item(2,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(2,1)->setCheckState(d.ui->table->item(2,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(2,2)->setCheckState(d.ui->table->item(2,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.ui->table->item(2,3)->setText("MODIFIED AS WELL"); //User should not be able to modify this

  //Check that data is modified by GUI

  assert(d.ui->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  assert(d.ui->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(1,1)->text() == "");
  assert(d.ui->table->item(1,2)->text() == "");

  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  assert(d.ui->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  assert(d.ui->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD assert(d.ui->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  assert(d.ui->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  assert(d.ui->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));

}
#endif