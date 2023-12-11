/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/29 18:55
* @version: 1.0
* @description: 
********************************************************************************/


#include "fatfs_user.h"
#include "sdio.h"

// 每个设备挂载都需要一个
/********************************************************************************
* SD 文件系统操作对象
********************************************************************************/
FATFS sd_fs;
FIL sd_fil;
FRESULT sd_res;
const char sd_path[] = "0:";

/********************************************************************************
* FLASH 文件系统操作对象
********************************************************************************/
FATFS flash_fs;
// 文件对象
FIL flash_fil;
// 结果变量
FRESULT res;
// 逻辑路径
const char flash_path[] = "1:";
// 字节读取与字节写入
UINT bw, br;



// 写入缓存数组
const uint8_t json_str[] = "{\"002\":\"1202015\",\"001\":\"00004\"}";
// 读取缓存数组
uint8_t file_read_buff[4096];

void fatfs_init_user(void) {
    sdio_sd_init();

    // 测试
    fatfs_func_test();
}

/********************************************************************************
* 文件系统读写测试
********************************************************************************/
static void fatfs_func_test(void) {
    printf("fatfs_func_test is running\r\n");
    // sd 挂载
    res = file_sys_mount(&flash_fs, flash_path, 1);
    // 打开文件
    if (res == FR_OK)
        res = f_open(&flash_fil, "1:flash.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      //打开/创建文件
    printf("open_&_mkdir -> %d\r\n", res);
    if (res == FR_OK) {
        if (file_sys_write(&flash_fil, (void *) json_str, sizeof(json_str)) == FR_OK) {
            if (file_sys_read(&flash_fil, (void *) file_read_buff, sizeof(file_read_buff)) == FR_OK) {
                printf("the_read_buff -> %s\r\n", file_read_buff);
            }
            printf("write ok");
        }
    }
    // 关闭文件
    f_close(&flash_fil);
    // 取消挂载
    f_mount(NULL, flash_path, 1);
}

/********************************************************************************
* 文件系统挂载
********************************************************************************/
static FRESULT file_sys_mount(FATFS *fs_obj, const char *fs_path, uint8_t dev_index) {
    // 挂载文件系统
    res = f_mount(fs_obj, fs_path, 1);
    printf("res -> %d\r\n", res);
    // 如果没有文件系统就格式化并创建文件系统
    if (res == FR_NO_FILESYSTEM) {
        printf("fs system is not exist,prepare to init one\r\n");
        // 格式化
        res = f_mkfs(fs_path, 0, 0);
        if (res == FR_OK) {
            printf("success to format fs system\r\n");
            // 格式化后，先取消挂载
            res = f_mount(NULL, fs_path, 1);
            // 重新挂载
            res = f_mount(fs_obj, fs_path, 1);
        } else {
            printf("format failed\r\n");
            return FR_INT_ERR;
        }
    } else if (res != FR_OK) {
        printf("fs_system_mount_failed ! -> (%d)\r\n", res);
        return FR_INT_ERR;
    } else {
        printf("fs_system_mount -> %d\r\n", res);
        /*// 逻辑驱动器上创建 FAT 卷，首次可以执行，之后需注释防止初始化
        if (dev_index == 0) {
            res = f_mkfs(fs_path, 0, 512);
        } else if (dev_index == 1) {
            res = f_mkfs(fs_path, 0, 4096);
        }*/
    }
    return res;
}

/********************************************************************************
* 文件写入
 * @param   file_obj 文件操作对象
 * @param   write_buff 写入的缓冲数组
 * @param   num_to_write 类型范围内要写入的字节数
********************************************************************************/
static FRESULT file_sys_write(FIL *file_obj, void *write_buff, uint32_t num_to_write) {
    res = f_write(file_obj, write_buff, num_to_write, &bw);        //写内容
    printf("write_status -> %d, num_of_write -> %d\r\n", res, bw);
    return res;
}

/*******************************************g*************************************
* 文件读取
 * @param   file_obj 文件操作对象
 * @param   read_buff 读取的缓冲数组
 * @param   num_to_read 类型范围内要读取的字节数
********************************************************************************/
static FRESULT file_sys_read(FIL *file_obj, void *read_buff, uint32_t num_to_read) {
    /* 移动打开的文件对象的文件读 / 写指针到 0
     * ofs: 距文件顶部的字节偏移量 */
    f_lseek(file_obj, 0);
    /* 读取文件
     * btr: UINT 类型范围内要读取的字节数 */
    res = f_read(file_obj, read_buff, num_to_read, &br);
    printf("read_status -> %d, num_of_read -> %d\r\n", res, br);
    return res;
}

