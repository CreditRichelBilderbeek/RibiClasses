#include "qtconceptmapqtnodedialog.h"
#include <boost/test/unit_test.hpp>

#include <sstream>

#include <boost/lambda/lambda.hpp>

#include <QDesktopWidget>
#include <QVBoxLayout>

#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapnodedialog.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtconceptmapqtnode.h"
#include "qtitemdisplaystrategy.h"
#include "qtroundededitrectitemdialog.h"

#include "testtimer.h"
#include "trace.h"

#include "ui_qtconceptmapqtnodedialog.h"
//#include "ui_qtconceptmapnodedialog.h"
//#include "ui_qtroundededitrectitemdialog.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_qtqtnodedialog_test)
{
  const bool verbose{false};
  QtQtNodeDialog dialog;
  const auto node = NodeFactory().GetTest(1);
  const boost::shared_ptr<QtNode> qtnode{new QtNode(node)};
  dialog.SetQtNode(qtnode);
  if (verbose) { TRACE("SetUiX and GetUiX must be symmetric"); }
  {
    const double old_x{dialog.GetUiX()};
    const double new_x{old_x + 10.0};
    dialog.SetUiX(new_x);
    BOOST_CHECK(std::abs(dialog.GetUiX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("SetUiY and GetUiY must be symmetric"); }
  {
    const double old_y{dialog.GetUiY()};
    const double new_y{old_y + 10.0};
    dialog.SetUiY(new_y);
    BOOST_CHECK(std::abs(dialog.GetUiY() - new_y) < 2.0);
  }
}
