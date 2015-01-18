#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "gtest/gtest.h"

using namespace std;

// gettimeofday
TEST(SystemCallTest, GetTimeOfDay)
{
  int count = 1000* 1000;
//  struct timeval begin_time;
//  gettimeofday(&begin_time, NULL);
  struct timeval tmp_time;
  for (int i = 0; i < count; i++)
    gettimeofday(&tmp_time, NULL);
  cout << "gettimeofday count is " << count << endl;

//  struct timeval end_time;
//  gettimeofday(&end_time, NULL);
//  cout << (end_time.tv_sec * 1000 + end_time.tv_usec / 1000) - \
//    (begin_time.tv_sec * 1000 + begin_time.tv_usec / 1000) << endl;
}

// fork
// 系统对用户创建的进程数限制个数不定 可用ulimit -u 查看
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
// 正常情况下创建线程的性能比创建进程快一个数量级，每秒10W
TEST(SystemCallTest, Pthread)
{
  pthread_t pid;
  int count = 400;
  for (int i = 0; i < count; i++)
    pthread_create(&pid, NULL, test_thread, NULL);
  cout << "pthread count is " <<  count << endl;
//  pthread_join(pid, NULL);
}

