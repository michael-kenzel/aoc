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
	auto matches =
		std::views::istream<card>(std::cin) | std::views::transform(&card::matches);

#if 0
	auto sum = std::ranges::fold_left(
		matches | std::views::transform([](int m) { return m ? 1 << (m - 1) : 0; }), 0LL, std::plus());
#else
	auto card_matches = matches | std::ranges::to<std::vector>();

	std::vector<int> delta(card_matches.size());

	long long sum = 0;
	int curr_cards = 1;

	for (auto [i, m] : std::views::enumerate(card_matches))
	{
		sum += curr_cards;
		delta[i + m] -= curr_cards;
		curr_cards += curr_cards + delta[i];
	}
#endif

	std::cout << sum << '\n';
}
