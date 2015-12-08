#include<stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

typedef struct Tx_log_entry {
    int txid;
    char op[8];
} Tx_log_entry;

int main(void)
{

    int fd = open("seek", O_CREAT | O_RDWR | O_APPEND, 0666);

    Tx_log_entry entry;
    int i;

    for(i=0; i<10; i++) {
        entry.txid = i;
        sprintf(entry.op, "%s", "hello");
        write(fd, (char *)&entry, sizeof(entry));
        fsync(fd);
    }


    for(i=1; lseek(fd, -i*sizeof(entry), SEEK_END) >= 0; i++) {
        read(fd, (char *)&entry, sizeof(entry));
        printf("%d %s\n", entry.txid, entry.op);
    }

    close(fd);

    return 0;
} 


