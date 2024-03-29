/*****************************************************************************/
/*  sample.h - plugin example for LibreCAD                                   */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef DELOBJECT_H
#define DELOBJECT_H

#include "qc_plugininterface.h"
#include <QDialog>
class QLineEdit;
class Plug_Entity;

class LC_DelObject : public QObject, QC_PluginInterface
{
    Q_OBJECT
     Q_INTERFACES(QC_PluginInterface)
     Q_PLUGIN_METADATA(IID "org.librecad.delobject" FILE  "delobject.json")

 public:
    virtual PluginCapabilities getCapabilities() const;
    virtual QString name() const;
    virtual void execComm(Document_Interface *doc,
                                       QWidget *parent, QString cmd);

 private:
    bool sameObj(Plug_Entity *sample, Plug_Entity *checkEnt);

};



class lc_DelObjectdlg : public QDialog
{
    Q_OBJECT

public:
    explicit lc_DelObjectdlg(QWidget *parent = 0);
    ~lc_DelObjectdlg();

public slots:
//    void processAction(QStringList *commandList);
    void processAction(Document_Interface *doc);
    void checkAccept();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void readSettings();
    void writeSettings();
    bool failGUI(QString *msg);

private:
    QString errmsg;
    QLineEdit *startxedit;
    QLineEdit *startyedit;
    QLineEdit *endxedit;
    QLineEdit *endyedit;
};

#endif // SAMPLE_H
