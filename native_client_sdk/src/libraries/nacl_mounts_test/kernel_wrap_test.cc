/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "nacl_mounts/kernel_proxy.h"
#include "nacl_mounts/kernel_intercept.h"
#include "nacl_mounts/kernel_wrap.h"
#include "kernel_proxy_mock.h"

using ::testing::StrEq;
using ::testing::_;

namespace {

#define COMPARE_FIELD(f) \
  if (arg->f != statbuf->f) { \
    *result_listener << "mismatch of field \""#f"\". " \
      "expected: " << statbuf->f << \
      " actual: " << arg->f; \
    return false; \
  }

MATCHER_P(IsEqualToStatbuf, statbuf, "") {
  COMPARE_FIELD(st_dev);
  COMPARE_FIELD(st_ino);
  COMPARE_FIELD(st_mode);
  COMPARE_FIELD(st_nlink);
  COMPARE_FIELD(st_uid);
  COMPARE_FIELD(st_gid);
  COMPARE_FIELD(st_rdev);
  COMPARE_FIELD(st_size);
  COMPARE_FIELD(st_atime);
  COMPARE_FIELD(st_mtime);
  COMPARE_FIELD(st_ctime);
  return true;
}

#undef COMPARE_FIELD

ACTION_P(SetStat, statbuf) {
  memset(arg1, 0, sizeof(struct stat));
  arg1->st_dev = statbuf->st_dev;
  arg1->st_ino = statbuf->st_ino;
  arg1->st_mode = statbuf->st_mode;
  arg1->st_nlink = statbuf->st_nlink;
  arg1->st_uid = statbuf->st_uid;
  arg1->st_gid = statbuf->st_gid;
  arg1->st_rdev = statbuf->st_rdev;
  arg1->st_size = statbuf->st_size;
  arg1->st_atime = statbuf->st_atime;
  arg1->st_mtime = statbuf->st_mtime;
  arg1->st_ctime = statbuf->st_ctime;
  return 0;
}

void MakeDummyStatbuf(struct stat* statbuf) {
  memset(&statbuf[0], 0, sizeof(struct stat));
  statbuf->st_dev = 1;
  statbuf->st_ino = 2;
  statbuf->st_mode = 3;
  statbuf->st_nlink = 4;
  statbuf->st_uid = 5;
  statbuf->st_gid = 6;
  statbuf->st_rdev = 7;
  statbuf->st_size = 8;
  statbuf->st_atime = 9;
  statbuf->st_mtime = 10;
  statbuf->st_ctime = 11;
}

}  // namespace

TEST(KernelWrap, access) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, access(StrEq("access"), 12)).Times(1);
  access("access", 12);
}

TEST(KernelWrap, chdir) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, chdir(StrEq("chdir"))).Times(1);
  chdir("chdir");
}

TEST(KernelWrap, chmod) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, chmod(StrEq("chmod"), 23)).Times(1);
  chmod("chmod", 23);
}

TEST(KernelWrap, close) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, close(34)).Times(1);
  close(34);
}

TEST(KernelWrap, dup) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, dup(123)).Times(1);
  dup(123);
}

TEST(KernelWrap, fstat) {
  KernelProxyMock mock;
  ki_init(&mock);
  struct stat in_statbuf;
  MakeDummyStatbuf(&in_statbuf);
  EXPECT_CALL(mock, fstat(234, _))
      .Times(1)
      .WillOnce(SetStat(&in_statbuf));
  struct stat out_statbuf;
  fstat(234, &out_statbuf);
  EXPECT_THAT(&in_statbuf, IsEqualToStatbuf(&out_statbuf));
}

TEST(KernelWrap, fsync) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, fsync(345)).Times(1);
  fsync(345);
}

TEST(KernelWrap, getcwd) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, getcwd(StrEq("getcwd"), 1)).Times(1);
  char buffer[] = "getcwd";
  getcwd(buffer, 1);
}

TEST(KernelWrap, getdents) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, getdents(456, NULL, 567)).Times(1);
  getdents(456, NULL, 567);
}

// gcc gives error: getwd is deprecated.
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
TEST(KernelWrap, getwd) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, getwd(StrEq("getwd"))).Times(1);
  char buffer[] = "getwd";
  getwd(buffer);
}
#if defined(__GNUC__)
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#endif

TEST(KernelWrap, isatty) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, isatty(678)).Times(1);
  isatty(678);
}

TEST(KernelWrap, lseek) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, lseek(789, 891, 912)).Times(1);
  lseek(789, 891, 912);
}

TEST(KernelWrap, mkdir) {
  KernelProxyMock mock;
  ki_init(&mock);
#if defined(WIN32)
  EXPECT_CALL(mock, mkdir(StrEq("mkdir"), 0777)).Times(1);
  mkdir("mkdir");
#else
  EXPECT_CALL(mock, mkdir(StrEq("mkdir"), 1234)).Times(1);
  mkdir("mkdir", 1234);
#endif
}

TEST(KernelWrap, mount) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock,
      mount(StrEq("mount1"), StrEq("mount2"), StrEq("mount3"), 2345, NULL))
      .Times(1);
  mount("mount1", "mount2", "mount3", 2345, NULL);
}

TEST(KernelWrap, open) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, open(StrEq("open"), 3456)).Times(1);
  open("open", 3456);
}

TEST(KernelWrap, read) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, read(4567, NULL, 5678)).Times(1);
  read(4567, NULL, 5678);
}

TEST(KernelWrap, remove) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, remove(StrEq("remove"))).Times(1);
  remove("remove");
}

TEST(KernelWrap, rmdir) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, rmdir(StrEq("rmdir"))).Times(1);
  rmdir("rmdir");
}

TEST(KernelWrap, stat) {
  KernelProxyMock mock;
  ki_init(&mock);
  struct stat in_statbuf;
  MakeDummyStatbuf(&in_statbuf);
  EXPECT_CALL(mock, stat(StrEq("stat"), _))
      .Times(1)
      .WillOnce(SetStat(&in_statbuf));
  struct stat out_statbuf;
  stat("stat", &out_statbuf);
  EXPECT_THAT(&in_statbuf, IsEqualToStatbuf(&out_statbuf));
}

TEST(KernelWrap, umount) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, umount(StrEq("umount"))).Times(1);
  umount("umount");
}

TEST(KernelWrap, unlink) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, unlink(StrEq("unlink"))).Times(1);
  unlink("unlink");
}

TEST(KernelWrap, write) {
  KernelProxyMock mock;
  ki_init(&mock);
  EXPECT_CALL(mock, write(6789, NULL, 7891)).Times(1);
  write(6789, NULL, 7891);
}
