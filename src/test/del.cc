#include "czmq.h"
#include "../account.pb.h"
#include "../account_constants.h"

int main(int argc, char **argv)
{
    char *name = argv[1];
    
    zctx_t *ctx = zctx_new();
    void *socket = zsocket_new(ctx, ZMQ_REQ);
    zsocket_connect(socket, "tcp://localhost:5555");

    zmsg_t *msg = zmsg_new();
    zmsg_addstr(msg, ACCOUNT::DEL);
    zmsg_addstr(msg, name);

    zmsg_send(&msg, socket);

    //recv
    msg = zmsg_recv(socket);
    char *status = zmsg_popstr(msg);
    if(status[0] == 0x01)
    {
        zmsg_popstr(msg);
        char *ret_msg = zmsg_popstr(msg);
        printf("%s\n", ret_msg);

    }
    else
    {
        char *err_msg = zmsg_popstr(msg);
        printf("ERR: %s\n", err_msg);
    }

    zmsg_destroy(&msg);
    zctx_destroy(&ctx);

    return 0;
}

