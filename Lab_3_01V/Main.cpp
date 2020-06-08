#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "Picture.h"

const double grid_step = 0.1;
const double x_start = -8;
const double y_start = -5;


const double x_end = 8;
const double y_end = 5;

struct Point {
    long double x;
    long double y;
    long double value;
};

struct Vector {
    long double x;
    long double y;
    long double x_value;
    long double y_value;
};

std::pair<int, int> getNearestX(const std::vector<Point>& field, long double x, long double y);
std::pair<int, int> getNearestY(const std::vector<Point>& field, long double x, long double y);
long double distance(long double a_x, long double a_y, long double b_x, long double b_y);

int main(int argc, char** argv)
{
    std::ifstream input;
    std::vector<Point> electricPotential;
    std::vector<Vector> electricField;
    Point dotBuffer;
    char buffer;
    Pixel* image;
    input.open(argv[1], std::ios_base::in);
    input >> buffer;
    while (buffer == '%')
    {
        input.ignore(256, '\n');
        input >> buffer;
    }
    input.seekg(-1, std::ios_base::cur);
    while (!input.eof())
    {
        input >> dotBuffer.x;
        input >> buffer;
        if (input.eof())
            break;
        if ((buffer == 'i'))
        {
            input.ignore(256, '\n');
            input >> dotBuffer.x;
        }
        else
        {
            input.seekg(-1, std::ios_base::cur);
        }
        input >> dotBuffer.y >> dotBuffer.value;
        
        electricPotential.push_back(dotBuffer);
    }
    for (double y = y_start; y <= y_end; y += grid_step)
    {
        for (double x = x_start; x <= x_end; x += grid_step)
        {
            std::pair<int, int> x_dots = getNearestX(electricPotential, x, y);
            std::pair<int, int>  y_dots = getNearestY(electricPotential, x, y);

            electricField.push_back({x, y, 
            (electricPotential[x_dots.first].value - electricPotential[x_dots.second].value) / (electricPotential[x_dots.first].x - electricPotential[x_dots.second].x),
                    (electricPotential[y_dots.first].value - electricPotential[y_dots.second].value)  / (electricPotential[y_dots.first].y - electricPotential[y_dots.second].y)});
        }
    }
    double maxField = atof(argv[3]);
    image = new Pixel[electricField.size()];
    for (int i = 0; i < electricField.size(); i++)
    {
        image[i].R = 0;
        image[i].G = 0;
        image[i].B = 0;
    }
    for (int i = 0; i < electricField.size(); i++)
    {
        double buffer;
        buffer = (distance(0, 0, abs(electricField[i].x_value), abs(electricField[i].y_value)) / maxField);
        if (buffer > 1)
            buffer = 1;
        if (buffer < 0.25)
        {
            image[i].B = buffer * 4 * 200;
        }
        else
        {
            image[i].R = ((buffer - 0.25) * 255)/0.75;
            image[i].B = 255 - image[i].R;
        }
    }
    int image_x = (x_end - x_start) / grid_step + 1, image_y = (y_end - y_start) / grid_step + 1;
    Picture NewPic(image_y, image_x, image);
    NewPic.SaveToSingleFile(argv[2]);
    return 0;
}

std::pair<int, int> getNearestX(const std::vector<Point>& field, long double x, long double y)
{
    double distance_up = 1e308, distance_down = 1e308;
    int up = 0, down = 0;
    for (int i = 0; i < field.size(); i++)
    {
        if ((distance(field[i].x, field[i].y, x, y) < distance_up)&&(field[i].x > x))
        {
            distance_up = distance(field[i].x, field[i].y, x, y);
            up = i;
        }
        if ((distance(field[i].x, field[i].y, x, y) < distance_down) && (field[i].x < x))
        {
            distance_down = distance(field[i].x, field[i].y, x, y);
            down = i;
        }
    }
    return {up, down};
}

std::pair<int, int> getNearestY(const std::vector<Point>& field, long double x, long  double y)
{
    double distance_up = 1e308, distance_down = 1e308;
    int up = 0, down = 0;
    for (int i = 0; i < field.size(); i++)
    {
        if ((distance(field[i].x, field[i].y, x, y) < distance_up) && (field[i].y > y))
        {
            distance_up = distance(field[i].x, field[i].y, x, y);
            up = i;
        }
        if ((distance(field[i].x, field[i].y, x, y) < distance_down) && (field[i].y < y))
        {
            distance_down = distance(field[i].x, field[i].y, x, y);
            down = i;
        }
    }
    return { up, down };
}

long double distance(long double a_x, long double a_y, long double b_x, long double b_y)
{
    return sqrt(pow(a_x - b_x, 2) + pow(a_y - b_y, 2));
}



