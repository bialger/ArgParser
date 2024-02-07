#ifndef ARGPARSERUNITTESTSUITE_HPP_
#define ARGPARSERUNITTESTSUITE_HPP_

#include <filesystem>
#include <string>

#include <gtest/gtest.h>

struct ArgParserUnitTestSuite : public testing::Test { // special test structure
  const std::string kTemporaryDirectoryName = "./gtest_tmp";
  const std::string kTemporaryFileName = kTemporaryDirectoryName + "/temp_file";

  void SetUp() override; // method that is called at the beginning of every test

  void TearDown() override; // method that is called at the end of every test
};

#endif //ARGPARSERUNITTESTSUITE_HPP_
