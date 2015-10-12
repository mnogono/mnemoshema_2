//---------------------------------------------------------------------------
#ifndef chart_vertical_lineH
#define chart_vertical_lineH

#include "chart_trackable.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TChartVerticalLine :
	public TChartTrackable {

	public:
    	TShape *line;

		TLabel *label;

        TShape *box;

		// local time in days
		double trackLocalTime;

		double trackValue;

		TChart *chart;
	protected:
		String id;

		const TRecord *record;

		bool isSuperline;

	public:
		TChartVerticalLine(TChart *chart, const TRecord *record);

		TChartVerticalLine(const TChartVerticalLine &chartVerticalLine);

		void Init(TChart *chart, const TRecord *record);

		__fastcall virtual ~TChartVerticalLine();

		//return local time of the chart object
		virtual double GetLocalTime() const;

		//set trackable local time
		virtual void SetLocalTime(double localTime);

		//set trackable value
		virtual void SetValue(double value);

		virtual bool TrackValue();

		virtual double GetValue() const;

		void Highlight(bool highlight);

		void UpdateBox();

		//move trackable object to certain time
		virtual void Move(int x, int y);

		//show trackable element
		virtual void SetVisible(bool visible);

		//hide trackable element
		virtual bool IsVisible() const;

		//update caption box width and height
        void UpdateCaptionBox();

		void SetCaption(String caption);

		void SetCaption(String caption, double localTime, const std::map<TChartSeries *, double> &seriesValues);

		//DYNAMIC void __fastcall MouseMove(TShiftState, int, int);

		/*DYNAMIC */ /*void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);*/
		void __fastcall MouseDown(System::TObject* Sender, System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);

		/*DYNAMIC */ /*void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);*/
		void __fastcall MouseUp(System::TObject* Sender, System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);

		void __fastcall MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual bool IsEqual(TChartTrackable *chartTrackable);

		virtual int GetType();

		virtual void UpdateCaption();

		void MarkAsSuperline(bool isSuperline);

		bool IsSuperline();
	};

#pragma option pop

#endif
