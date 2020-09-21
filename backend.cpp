#include "backend.h"

BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    connect(&mSelectedPort, &QSerialPort::readyRead,
    [&]() {
        appendReadingHistory(QString(mSelectedPort.readAll()));
    });
    connect(&mSelectedPort, &QSerialPort::errorOccurred,
    [&](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::NoError) {
            return;
        }

        mSelectedPort.clearError();
        QString message = tr("Error. Port name: %1. Error code: %2.").arg(
                              mSelectedPort.portName(), QString::number(error));
        appendReadingHistory(message);

        if (error == QSerialPort::PermissionError ||
                error == QSerialPort::DeviceNotFoundError ||
                error == QSerialPort::ResourceError) {
            mSelectedPort.setPort(QSerialPortInfo());

            if (mSelectedPort.isOpen()) {
                mSelectedPort.close();
            }

            setSelectedPortIndex(-1);
        }
    });
    setSelectedPortIndex(-1);
}

QStringList BackEnd::availablePortsNames()
{
    QStringList portNames({""});

    foreach (const QSerialPortInfo &info, mAvailablePorts) {
        portNames.append(info.portName());
    }

    return portNames;
}

int BackEnd::selectedPortIndex()
{
    return mSelectedPortIndex;
}

QString BackEnd::readingHistory()
{
    return mReadingHistory;
}

void BackEnd::setSelectedPortIndex(int index)
{
    if (index == mSelectedPortIndex) {
        return;
    }

    if (mSelectedPort.isOpen()) {
        closeSelectedPort();
    }

    if (index < 1) {
        mSelectedPort.setPort(QSerialPortInfo());
    } else {
        QSerialPortInfo info = mAvailablePorts.at(index - 1);
        mSelectedPort.setPort(info);

        if (!mSelectedPort.isOpen()) {
            if (!mSelectedPort.open(QIODevice::ReadOnly)) {
                appendReadingHistory(tr("Cannot open port. Port name: %1.")
                                     .arg(info.portName()));
                return;
            }
        }

        appendReadingHistory(tr("Opened port. Port name: %1.")
                             .arg(info.portName()));
    }

    mSelectedPortIndex = index;
    emit selectedPortIndexChanged();
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
    appendReadingHistory(tr("Closing port. Port name: %1.")
                         .arg(mSelectedPort.portName()));
    mSelectedPort.clear(QSerialPort::Input);
    mSelectedPort.close();
}
