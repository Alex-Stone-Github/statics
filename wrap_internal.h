#pragma once

#include <span>

/// Check all of the required 'strings' in required are fullfilled by available
bool areRequirementsSatisfied(
	std::span<char const*> required, std::span<char const*> available);
