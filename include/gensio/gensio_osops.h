/*
 *  gensio - A library for abstracting stream I/O
 *  Copyright (C) 2019  Corey Minyard <minyard@acm.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 */

#ifndef GENSIO_OSOPS_H
#define GENSIO_OSOPS_H

#include <sys/types.h> /* For pid_t */
struct sockaddr;

/* To avoid having to include netinet/sctp.h here. */
struct sctp_sndrcvinfo;

#include <gensio/gensio.h>

int gensio_os_write(struct gensio_os_funcs *o,
		    int fd, const struct gensio_sg *sg, gensiods sglen,
		    gensiods *rcount);

int gensio_os_read(struct gensio_os_funcs *o,
		   int fd, void *buf, gensiods buflen, gensiods *rcount);

int gensio_os_recv(struct gensio_os_funcs *o,
		   int fd, void *buf, gensiods buflen, gensiods *rcount,
		   int flags);

int gensio_os_send(struct gensio_os_funcs *o,
		   int fd, const struct gensio_sg *sg, gensiods sglen,
		   gensiods *rcount, int flags);

int gensio_os_sendto(struct gensio_os_funcs *o,
		     int fd, const struct gensio_sg *sg, gensiods sglen,
		     gensiods *rcount, int flags,
		     const struct sockaddr *raddr,socklen_t raddrlen);

int gensio_os_recvfrom(struct gensio_os_funcs *o,
		       int fd, void *buf, gensiods buflen, gensiods *rcount,
		       int flags, struct sockaddr *raddr, socklen_t *raddrlen);

int gensio_os_accept(struct gensio_os_funcs *o,
		     int fd, struct sockaddr *addr, socklen_t *addrlen,
		     int *newsock);

int gensio_os_sctp_recvmsg(struct gensio_os_funcs *o,
			   int fd, void *msg, gensiods len, gensiods *rcount,
			   struct sctp_sndrcvinfo *sinfo, int *msg_flags);

int gensio_os_sctp_send(struct gensio_os_funcs *o,
			int fd, const struct gensio_sg *sg, gensiods sglen,
			gensiods *rcount,
                        const struct sctp_sndrcvinfo *sinfo, uint32_t flags);

int gensio_setupnewprog(void);

int gensio_setup_child_on_pty(struct gensio_os_funcs *o,
			      char *const argv[], const char **env,
			      int *rptym, pid_t *rpid);

int gensio_get_random(struct gensio_os_funcs *o,
		      void *data, unsigned int len);

struct opensocks
{
    int fd;
    int family;
    unsigned int port;
};

/*
 * Open a set of sockets given the addrinfo list, one per address.
 * Return the actual number of sockets opened in nr_fds.  Set the
 * I/O handler to readhndlr, with the given data.
 *
 * Note that if the function is unable to open an address, it just
 * goes on.  It returns NULL if it is unable to open any addresses.
 * Also, open IPV6 addresses first.  This way, addresses in shared
 * namespaces (like IPV4 and IPV6 on INADDR6_ANY) will work properly
 */
int gensio_open_socket(struct gensio_os_funcs *o,
		       struct gensio_addrinfo *ai,
		       void (*readhndlr)(int, void *),
		       void (*writehndlr)(int, void *),
		       void (*fd_handler_cleared)(int, void *),
		       void *data,
		       struct opensocks **socks, unsigned int *nr_fds);

struct gensio_listen_scan_info {
    unsigned int curr;
    unsigned int start;
    unsigned int reqport;
};

/*
 * Setup a receiving socket given the socket() parameters.  If do_listen
 * is true, call listen on the socket.  This sets nonblocking, reuse,
 * does a bind, etc.
 *
 * The new file descriptor is returned in rfd, and the chosen port is
 * returned in port.  If the address has a port set to 0, this
 * function choose random port from the IANA dynamic range.
 *
 * The rsi parameter is used for handling port 0 for multiple sockets.
 * Pass in a zero-ed structure here.  If the code encounters a zero
 * port, it will set some information int the rsi structure and for
 * every other zero port in future calls it will choose the same port.
 * If it chooses a port that is already in use on one of the later
 * addresses with a zero port, you can clear the reqport member, close
 * everything, and start over.  The function will continue scanning
 * from the next port.  If it returns GE_ADDRINUSE and the curr and
 * start value are the same, then no port was found that could be
 * opened on all addresses.  Errors besides GE_ADDRINUSE should be
 * treated as immediate errors, something else went wrong.
 */
int gensio_setup_listen_socket(struct gensio_os_funcs *o, bool do_listen,
			       int family, int socktype, int protocol,
			       int flags,
			       struct sockaddr *addr, socklen_t addrlen,
			       void (*readhndlr)(int, void *),
			       void (*writehndlr)(int, void *), void *data,
			       void (*fd_handler_cleared)(int, void *),
			       int (*call_b4_listen)(int, void *),
			       int *rfd, unsigned int *port,
			       struct gensio_listen_scan_info *rsi);

/* Returns a NULL if the fd is ok, a non-NULL error string if not */
const char *gensio_check_tcpd_ok(int new_fd);

/*
 * Extract/set the port on a sockaddr.  If the sockaddr is not AF_INET
 * or AF_INET6, the get functions returns a gensio error.
 */
int gensio_sockaddr_get_port(const struct sockaddr *s, unsigned int *port);
int gensio_sockaddr_set_port(const struct sockaddr *s, unsigned int port);

/*
 * A routine for converting a sockaddr to a numeric IP address.
 *
 * If addrlen is non-NULL and is non-zero, it is compared against what
 * the actual address length should have been and EINVAL is returned
 * if it doesn't match.  If addrlen is non-NULL and is zero, it will
 * be updated to the address length.
 *
 * The output is put into buf starting at *epos (or zero if epos is NULL)
 * and will fill in buf up to buf + buflen.  If the buffer is not large
 * enough, it is truncated, but if epos is not NULL, it will be set to the
 * byte position where the ending NIL character would have been, one less
 * than the buflen that would have been required to hold the entire buffer.
 *
 * If addr is not AF_INET or AF_INET6, return EINVAL.
 */
int gensio_sockaddr_to_str(const struct sockaddr *addr, socklen_t *addrlen,
			   char *buf, gensiods *epos, gensiods buflen);

/*
 * Compare two sockaddr structure and return TRUE if they are equal
 * and FALSE if not.  Only works for AF_INET and AF_INET6.
 * If compare_ports is false, then the port comparison is ignored.
 */
bool gensio_sockaddr_equal(const struct sockaddr *a1, socklen_t l1,
			   const struct sockaddr *a2, socklen_t l2,
			   bool compare_ports);

#endif /* GENSIO_OSOPS_H */
