import std;

template <char c>
std::istream& skip_until(std::istream& in)
{
	return in.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

int main()
{
	auto times = std::views::istream<long long>(std::cin >> skip_until<':'>) | std::ranges::to<std::vector>();
	std::cin.clear();
	auto distances = std::views::istream<long long>(std::cin >> skip_until<':'>) | std::ranges::to<std::vector>();

	auto score = std::ranges::fold_left(
		std::views::zip(times, distances) |
		std::views::transform([](auto race)
		{
			auto [t, d] = race;

			// x * (t - x) > d
			auto dist = [=](auto x) { return x * (t - x); };

			auto min = *std::ranges::lower_bound(std::views::iota(0, t / 2), d, std::less_equal<>(), dist);
			auto max = *std::ranges::upper_bound(std::views::iota(t / 2, t), d, std::greater_equal<>(), dist);

			return max - min;
		}), 1LL, std::multiplies());

	std::cout << score << '\n';
}
