#include "settingscontroller.h"
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

void cleanSettings() {
    QSettings cleanHelper;
    cleanHelper.beginGroup("iconview/id_count");
    cleanHelper.clear();
    cleanHelper.endGroup();

    cleanHelper.beginGroup("iconview/id_title_table");
    cleanHelper.clear();
    cleanHelper.endGroup();

    cleanHelper.beginGroup("iconview/id_state_table");
    cleanHelper.clear();
    cleanHelper.endGroup();
}

void removeDirs() {
    QDir dirHelper;
    /* NOTE:!!!!!!!
     * be careful to remove a unsure dir.
     */
    bool isDir = dirHelper.setCurrent(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.iconviews_on_desktop_test");
    if (isDir) {
        qDebug()<<dirHelper.removeRecursively()<<"  delete done";
    }
}

SettingsController::SettingsController(QObject *parent) : QObject(parent)
{

}

void SettingsController::resetAll() {
    cleanSettings();
    removeDirs();
    exit(0);
}
