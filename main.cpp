#include <optional>
#include <print>
#include <array>
#include <cstdlib>
#include <cstdlib>
#include <span>

#include "wrap.h"

/// What does a writer look like?
template <typename T>
concept Writable = requires(T example, std::span<std::byte> bytes) {
	{ example.write(bytes) } -> std::same_as<size_t>;
};


/// Let's create an example writer object
struct Stdout {
	size_t write(std::span<std::byte> bytes) {
		return 0;
	}
};


/// Lets take in a writer and do some stuff with it
void writesomestuff(Writable auto writer) {
	// Get some bytes and write some stuff
	std::array<std::byte, 3> arr = {std::byte{3}, std::byte{4}, std::byte{5}};
	writer.write(arr);

	nothing(); // from wrap h
}

int main() {
	// Print
	std::println("Hello, World!");
	std::println("what does this mean");

	// This is basically a null
	std::optional<int> something = std::nullopt;
	something.reset();

	// Examples
	Stdout output{};
	writesomestuff(output);

	return EXIT_SUCCESS;
}
