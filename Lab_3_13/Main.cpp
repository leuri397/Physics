#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    double x;
    double y;
    double z;
};

struct Vector {
    double x;
    double y;
    double z;
};

struct FieldEntry {
    Point coordinates;
    Vector field;
    bool operator> (const FieldEntry& right) {
        return coordinates.z > right.coordinates.z;
    }
    bool operator< (const FieldEntry& right) {
        return coordinates.z < right.coordinates.z;
    }
};

double module3D(double x, double y, double z);
double module2D(double x, double y);

int main(int argc, char** argv)
{
    std::ifstream input;
    std::ofstream output;
    std::vector<FieldEntry> magneticField;
    std::vector<double> gradient;
    std::vector<double> flatGradient;
    std::vector<double> fieldModule;
    std::vector<double> flatField;
    FieldEntry fieldBuffer;
    char buffer;
    input.open(argv[1], std::ios_base::in);
    output.open(argv[2], std::ios_base::out);
    input >> buffer;
    while (buffer == '%')
    {
        input.ignore(256, '\n');
        input >> buffer;
    }
    input.seekg(-1, std::ios_base::cur);
    while (!input.eof())
    {
        input >> fieldBuffer.coordinates.x >> fieldBuffer.coordinates.y >>
            fieldBuffer.coordinates.z >> fieldBuffer.field.x >> fieldBuffer.field.y >>
            fieldBuffer.field.z;
        magneticField.push_back(fieldBuffer);
    }
    std::sort(magneticField.begin(), magneticField.end());
    for (int i = 0; i < magneticField.size(); i++)
    {
        flatField.push_back(module2D(magneticField[i].field.x, magneticField[i].field.y));
        fieldModule.push_back(module3D(magneticField[i].field.x, magneticField[i].field.y, magneticField[i].field.z));
        if (i < 1)
        {
            flatGradient.push_back(1);
            gradient.push_back(1);
        }
        else
        {
            flatGradient.push_back(*(flatField.end() - 2) / *(flatField.end() - 1));
            gradient.push_back(magneticField[i - 1].field.z / magneticField[i].field.z);
        }
    }

    output << "Field values:\n";
    output << "Z\t\t|B|\t\tGBz\t\tGBxy\n";
    output.precision(8);
    output << std::fixed;
    for (int i = 0; i < magneticField.size(); i++)
    {
        output << magneticField[i].coordinates.z << '\t' <<
            fieldModule[i] << '\t' <<
            gradient[i]<<'\t' <<
            flatGradient[i] << '\n';
    }
}

double module3D(double x, double y, double z)
{
    double sum = pow(x, 2) + pow(y, 2) + pow(z, 2);
    return sqrt(sum);
}

double module2D(double x, double y)
{
    double sum = pow(x, 2) + pow(y, 2);
    return sqrt(sum);
}
