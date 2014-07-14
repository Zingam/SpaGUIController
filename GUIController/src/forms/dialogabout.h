#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

#include "../custom/structures.h"

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(const ProgramSettings& programSettings, QWidget *parent = 0);
    ~DialogAbout();

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
