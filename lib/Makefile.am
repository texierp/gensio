
lib_LTLIBRARIES = libgensio.la

noinst_HEADERS = telnet.h heap.h utils.h uucplock.h buffer.h \
	gensio_filter_ssl.h gensio_filter_telnet.h gensio_ll_ipmisol.h \
	gensio_filter_certauth.h gensio_filter_msgdelim.h \
	gensio_filter_relpkt.h gensio_filter_trace.h

libgensio_la_SOURCES = \
	gensio.c gensio_osops.c gensio_net.c gensio_udp.c gensio_stdio.c \
	sergensio.c sergensio_telnet.c sergensio_serialdev.c uucplock.c \
	gensio_selector.c waiter.c \
	gensio_ssl.c gensio_base.c gensio_filter_ssl.c \
	gensio_filter_telnet.c telnet.c buffer.c \
	gensio_ll_fd.c gensio_ll_gensio.c gensio_acc.c gensio_acc_gensio.c \
	gensio_ll_ipmisol.c sergensio_ipmisol.c \
	utils.c selector.c gensio_sctp.c \
	gensio_filter_certauth.c gensio_certauth.c gensio_pty.c \
	gensio_dummy.c gensio_echo.c gensio_mux.c gensio_file.c \
	gensio_filter_msgdelim.c gensio_msgdelim.c \
	gensio_filter_relpkt.c gensio_relpkt.c \
	gensio_filter_trace.c gensio_trace.c

libgensio_la_LDFLAGS = $(OPENSSL_LIBS)

EXTRA_DIST = README.rst libgensio.pc.in CMakeLists.txt

# This variable must have 'exec' in its name, in order to be installed
# by 'install-exec' target (instead of default 'install-data')
pkgconfigexecdir = $(libdir)/pkgconfig
pkgconfigexec_DATA = libgensio.pc
