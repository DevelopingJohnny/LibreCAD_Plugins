
#ifndef BOXJOINTS_H
#define BOXJOINTS_H

#include "qc_plugininterface.h"
#include <QDialog>
#include <QDebug>
#include <QtMath>
#include "document_interface.h"


class QLineEdit;
class LC_BoxJoints_Functions;
class LC_BoxJoints_genSettings;
struct genSettings;
struct preSettings;


//Plugin Class
class LC_BoxJoints : public QObject, QC_PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(QC_PluginInterface)
    Q_PLUGIN_METADATA(IID "org.librecad.boxjoints" FILE "boxjoints.json")

public:
    virtual PluginCapabilities getCapabilities() const;
    virtual QString name() const;
    virtual void execComm(Document_Interface *doc,
                                       QWidget *parent, QString cmd);

    bool isJointInLine() {
        return JOINT_IN_LINE;
    }
    bool isCenterJoint() {
        return CENTER_JOINT;
    }
    bool isOffsetJoint() {
        return OFFSET_JOINT;
    }
    bool isLaserCompensationOn() {
        return LASER_COMPENSATION_ON;
    }
    double getTabLength() {
        return TAB_LENGTH;
    }
    double getMaterialThikness() {
        return MATERIAL_THIKNESS;
    }
    double getLaserCompensation() {
        return LASER_COMPENSATION;
    }

private:
    void doSelection(Document_Interface *doc, QWidget *parent);
    void prepareGen(Document_Interface *doc, QWidget *parent, preSettings settings);

public slots:
    void jointInLineChanged(int value);
    void centerJointChanged(int value);
    void tabLengthChanged(double value);
    void matThiknessChanged(double value);
    void laserCompEnableChanged(int value);
    void laserCompChanged(double value);

private:
    bool JOINT_IN_LINE = false; // Wichtig wenn mit Außenmaß gezeichnet wird.
    bool CENTER_JOINT = false;
    bool OFFSET_JOINT = false;
    bool LASER_COMPENSATION_ON = false;

    int jointNumber = 0;
    double TAB_LENGTH = 6;
    //double TAB_LENGTH_TOLERANCE = 0.25; //0.25 = 25% not in use

    double MATERIAL_THIKNESS = 4;
    double LASER_COMPENSATION = 0.25; //Tab Differance
    // flaot spaceLength; einfachs hablber weggelassen

protected:
    bool REMOVE_LINE = false;
    QVariant LINE_LAYER = 0;
    QVariant LINE_COLOR = 0;
    QVariant LINE_TYPE = 0;
    QVariant LINE_WIDTH = 0;

    bool DO_JOINT_LAYER = false;
    QVariant JOINT_LAYER = 0;
    QVariant JOINT_COLOR = 0;
    QVariant JOINT_TYPE = 0;
    QVariant JOINT_WIDTH = 0;
};


class PreSettings {

public:

    QPointF * getLineA_Start() {return lineA_start;}
    QPointF * getLineA_End() {return lineA_end;}
    QPointF * getLineB_Start() {return lineB_start;}
    QPointF * getLineB_End() {return lineB_end;}

    void setLineA_Start(double x, double y) {lineA_start->setX(x); lineA_start->setY(y);}
    void setLineA_End(double x, double y)   {lineA_end->setX(x);   lineA_end->setY(y);}
    void setLineB_Start(double x, double y) {lineB_start->setX(x); lineB_start->setY(y);}
    void setLineB_End(double x, double y)   {lineB_end->setX(x);   lineB_end->setY(y);}

private:

    QPointF *lineA_start = new QPointF(0,0);
    QPointF *lineA_end = new QPointF(0,0);
    QPointF *lineB_start = new QPointF(0,0);
    QPointF *lineB_end = new QPointF(0,0);

};

struct preSettings {
    QPointF lineA_startPos;
    QPointF lineA_endPos;
    QPointF lineB_startPos;
    QPointF lineB_endPos;
};


class LC_BoxJoints_Functions {

public:
    // int -1 = Left, 1 = Right, 0 = OnLine
    static int getLinesSide(QPointF start, QPointF end, QPointF point) {
        QPointF midPoint = (start + end) / 2;

        double lineAngle = atan2(end.y() - start.y(),
                                 end.x() - start.x());
        double pointAngle = atan2(point.y() - midPoint.y(),
                                  point.x() - midPoint.x());
        double pointAngleToStart = atan2(point.y() - start.y(),
                                         point.x() - start.x());

        // Angle Correction, negativ to positiv
        if (lineAngle < 0) {
            lineAngle += 2*M_PI;
        }
        if (pointAngle < 0) {
            pointAngle += 2*M_PI;
        }
        if (pointAngleToStart < 0) {
            pointAngleToStart += 2*M_PI;
        }

        //Angles Offset = LineAngle
        //       Target = PointAngle
        double offset = qRadiansToDegrees(lineAngle);
        double targetAngleFromMidP = qRadiansToDegrees(pointAngle);
        double targetAngleFromStart = qRadiansToDegrees(pointAngleToStart);

        qDebug() << "## - getLinesSide";
        qDebug() << "## LineAngle" << lineAngle << " / " << offset;
        qDebug() << "## PointAngle" << pointAngle << " / " << targetAngleFromMidP;

        qDebug() << "## "<< targetAngleFromMidP <<" > "<< 0+offset <<" & <= " << 180+offset;
        if (targetAngleFromStart == offset ||
                targetAngleFromMidP +180 == offset ||
                targetAngleFromMidP == offset +180) {
            qDebug() << "## else if - target == offset";
            return 0;
        }
        else if (targetAngleFromMidP > 0 +offset && targetAngleFromMidP <= 180 +offset) {
            qDebug() << "## if true @ left Side";
            return -1;
        }
        else if (180 +offset > 360 && targetAngleFromMidP <= (180 +offset) -360) {
            qDebug() << "## else if true @ left Side";
            return -1;
        }

        qDebug() << "## if false @ right Side";
        return 1; //Right Side of Line
    }

    static QPointF getEntityStart(Plug_Entity *entity) {
        QHash<int, QVariant> data;
        entity->getData(&data);
        QPointF vector = QPointF(data.value(DPI::STARTX).toDouble(),
                                 data.value(DPI::STARTY).toDouble());
        data.clear();
        return vector;
    }
    static QPointF getEntityEnd(Plug_Entity *entity) {
        QHash<int, QVariant> data;
        entity->getData(&data);
        QPointF vector = QPointF(data.value(DPI::ENDX).toDouble(),
                                 data.value(DPI::ENDY).toDouble());
        data.clear();
        return vector;
    }

    static void changeToLayerData(QHash<int, QVariant> newData, Plug_Entity *ent) {
        QHash<int, QVariant> data;
        ent->getData(&data);
        data.insert(DPI::LAYER, newData.value(DPI::LAYER));
        data.insert(DPI::COLOR, newData.value(DPI::COLOR));
        data.insert(DPI::LTYPE, newData.value(DPI::LTYPE));
        data.insert(DPI::LWIDTH, newData.value(DPI::LWIDTH));
        ent->updateData(&data);
    }

    static double getLength(QPointF *pos1, QPointF *pos2) {
        qreal min_ank = getMin(pos1->x(), pos2->x());
        qreal max_ank = getMax(pos1->x(), pos2->x());
        double length_ank = min_ank - max_ank;

        qreal min_geg = getMin(pos1->y(), pos2->y());
        qreal max_geg = getMax(pos1->y(), pos2->y());
        double length_geg = min_geg - max_geg;

        return sqrt(length_ank*length_ank + length_geg*length_geg);
    }
    static bool isBiggerThan(QPointF pos1, QPointF pos2) {
        if (pos1.x() > pos2.x()) {
            if (pos1.y() > pos2.y()) {
                return true;
            }
        }
        return false;
    }
    static QPointF * getMin(QPointF *pos1, QPointF *pos2) {
        if (pos1->x() < pos2->x()) {
            return pos1;
        }
        else if (pos1->y() < pos2->y()) {
            return pos1;
        }
        return pos2;
    }
    static QPointF * getMax(QPointF *pos1, QPointF *pos2) {
        if (pos1->x() > pos2->x()) {
            return pos1;
        }
        else if (pos1->y() > pos2->y()) {
            return pos1;
        }
        return pos2;
    }
    static qreal getMin(qreal a, qreal b) {
        if (a >= b) {
            return b;
        } else {
            return a;
        }
    }
    static qreal getMax(qreal a, qreal b) {
        if (a >= b) {
            return a;
        } else {
            return b;
        }
    }
};


#endif
