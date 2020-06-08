#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Picture.h"
using namespace std;
#define MAX_VALUE 255

Picture::Picture(int _height, int _width, Pixel* _image) : height(_height), width(_width), image(_image)
{
}

void Picture::SaveToSingleFile(std::string filename)
{
	ofstream output;
	output.open(filename, ios_base::out | ios_base::binary);
	if (!output.is_open())
		throw std::runtime_error("File writing error");
	output << "P6" << '\n';
	output << width << ' ' << height << '\n' << MAX_VALUE << '\n';
	char* image_char = (char*)image;
	output.write(image_char, width * height * 3);
	if (output.fail())
		throw std::runtime_error("File writing error");
	output.close();
}

Picture::~Picture()
{
	delete[] image;
}