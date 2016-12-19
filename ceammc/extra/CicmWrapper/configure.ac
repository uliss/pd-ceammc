dnl Process this file with autoconf to produce a configure script.
AC_INIT([CicmWrapper], 0.2.1-git, Pierre Guillot,[CicmWrapper],[https://github.com/CICM/CicmWrapper])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_FILES([Sources/Makefile])

AC_SUBST(PD_CPPFLAGS)
AC_SUBST(PD_LDFLAGS)

AM_INIT_AUTOMAKE([1.10 foreign])

AC_ARG_WITH([pd],       [  --with-pd=</path/to/pd> where to look for pd-headers and and -libs])

AC_PROG_CXX
AC_PROG_LIBTOOL


if test "x$with_pd" != "x"; then
 if test -d "${with_pd}"; then
   PDPATH=${with_pd}
 fi
 if test -d "${PDPATH}/src"; then
  AC_MSG_RESULT([adding ${PDPATH}/src to INCLUDES])
  PD_CPPFLAGS="-I${PDPATH}/src ${CPPFLAGS}"

  AC_MSG_RESULT([adding ${PDPATH}/src to LDFLAGS])
  PD_LDFLAGS="-L${PDPATH}/src ${LDFLAGS}"
 else
  if test -d "${PDPATH}"; then
   AC_MSG_RESULT([adding ${PDPATH} to INCLUDES])
   PD_CPPFLAGS="-I${PDPATH} ${CPPFLAGS}"
  fi
 fi
 if test -d "${PDPATH}/bin"; then
  AC_MSG_RESULT([adding ${PDPATH}/bin to LDFLAGS])
  PD_LDFLAGS="-L${PDPATH}/bin ${LDFLAGS}"
 else
  if test -d "${PDPATH}"; then
   AC_MSG_RESULT([adding ${PDPATH} to LDFLAGS])
   PD_LDFLAGS="-L${PDPATH} ${LDFLAGS}"
  fi
 fi
else 
  AC_MSG_RESULT([no pd path given, trying with pkg-config data])
  PD_CPPFLAGS=`pkg-config --cflags pd`
  PD_LDFLAGS=`pkg-config --libs pd`
fi

AC_OUTPUT(Makefile)
