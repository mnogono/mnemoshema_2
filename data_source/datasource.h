//---------------------------------------------------------------------------
#ifndef datasourceH
#define datasourceH
//---------------------------------------------------------------------------

template<class T>
class TDatasource {
	public:
		virtual void Add(const T &d) = 0;

		virtual void Resize(size_t size) = 0;

		virtual T & operator [] (size_t index) = 0;

		virtual size_t Size() = 0;

		virtual void Remove(size_t index) = 0;

		virtual void Insert(size_t indexBefore, const T &d) = 0;
};

#endif
