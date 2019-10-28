#ifndef BJ_GENERATE_H
#define BJ_GENERATE_H


#include "document_interface.h"
#include "boxjoints.h"

class LC_BoxJoints_GenerateSettings;


class LC_BoxJoints_Generate {

public:
    static void genJoint(LC_BoxJoints *main, Document_Interface *doc,
                         LC_BoxJoints_GenerateSettings *settings);
    static void genSlots(LC_BoxJoints *main, Document_Interface *doc,
                         LC_BoxJoints_GenerateSettings *settings);

    bool isCenterJoint() {return CENTER_JOINT;}
    bool isOffsetJoint() {return OFFSET_JOINT;}
    bool isLaserCompensationOn() {return LASER_COMPENSATION_ON;}
    double getTabLength() {return TAB_LENGTH;}
    double getMaterialThikness() {return MATERIAL_THIKNESS;}
    double getLaserCompensation() {return LASER_COMPENSATION;}

public slots:
    void centerJointChanged(int value);
    void tabLengthChanged(double value);
    void matThiknessChanged(double value);
    void laserCompEnableChanged(int value);
    void laserCompChanged(double value);

private:
    bool CENTER_JOINT = false;
    bool OFFSET_JOINT = false;
    bool LASER_COMPENSATION_ON = false;

    double TAB_LENGTH = 6;
    //double TAB_LENGTH_TOLERANCE = 0.25; //0.25 = 25% not in use

    double MATERIAL_THIKNESS = 4;
    double LASER_COMPENSATION = 0.25; //Tab Differance


};


class LC_BoxJoints_GenerateSettings {


public:
    bool isAtRightSide() {
        return atRightSide;
    }
    bool isTabsInverted() {
        return tabsInverted;
    }

    QPointF * getJointStart() {
        return jointStart;
    }
    QPointF * getJointEnd() {
        return jointEnd;
    }

    void setAtRightSide(bool value) {
        atRightSide = value;
    }
    void setTabsInverted(bool value) {
        tabsInverted = value;
    }

    void setJointStart(QPointF *vector) {
        jointStart = vector;
    }
    void setJointEnd(QPointF *vector) {
        jointEnd = vector;
    }

private:
    bool atRightSide = false; //false = Left
    bool tabsInverted = false;

    QPointF *jointStart;
    QPointF *jointEnd;
};

#endif // BJ_GENERATE_H
