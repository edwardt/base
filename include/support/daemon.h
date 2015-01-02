/**
 * @file daemon.h
 * @brief Utility function for making a program into a daemon.
 *
 * @author cjeong
 */
#ifndef MV_DAEMON_H
#define MV_DAEMON_H

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

/* makes the program which calls this function a daemon; takes a bitmask
   argument, flags, which inhibit some of the steps; returns 0 on success, 
   -1 on error */
int daemon_init(int flags);

#ifdef __cplusplus
}
#endif

#endif /* MV_DAEMON_H */
