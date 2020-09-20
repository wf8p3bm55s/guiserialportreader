#ifndef BACKEND_H
#define BACKEND_H

#include <qqml.h>
#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availablePortsNames READ availablePortsNames NOTIFY availablePortsChanged)
    Q_PROPERTY(QString readingHistory READ readingHistory NOTIFY readingHistoryChanged)
    QML_ELEMENT
public:
    explicit BackEnd(QObject *parent = nullptr);
    QStringList availablePortsNames();

    QString readingHistory();
    void appendReadingHistory(QString text);

    Q_INVOKABLE void selectPort(int index);
    Q_INVOKABLE void updateAvailablePorts();
signals:
    void availablePortsChanged();
    void readingHistoryChanged();
private:
    void closeSelectedPort();

    QList<QSerialPortInfo> mAvailablePorts;
    QSerialPort mSelectedPort;
    QString mReadingHistory;
};

#endif // BACKEND_H
