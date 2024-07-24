#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <unistd.h>

int main(int argc, char **argv) {
    // Variable declarations
    char *sh;
    gid_t gid;
    uid_t uid;
    char *arg[2];

    // Check if the input argument is equal to 423
    if (atoi(argv[1]) == 423) {
        // Duplicate the string "/bin/sh" and store it in sh
        sh = strdup("/bin/sh");

        // Get the effective group ID and user ID of the calling process
        gid = getegid();
        uid = geteuid();

        // Set the effective group and user IDs
        setresgid(gid, gid, gid);
        setresuid(uid, uid, uid);

        // Prepare arguments for execv
        arg[0] = "sh";
        arg[1] = NULL;

        // Execute the shell
        execv("/bin/sh", arg);
    } else {
        // Print "No !" to stderr if the input argument is not equal to 423
        fwrite("No !\n", 1, 5, stderr);
    }

    // Return 0 to indicate successful execution (optional)
    return 0;
}
