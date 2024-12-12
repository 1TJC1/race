#include "main.h"
typedef struct
{
	unsigned int zigbee_id;
	unsigned int zigbee_ip;
	unsigned char  zigbee_channel;
} system_zigbee;
extern system_zigbee Information;
//system_zigbee Information={0x1100, 0x1000,0x11	};
void changeIP(unsigned int n);
void changeID(unsigned int n);
void changeCHANNEL(unsigned char n);
void cq(void);
void sendmessage(unsigned char port,unsigned int add,unsigned char *d,unsigned char len);
void sendmessages(unsigned char port,unsigned char *d,unsigned char len);
void check_config(unsigned char config);
