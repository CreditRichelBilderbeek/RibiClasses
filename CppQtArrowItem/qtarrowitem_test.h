#ifndef QTARROWITEM_TEST_H
#define QTARROWITEM_TEST_H

#include <QtTest/QtTest>
namespace ribi {

class qtarrowitem_test : public QObject
{
    Q_OBJECT

private slots:
  void can_default_construct() const noexcept;
};

} //~namespace ribi

#endif // QTARROWITEM_TEST_H
