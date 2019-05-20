/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __SCHED_BINARY_MANAGER_BINARY_MANAGER_H
#define __SCHED_BINARY_MANAGER_BINARY_MANAGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/binary_manager.h>

#ifdef CONFIG_BINARY_MANAGER

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
#define BINARY_MANAGER_NAME        "binary_manager"           /* Binary manager thread name */
#define BINARY_MANAGER_STACKSIZE   2048                       /* Binary manager thread stack size */
#define BINARY_MANAGER_PRIORITY    250                        /* Binary manager thread priority */

/* Loading Thread information */
#define LOADINGTHD_NAME            "bm_loader"                 /* Loading thread name */
#define LOADINGTHD_STACKSIZE       2048                        /* Loading thread stack size */
#define LOADINGTHD_PRIORITY        200                         /* Loading thread priority */

/* Supported binary types */
#define BIN_TYPE_BIN               0                          /* 'bin' type for kernel binary */
#define BIN_TYPE_ELF               1                          /* 'elf' type for user binary */

/* Binary information configuration */
#define PARTS_PER_BIN              2                          /* The number of partitions per binary */
#define BIN_VER_MAX                16                         /* The maximum length of binary version */
#define KERNEL_VER_MAX             8                          /* The maximum length of kernel version */

#define CHECKSUM_SIZE              4
#define CRC_BUFFER_SIZE            512

/* The number of arguments for loading thread */
#define LOADTHD_ARGC               2

#define BINMGR_DEVNAME_FMT         "/dev/mtdblock%d"

/* Loading thread cmd types */
enum loading_thread_cmd {
	LOADCMD_LOAD = 0,
	LOADCMD_LOAD_ALL = 1,
	LOADCMD_RELOAD = 2,
	LOADCMD_LOAD_MAX,
};

/* Binary data type in binary table */
struct binmgr_bininfo_s {
	pid_t bin_id;
	uint32_t bin_size;
	uint32_t ram_size;
	uint16_t bin_offset;
	uint16_t inuse_idx;
	uint32_t part_size;
	int8_t part_num[PARTS_PER_BIN];
	char name[BIN_NAME_MAX];
	char bin_ver[BIN_VER_MAX];
	char kernel_ver[KERNEL_VER_MAX];
};
typedef struct binmgr_bininfo_s binmgr_bininfo_t;

#define BIN_ID(bin_idx)                                 bin_table[bin_idx].bin_id
#define BIN_SIZE(bin_idx)                               bin_table[bin_idx].bin_size
#define BIN_RAMSIZE(bin_idx)                            bin_table[bin_idx].ram_size
#define BIN_OFFSET(bin_idx)                             bin_table[bin_idx].bin_offset
#define BIN_USEIDX(bin_idx)                             bin_table[bin_idx].inuse_idx

#define BIN_PARTSIZE(bin_idx)                           bin_table[bin_idx].part_size
#define BIN_PARTNUM(bin_idx, part_idx)                  bin_table[bin_idx].part_num[part_idx]

#define BIN_NAME(bin_idx)                               bin_table[bin_idx].name
#define BIN_VER(bin_idx)                                bin_table[bin_idx].bin_ver
#define BIN_KERNEL_VER(bin_idx)                         bin_table[bin_idx].kernel_ver

extern binmgr_bininfo_t bin_table[BINARY_COUNT];

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: binary_manager_recovery
 *
 * Description:
 *   This function will receive the faulty pid and check if its binary id is one
 *   of the registered binary with binary manager.
 *   If the binary is registered, it excludes its children from scheduling
 *   and creates loading thread which will terminate them and load binary again.
 *   Otherwise, board will be rebooted.
 *
 * Input parameters:
 *   pid   -   The pid of recovery message
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_recovery(int pid);
#endif

int binary_manager_load_binary(int bin_idx);
int binary_manager_loading(char *loading_data[]);
int binary_manager_get_binary_count(void);
int binary_manager_get_index_with_binid(int bin_id);
int binary_manager_get_info_with_name(int request_pid, char *bin_name);
int binary_manager_get_info_all(int request_pid);

int binary_manager_send_response(char *q_name, void *response_msg, int msg_size);

/****************************************************************************
 * Binary Manager Main Thread
 ****************************************************************************/
int binary_manager(int argc, char *argv[]);

#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* __SCHED_BINARY_MANAGER_BINARY_MANAGER_H */