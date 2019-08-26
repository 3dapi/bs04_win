#include <set>
#include <string.h>

struct TFileInfo
{
	std::string prim_name;
	std::string file_name;

	// in class operator
	friend bool operator<(const TFileInfo& s1, const TFileInfo& s2)
	{
		return s1.prim_name < s2.prim_name;
	}
	friend bool operator>(const TFileInfo& s1, const TFileInfo& s2)
	{
		return s1.prim_name > s2.prim_name;
	}
};

// using functor
struct FunctorCmp
{
	bool operator()(const TFileInfo& s1, const TFileInfo& s2) const
	{
		return strcmp(s1.prim_name.c_str(), s2.prim_name.c_str()) < 0;
	}
};
typedef std::set<TFileInfo,   std::greater<TFileInfo> > FILEINFOSET;
//typedef std::set<TFileInfo,   FunctorCmp              > FILEINFOSET;

int main()
{
	FILEINFOSET file_info;
	file_info.insert({"1", "100"});
	file_info.insert({"3", "300"});
	file_info.insert({"7", "700"});
	file_info.insert({"6", "600"});
	file_info.insert({"5", "500"});
	file_info.insert({"2", "200"});
	file_info.insert({"5", "1"  });
	file_info.insert({"3", "1"  });
	file_info.insert({"4", "400"});
	file_info.insert({"1", "1"  });

	const auto& ss0 = *std::next(file_info.begin(), 3);
	const auto& ss1 = *std::next(file_info.begin(), 4);
	const auto& ss2 = *std::next(file_info.begin(), 5);

	size_t buf_len = 32;
	std::string buf_data(buf_len+4,0);
	size_t buf_read{};
	while((buf_read = fread(&buf_data[0], sizeof(char), buf_len, stdin)) > 0)
		break;
	return 0;
}
