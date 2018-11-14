#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "ubus_client.h"
#include "event.h"

struct event ev;
struct event_base *main_base;

void myfunc(const int fd, const short which, void *arg)
{
	char buf[100] = {0};
	int ret = read(fd,buf,sizeof(buf));
	if (ret > 0)
	{
		//printf("read buf:%s\n",buf);
	}
	Send_user_message();
}

void QUIT()
{
	exit(0);
}
int main(int argc,char *argv[])
{
	signal(SIGINT,QUIT);
	init_ubus();
	main_base = event_init();
	event_set(&ev,0,EV_READ|EV_PERSIST,myfunc,main_base);
        event_base_set(main_base, &ev);
        event_add(&ev,NULL);
        event_base_loop(main_base,0);
	return 0;
}
