//---------------------------------------------------------------------------
#ifndef chart_trackableH
#define chart_trackableH
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

//each trackable cart element on the chart must be linked with certain time
class TChartTrackable {
	public:
		virtual __fastcall ~TChartTrackable();

		//return local time of the chart object
		virtual double GetLocalTime() const = 0;

		//set trackable local time
		virtual void SetLocalTime(double localTime) = 0;

		//set trackable value
		virtual void SetValue(double value) = 0;

		virtual double GetValue() const = 0;

		virtual bool TrackValue() = 0;

		//move trackable object to certain time
		virtual void Move(int x, int y) = 0;

		//show trackable element
		virtual void SetVisible(bool visible) = 0;

		//hide trackable element
		virtual bool IsVisible() const = 0;

		//check is chartTrackable element equal with current
		virtual bool IsEqual(TChartTrackable *chartTrackable) = 0;

		virtual int GetType() = 0;
};

#pragma option pop


#endif
