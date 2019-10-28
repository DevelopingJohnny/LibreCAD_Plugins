#ifndef BJ_WINDOW_H
#define BJ_WINDOW_H

#include "boxjoints.h"
#include <QWidget>

class QDoubleSpinBox;
class QRadioButton;
class QCheckBox;

class bj_Window : public QWidget
{
    Q_OBJECT
public:
    explicit bj_Window(LC_BoxJoints *boxjoints, QWidget *parent = 0);
    void readSettings();

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    LC_BoxJoints *boxJoints;
    QCheckBox *checkBox_jointAtInLine;
    QCheckBox *checkBox_centerJoint;
    QDoubleSpinBox *doubleSpinBox_tabLength;
    QDoubleSpinBox *doubleSpinBox_matThikness;
    QCheckBox *checkBox_laserCompEnable;
    QDoubleSpinBox *doubleSpinBox_laserCompensation;

    void writeSettings();

};

#endif // BJ_WINDOW_H
