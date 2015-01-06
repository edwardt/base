/**
 * @file rtutil.h
 *
 * @brief Utility function.
 */
#ifndef MVRT_UTIL_H
#define MVRT_UTIL_H


/*
 * For making a program into a daemon.
 */
/* bitmask values for <flags> argument of daemon_init() */
#define BD_NO_CHDIR              0x01  /* don't chdir("/") */
#define BD_NO_CLOSE_FILES        0x02  /* don't close all open files */
#define BD_NO_REDIRECT_STD_FDS   0x04  /* dont' redirect stdin, stdout, and 
                                          stderr to /dev/null */
#define BD_NO_UMASK0             0x10  /* don't do umask(0) */
#define BD_MAX_CLOSE             8192  /* max file descriptors to close when
                                          sysconf(_SC_OPEN_MAX) is indet. */

#ifdef __cplusplus
extern "C" {
#endif


/* Makes the program which calls this function a daemon. Takes a bitmask
   argument, flags, which inhibit some of the steps; returns 0 on success, 
   -1 on error. */
extern int daemon_init(int flags);

/* Registers signal handler. */
extern void register_signal_handler(int signo, void (*fptr)(int)) ;


#ifdef __cplusplus
}
#endif

#endif /* MVRT_UTIL_H */
