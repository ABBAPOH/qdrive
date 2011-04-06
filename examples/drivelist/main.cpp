#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <QDriveInfo>

#include <QDriveController>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDriveController c;

    foreach (const QDriveInfo &drive, QDriveInfo::drives()) {
        qDebug() << "==============";
        if (drive.isValid()) {
            if (drive.isReady()) {
                qDebug() << drive.rootPath();
                if (drive.isRoot())
                    qDebug() << "   " << "isRoot:" << drive.isRoot();
                if (drive.isReadOnly())
                    qDebug() << "   " << "isReadOnly:" << drive.isReadOnly();
                qDebug() << "   " << "name:" << drive.name();
                qDebug() << "   " << "fileSystemName:" << drive.fileSystemName();
                qDebug() << "   " << "device:" << drive.device();
                qDebug() << "   " << "size:" << drive.bytesTotal()/1000/1000 << "MB";
                qDebug() << "   " << "freeSize:" << drive.bytesFree()/1000/1000 << "MB";
                qDebug() << "   " << "availableSize:" << drive.bytesAvailable()/1000/1000 << "MB";
                switch(drive.type()) {
                case QDriveInfo::InvalidDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::InvalidDrive";
                    break;
                case QDriveInfo::InternalDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::InternalDrive";
                    break;
                case QDriveInfo::RemovableDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::RemovableDrive";
                    break;
                case QDriveInfo::RemoteDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::RemoteDrive";
                    break;
                case QDriveInfo::CdromDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::CdromDrive";
                    break;
                case QDriveInfo::InternalFlashDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::InternalFlashDrive";
                    break;
                case QDriveInfo::RamDrive:
                    qDebug() << "   " << "type:" << "QDriveInfo::RamDrive";
                    break;
                default:
                    break;
                }
            } else {
                qDebug() << "    " <<  drive.rootPath() << "is not ready";
            }
        } else {
            qDebug() << "    " <<  drive.rootPath() << "is not valid";
        }
    }

    return app.exec();
}
