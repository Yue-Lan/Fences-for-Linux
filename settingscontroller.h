#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>

class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(QObject *parent = nullptr);

signals:

public slots:
    void resetAll();
};

#endif // SETTINGSCONTROLLER_H
