#include "backend.h"

BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    connect(&mSelectedPort, &QSerialPort::readyRead,
    [&]() {
        appendReadingHistory(QString(mSelectedPort.readAll()));
    });
    connect(&mSelectedPort, &QSerialPort::errorOccurred,
    [&](QSerialPort::SerialPortError error) {
        switch (error) {
            case QSerialPort::NoError:
                return;
                break;

            case QSerialPort::PermissionError:
            case QSerialPort::DeviceNotFoundError:
            case QSerialPort::ResourceError:
                break;

            default:
                appendReadingHistory(tr("Port error. Code: ") +
                                     QString::number(error));
                break;
        }

        mSelectedPort.clearError();
    });
}

QStringList BackEnd::availablePortsNames()
{
    QStringList portNames;

    foreach (const QSerialPortInfo &info, mAvailablePorts) {
        portNames.append(info.portName());
    }

    return portNames;
}

QString BackEnd::readingHistory()
{
    return mReadingHistory;
}

void BackEnd::selectPort(int index)
{
    closeSelectedPort();

    if (index != -1) {
        QSerialPortInfo info = mAvailablePorts.at(index);
        mSelectedPort.setPort(info);

        if (!mSelectedPort.isOpen()) {
            if (!mSelectedPort.open(QIODevice::ReadOnly)) {
                appendReadingHistory(tr("Cannot open port ") + info.portName());
                return;
            }
        }

        appendReadingHistory(tr("Opened port ") + info.portName());
    }
}

void BackEnd::appendReadingHistory(QString text)
{
    mReadingHistory.append(text + "\n");
    emit readingHistoryChanged();
}

void BackEnd::updateAvailablePorts()
{
    mAvailablePorts = QSerialPortInfo::availablePorts();
    emit availablePortsChanged();
}

void BackEnd::closeSelectedPort()
{
    if (mSelectedPort.isOpen()) {
        appendReadingHistory(tr("Closing port ") + mSelectedPort.portName());
        mSelectedPort.clear(QSerialPort::Input);
        mSelectedPort.close();
    }
}
