#ifndef CONCEPTMAPEDGEISSELECTED_H
#define CONCEPTMAPEDGEISSELECTED_H

#include <boost/graph/properties.hpp>

namespace boost {
  enum edge_is_selected_t { edge_is_selected = 314159 };
  BOOST_INSTALL_PROPERTY(edge, is_selected);
}

#endif // CONCEPTMAPEDGEISSELECTED_H
