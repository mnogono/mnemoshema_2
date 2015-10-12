//---------------------------------------------------------------------------
#ifndef string_grid_datasourceH
#define string_grid_datasourceH
//---------------------------------------------------------------------------
template<class T>
class TStringGridDatasource : public TStringGrid {
	public:
		TDatasource<T> *datasource;
	public:
		__fastcall virtual TStringGridDatasource(System::Classes::TComponent* AOwner, TDatasource<T> *datasource)  : TStringGrid(AOwner) {
			this->datasource = datasource;
		}

		/**
		build string grid using data from datasource
		*/
		void Build() {
			//size_t rows = datasource.data.size();
			size_t rows = datasource->Size();
			if (rows == 0) {
				return Clear();
			}

			RowCount = FixedRows + rows;

			for (size_t row = 0; row < rows; ++row) {
				for (int col = 0; col < ColCount; ++col) {
					Cells[col][row + FixedRows] = (*datasource)[row].Value(col).c_str();
				}
			}
		}

		/**
		set row count = fixed rows + 1, and clear all non fixed rows
		*/
		void Clear() {
			RowCount = FixedRows + 1;
			for (int col = 0; col < ColCount; ++col) {
				Cells[col][FixedRows] = "";
			}
		}

		/**
		set datasource to the string grid container
		*/
		void SetDatasource(TDatasource<T> *datasource);
};

#endif
