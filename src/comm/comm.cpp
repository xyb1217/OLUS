#include "comm.h"

void n2h_oluph(OLUPH & oluph)
{
	oluph.dev_id = ntohl(oluph.dev_id);
	return ;
}

void h2n_oluph(OLUPH & oluph)
{
	oluph.dev_id = htonl(oluph.dev_id);
	return ;
}


void n2h_firmware_resp(FirmwareResp & firmware_resp)
{
    firmware_resp.down_size = ntohl(firmware_resp.down_size);
    return ;
}


void h2n_firmware_resp(FirmwareResp & firmware_resp)
{
	firmware_resp.down_size = htonl(firmware_resp.down_size);
	return ;
}

pthread_mutex_t mutex_write;


