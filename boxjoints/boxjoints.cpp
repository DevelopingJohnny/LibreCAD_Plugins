
#include <QDebug>

#include <QApplication>
#include <QPushButton>

#include <QFrame>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QDoubleValidator>

#include <QtMath>

#include "document_interface.h"
#include "boxjoints.h"
#include "bj_window.cpp"
#include "bj_generate.h"

// Returns Plugin-Name for LibreCAD
QString LC_BoxJoints::name() const
{
    return (tr("BoxJoints"));
}

// Setup for menu entry, should not be needed for external Plugins
PluginCapabilities LC_BoxJoints::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("plugins_menu", tr("BoxJoints"));
    return pluginCapabilities;
}

// Setup menu in Different Entry
/*QString LC_BoxJoints::menu() const {
    return ("Help");
}*/

// Gets called to execute
void LC_BoxJoints::execComm(Document_Interface *doc,
                                QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(parent);
    Q_UNUSED(cmd);

    qDebug() << "Starting BoxJoints-Plugin";

    bj_Window *bj_window = new bj_Window(this);
    bj_window->show();

    doSelection(doc, parent);


    bj_window->close();
}


void LC_BoxJoints::doSelection(Document_Interface *doc, QWidget *parent) {
    qDebug() << "doSelection - begin";

    Plug_Entity *lineA, *lineB;
    lineA = doc->getEnt(tr("Select LineA"));
    if (!lineA) return;
    lineB = doc->getEnt(tr("Select LineB"));
    if (!lineB) return;

    preSettings preSettings;// = new PreSettings;

    preSettings.lineA_startPos = LC_BoxJoints_Functions::getEntityStart(lineA);
    preSettings.lineA_endPos = LC_BoxJoints_Functions::getEntityEnd(lineA);

    preSettings.lineB_startPos = LC_BoxJoints_Functions::getEntityStart(lineB);
    preSettings.lineB_endPos = LC_BoxJoints_Functions::getEntityEnd(lineB);

    double length_first = LC_BoxJoints_Functions::getLength(&preSettings.lineA_startPos,
                                                            &preSettings.lineA_endPos);
    double length_second = LC_BoxJoints_Functions::getLength(&preSettings.lineB_startPos,
                                                             &preSettings.lineB_endPos);

    if (length_first == length_second) {
        prepareGen(doc, parent, preSettings);
        doc->updateView();
    } else {
        QString printable1 = QStringLiteral("LineA & LineB are not the same length.");
        QMessageBox::information ( parent, "LibreCAD rules", printable1);
    }

    delete lineA; //UnSelect
    delete lineB; //UnSelect
    doSelection(doc, parent);
}


void LC_BoxJoints::prepareGen(Document_Interface *doc, QWidget *parent, preSettings preSettings) {
    jointNumber ++;
    qDebug() << "prepareGen - begin" << jointNumber;

    LC_BoxJoints_GenerateSettings *lineA_genSettings = new LC_BoxJoints_GenerateSettings();
    LC_BoxJoints_GenerateSettings *lineB_genSettings = new LC_BoxJoints_GenerateSettings();

    QPointF lineA_Start = preSettings.lineA_startPos;
    QPointF lineA_End   = preSettings.lineA_endPos;

    QPointF lineB_Start = preSettings.lineB_startPos;
    QPointF lineB_End   = preSettings.lineB_endPos;

    //Calculate Middle Point of Lines
    QPointF lineA_midPoint;
    QPointF lineB_midPoint;
    lineA_midPoint = lineA_Start + lineA_End;
    lineA_midPoint /= 2;

    lineB_midPoint = lineB_Start + lineB_End;
    lineB_midPoint /= 2;

    QString printable1;
    //Are they differt Lines?
    if (lineA_midPoint == lineB_midPoint) {
        printable1 = QStringLiteral("LineA & LineB have to be different Lines.");
        QMessageBox::information ( parent, "LibreCAD rules", printable1);
        return;
    }

    //Ask for Directens - outer Sides
    QPointF pointA, pointB;
    doc->getPoint(&pointA, tr("Point Direction for LineA"));
    //if (!pointA) return;
    doc->getPoint(&pointB, tr("Point Direction for LineB"));
    //if (!pointB) return;


    // ## LineA ##

    qDebug() << "### LineA";
    lineA_genSettings->setJointStart(&lineA_Start);
    lineA_genSettings->setJointEnd(&lineA_End);

    int linesSideA = LC_BoxJoints_Functions::getLinesSide(lineA_Start, lineA_End, pointA);
    if (linesSideA == -1) { //Left
        lineA_genSettings->setAtRightSide(false);
    }
    else if (linesSideA == 1) { //Right
        lineA_genSettings->setAtRightSide(true);
    }


    // ## LineB ##

    qDebug() << "### LineB";
    lineB_genSettings->setJointStart(&lineB_Start);
    lineB_genSettings->setJointEnd(&lineB_End);

    int linesSideB = LC_BoxJoints_Functions::getLinesSide(lineB_Start, lineB_End, pointB);
    if (linesSideB == -1) { //Left
        lineB_genSettings->setAtRightSide(false);
    }
    else if (linesSideB == 1) { //Right
        lineB_genSettings->setAtRightSide(true);
    }
    if (linesSideA != 0 && linesSideB != 0) {
        lineB_genSettings->setTabsInverted(true);
    }


    qDebug() << "genJoint A";
    if (linesSideA == 0) {
        LC_BoxJoints_Generate::genSlots(this, doc, lineA_genSettings);
    } else
        LC_BoxJoints_Generate::genJoint(this, doc, lineA_genSettings);

    qDebug() << "genJoint B";
    if (linesSideB == 0) {
        LC_BoxJoints_Generate::genSlots(this, doc, lineB_genSettings);
    } else
        LC_BoxJoints_Generate::genJoint(this, doc, lineB_genSettings);

    delete(lineA_genSettings);
    delete(lineB_genSettings);
}



void LC_BoxJoints::jointInLineChanged(int value)
{
    qDebug("[Info] JOINT_IN_LINE changed");
    JOINT_IN_LINE = value;
}
void LC_BoxJoints::centerJointChanged(int value)
{
    qDebug("[Info] CENTER_JOINT changed");
    CENTER_JOINT = value;
}
void LC_BoxJoints::tabLengthChanged(double value)
{
    qDebug("[Info] TAB_LENGTH changed");
    TAB_LENGTH = value;
}
void LC_BoxJoints::matThiknessChanged(double value)
{
    qDebug("[Info] MATERIAL_THIKNESS changed");
    MATERIAL_THIKNESS = value;
}
void LC_BoxJoints::laserCompEnableChanged(int value)
{
    qDebug("[Info] LASER_COMPENSATION_ON changed");
    LASER_COMPENSATION_ON = value;
}
void LC_BoxJoints::laserCompChanged(double value)
{
    qDebug("[Info] LASER_COMPENSATION changed");
    LASER_COMPENSATION = value;
}

