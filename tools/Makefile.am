
AM_CPPFLAGS = -DSYSCONFDIR="\"${sysconfdir}\""

noinst_LIBRARIES = libgensiotool.a

libgensiotool_a_SOURCES = ioinfo.c ser_ioinfo.c utils.c localports.c

bin_PROGRAMS = gensiot @GTLSSH@
sbin_PROGRAMS = @GTLSSHD@
EXTRA_PROGRAMS = gtlsshd gtlssh

bin_SCRIPTS = gtlssh-keygen

do_subst = sed -e 's,[@]SYSCONFDIR[@],$(sysconfdir),g'

gtlssh-keygen: gtlssh-keygen.in
	$(do_subst) <$(srcdir)/gtlssh-keygen.in >gtlssh-keygen
	chmod +x gtlssh-keygen

gensiot_SOURCES = gensiotool.c

gtlssh_SOURCES = gtlssh.c

gtlsshd_SOURCES = gtlsshd.c

noinst_HEADERS = ioinfo.h ser_ioinfo.h utils.h localports.h

gensiot_LDADD = libgensiotool.a $(top_builddir)/lib/libgensio.la

gtlssh_LDADD = libgensiotool.a $(top_builddir)/lib/libgensio.la $(OPENSSL_LIBS)

gtlsshd_LDADD = libgensiotool.a $(top_builddir)/lib/libgensio.la @PAMLIB@ \
	 $(OPENSSL_LIBS)

man_MANS = gensiot.1 gtlsshd.8 gtlssh.1 gtlssh-keygen.1

EXTRA_DIST = $(man_MANS) gtlssh-keygen.in CMakeLists.txt

