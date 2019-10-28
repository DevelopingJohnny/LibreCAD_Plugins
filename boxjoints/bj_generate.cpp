
#include <QDebug>
#include <QPointF>

#include "bj_generate.h"

#include "boxjoints.h"


void LC_BoxJoints_Generate::genJoint(LC_BoxJoints *main, Document_Interface *doc,
                                     LC_BoxJoints_GenerateSettings *settings)
{
    qDebug() << "Begin genJoint in Generate";
    //Global Settings
    bool centerJoint = main->isCenterJoint();
    bool jointInLine = main->isJointInLine();
    bool laserCompensationOn = main->isLaserCompensationOn();

    double TAB_LENGTH = main->getTabLength();
    double materialThikness = main->getMaterialThikness();
    double laserCompansation = main->getLaserCompensation(); //per Tab
    double laserCompensation1_2 = laserCompansation/2; //per Cut

    //Settings
    // tabsInverted: true = Right, false = Left
    bool atRightSide = settings->isAtRightSide();
    bool tabsInverted = settings->isTabsInverted();

    QPointF *jointStart, *jointEnd;
    jointStart = settings->getJointStart();
    jointEnd = settings->getJointEnd();

    //Calculations
    double jointAngle = atan2(jointEnd->y() - jointStart->y(),
                              jointEnd->x() - jointStart->x());
    double sinAngle = sin(jointAngle);  // sin * y
    double cosAngle = cos(jointAngle);  // cos * x

    double length = LC_BoxJoints_Functions::getLength(jointStart, jointEnd);

    int num = (int) (length / TAB_LENGTH);
    if (num % 2 == 0) {num--;}
    double lengthDiff = length - (num * TAB_LENGTH);

    double lengthCorrection = 0;
    double lengthForCenter = 0;

    if (centerJoint) {
        lengthForCenter = lengthDiff / 2;
    }
    else {  //add lengthDiff
        if (lengthDiff > 0) {
            lengthCorrection = lengthDiff / num;
        }
    }

    int tabDirection = (atRightSide) ? -1 : 1; //tabsInverted

    //generating Joints
    for (int i = 0; i < num; i++) {

        bool isTab = (i % 2 == (tabsInverted ? 0:1)); // 1, 3, 5, ...
        double startX, startY, endX, endY;

        //Init Start & End
        startX = 0;
        startY = 0;
        endX = 0;
        endY = 0;

        //Init Cordinates
        startX += (TAB_LENGTH * i);
        startY += (TAB_LENGTH * i);
        endX += (TAB_LENGTH * (i+1));
        endY += (TAB_LENGTH * (i+1));

        //Center Joint
        if (centerJoint) {
            if (i == 0) {
                endX += lengthForCenter;
                endY += lengthForCenter;
            } else if (i >= num-1) {
                startX += lengthForCenter;
                startY += lengthForCenter;
                endX += lengthForCenter *2;
                endY += lengthForCenter *2;
            } else {
                startX += lengthForCenter;
                startY += lengthForCenter;
                endX += lengthForCenter;
                endY += lengthForCenter;
            }
        } else { //Length Correction
            startX += lengthCorrection * i;
            startY += lengthCorrection * i;
            endX += lengthCorrection * (i+1);
            endY += lengthCorrection * (i+1);
        }

        // LaserCompenstaion
        if (laserCompensationOn) {
            int laserCompensationDir;
            if (isTab) {
                laserCompensationDir = 1;
            } else {
                laserCompensationDir = -1;
            }

            //First
            if (i == 0) {
                endX += laserCompensation1_2 *laserCompensationDir;
                endY += laserCompensation1_2 *laserCompensationDir;
            } else if (i >= num -1) { //Last one - be careful
                startX += laserCompensation1_2 *-laserCompensationDir;
                startY += laserCompensation1_2 *-laserCompensationDir;
            } else {
                startX += laserCompensation1_2 *-laserCompensationDir;
                startY += laserCompensation1_2 *-laserCompensationDir;
                endX += laserCompensation1_2 *laserCompensationDir;
                endY += laserCompensation1_2 *laserCompensationDir;
            }
        }

        // Calculate Final Position
        startX *= cosAngle;
        startY *= sinAngle;
        endX *= cosAngle;
        endY *= sinAngle;

        //Joint In Line
        if (jointInLine) {
            int offsetDir = (atRightSide ? 1:-1); //Right = UP, Left = DOWN
            startX += materialThikness * cos(jointAngle + (M_PI_2 *offsetDir));
            startY += materialThikness * sin(jointAngle + (M_PI_2 *offsetDir));
            endX += materialThikness * cos(jointAngle + (M_PI_2 *offsetDir));
            endY += materialThikness * sin(jointAngle + (M_PI_2 *offsetDir));
        }
        // Offset - Move Down
        /*if (jointInLine && !tabsInverted) {
            startX += materialThikness * cos(jointAngle + (-M_PI_2));
            startY += materialThikness * sin(jointAngle + (-M_PI_2));
            endX += materialThikness * cos(jointAngle + (-M_PI_2));
            endY += materialThikness * sin(jointAngle + (-M_PI_2));
        } // Offset - Move Up
        else if (jointInLine && tabsInverted) {
            startX += materialThikness * cos(jointAngle + (M_PI_2));
            startY += materialThikness * sin(jointAngle + (M_PI_2));
            endX += materialThikness * cos(jointAngle + (M_PI_2));
            endY += materialThikness * sin(jointAngle + (M_PI_2));
        }*/

        // Tab - Move to Material Thinkness
        if (isTab) {
            startX += materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
            startY += materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
            endX += materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
            endY += materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
        }

        QPointF start(jointStart->x() + startX, jointStart->y() + startY),
                end(jointStart->x() + endX, jointStart->y() + endY);
        doc->addLine(&start, &end);

        // ++ Vertical Line ++
        if (i == 0 && isTab && !jointInLine) {
            double startX_v, startY_v, endX_v, endY_v;
            startX_v = startX;
            startY_v = startY;
            endX_v = startX;
            endY_v = startY;

            if (isTab) {
                endX_v -= materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
                endY_v -= materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
            } else {
                endX_v += materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
                endY_v += materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
            }

            QPointF start_vert(jointStart->x() + startX_v, jointStart->y() + startY_v),
                    end_vert(jointStart->x() + endX_v, jointStart->y() + endY_v);
            doc->addLine(&start_vert, &end_vert);
        }

        if ((i < num -1) || (i == num -1 && isTab && !jointInLine)) {
            double startX_v, startY_v, endX_v, endY_v;
            startX_v = endX;
            startY_v = endY;
            endX_v = endX;
            endY_v = endY;

            if (isTab) {
                endX_v -= materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
                endY_v -= materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
            } else {
                endX_v += materialThikness * cos(jointAngle + (M_PI_2 *tabDirection));
                endY_v += materialThikness * sin(jointAngle + (M_PI_2 *tabDirection));
            }

            QPointF start_vert(jointStart->x() + startX_v, jointStart->y() + startY_v),
                    end_vert(jointStart->x() + endX_v, jointStart->y() + endY_v);
            doc->addLine(&start_vert, &end_vert);
        }


    }
    qDebug() << "Joint generated.";
}


void LC_BoxJoints_Generate::genSlots(LC_BoxJoints *main, Document_Interface *doc,
                                     LC_BoxJoints_GenerateSettings *settings)
{
    qDebug() << "Begin genSlots in Generate";
    //Global Settings
    bool centerSlots = main->isCenterJoint();
    bool laserCompensationOn = main->isLaserCompensationOn();

    double TAB_LENGTH = main->getTabLength();
    double materialThikness = main->getMaterialThikness();
    double materialThikness_2 = materialThikness/2;
    double laserCompansation = main->getLaserCompensation(); //per Tab
    double laserCompensation1_2 = laserCompansation/2; //per Cut

    //Settings
    //bool atRightSide = settings->isAtRightSide();
    bool tabsInverted = settings->isTabsInverted();

    QPointF *jointStart, *jointEnd;
    jointStart = settings->getJointStart();
    jointEnd = settings->getJointEnd();

    //Calculations
    double jointAngle = atan2(jointEnd->y() - jointStart->y(),
                              jointEnd->x() - jointStart->x());
    double sinAngle = sin(jointAngle);  // sin * y
    double cosAngle = cos(jointAngle);  // cos * x

    double length = LC_BoxJoints_Functions::getLength(jointStart, jointEnd);

    int num = (int) (length / TAB_LENGTH);
    if (num % 2 == 0) {num--;}
    double lengthDiff = length - (num * TAB_LENGTH);

    double lengthCorrection = 0;
    double lengthForCenter = 0;

    if (centerSlots) {
        lengthForCenter = lengthDiff / 2;
    }
    else {  //add lengthDiff
        if (lengthDiff > 0) {
            lengthCorrection = lengthDiff / num;
        }
    }

    //generating Slots
    for (int i = 0; i < num; i++) {

        bool isTab = (i % 2 == (tabsInverted ? 0:1)); // 1, 3, 5, ...

        if (!isTab) {
            continue; // move to next Int
        }

        double startX, startY, endX, endY;

        //Init Start & End
        startX = 0;
        startY = 0;
        endX = 0;
        endY = 0;

        //Init Cordinates
        startX += (TAB_LENGTH * i);
        startY += (TAB_LENGTH * i);
        endX += (TAB_LENGTH * (i+1));
        endY += (TAB_LENGTH * (i+1));

        //Center Slots
        if (centerSlots) {
            if (i == 0) {
                endX += lengthForCenter;
                endY += lengthForCenter;
            } else if (i >= num-1) {
                startX += lengthForCenter;
                startY += lengthForCenter;
                endX += lengthForCenter *2;
                endY += lengthForCenter *2;
            } else {
                startX += lengthForCenter;
                startY += lengthForCenter;
                endX += lengthForCenter;
                endY += lengthForCenter;
            }
        } else { //Length Correction
            startX += lengthCorrection * i;
            startY += lengthCorrection * i;
            endX += lengthCorrection * (i+1);
            endY += lengthCorrection * (i+1);
        }

        // LaserCompenstaion
        if (laserCompensationOn) {
            int laserCompensationDir;
            if (isTab) {
                laserCompensationDir = -1; //1
            } else {
                laserCompensationDir = 1; //-1
            }

            //First
            if (i == 0) {
                endX += laserCompensation1_2 *laserCompensationDir;
                endY += laserCompensation1_2 *laserCompensationDir;
            } else if (i >= num -1) { //Last one - be careful
                startX += laserCompensation1_2 *-laserCompensationDir;
                startY += laserCompensation1_2 *-laserCompensationDir;
            } else {
                startX += laserCompensation1_2 *-laserCompensationDir;
                startY += laserCompensation1_2 *-laserCompensationDir;
                endX += laserCompensation1_2 *laserCompensationDir;
                endY += laserCompensation1_2 *laserCompensationDir;
            }
        }

        // Calculate Final Position
        startX *= cosAngle; //  #Corn1----------------Corn2#
        startY *= sinAngle; //  |                          |
        endX *= cosAngle;   //  *Start - - - - - - - - -End*
        endY *= sinAngle;   //  |                          |
                            //  #Corn4----------------Corn3#

        double corn1X, corn1Y;
        double corn2X, corn2Y;
        double corn3X, corn3Y;
        double corn4X, corn4Y;

        corn1X = startX + materialThikness_2 * cos(jointAngle + (M_PI_2));
        corn1Y = startY + materialThikness_2 * sin(jointAngle + (M_PI_2));

        corn2X = endX + materialThikness_2 * cos(jointAngle + (M_PI_2));
        corn2Y = endY + materialThikness_2 * sin(jointAngle + (M_PI_2));

        corn3X = endX + materialThikness_2 * cos(jointAngle + (-M_PI_2));
        corn3Y = endY + materialThikness_2 * sin(jointAngle + (-M_PI_2));

        corn4X = startX + materialThikness_2 * cos(jointAngle + (-M_PI_2));
        corn4Y = startY + materialThikness_2 * sin(jointAngle + (-M_PI_2));

        if (isTab) {
            QPointF corn1(jointStart->x() + corn1X, jointStart->y() + corn1Y),
                    corn2(jointStart->x() + corn2X, jointStart->y() + corn2Y),
                    corn3(jointStart->x() + corn3X, jointStart->y() + corn3Y),
                    corn4(jointStart->x() + corn4X, jointStart->y() + corn4Y);

/*            doc->addLine(&corn1, &corn2);
            doc->addLine(&corn2, &corn3);
            doc->addLine(&corn3, &corn4);
            doc->addLine(&corn4, &corn1);*/

/*            std::vector<QPointF> points = {corn1, corn2, corn3, corn4};
            doc->addLines(points, true);*/

            std::vector<Plug_VertexData> points = {Plug_VertexData(corn1, 0),
                                                   Plug_VertexData(corn2, 0),
                                                   Plug_VertexData(corn3, 0),
                                                   Plug_VertexData(corn4, 0)};

            doc->addPolyline(points, true);
        }
    }
    qDebug() << "Slots - generated.";
}
