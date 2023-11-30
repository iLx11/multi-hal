/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/29 18:55
* @version: 1.0
* @description: 
********************************************************************************/


#include "fatfs_user.h"

// ÿ���豸���ض���Ҫһ��
FATFS fs_object;
// �ļ�����
FIL fil;
// �������
FRESULT res;
// �߼�·��
char user_path[4] = "0:";
// �ֽڶ�ȡ���ֽ�д��
UINT bw, br;

// д�뻺������
const uint8_t json_str[] = "{\"002\":\"1202015\",\"001\":\"00004\"}";
// ��ȡ��������
uint8_t file_read_buff[4096] = {0};

void fatfs_init_user(void) {
    // ����
    file_sys_mount();
    // ����
    fatfs_func_test();
}
/********************************************************************************
* �ļ�ϵͳ��д����
********************************************************************************/
static void fatfs_func_test(void) {
    // ���ļ�
    res = f_open(&fil, "0:1.bin", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      //��/�����ļ�
    printf("open_&_mkdir -> %d\r\n", res);
    if (res == FR_OK) {
        if (file_sys_write(&fil, (void *) json_str, sizeof(json_str)) == FR_OK) {
            if (file_sys_read(&fil, (void *) file_read_buff, sizeof(file_read_buff)) == FR_OK) {
                printf("the_read_buff -> %s\r\n", file_read_buff);
            }
        }
        // �ر��ļ�
        f_close(&fil);
    }
}

/********************************************************************************
* �ļ�ϵͳ����
********************************************************************************/
static void file_sys_mount(void) {
    // �����ļ�ϵͳ
    res = f_mount(&fs_object, user_path, 1);
    // ���û���ļ�ϵͳ�͸�ʽ���������ļ�ϵͳ
    if(res == FR_NO_FILESYSTEM) {
        printf("fs system is not exist,prepare to init one\r\n");
        // ��ʽ��
        res = f_mkfs(user_path, 0, 0);
        if(res == FR_OK) {
            printf("success to format fs system\r\n");
            // ��ʽ������ȡ������
            res = f_mount(NULL, user_path, 1);
            // ���¹���
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
        // �߼��������ϴ��� FAT ��
        res = f_mkfs(user_path, 0, 4096);
    }
}

/********************************************************************************
* �ļ�д��
 * @param   write_buff д��Ļ�������
 * @param   num_to_write ���ͷ�Χ��Ҫд����ֽ���
********************************************************************************/
static FRESULT file_sys_write(FIL* file_obj, void* write_buff, uint32_t num_to_write) {
    res = f_write(file_obj, write_buff, num_to_write, &bw);        //д����
    printf("write_status -> %d, num_of_write -> %d\r\n", res, bw);
    return res;
}

/*******************************************g*************************************
* �ļ���ȡ
 * @param   read_buff ��ȡ�Ļ�������
 * @param   num_to_read ���ͷ�Χ��Ҫ��ȡ���ֽ���
********************************************************************************/
static FRESULT file_sys_read(FIL* file_obj, void* read_buff, uint32_t num_to_read) {
    /* �ƶ��򿪵��ļ�������ļ��� / дָ�뵽 0
     * ofs: ���ļ��������ֽ�ƫ���� */
    f_lseek(file_obj, 0);
    /* ��ȡ�ļ�
     * btr: UINT ���ͷ�Χ��Ҫ��ȡ���ֽ��� */
    res = f_read(file_obj, read_buff, num_to_read, &br);
    printf("read_status -> %d, num_of_read -> %d\r\n", res, br);
    return res;
}

