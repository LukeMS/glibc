GLIBC_PROVIDES dnl See aclocal.m4 in the top level source directory.
# Local configure fragment for sysdeps/unix/sysv/linux/x86.

if test x"$enable_cet" = xdefault || test x"$enable_cet" = xyes; then
  # See whether GCC supports CET.
  libc_cv_x86_cet_cflags="-mibt -mshstk"
  AC_CACHE_CHECK(whether $CC supports $libc_cv_x86_cet_cflags,
		 libc_cv_x86_cet, [dnl
		 echo 'void foo (void) { }' > conftest.c
		 libc_cv_x86_cet=no
		 if AC_TRY_COMMAND(${CC-cc} -S $CFLAGS conftest.c $libc_cv_x86_cet_cflags -o conftest.s 1>&AS_MESSAGE_LOG_FD); then
		   libc_cv_x86_cet=yes
		 fi
		 rm -rf conftest*])

  if test $libc_cv_x86_cet = yes; then
    if test x"$enable_cet" = xdefault; then
      enable_cet=yes
    fi
  else
    if test x"$enable_cet" = xdefault; then
      enable_cet=no
    else
       AC_MSG_ERROR([$CC doesn't support $libc_cv_x86_cet_cflags])
    fi
    libc_cv_x86_cet_cflags=
  fi
else
  libc_cv_x86_cet_cflags=
fi
LIBC_CONFIG_VAR([cet_cflags], [$libc_cv_x86_cet_cflags])
