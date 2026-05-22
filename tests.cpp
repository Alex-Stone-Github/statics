#include <algorithm>
#include <gtest/gtest.h>
#include <print>
#include <vector>
#include <algorithm>
#include <vector>
#include <array>

#include "wrap.h"
#include "wrap_internal.h"

TEST(AreRequirementsSatisfied, SetDifference) {
	std::array<char const*, 3> first = {"one", "two", "three"};
	std::array<char const*, 3> second = {"f", "three", "de"};
	std::array<char const*, 1> three = {"f"};

	std::vector<char const*> needed = {"VK_LAYER_KHRONOS_validation"};
	std::vector<char const*> have = {
		"VK_LAYER_MESA_device_select",
		"VK_LAYER_KHRONOS_validation",
		"VKalsekdfj",
	};

	EXPECT_FALSE(areRequirementsSatisfied(first, second));
	EXPECT_TRUE(areRequirementsSatisfied(three, second));
	EXPECT_TRUE(areRequirementsSatisfied(needed, have));
}

TEST(CommonSense, Pass) {
	EXPECT_EQ(3 * 3, 9);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
