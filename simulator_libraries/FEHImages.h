#include <stdlib.h>
#include <FEHLCD.h>
#include <fstream>
#include <iostream>

class FEHImage
{
	public:
		FEHImage();
		void Open(const char *);
		void Draw(int, int);
		void Close();
	private:
		int rows,cols;
		int *saved_image;
};
