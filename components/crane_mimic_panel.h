//---------------------------------------------------------------------------

#ifndef CraneMimicPanelH
#define CraneMimicPanelH

#include "Crane.h"
#include "request_mnemoshema_data.h"
#include "request_http_data.h"
//---------------------------------------------------------------------------

//TODO implement IMnemoshemaView

class TCraneMimicPanel : public sysObserver::IObserver {
	private:
		//bitmap for mimic panel
		TBitmap *bitmapBackground;

		TCanvas *canvas;

		const TSensor *crane1x;

		const TSensor *crane1y;

		const TSensor *crane1d;

		const TSensor *crane2x;

		const TSensor *crane2y;

		TCrane *crane1;

		TCrane *crane2;

	public:
    	//TODO maybe here point to mimic panel

		//all available cranes for drawing
		std::list<TCrane *> cranes;

		/**
		Create mimic panel
		@param {TCanvas *} canvas where crane can be displayed
		*/
		//TCraneMimicPanel(TCanvas *canvas);
		TCraneMimicPanel(TCanvas *canvas, TBitmap *bitmap);

		/**
		add new crane
		*/
		void addCrane(TCrane *crane);

		/**
		draw all items on mimic panel
		*/
		void draw();

		/**
		draw background image for clear all
		*/
		void clear();

		/**
		return crane
		*/
		TCrane * getCrane();

		void move(void *sensorId, double x, double y);

		void rotate(void *sensorId, double angleDegree);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);
	private:
		void initializeSensors();

		void initializeCranes();
};

#endif
