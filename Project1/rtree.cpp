#include "rtree.h"
#include "spatialindex/RTree.h"

#define FillFactor 0.9
#define IndexCapacity 10 
#define LeafCapacity 50


int main()
{
	//shared_ptr<int> s{ make_shared<int>(15) };
	//auto b = s;
	//cout << s.use_count() << endl;
	//auto c = s.get();
	//cout << s.use_count() << endl;
	//cout << *c << endl;

	//shared_ptr<int> s1{ make_shared<int>(15) };
	//auto b1 = s1;
	//cout << s1.use_count() << endl;
	//auto c1 = s1;
	//cout << s1.use_count() << endl;
	//cout << *c1 << endl;

	using namespace SpatialIndex;
	id_type  indexIdentifier;
	std::string_view path_view = "data.txt";
	std::string path_str = { path_view.data(), path_view.size() };
	const char* path_c = path_str.c_str();

	std::shared_ptr<std::map<SpatialIndex::id_type, std::string>> id_tiles = make_shared<std::map<SpatialIndex::id_type, std::string>>();

	GEOSDataStreamFileTile stream(path_c, id_tiles.get());//c_str()字符串后有'\0'，而data()没有。
	IStorageManager* storage = StorageManager::createNewMemoryStorageManager();
	ISpatialIndex* spidx = RTree::createAndBulkLoadNewRTree(RTree::BLM_STR, stream, *storage,
		FillFactor,
		IndexCapacity,
		LeafCapacity,
		3,
		RTree::RV_RSTAR, indexIdentifier);

	double low[3] = {0, 0, 0};
	double high[3] = {100, 100, 100};
	std::string str_low;
	std::string str_high;

	//getline(cin, str_low);
	//cout << str_low << endl;

	//getline(cin, str_high);
	//cout << str_high << endl;

	Region r(low, high, 3);
	MyVisitor vis;
	spidx->intersectsWithQuery(r, vis);
	//spidx->containsWhatQuery(r, vis);

	//for (auto iter = (*id_tiles).begin(); iter != (*id_tiles).end(); ++iter)
	//{
	//	cout << iter->first << ":" << iter->second << endl;
	//}

	for (uint32_t i = 0; i < vis.matches.size(); i++)
	{
		cout << "id:" << (*id_tiles)[vis.matches[i]] << endl;//tile id
	}



	vis.matches.clear();

	return 0;
}
