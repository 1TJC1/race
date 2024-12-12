#ifndef _SGP30_H_
#define _SGP30_H_

#include "main.h"
#include <stdio.h>

#define SGP30_ADDR          0xb0
//#define	SGP30_ADDR_WRITE	SGP30_ADDR<<1       //0xb0
//#define	SGP30_ADDR_READ		(SGP30_ADDR<<1)+1   //0xb1


typedef struct sgp30_data_st {
    u16 co2;
    u16 tvoc;
}sgp30_data_t;

typedef enum sgp30_cmd_en {
    /* 初始化空气质量测量 */
    INIT_AIR_QUALITY = 0x2003,
    
    /* 开始空气质量测量 */
    MEASURE_AIR_QUALITY = 0x2008
    
} sgp30_cmd_t;

extern sgp30_data_t      sgp30_data;

void sgp30_init(void);
void sgp30_read(void);


#endif /* _SGP30_H_ */
