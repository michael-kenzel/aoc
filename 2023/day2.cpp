import std;

template <char... C>
std::istream& skip_until(std::istream& in)
{
	while(true)
		if (int c = in.get(); !in || ((static_cast<char>(c) == C) || ...))
			break;
	return in;
}

struct cubes
{
	int r = 0, g = 0, b = 0;

	void add_r(int count) { r = std::max(r, count); }
	void add_g(int count) { g = std::max(g, count); }
	void add_b(int count) { b = std::max(b, count); }

	auto power() const { return r * g * b; }

	friend bool operator <=(const cubes& x, const cubes& y) { return x.r <= y.r && x.g <= y.g && x.b <= y.b; }
};

std::istream& operator >>(std::istream& in, cubes& cubes)
{
	std::istream::sentry s(in);

	if (!in)
		return in;

	cubes = {};

	while (true)
	{
		int count;
		char color;

		if (!(in >> count >> color))
		{
			in.clear(in.rdstate() & ~std::ios::failbit);
			break;
		}

		switch (color)
		{
			case 'r': cubes.add_r(count); break;
			case 'g': cubes.add_g(count); break;
			case 'b': cubes.add_b(count); break;
			default: in.setstate(std::ios::failbit); return in;
		}

		in >> skip_until<',', ';', '\n'>;
	}

	return in;
}

struct game : cubes { int id; };

std::istream& operator >>(std::istream& in, game& game)
{
	return in >> skip_until<' '> >> game.id >> skip_until<':'> >> static_cast<cubes&>(game);
}

int main()
{
	auto games = std::views::istream<game>(std::cin);

#if 0
	auto sum = std::ranges::fold_left(
		games | std::views::filter([](const game& game) { return game <= cubes { .r = 12, .g = 13, .b = 14 }; })
		      | std::views::transform(&game::id), 0LL, std::plus());
#else
	auto sum = std::ranges::fold_left(
		games | std::views::transform(&game::power), 0LL, std::plus());
#endif

	std::cout << sum << '\n';
}
