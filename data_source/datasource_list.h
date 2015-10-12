//---------------------------------------------------------------------------
#ifndef datasource_listH
#define datasource_listH
//---------------------------------------------------------------------------

template<class T>
class TDatasourceList : public TDatasource<T> {
	public:
		std::list<T> data;
	public:
		TDatasourceList() {}

		void Add(const T &d) {
			data.push_back(d);
		}

		void Resize(size_t size) {
			data.resize(size);
		}

		T & operator [] (size_t index) {
			int i = 0;
			std::list<T>::iterator it = data.begin();
			std::list<T>::iterator itEnd = data.end();
			while (i < index, it != itEnd) {
				++it;
			}
			return *it;
		}

		size_t Size() {
			return data.size();
		}

		void Remove(size_t index) {
			data.erase(index);
		}

		void Insert(size_t indexBefore, const T &d) {
        	data.insert(indexBefore, d);
		}
};

#endif
