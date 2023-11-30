/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
/*#include "usbdisk.h"	*//* Example: Header file of existing USB MSD control module *//*
#include "atadrive.h"	*//* Example: Header file of existing ATA harddisk control module *//*
#include "sdcard.h"		*//* Example: Header file of existing MMC/SDC contorl module */
#include "flash.h"
#define DEV_FLASH 0
/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
        BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat;
    if (read_flash_id() == 0X5217)
        stat = 0;
    else
        stat = STA_NOINIT;
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
        BYTE pdrv                /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat;
    flash_init();
    stat = disk_status(DEV_FLASH);
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
        BYTE pdrv,        /* Physical drive nmuber to identify the drive */
        BYTE *buff,        /* Data buffer to store read data */
        DWORD sector,    /* Start sector in LBA */
        UINT count        /* Number of sectors to read */
) {
    DRESULT res;
    read_flash(buff, sector * 4096, count * 4096);
    res = RES_OK;
    return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
        BYTE pdrv,            /* Physical drive nmuber to identify the drive */
        const BYTE *buff,    /* Data to be written */
        DWORD sector,        /* Start sector in LBA */
        UINT count            /* Number of sectors to write */
) {
    DRESULT res;
    write_to_flash((uint8_t *) buff, sector * 4096, count * 4096);
    res = RES_OK;
    return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
        BYTE pdrv,        /* Physical drive nmuber (0..) */
        BYTE cmd,        /* Control code */
        void *buff        /* Buffer to send/receive control data */
) {
    DRESULT res;
// Process of the command for the RAM drive
    switch (cmd) {
        //返回扇区个数
        case GET_SECTOR_COUNT:
            *(DWORD *) buff = 4096;
            //返回每个扇区大小
            break;
        case GET_SECTOR_SIZE:
            *(WORD *) buff = 4096;
            break;
            //返回擦除扇区的最小个数(单位为扇区)
        case GET_BLOCK_SIZE:
            *(WORD *) buff = 1;
            break;
    }
    res = RES_OK;
    return res;
}

DWORD get_fattime() {
    return 0;
}