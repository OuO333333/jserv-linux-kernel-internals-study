#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

void *hi_prio_t1(void *p)
{
    int fd;
    char errmsg[256];
    if ((fd = open("/dev/mychardev-0", O_RDONLY)) < 0)
    {
        strerror_r(errno, errmsg, 256);
        printf("T1 open /dev/demo failed. error:%s\n", errmsg);
        return NULL;
    }
    struct timeval startAt, stopAt;
    gettimeofday(&startAt, NULL);
    printf("T1 start at %ld.%09ld\n", startAt.tv_sec, startAt.tv_usec);
    sleep(3);
    gettimeofday(&stopAt, NULL);
    double elapsed = (stopAt.tv_sec - startAt.tv_sec) + (stopAt.tv_usec - startAt.tv_usec) / 1000000000.0;
    printf("T1 stop at %ld.%09ld. elapse: %.9f seconds.\n",
           stopAt.tv_sec, stopAt.tv_usec, elapsed);
    close(fd);
    return NULL;
}

void *low_prio_t2(void *p)
{

    char buf[20];
    char errmsg[256];
    int fd, len;
    struct timeval startAt, stopAt;
    gettimeofday(&startAt, NULL);

    printf("T2 start at %ld.%09ld\n", startAt.tv_sec, startAt.tv_usec);
    sleep(1);
    if ((fd = open("/dev/mychardev-0", O_RDONLY)) < 0)
    {
        strerror_r(errno, errmsg, 256);
        printf("T2 open /dev/demo failed. error:%s\n", errmsg);
        return NULL;
    }

    if ((len = read(fd, buf, 19)) < 0)
    {
        strerror_r(errno, errmsg, 256);
        printf("T2 read failed. error:%s\n", errmsg);
        return NULL;
    }
    buf[len] = '\0';

    gettimeofday(&stopAt, NULL);
    double elapsed = (stopAt.tv_sec - startAt.tv_sec) + (stopAt.tv_usec - startAt.tv_usec) / 1000000000.0;
    printf("T2 stop at %ld.%09ld. elapse: %.9f seconds.\n",
           stopAt.tv_sec, stopAt.tv_usec, elapsed);
    close(fd);
    return NULL;
}

void run_t1()
{
    pthread_t t1;
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_create(&t1, &attr, hi_prio_t1, NULL);

    pthread_join(t1, NULL);
}
void run_t2()
{
    pthread_t t2;
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_create(&t2, &attr, low_prio_t2, NULL);

    pthread_join(t2, NULL);
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Incorrect format.\n");
        return 0;
    }
    if (argc == 1)
    {
        pthread_t t1, t2;
        pthread_attr_t attr;
        struct sched_param param;

        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr, SCHED_RR);

        param.sched_priority = 50;
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_create(&t1, &attr, hi_prio_t1, NULL);

        param.sched_priority = 30;
        pthread_attr_setschedparam(&attr, &param);
        pthread_create(&t2, &attr, low_prio_t2, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_attr_destroy(&attr);
        return 0;
    }
    if (strcmp(argv[1], "1") == 0)
    {
        run_t1();
        return 0;
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        run_t2();
        return 0;
    }
    return 0;
}
