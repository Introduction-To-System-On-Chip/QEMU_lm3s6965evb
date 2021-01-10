#ifndef MPU_CONFIGURE_H
#define MPU_CONFIGURE_H

#include <stdbool.h>

void disableMPU(bool manualConfiguration);

void ManualInitMPU(void);

void CmsisInitMPU(void);

#endif // MPU_CONFIGURE_H
