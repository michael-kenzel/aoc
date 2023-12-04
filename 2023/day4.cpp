import std;

template <char c>
std::istream& skip_until(std::istream& in)
{
	return in.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

std::istream& operator >>(std::istream& in, std::bitset<100>& numbers)
{
	std::istream::sentry s(in);

	if (s)
	{
		numbers.reset();

		for (int n; in >> n;)
			numbers[n] = true;

		if (numbers.any())
			in.clear(in.rdstate() & ~std::ios::failbit);
	}

	return in;
}

struct card
{
	std::bitset<100> win;
	std::bitset<100> have;

	int matches() const { return static_cast<int>((win & have).count()); }
};

std::istream& operator >>(std::istream& in, card& card)
{
	return in >> skip_until<':'> >> card.win >> skip_until<'|'> >> card.have;
}

int main()
{
	auto card_matches =
		std::views::istream<card>(std::cin) |
		std::views::transform(&card::matches) |
		std::ranges::to<std::vector>();

	auto card_values =
		card_matches |
		std::views::transform([](int n) { return n ? 1 << (n - 1) : 0; });

#if 0
	auto sum = std::reduce(std::begin(card_values), std::end(card_values), 0LL);
#else
	std::vector<int> copies(card_matches.size(), 1);

	for (auto [n, matches] : std::views::enumerate(card_matches))
	{
		for (auto i = n + 1; i <= n + matches; ++i)
			copies[i] += copies[n];
	}

	auto sum = std::reduce(std::begin(copies), std::end(copies), 0LL);
#endif

	std::cout << sum << '\n';
}
