#include <common.h>

#include "connection.h"

bool listener_create(u16 port);
void listener_destroy();

bool listener_listen(connection* socket);