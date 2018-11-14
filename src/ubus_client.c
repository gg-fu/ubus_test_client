#include "ubus_client.h"
#define DBG_UBUS_CLIENT 1
static int id;
struct blob_buf b;
static struct ubus_subscriber event;
static struct ubus_context *ctx;

/*static const struct blobmsg_policy people[] = {
        [name] = { .name = "name", .type = BLOBMSG_TYPE_STRING},
        [age]  = { .name = "age", .type = BLOBMSG_TYPE_INT32 },
        [high] = { .name = "high", .type = BLOBMSG_TYPE_STRING},
        [weight] = { .name = "weight", .type = BLOBMSG_TYPE_STRING},
        [address] = { .anem = "address", .type = BLOBMSG_TYPE_STRING},
};*/

void Bru_create_pthread(void *(*func)(void *),void *arg)
{
        pthread_t thread;
        pthread_attr_t attr;
        int ret;
        pthread_attr_init(&attr);

        ret = pthread_create(&thread,&attr,func,arg);
        if (ret!=0)
        {
                printf("pthread_create failed\n");
                exit(1);
        }
}

enum {
        name,
        age,
        address,
        __EXCUE_MAX
};

static const struct blobmsg_policy get_address_policy[__EXCUE_MAX] = {
  [name] = { .name = "name", .type = BLOBMSG_TYPE_STRING},
  [age] = { .name = "age", .type = BLOBMSG_TYPE_INT8},
  [address] = { .name = "address", .type = BLOBMSG_TYPE_STRING},
};


static int Recv(struct ubus_context *ctx, struct ubus_object *obj,
                struct ubus_request_data *req,
                const char *method, struct blob_attr *msg)
{
        printf("recive data from ubus_server\n");
	 struct blob_attr *tb[__EXCUE_MAX];
        char *name_;
        int age_;
        char *address_;
        void *dtable;

        blobmsg_parse(get_address_policy, __EXCUE_MAX, tb, blob_data(msg), blob_len(msg));

        if (!tb[name] || !tb[age] || !tb[address])
                return UBUS_STATUS_INVALID_ARGUMENT;

        name_ = blobmsg_get_string(tb[name]);
        address_ = blobmsg_get_string(tb[address]);
        age_ = blobmsg_get_u8(tb[age]);
        printf("name:%s,address:%s,age:%d\n",name_,address_,age_);

}

void handle_remove(struct ubus_context *ctx,
                          struct ubus_subscriber *obj, uint32_t id)
{
        printf("remove .......\n");
	exit(0);
}

void *client_main(void *arg)
{
        int ret ;
        for(;;)
        {
                event.cb = Recv;
                event.remove_cb = handle_remove;
                ret = ubus_register_subscriber(ctx,&event);
                if (ret)
                {
                        printf("ubus_register_subscriber failed\n");
                        return NULL;
                }
                if (ubus_lookup_id(ctx, "server", &id)) {
                        printf("ubus_loojup_id failed \n");
                        return NULL;
                }
                ret = ubus_subscribe(ctx, &event, id);
                if(ret)
                        printf("Failed to subscribe: %s\n", ubus_strerror(ret));
                uloop_run();
        }
}

static void report_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
        printf("report_cb\n");
}
int Send_user_message()
{

        blobmsg_buf_init(&b);
        blobmsg_add_string(&b, "name","Bruvin");
        blobmsg_add_u8(&b, "age",24);
        blobmsg_add_string(&b,"address","guangdong shenzhen");

        ubus_invoke(ctx, id, "name", b.head, report_cb, 0, 500);
}

void init_ubus(void)
{
        const char *socket = "/var/run/ubus.sock";
        uloop_init();

        signal(SIGPIPE,SIG_IGN);
        ctx = ubus_connect(socket);
        if(!ctx)
        {
                printf("ubus connect failed\n");
                return;
        }
        ubus_add_uloop(ctx);
        Bru_create_pthread(client_main,NULL);
}

