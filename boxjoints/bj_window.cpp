
#include "bj_window.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFrame>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QCheckBox>

#include <QSettings>
#include <QCloseEvent>

bj_Window::bj_Window(LC_BoxJoints *boxjoints, QWidget *parent) : QWidget(parent)
{
    boxJoints = boxjoints;
    setWindowTitle(tr("BoxJoints - Settings"));
    //setFixedSize(180, 240);
    setWindowOpacity(0.8);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    //First Group - Joint
    QGroupBox *groupBox_joint = new QGroupBox(tr("Joint"));
    QVBoxLayout *jointLayout = new QVBoxLayout;


    QFrame *inLineFrame = new QFrame;
    QLabel *label_jointAtInLine = new QLabel(tr("Create Joint in Line:"));

    checkBox_jointAtInLine = new QCheckBox();
    QHBoxLayout *inLineLayout = new QHBoxLayout;
    inLineLayout->addWidget(label_jointAtInLine);
    inLineLayout->addWidget(checkBox_jointAtInLine);
    inLineFrame->setLayout(inLineLayout);

    QFrame *typeFrame = new QFrame;
    QLabel *label_fixedTabLength = new QLabel(tr("Fixed Tab Length:"));

    checkBox_centerJoint = new QCheckBox();
    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(label_fixedTabLength);
    typeLayout->addWidget(checkBox_centerJoint);
    typeFrame->setLayout(typeLayout);

    QFrame *tabFrame = new QFrame;
    QLabel *label_tabLength = new QLabel(tr("Tab Length:"));

    doubleSpinBox_tabLength = new QDoubleSpinBox();
    doubleSpinBox_tabLength->setDecimals(2);
    doubleSpinBox_tabLength->setSingleStep(0.50);
    QHBoxLayout *tabLayout = new QHBoxLayout;
    tabLayout->addWidget(label_tabLength);
    tabLayout->addWidget(doubleSpinBox_tabLength);
    tabFrame->setLayout(tabLayout);

    QFrame *matFrame = new QFrame;
    QLabel *label_materialThikness = new QLabel(tr("Material Thikness:"));

    doubleSpinBox_matThikness = new QDoubleSpinBox();
    doubleSpinBox_matThikness->setDecimals(2);
    doubleSpinBox_matThikness->setSingleStep(0.50);
    QHBoxLayout *matLayout = new QHBoxLayout;
    matLayout->addWidget(label_materialThikness);
    matLayout->addWidget(doubleSpinBox_matThikness);
    matFrame->setLayout(matLayout);

    jointLayout->addWidget(inLineFrame);
    jointLayout->addWidget(typeFrame);
    jointLayout->addWidget(tabFrame);
    jointLayout->addWidget(matFrame);
    groupBox_joint->setLayout(jointLayout);
    mainLayout->addWidget(groupBox_joint);


    //Secound Group - LaserCompensation
    QGroupBox *groupBox_laser = new QGroupBox(tr("Laser Compensation"));
    QVBoxLayout *laserLayout = new QVBoxLayout;

    QFrame *enableFrame = new QFrame;
    QLabel *label_enable = new QLabel(tr("Enable:"));

    checkBox_laserCompEnable = new QCheckBox();
    QHBoxLayout *enableLayout = new QHBoxLayout;
    enableLayout->addWidget(label_enable);
    enableLayout->addWidget(checkBox_laserCompEnable);
    enableFrame->setLayout(enableLayout);

    QFrame *compFrame = new QFrame;
    QLabel *label_laserCompensation = new QLabel(tr("Laser Compensation:"));

    doubleSpinBox_laserCompensation = new QDoubleSpinBox();
    doubleSpinBox_laserCompensation->setDecimals(2);
    doubleSpinBox_laserCompensation->setSingleStep(0.05);
    QHBoxLayout *compLayout = new QHBoxLayout;
    compLayout->addWidget(label_laserCompensation);
    compLayout->addWidget(doubleSpinBox_laserCompensation);
    compFrame->setLayout(compLayout);

    laserLayout->addWidget(enableFrame);
    laserLayout->addWidget(compFrame);
    groupBox_laser->setLayout(laserLayout);
    mainLayout->addWidget(groupBox_laser);


    connect(checkBox_jointAtInLine, SIGNAL(stateChanged(int)), boxJoints, SLOT(jointInLineChanged(int)));
    connect(checkBox_centerJoint, SIGNAL(stateChanged(int)), boxJoints, SLOT(centerJointChanged(int)));
    connect(doubleSpinBox_tabLength, SIGNAL(valueChanged(double)), boxJoints, SLOT(tabLengthChanged(double)));
    connect(doubleSpinBox_matThikness, SIGNAL(valueChanged(double)), boxJoints, SLOT(matThiknessChanged(double)));
    connect(checkBox_laserCompEnable, SIGNAL(stateChanged(int)), boxJoints, SLOT(laserCompEnableChanged(int)));
    connect(doubleSpinBox_laserCompensation, SIGNAL(valueChanged(double)), boxJoints, SLOT(laserCompChanged(double)));


    //connect(boxjoints, SIGNAL(destroyed()), this->parentWidget(), SLOT(closethis->parentWidget));

    readSettings();
}


void bj_Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

void bj_Window::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void bj_Window::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "boxjoints");
    bool fixedTabLength = settings.value("fixedTabLength", false).toBool();
    double tabLength = settings.value("tabLength", 6.00).toDouble();
    double matThikness = settings.value("matThikness", 4.00).toDouble();
    bool laserCompEnable = settings.value("laserCompEnable").toBool();
    double laserComp = settings.value("laserCompensation", 0.25).toDouble();

    checkBox_centerJoint->setChecked(fixedTabLength);
    doubleSpinBox_tabLength->setValue(tabLength);
    doubleSpinBox_matThikness->setValue(matThikness);
    checkBox_laserCompEnable->setChecked(laserCompEnable);
    doubleSpinBox_laserCompensation->setValue(laserComp);
}

void bj_Window::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "boxjoints");
    settings.setValue("fixedTabLength", checkBox_centerJoint->isChecked());
    settings.setValue("tabLength", doubleSpinBox_tabLength->value());
    settings.setValue("matThikness", doubleSpinBox_matThikness->value());
    settings.setValue("laserCompEnable", checkBox_laserCompEnable->isChecked());
    settings.setValue("laserCompensation", doubleSpinBox_laserCompensation->value());
}
