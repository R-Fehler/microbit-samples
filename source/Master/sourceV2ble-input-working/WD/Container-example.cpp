vector<char> v;
for (char i = 'a'; i < 'e'; i++)
{
	v.push_back(i);
}
for (auto i = v.begin(); i != v.end(); i++)
{
	uBit.display.scroll(*i);
	// uBit.display.scroll(v[*i]);
}
std::reverse(v.begin(), v.end());
for (auto i = v.begin(); i != v.end(); i++)
{
	uBit.display.scroll(*i);
	// uBit.display.scroll(v[*i]);
}
uBit.display.scroll(int(v.size()));
uBit.display.scroll(int(v.capacity()));

std::map<int, ManagedString> catalog;
catalog[1] = ManagedString("Snatch");
