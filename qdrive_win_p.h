#ifndef QDRIVE_WIN_P_H
#define QDRIVE_WIN_P_H

#include "qdrive_p.h"

class QDrivePrivate : public QDrivePrivateBase
{
public:
    QDrivePrivate();

    void stat(uint requiredFlags);
    void getVolumeInformation();
    void getDiskFreeSpace();
};

#endif // QDRIVE_WIN_P_H
