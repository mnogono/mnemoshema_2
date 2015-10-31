//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_label.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TChartLabel::TChartLabel(TWinControl *owner) : TLabel(owner) {
	background = clWhite;

	border = clBlack;
}

void __fastcall TChartLabel::Paint(void) {
	TLabel::Paint();

	Canvas->Brush->Color = background;

	Canvas->Pen->Color = border;

	Canvas->Rectangle(ClientRect);

	Canvas->TextOutW(10, 10, Caption);
}


void __fastcall TChartLabel::MouseMove(TShiftState state, int x, int y) {
}

