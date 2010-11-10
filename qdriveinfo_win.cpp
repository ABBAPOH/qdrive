#include "qdriveinfo.h"
#include "qdriveinfo_p.h"

#include <qplatformdefs.h>

QList<QDriveInfo> QDriveInfoPrivate::drives()
{
    QList<QDriveInfo> drives;

    char driveName[] = "A:/";
    quint32 driveBits = (quint32)GetLogicalDrives() & 0x3ffffff;
    while (driveBits) {
        if (driveBits & 1)
            drives.append(QDriveInfo(QLatin1String(driveName)));
        driveName[0]++;
        driveBits = driveBits >> 1;
    }

    return drives;
}

void QDriveInfoPrivate::stat(uint requiredFlags)
{
    if (data->getCachedFlag(requiredFlags))
        return;

    uint bitmask = 0;
    bitmask = CachedValidFlag | CachedReadyFlag |
              CachedNameFlag | CachedFileSystemNameFlag;
    if (requiredFlags & bitmask) {
        getVolumeInformation();
        data->setCachedFlag(bitmask);
    }

    bitmask = CachedAvailableSizeFlag | CachedFreeSizeFlag | CachedSizeFlag;
    if (requiredFlags & bitmask) {
        getDiskFreeSpace();
        data->setCachedFlag(bitmask);
    }

    bitmask = CachedDeviceFlag;
    if (requiredFlags & bitmask) {
        getDevice();
        data->setCachedFlag(bitmask);
    }

    bitmask = CachedTypeFlag;
    if (requiredFlags & bitmask) {
        getType();
        data->setCachedFlag(bitmask);
    }
}

void QDriveInfoPrivate::getVolumeInformation()
{
    wchar_t nameBuf[MAX_PATH];
    wchar_t fileSystemNameBuf[MAX_PATH];
    bool result = GetVolumeInformation((wchar_t *)data->rootPath.utf16(),
                                       nameBuf, MAX_PATH,
                                       0, 0, 0,
                                       fileSystemNameBuf, MAX_PATH);
    if (!result) {
        data->ready = false;
        data->valid = (GetLastError() == ERROR_NOT_READY);
    } else {
        data->ready = true;
        data->valid = true;

        data->name = QString::fromWCharArray(nameBuf);
        data->fileSystemName = QString::fromWCharArray(fileSystemNameBuf);
    }
}

void QDriveInfoPrivate::getDiskFreeSpace()
{
    bool result = GetDiskFreeSpaceEx((wchar_t *)data->rootPath.utf16(),
                                     (PULARGE_INTEGER)&data->availableSize,
                                     (PULARGE_INTEGER)&data->totalSize,
                                     (PULARGE_INTEGER)&data->freeSize);
    if (!result) {
//        DWORD error = GetLastError();
//        if (error == ERROR_NOT_READY)
//            data->ready = false;
    }
}

void QDriveInfoPrivate::getDevice()
{
    wchar_t deviceBuffer[MAX_PATH];
    bool result = GetVolumeNameForVolumeMountPoint((wchar_t *)data->rootPath.utf16(), deviceBuffer, MAX_PATH);
    if (result)
        data->device = QString::fromWCharArray(deviceBuffer);
}

static inline QDriveInfo::DriveType determineType(const QString &rootPath)
{
#if !defined(Q_OS_WINCE)
    uint result = GetDriveType((wchar_t *)rootPath.utf16());
    switch (result) {
    case DRIVE_REMOVABLE:
        return QDriveInfo::RemovableDrive;

    case DRIVE_FIXED:
        return QDriveInfo::InternalDrive;

    case DRIVE_REMOTE:
        return QDriveInfo::RemoteDrive;

    case DRIVE_CDROM:
        return QDriveInfo::CdromDrive;

    case DRIVE_RAMDISK:
        return QDriveInfo::RamDrive;

    case DRIVE_UNKNOWN:
    case DRIVE_NO_ROOT_DIR:
    // fall through
    default:
        break;
    };
#else
    Q_UNUSED(rootPath)
#endif
    return QDriveInfo::InvalidDrive;
}

void QDriveInfoPrivate::getType()
{
    stat(CachedRootPathFlag); // we need a root path to get info

    data->type = determineType(data->rootPath);
}