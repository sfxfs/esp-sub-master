#ifndef SUB_MAIN_PB_COM_H
#define SUB_MAIN_PB_COM_H

int protobuf_commu_init(void);
int protobuf_commu_start_thread(void);
int protobuf_commu_send_resp(const pb_msgdesc_t *messagetype, void *message);

#endif
