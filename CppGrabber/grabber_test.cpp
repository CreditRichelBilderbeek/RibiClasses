#include "grabber_test.h"
#include "grabber.h"
#include <boost/test/unit_test.hpp>

void grabber_test::can_default_construct()
{
  QVERIFY(!ribi::Grabber(0).GetVersion().empty());
}
