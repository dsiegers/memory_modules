#ifndef IOCTL_H
#define IOCTL_H

#include <linux/ioctl.h>

typedef struct {
    int i;
    int j;
} lkmc_ioctl_struct;
#define LKMC_IOCTL_MAGIC 0x33
#define LKMC_IOCTL_INC     _IOWR(LKMC_IOCTL_MAGIC, 0, int)
#define LKMC_IOCTL_INC_DEC _IOWR(LKMC_IOCTL_MAGIC, 1, lkmc_ioctl_struct)

#endif
