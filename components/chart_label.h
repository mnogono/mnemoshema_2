//---------------------------------------------------------------------------
#ifndef chart_labelH
#define chart_labelH
//---------------------------------------------------------------------------

/** draw label with borders and background color */
class TChartLabel : public TLabel {
	public:
		TColor background;

		TColor border;
	public:
		__fastcall TChartLabel(TWinControl *owner);

		virtual void __fastcall Paint(void);

		DYNAMIC void __fastcall MouseMove(TShiftState, int, int);
};

#endif
