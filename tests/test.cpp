#include "pch.h"
#include "subnet_filter.h"
#include "range_filter.h"
#include "and_filter.h"
#include "factory_filter.h"
#include "create_filter.h"   
#include "process_stream.h" 
#include "filter_type.h"
#include "filter_rule.h"

TEST(SubnetFilterTest, ValidCIDR) {
    EXPECT_NO_THROW(subnet_filter("192.168.1.0/24"));
    EXPECT_NO_THROW(subnet_filter("0.0.0.0/0"));
    EXPECT_NO_THROW(subnet_filter("255.255.255.255/32"));
    EXPECT_NO_THROW(subnet_filter("10.0.0.0/8"));
}

TEST(SubnetFilterTest, InvalidCIDR) {
    EXPECT_THROW(subnet_filter("192.168.1.1"), std::runtime_error);
    EXPECT_THROW(subnet_filter("192.168.1.0/33"), std::runtime_error);
    EXPECT_THROW(subnet_filter("192.168.1.0/-1"), std::runtime_error);
    EXPECT_THROW(subnet_filter("valera"), std::runtime_error);
    EXPECT_THROW(subnet_filter(""), std::runtime_error);
}

TEST(SubnetFilterTest, Match) {
    subnet_filter subnet("192.168.1.0/24");

    EXPECT_TRUE(subnet.match("192.168.1.1"));
    EXPECT_TRUE(subnet.match("192.168.1.100"));
    EXPECT_TRUE(subnet.match("192.168.1.255"));

    EXPECT_FALSE(subnet.match("192.168.2.1"));
    EXPECT_FALSE(subnet.match("10.0.0.1"));
    EXPECT_FALSE(subnet.match("8.8.8.8"));
}

TEST(SubnetFilterTest, EdgeCases) {
    subnet_filter exact("192.168.1.1/32");
    EXPECT_TRUE(exact.match("192.168.1.1"));
    EXPECT_FALSE(exact.match("192.168.1.2"));

    subnet_filter all("0.0.0.0/0");
    EXPECT_TRUE(all.match("192.168.1.1"));
    EXPECT_TRUE(all.match("8.8.8.8"));

    subnet_filter mask1("128.0.0.0/1");
    EXPECT_TRUE(mask1.match("192.168.1.1"));
    EXPECT_FALSE(mask1.match("64.0.0.1"));
}

TEST(RangeFilterTest, ValidRange) {
    EXPECT_NO_THROW(range_filter("10.0.0.1-10.0.0.255"));
    EXPECT_NO_THROW(range_filter("192.168.1.1-192.168.1.1"));
    EXPECT_NO_THROW(range_filter("0.0.0.0-255.255.255.255"));
}

TEST(RangeFilterTest, InvalidRange) {
    EXPECT_THROW(range_filter("10.0.0.1"), std::runtime_error);
    EXPECT_THROW(range_filter("10.0.0.777-10.0.0.1"), std::runtime_error);
    EXPECT_THROW(range_filter("vityaaka"), std::runtime_error);
    EXPECT_THROW(range_filter(""), std::runtime_error);
}

TEST(RangeFilterTest, Match) {
    range_filter range("10.0.0.1-10.0.0.255");

    EXPECT_TRUE(range.match("10.0.0.1"));
    EXPECT_TRUE(range.match("10.0.0.50"));
    EXPECT_TRUE(range.match("10.0.0.255"));

    EXPECT_FALSE(range.match("10.0.1.1"));
    EXPECT_FALSE(range.match("9.255.255.255"));
}

TEST(RangeFilterTest, EdgeCases) {
    range_filter single("192.168.1.1-192.168.1.1");
    EXPECT_TRUE(single.match("192.168.1.1"));
    EXPECT_FALSE(single.match("192.168.1.2"));

    range_filter full("0.0.0.0-255.255.255.255");
    EXPECT_TRUE(full.match("192.168.1.1"));
    EXPECT_TRUE(full.match("0.0.0.0"));
    EXPECT_TRUE(full.match("255.255.255.255"));
}


TEST(AndFilterTest, SingleFilter) {
    and_filter and_f;
    and_f.add_filter(std::make_unique<subnet_filter>("192.168.1.0/24"));

    EXPECT_TRUE(and_f.match("192.168.1.1"));
    EXPECT_FALSE(and_f.match("192.168.2.1"));
}

TEST(AndFilterTest, MultipleFilters) {
    and_filter and_f;
    and_f.add_filter(std::make_unique<subnet_filter>("192.168.1.0/24"));
    and_f.add_filter(std::make_unique<range_filter>("192.168.1.10-192.168.1.20"));

    EXPECT_TRUE(and_f.match("192.168.1.15"));
    EXPECT_FALSE(and_f.match("192.168.1.5"));
    EXPECT_FALSE(and_f.match("192.168.2.15"));
}

TEST(AndFilterTest, ThreeFilters) {
    and_filter and_f;
    and_f.add_filter(std::make_unique<subnet_filter>("192.168.1.0/24"));
    and_f.add_filter(std::make_unique<subnet_filter>("192.168.1.0/28"));
    and_f.add_filter(std::make_unique<range_filter>("192.168.1.5-192.168.1.10"));

    EXPECT_TRUE(and_f.match("192.168.1.5"));
    EXPECT_TRUE(and_f.match("192.168.1.10"));
    EXPECT_FALSE(and_f.match("192.168.1.4"));
    EXPECT_FALSE(and_f.match("192.168.1.11"));
}

TEST(AndFilterTest, Empty) {
    and_filter and_f;
    EXPECT_TRUE(and_f.match("any.ip"));
}

TEST(BrokenDataTest, InvalidSubnetFormats) {
    EXPECT_THROW(subnet_filter(""), std::runtime_error);
    EXPECT_THROW(subnet_filter("192.168.1.1"), std::runtime_error);
    EXPECT_THROW(subnet_filter("192.168.1.0/33"), std::runtime_error);
    EXPECT_THROW(subnet_filter("not-an-ip/24"), std::runtime_error);
}

TEST(BrokenDataTest, InvalidRangeFormats) {
    EXPECT_THROW(range_filter(""), std::runtime_error);
    EXPECT_THROW(range_filter("10.0.0.1"), std::runtime_error);
    EXPECT_THROW(range_filter("10.0.0.255-10.0.0.1"), std::runtime_error);
    EXPECT_THROW(range_filter("abc-def"), std::runtime_error);
}

TEST(EdgeCasesTest, SubnetBoundaries) {
    subnet_filter subnet("192.168.1.0/24");
    EXPECT_TRUE(subnet.match("192.168.1.0"));
    EXPECT_TRUE(subnet.match("192.168.1.255"));

    subnet_filter exact("192.168.1.1/32");
    EXPECT_TRUE(exact.match("192.168.1.1"));
}

TEST(EdgeCasesTest, RangeBoundaries) {
    range_filter range("192.168.1.1-192.168.1.10");
    EXPECT_TRUE(range.match("192.168.1.1"));
    EXPECT_TRUE(range.match("192.168.1.10"));
    EXPECT_FALSE(range.match("192.168.1.0"));
    EXPECT_FALSE(range.match("192.168.1.11"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}