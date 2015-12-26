#ifndef CONCEPTMAPVERTEXCUSTOMTYPE_H
#define CONCEPTMAPVERTEXCUSTOMTYPE_H

#include <boost/graph/properties.hpp>

namespace boost {
  enum vertex_custom_type_t { vertex_custom_type = 314 };
  BOOST_INSTALL_PROPERTY(vertex,custom_type);
}

#endif // CONCEPTMAPVERTEXCUSTOMTYPE_H
