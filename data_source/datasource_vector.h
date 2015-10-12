//---------------------------------------------------------------------------
#ifndef datasource_vectorH
#define datasource_vectorH
//---------------------------------------------------------------------------

template<class T>
class TDatasourceVector : public TDatasource<T> {
	public:
		std::vector<T> data;
	public:
		TDatasourceVector() {}

		void Add(const T &d) {
			data.push_back(d);
		}

		void Resize(size_t size) {
			data.resize(size);
		}

		T & operator [] (size_t index) {
			return data[index];
		}

		size_t Size() {
			return data.size();
		}

		void Remove(size_t index) {
			data.erase(data.begin() + index);
		}

		void Insert(size_t indexBefore, const T &d) {
        	data.insert(data.begin() + indexBefore, d);
		}
};

#endif
