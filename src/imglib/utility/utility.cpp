
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
        case imglib::ColorSpace::RGB:
        case imglib::ColorSpace::YCbCr:
        case imglib::ColorSpace::CMYK:
        case imglib::ColorSpace::YCCK:
            return numChannels == 3;
        default:
            return false;
        }
	}
}