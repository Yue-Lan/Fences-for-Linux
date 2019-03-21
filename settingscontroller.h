#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>

class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(QObject *parent = nullptr);

Q_SIGNALS:

public Q_SLOTS:
    void resetAll();
};

#endif // SETTINGSCONTROLLER_H
