#include "pch.h"
#pragma hdrstop

#include <TestFramework.hpp>

struct TEventRow {
	std::wstring datetime;
	std::wstring sensorName;
	std::wstring event;

    TEventRow() {}

	TEventRow(
		std::wstring datetime,
		std::wstring sensorName,
	    std::wstring event) : datetime(datetime), sensorName(sensorName), event(event) {}
};

class TTestTDatasource : public TTestCase
{
public:
  __fastcall virtual TTestTDatasource(System::String name) : TTestCase(name) {}
  virtual void __fastcall SetUp();
  virtual void __fastcall TearDown();

__published:
  void __fastcall TestDatasourceCreate();
  void __fastcall TestDatasourceRemove();
  void __fastcall TestDatasourceIteration();
};


void __fastcall TTestTDatasource::SetUp() {}

void __fastcall TTestTDatasource::TearDown() {}

void __fastcall TTestTDatasource::TestDatasourceCreate() {
	TDatasourceVector<TEventRow> datasource;
	datasource.Add(TEventRow(L"07/30 12:13", L"Temperature1", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:14", L"Temperature2", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:15", L"Temperature3", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:13", L"Temperature4", L"value > 300*C"));

	CheckTrue(std::wstring(L"07/30 12:13") == datasource[0].datetime, "value 0 is wrong (0)");

	TEventRow &eventRow = datasource[0];
	datasource[0].datetime = std::wstring(L"07/30 12:14");

	CheckFalse(std::wstring(L"07/30 12:13") == datasource[0].datetime, "value 0 is wrong (1)");
}

void __fastcall TTestTDatasource::TestDatasourceRemove() {
	TDatasourceVector<TEventRow> datasource;
	datasource.Add(TEventRow(L"07/30 12:13", L"Temperature1", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:14", L"Temperature2", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:15", L"Temperature3", L"value > 300*C"));
	datasource.Add(TEventRow(L"07/30 12:13", L"Temperature4", L"value > 300*C"));

	CheckTrue(datasource.Size() == 4, "datasource size is wrong (4)");

	datasource.Remove(0);

	CheckTrue(datasource.Size() == 3, "datasource size is wrong (3)");

	CheckTrue(datasource[0].datetime == std::wstring(L"07/30 12:14"), "check [0] value is wrong");

	datasource.Remove(0);
	datasource.Remove(0);
	datasource.Remove(0);

	CheckTrue(datasource.Size() == 0, "datasource size is wrong (0)");

	datasource.Insert(0, TEventRow(L"07/31 12:00", L"", L""));

	CheckTrue(datasource.Size() == 1, "datasource size is wrong (1)");

	CheckTrue(datasource[0].datetime == std::wstring(L"07/31 12:00"), "check [0] value is wrong");

	/*
	std::vector<TEventRow> v;
	std::list<TEventRow> l;

	std::iterator<std::input_iterator_tag, TEventRow> it;
	it = v.begin();
	*/
	//std::istream_iterator<TEventRow> it = v.begin();
	//std::iterator<std::random_access_iterator_tag, TEventRow> it = v.begin();
}

void __fastcall TTestTDatasource::TestDatasourceIteration() {

}

static void registerTests() {
  Testframework::RegisterTest(TTestTDatasource::Suite());
}

#pragma startup registerTests 33
