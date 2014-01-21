#include <errno.h>
#include <fcntl.h>
#include <cutils/log.h>

int houdini_hook_open_neon(int myuid, int flags, int mode)
{
    int fd = -1;

    return fd;
}

int houdini_hook_open(const char *path, int flags, int mode)
{
    int fd = -1;

    if (fd == -1) {
        fd = TEMP_FAILURE_RETRY(open(path, flags, mode));
    }

    return fd;
}
