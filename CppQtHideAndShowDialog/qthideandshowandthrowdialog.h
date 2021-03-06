#ifndef QTHIDEANDSHOWANDTHROWDIALOG_H
#define QTHIDEANDSHOWANDTHROWDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace ribi {

///A QtHideAndShowDialog that can throw
class QtHideAndShowAndThrowDialog : public QtHideAndShowDialog
{
public:
  QtHideAndShowAndThrowDialog(QWidget* parent = 0);

public slots:
  ///Throws an exception
  void do_throw();
};

} //~namespace ribi

#endif // QTHIDEANDSHOWANDTHROWDIALOG_H
