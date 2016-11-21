
#include "LicensePlateNumber.h"

int main()
{
    CarLicenseDetect carLicenseDetect;

    carLicenseDetect.loadImage("../data/car.jpg", 1);
    carLicenseDetect.show();
    carLicenseDetect.carLicensePlateExtract();
    carLicenseDetect.show();
    return 0;
}
