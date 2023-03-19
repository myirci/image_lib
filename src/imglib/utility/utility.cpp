
#include <imglib/utility/utility.hpp>

namespace imglib
{
	bool IsValid(ColorSpace cSpace, size_t numChannels)
	{
        switch (cSpace)
        {
        case imglib::ColorSpace::Unspecified:
            return numChannels > 0;
        case imglib::ColorSpace::GrayScale:
            return numChannels == 1;
        case imglib::ColorSpace::GrayScaleAlpha:
            return numChannels == 2;
        case imglib::ColorSpace::RGB:
        case imglib::ColorSpace::YCbCr:
        case imglib::ColorSpace::YCCK:
            return numChannels == 3;
        case imglib::ColorSpace::CMYK:
        case imglib::ColorSpace::RGBA:
            return numChannels == 4;
        default:
            return false;
        }
	}
}