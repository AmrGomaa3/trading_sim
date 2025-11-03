#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "../test_data/"
#endif


#include "../include/util.h"
#include <gtest/gtest.h>
#include <string>
#include <stdexcept>


TEST(TestUtil, FailToOpenFile) {
    EXPECT_THROW(read_file(std::string(TEST_DATA_DIR) + "invalid_name.csv"), std::runtime_error);
}


TEST(TestUtil, CorruptDataError) {
    EXPECT_THROW(read_file(std::string(TEST_DATA_DIR) + "invalid_data.csv"), std::runtime_error);
}


TEST(TestUtil, ValidFileWithNoErrors) {
    EXPECT_NO_THROW(read_file(std::string(TEST_DATA_DIR) + "valid_data.csv"));
}

