#include <algorithm>
#include <functional>
#include <string>
#include <cctype>
#include <set>
#include <vector>

std::vector<std::string> vec_str;

std::function<bool(const std::string&,const std::string&)> str_asc=
[](const std::string& s1, const std::string& s2)->bool
{
	std::string t1(s1);
	std::string t2(s2);
	std::transform(t1.begin(), t1.end(), t1.begin(), std::tolower);
	std::transform(t2.begin(), t2.end(), t2.begin(), std::tolower);
	if(!t1.compare(t2))
		return s1<s2;
	return t1<t2;
};

struct cmp_asc { bool operator()(const std::string& s1, const std::string& s2) { return str_asc(s1,s2); } };
//std::set<std::string, decltype(str_asc)> set_str(str_asc);
std::set<std::string, cmp_asc> set_str;

int main(int, char**)
{
	vec_str.push_back(std::string("Hello"));
	vec_str.push_back("world");
	vec_str.push_back("welcome");
	vec_str.push_back("Hi");
	vec_str.push_back("AlPha");
	vec_str.push_back("Alpha");
	vec_str.push_back("Beta");

	std::sort(vec_str.begin(), vec_str.end(),str_asc);
	for(const auto& str : vec_str)
		printf("vec str: %s\n", str.c_str());

	printf("-------------------------------------------------------\n");

	set_str.insert(std::string("Hello"));
	set_str.insert("world");
	set_str.insert("welcome");
	set_str.insert("Hi");
	set_str.insert("AlPha");
	set_str.insert("Alpha");
	set_str.insert("Beta");

	for(const auto& str : set_str)
		printf("set str: %s\n", str.c_str());

	return 0;
}
