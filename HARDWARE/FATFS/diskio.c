/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "flash.h"
#include "sdio.h"

extern SD_HandleTypeDef hsd;

// 定义多个物理驱动
#define DEV_SD 0
#define DEV_FLASH 1

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
        BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat;
//    if(pdrv == DEV_SD) {
//        if(get_sd_state() == HAL_ERROR)
//            stat = STA_NOINIT;
//        else
//            stat = RES_OK;
//    }else if(pdrv == DEV_FLASH) {
//        if (read_flash_id() == 0X5217)
//            stat = RES_OK;
//        else
//            stat = STA_NOINIT;
//    }
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
        BYTE pdrv                /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat;
    switch (pdrv) {
        case DEV_SD:
            sdio_sd_init();
            stat = RES_OK;
            break;
        case DEV_FLASH:
            flash_init();
            stat = RES_OK;
            break;
        default:
            stat = RES_PARERR;
    }
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
    if(!count) return RES_PARERR;

    switch (pdrv) {
        case DEV_SD:
            res = read_card((uint8_t *) buff, sector, count);
            while(res) {
                if(res != 2) sdio_sd_init();
                res = read_card((uint8_t *) buff, sector, count);
            }
            break;
        case DEV_FLASH:
            read_flash(buff, sector * 4096, count * 4096);
            res = RES_OK;
            break;
        default:
            res = RES_ERROR;
    }
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
    if(!count) return RES_PARERR;
    DRESULT res;
    switch (pdrv) {
        case DEV_SD:
            res = write_to_card((uint8_t *) buff, sector, count);
            while(res) {
                if(res != 2) sdio_sd_init();
                res = write_to_card((uint8_t *) buff, sector, count);
            }
            break;
        case DEV_FLASH:
            write_to_flash((uint8_t *) buff, sector * 4096, count * 4096);
            res = RES_OK;
            break;
        default:
            res = RES_ERROR;
    }
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
    if(pdrv == DEV_SD) {
        switch (cmd) {
            //返回扇区个数
            case CTRL_SYNC:
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = ((long long) hsd.SdCard.BlockNbr * hsd.SdCard.BlockSize) / 512;
//                *(DWORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
                //返回每个扇区大小
                break;
            case GET_SECTOR_SIZE:
                *(WORD *) buff = 512;
                break;
                //返回擦除扇区的最小个数(单位为扇区)
            case GET_BLOCK_SIZE:
                *(WORD *) buff = hsd.SdCard.BlockSize;
                break;
            default:
                res = RES_PARERR;
                break;
        }
    } else if(pdrv == DEV_FLASH) {
        switch (cmd) {
            case CTRL_SYNC:
                res = RES_OK;
                break;
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
    }
    res = RES_OK;
    return res;
}

DWORD get_fattime() {
    return 0;
}