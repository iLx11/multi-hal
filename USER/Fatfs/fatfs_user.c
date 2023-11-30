/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/29 18:55
* @version: 1.0
* @description: 
********************************************************************************/


#include "fatfs_user.h"

// 每个设备挂载都需要一个
FATFS fs_object;
// 文件对象
FIL fil;
// 结果变量
FRESULT res;
// 逻辑路径
char user_path[4] = "0:";
// 字节读取与字节写入
UINT bw, br;

// 写入缓存数组
const uint8_t json_str[] = "{\"002\":\"1202015\",\"001\":\"00004\"}";
// 读取缓存数组
uint8_t file_read_buff[4096] = {0};

void fatfs_init_user(void) {
    // 挂载
    file_sys_mount();
    // 测试
    fatfs_func_test();
}
/********************************************************************************
* 文件系统读写测试
********************************************************************************/
static void fatfs_func_test(void) {
    // 打开文件
    res = f_open(&fil, "0:1.bin", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      //打开/创建文件
    printf("open_&_mkdir -> %d\r\n", res);
    if (res == FR_OK) {
        if (file_sys_write(&fil, (void *) json_str, sizeof(json_str)) == FR_OK) {
            if (file_sys_read(&fil, (void *) file_read_buff, sizeof(file_read_buff)) == FR_OK) {
                printf("the_read_buff -> %s\r\n", file_read_buff);
            }
        }
        // 关闭文件
        f_close(&fil);
    }
}

/********************************************************************************
* 文件系统挂载
********************************************************************************/
static void file_sys_mount(void) {
    // 挂载文件系统
    res = f_mount(&fs_object, user_path, 1);
    // 如果没有文件系统就格式化并创建文件系统
    if(res == FR_NO_FILESYSTEM) {
        printf("fs system is not exist,prepare to init one\r\n");
        // 格式化
        res = f_mkfs(user_path, 0, 0);
        if(res == FR_OK) {
            printf("success to format fs system\r\n");
            // 格式化后，先取消挂载
            res = f_mount(NULL, user_path, 1);
            // 重新挂载
            res = f_mount(&fs_object, user_path, 1);
        } else {
            printf("format failed\r\n");
            while(1);
        }
    } else if(res != FR_OK) {
        printf("fs_system_mount_failed ! -> (%d)\r\n", res);
        while(1);
    } else {
        printf("fs_system_mount -> %d\r\n", res);
        // 逻辑驱动器上创建 FAT 卷
        res = f_mkfs(user_path, 0, 4096);
    }
}

/********************************************************************************
* 文件写入
 * @param   write_buff 写入的缓冲数组
 * @param   num_to_write 类型范围内要写入的字节数
********************************************************************************/
static FRESULT file_sys_write(FIL* file_obj, void* write_buff, uint32_t num_to_write) {
    res = f_write(file_obj, write_buff, num_to_write, &bw);        //写内容
    printf("write_status -> %d, num_of_write -> %d\r\n", res, bw);
    return res;
}

/*******************************************g*************************************
* 文件读取
 * @param   read_buff 读取的缓冲数组
 * @param   num_to_read 类型范围内要读取的字节数
********************************************************************************/
static FRESULT file_sys_read(FIL* file_obj, void* read_buff, uint32_t num_to_read) {
    /* 移动打开的文件对象的文件读 / 写指针到 0
     * ofs: 距文件顶部的字节偏移量 */
    f_lseek(file_obj, 0);
    /* 读取文件
     * btr: UINT 类型范围内要读取的字节数 */
    res = f_read(file_obj, read_buff, num_to_read, &br);
    printf("read_status -> %d, num_of_read -> %d\r\n", res, br);
    return res;
}

