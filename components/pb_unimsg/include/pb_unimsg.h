#ifndef PB_UNIMSG_H
#define PB_UNIMSG_H

#include "pb_common.h"

const pb_msgdesc_t *decode_command_unionmessage_type(pb_istream_t *stream);

bool decode_unionmessage_contents(pb_istream_t *stream,
                                    const pb_msgdesc_t *messagetype, void *dest_struct);

bool encode_response_unionmessage(pb_ostream_t *stream,
                                const pb_msgdesc_t *messagetype, void *message);

#endif
