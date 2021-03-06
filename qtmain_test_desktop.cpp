#include <iostream>
#include <QtTest/QtTest>
#include "grabber_test.h"
#include "qtarrowitem_test.h"
#include "qtdisplaypositem_test.h"
#include "qthideandshowdialog_test.h"
#include "qtscopeddisable_test.h"
#include "qtgraphics_test.h"
#include "qtimage_test.h"
#include "qtquadbezierarrowdialog_test.h"
#include "qtquadbezierarrowitem_test.h"
#include "qtroundededitrectitem_test.h"
#include "qtroundedrectitem_test.h"
#include "qtroundedrectitemdialog_test.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  //I am most interested in:
  //{ ribi::cmap::qtconceptmap_test t; error |= QTest::qExec(&t, argc, argv); }
  //return 0;

  //These are all tests in alphabetical order
  { ribi::grabber_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtarrowitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtdisplaypositem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtgraphics_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtimage_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtquadbezierarrowdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtquadbezierarrowitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtroundededitrectitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtroundedrectitemdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtroundedrectitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtscopeddisable_test t; error |= QTest::qExec(&t, argc, argv); }


  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
