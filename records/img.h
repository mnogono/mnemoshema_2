//---------------------------------------------------------------------------
#ifndef imgH
#define imgH
//---------------------------------------------------------------------------

struct TImg : public TRecord {

	String name;

	//image buffer
	char *img;

	//check sum of image buffer
	crc crc32;

	//image height
	int height;

	//image width
	int width;

	//image size bytes
	int size;

	Graphics::TBitmap *bitmap;

	TImg();

	String GetName() const;
};

#endif
