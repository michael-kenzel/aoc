import std;

template <char c>
std::istream& skip_until(std::istream& in)
{
	return in.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

struct mapping { long long source, dest, length; };

std::istream& operator >>(std::istream& in, mapping& mapping)
{
	return in >> mapping.dest >> mapping.source >> mapping.length;
}

void apply_mapping(std::ranges::random_access_range auto&& out, std::ranges::random_access_range auto&& in, std::ranges::input_range auto&& mapping)
{
	for (auto [source, dest, length] : mapping)
	{
		auto begin = std::ranges::lower_bound(in, source - 1);
		auto end = std::ranges::upper_bound(in, source + length);
		auto obegin = std::ranges::begin(out) + (begin - std::ranges::begin(in));
		std::ranges::transform(begin, end, obegin, [=](auto value) { return value - source + dest; });
	}
}

int main()
{
	auto& in = std::cin;

	auto seeds =
		std::views::istream<long long>(in >> skip_until<':'>) | std::ranges::to<std::vector>();

	in.clear(in.rdstate() & ~std::ios::failbit);

	std::vector<long long> buffer(seeds.size());

	auto a = &seeds;
	auto b = &buffer;

	while (in >> skip_until<':'>)
	{
		std::ranges::sort(*a);
		std::ranges::copy(*a, std::ranges::begin(*b));
		apply_mapping(*b, *a, std::views::istream<mapping>(in));
		std::swap(a, b);
		in.clear(in.rdstate() & ~std::ios::failbit);
	}


	auto min = *std::ranges::min_element(*a);

	std::cout << min << '\n';
}
