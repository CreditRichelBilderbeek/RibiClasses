#include "conceptmapcommandaddselectedrandom.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "trace.h"

ribi::cmap::CommandAddSelectedRandom::CommandAddSelectedRandom(const boost::shared_ptr<ConceptMap> conceptmap)
  :
    m_conceptmap{conceptmap},
    m_new_selected{conceptmap->GetSelected()},
    m_old_selected{conceptmap->GetSelected()}
{
  this->setText("add selected random");

  assert(conceptmap);
  if (conceptmap->GetNodes().empty())
  {
    throw std::logic_error("AddSelected needs nodes to focus on");
  }
  const auto to_add = m_conceptmap->GetRandomNodes(conceptmap->GetSelectedNodes());
  if (to_add.empty())
  {
    std::stringstream s;
    s << "AddSelected needs non-focused nodes to focus on,"
      << "currently has " << conceptmap->GetSelectedNodes().size()
      << " selected"
    ;
    throw std::logic_error(s.str());
  }
  std::copy(
    std::begin(to_add),
    std::end(to_add),
    std::back_inserter(m_new_selected.second)
  );
  assert(m_new_selected.second.size() > m_old_selected.second.size());
}

void ribi::cmap::CommandAddSelectedRandom::redo()
{
  m_conceptmap->SetSelected(m_new_selected);
}

void ribi::cmap::CommandAddSelectedRandom::undo()
{
  m_conceptmap->SetSelected(m_old_selected);
}
