#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "gtest/gtest.h"

using namespace std;

#define IterFrame(count, test_fun) \
  do { \
    int iter_count = count; \
    for (int i = 0; i < iter_count; i++) \
      test_fun; \
    cout << #test_fun << " count: " << iter_count << endl;\
  }while(0)\
  

// gettimeofday
TEST(SystemCallTest, GetTimeOfDay)
{
//  struct timeval begin_time;
//  gettimeofday(&begin_time, NULL);
  struct timeval tmp_time;
  IterFrame(1000* 1000, gettimeofday(&tmp_time, NULL));

//  struct timeval end_time;
//  gettimeofday(&end_time, NULL);
//  cout << (end_time.tv_sec * 1000 + end_time.tv_usec / 1000) - \
//    (begin_time.tv_sec * 1000 + begin_time.tv_usec / 1000) << endl;
}

// fork
// 系统对用户创建的进程数限制个数不定 可用ulimit -u 查看
// sysctl kernel.pid_max
// 性能：60～8000/s
// 瓶颈：页表复制 <2.6
// 并发：单CPU进程切换 30～100W/s
// K级以上稳定在30W/s
TEST(SystemCallTest, Fork)
{
  signal(SIGCHLD, SIG_IGN);
  int count = 400;
  int child_pid = 0;
  for (int i = 0; i < count; i++)
  {
    child_pid = fork();
    if(child_pid == 0) {
      sleep(10);
      exit(0);
    }
//    else if (child_pid < 0)
//    {
//      perror("fork error!");
//    }
  }
  cout << "fork count is " << count << endl;
}

void * test_thread(void*)
{
  sleep(10);
  return NULL;
}

// pthread
// sysctl kernel.threads-max
// 性能：5~10W/s 随着并发数的增加而降低，并发1W约5W/s
// 并发：单CPU线程切换50～150W/s K级以上并发50W/s
// 非COW情况下创建线程的性能比创建进程快一个数量级，每秒10W
// 但是在使用COW技术时两者性能相差不大
// 创建进程主要的开销就是页表的复制
TEST(SystemCallTest, Pthread)
{
  // ulimit -s
  // pthread_attr_setstacksize
  // 8K
  pthread_t pid;
  IterFrame(400, pthread_create(&pid, NULL, test_thread, NULL));

//  pthread_join(pid, NULL);
}

// malloc 1024
TEST(SystemCallTest, Malloc1k)
{
  IterFrame(100*1000, malloc(1024));
}

// malloc 1024*1024
TEST(SystemCallTest, Malloc1m)
{
  IterFrame(1000, malloc(1024*1024));
}

// memset 1024
TEST(SystemCallTest, Memset1k)
{
  char test_array[1024];
  IterFrame(100*1000, memset(test_array, 0, sizeof(test_array)));

}

// memset 1024*1024
TEST(SystemCallTest, Memset1m)
{
  char test_array[1024*1024];
  IterFrame(100*1000, memset(test_array, 0, sizeof(test_array)));
}

// malloc && memset 1024
TEST(SystemCallTest, Mallset1k)
{
  char* p = NULL;
  IterFrame(100*1000, p = (char*)malloc(1024);\
      memset(p, 0, 1024));
}

// malloc && memset 1024*1024
TEST(SystemCallTest, Mallset1m)
{
  char* p = NULL;
  IterFrame(10000, p = (char*)malloc(1024*1024);\
      memset(p, 0, 1024*1024));
}

// memcpy 1024
TEST(SystemCallTest, Memcpy1k)
{
  char test_array1[1024] = "";
  char test_array2[1024];
  IterFrame(100*1000, memcpy(test_array2, test_array1, sizeof(test_array2)));
}

static char test_array3[1024*1024] = "";
static char test_array4[1024*1024];
// memcpy 1024*1024
TEST(SystemCallTest, Memcpy1M)
{
  IterFrame(100*1000, memcpy(test_array4, test_array3, sizeof(test_array4)));
}

