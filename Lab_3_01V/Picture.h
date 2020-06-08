#pragma once
#include <string>
#include <vector>
struct Pixel {
	unsigned char R;
	unsigned char G;
	unsigned char B;
};
class Picture {
public:
	Picture(int height, int width, Pixel* image);
	~Picture();
	void SaveToSingleFile(std::string);
private:
	int height;
	int width;
	Pixel* image;
};