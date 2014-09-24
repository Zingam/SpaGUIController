#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QtWidgets/QDialog>
#include <QtGui/QPixmap>

#include "../custom/structures.h"


namespace Ui {
class DialogAbout;
}


class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(const ProgramSettings& programSettings,
                         QPixmap& pixmapLogo,
                         QWidget* parent = 0);
    ~DialogAbout();

private:
    void resizePixmapLogo(QPixmap& pixmap);

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
