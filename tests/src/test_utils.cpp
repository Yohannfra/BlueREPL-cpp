#include "utils.hpp"

#include <gtest/gtest.h>

TEST(test_utils, test_isMacAddress)
{
    // random
    EXPECT_FALSE(Utils::isMacAddress("lol"));

    // good upppercase
    EXPECT_TRUE(Utils::isMacAddress("FC32B125-4E7F-DD16-A05B-9E8025D2FA28"));

    // good lower case
    EXPECT_TRUE(Utils::isMacAddress("fc32b125-4e7f-dd16-a05b-9e8025d2fa28"));

    // one char more
    EXPECT_FALSE(Utils::isMacAddress("fc32b125-4e7f-dd16-a05b-9e8025d2fa28c"));

    // one char less
    EXPECT_FALSE(Utils::isMacAddress("fc32b125-4e7f-dd16-a05b-9e8025d2fa2"));

    // good mix lower and upppercase
    EXPECT_TRUE(Utils::isMacAddress("FC32B125-4e7f-dd16-A05B-9e8025d2fa28"));

    // bad char
    EXPECT_FALSE(Utils::isMacAddress("jC32B125-4e7f-dd16-A05B-9e8025d2fa28"));

    // empty
    EXPECT_FALSE(Utils::isMacAddress(""));
}

TEST(test_utils, test_isBinaryNumber)
{
    // empty
    EXPECT_FALSE(Utils::isBinaryNumber(""));

    // bad data
    EXPECT_FALSE(Utils::isBinaryNumber("abc"));
    EXPECT_FALSE(Utils::isBinaryNumber("123"));

    // ok
    EXPECT_TRUE(Utils::isBinaryNumber("10"));
    EXPECT_TRUE(Utils::isBinaryNumber("01"));
    EXPECT_TRUE(Utils::isBinaryNumber("0111000110"));
    EXPECT_TRUE(Utils::isBinaryNumber("0011011011"));
}

TEST(test_utils, test_isNumber)
{
    // positive number
    EXPECT_TRUE(Utils::isNumber("12"));
    EXPECT_TRUE(Utils::isNumber("1"));

    // negative number
    EXPECT_FALSE(Utils::isNumber("-12"));

    // Zero
    EXPECT_TRUE(Utils::isNumber("0"));

    // empty
    EXPECT_FALSE(Utils::isNumber(""));

    // letter after
    EXPECT_FALSE(Utils::isNumber("12a"));

    // letter before
    EXPECT_FALSE(Utils::isNumber("a12"));
}

TEST(test_utils, test_isAsciiWord)
{
    // lowercase
    EXPECT_TRUE(Utils::isAsciiWord("hello"));

    // upppercase
    EXPECT_TRUE(Utils::isAsciiWord("HELLO"));

    // underscore
    EXPECT_TRUE(Utils::isAsciiWord("_"));

    // underscore and chars
    EXPECT_TRUE(Utils::isAsciiWord("Hello_World"));

    // bad chars
    EXPECT_FALSE(Utils::isAsciiWord("Hello-World"));
    EXPECT_FALSE(Utils::isAsciiWord("HelloWorld1"));

    // space
    EXPECT_FALSE(Utils::isAsciiWord("Hello World"));
}

TEST(test_utils, test_parseHexStr)
{
    // text
    auto tmp = Utils::parseHexStr("lol");
    EXPECT_FALSE(tmp.has_value());

    // valid hex without 0x
    tmp = Utils::parseHexStr("12");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ(tmp.value().at(0), 0x12);
    EXPECT_EQ(tmp.value().size(), 1);

    // valid hex with 0x
    tmp = Utils::parseHexStr("0x44");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ(tmp.value().at(0), 0x44);
    EXPECT_EQ(tmp.value().size(), 1);

    // multiple values
    tmp = Utils::parseHexStr("0x12 0x22 0x45 0x56");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ(tmp.value().at(0), 0x12);
    EXPECT_EQ(tmp.value().at(1), 0x22);
    EXPECT_EQ(tmp.value().at(2), 0x45);
    EXPECT_EQ(tmp.value().at(3), 0x56);
    EXPECT_EQ(tmp.value().size(), 4);

    // Max value
    tmp = Utils::parseHexStr("0xff");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ((std::uint8_t)tmp.value().at(0), 0xff);

    // Zero
    tmp = Utils::parseHexStr("0x00");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ((std::uint8_t)tmp.value().at(0), 0x00);

    // Too big
    tmp = Utils::parseHexStr("0xff1");
    EXPECT_FALSE(tmp.has_value());
}

TEST(test_utils, test_parseBinStr)
{
    // text
    auto tmp = Utils::parseBinStr("lol");
    EXPECT_FALSE(tmp.has_value());

    // 1
    tmp = Utils::parseBinStr("1");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ((std::uint8_t)tmp.value().at(0), 0b1);

    // 0
    tmp = Utils::parseBinStr("0");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ((std::uint8_t)tmp.value().at(0), 0b0);

    // ff
    tmp = Utils::parseBinStr("11111111");
    EXPECT_TRUE(tmp.has_value());
    EXPECT_EQ((std::uint8_t)tmp.value().at(0), 0b11111111);

    // decimal number
    tmp = Utils::parseBinStr("123");
    EXPECT_FALSE(tmp.has_value());
}

TEST(test_utils, test_tokenizeLine)
{
    // empty
    auto tmp = Utils::tokenizeLine("");
    EXPECT_TRUE(tmp.empty());

    // hexa
    tmp = Utils::tokenizeLine("0x12");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["0x12"], BleDataType::HEXADECIMAL);

    // HEXA
    tmp = Utils::tokenizeLine("0X33");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["0X33"], BleDataType::HEXADECIMAL);

    // bin
    tmp = Utils::tokenizeLine("0b1100");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["0b1100"], BleDataType::BINARY);

    // BIN
    tmp = Utils::tokenizeLine("0B1100");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["0B1100"], BleDataType::BINARY);

    // Ascii
    tmp = Utils::tokenizeLine("'hello'");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["'hello'"], BleDataType::ASCII);

    // Ascii invalid (missing closing ')
    tmp = Utils::tokenizeLine("'hello");
    EXPECT_TRUE(tmp.empty());

    // Ascii tricky
    tmp = Utils::tokenizeLine("'0x44'");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["'0x44'"], BleDataType::ASCII);

    // Decimal
    tmp = Utils::tokenizeLine("221");
    EXPECT_FALSE(tmp.empty());
    ASSERT_EQ(tmp["221"], BleDataType::DECIMAL);

    // Invalid
    tmp = Utils::tokenizeLine("Hello");
    EXPECT_TRUE(tmp.empty());
    tmp = Utils::tokenizeLine("0c53");
    EXPECT_TRUE(tmp.empty());
}
