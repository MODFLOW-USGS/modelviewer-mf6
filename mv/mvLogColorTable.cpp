// mvLogColorTable.cpp: implementation of the mvLogColorTable class.
//
//////////////////////////////////////////////////////////////////////

#include "mvLogColorTable.h"

#include <vtkObjectFactory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtkStandardNewMacro(mvLogColorTable);

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construct with (minimum,maximum) range 1 to 10 (based on
// logarithmic values).
mvLogColorTable::mvLogColorTable()
{
    SetDefaultColorScheme();
}

mvLogColorTable::~mvLogColorTable()
{
}

// Generate lookup table from hue, saturation, value, alpha min/max values.
// Table is built from linear ramp of each value.
void mvLogColorTable::Build()
{
    const double   Exponent = 0.38;
    int            i, hueCase;
    float          hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc, ainc;
    float          rgba[4], alpha;
    unsigned char* c_rgba;

    int            maxIndex = this->NumberOfColors - 1;

    if (this->Table->GetNumberOfTuples() < 1 ||
        (this->GetMTime() > this->BuildTime &&
         this->InsertTime < this->BuildTime))
    {
        hinc = (this->HueRange[1] - this->HueRange[0]) / maxIndex;
        sinc = (this->SaturationRange[1] - this->SaturationRange[0]) / maxIndex;
        vinc = (this->ValueRange[1] - this->ValueRange[0]) / maxIndex;
        ainc = (this->AlphaRange[1] - this->AlphaRange[0]) / maxIndex;

        for (i = 0; i <= maxIndex; i++)
        {
            hue     = this->HueRange[0] + i * hinc;
            sat     = this->SaturationRange[0] + i * sinc;
            val     = this->ValueRange[0] + i * vinc;
            alpha   = this->AlphaRange[0] + i * ainc;

            hueCase = static_cast<int>(hue * 6);
            frac    = 6 * hue - hueCase;

            // The following conversion of frac is what distinguishes
            // mvColorTable from vtkLookupTable.  This spreads out the yellow
            // and aqua bands at the expense of the red, green and blue bands.
            // The value of Exponent was determined experimentally.
            if (GetTransformValues())
            {
                if (hueCase % 2)
                {
                    // hueCase is odd
                    frac = 1 - pow((1.0 - frac), Exponent);
                }
                else
                {
                    // hueCase is even
                    frac = pow((double)frac, Exponent);
                }
            }

            lx = val * (1.0 - sat);
            ly = val * (1.0 - sat * frac);
            lz = val * (1.0 - sat * (1.0 - frac));

            switch (hueCase)
            {
                /* 0<hue<1/6 */
            case 0:
            case 6:
                rgba[0] = val;
                rgba[1] = lz;
                rgba[2] = lx;
                break;
                /* 1/6<hue<2/6 */
            case 1:
                rgba[0] = ly;
                rgba[1] = val;
                rgba[2] = lx;
                break;
                /* 2/6<hue<3/6 */
            case 2:
                rgba[0] = lx;
                rgba[1] = val;
                rgba[2] = lz;
                break;
                /* 3/6<hue/4/6 */
            case 3:
                rgba[0] = lx;
                rgba[1] = ly;
                rgba[2] = val;
                break;
                /* 4/6<hue<5/6 */
            case 4:
                rgba[0] = lz;
                rgba[1] = lx;
                rgba[2] = val;
                break;
                /* 5/6<hue<1 */
            case 5:
                rgba[0] = val;
                rgba[1] = lx;
                rgba[2] = ly;
                break;
            }
            rgba[3] = alpha;

            c_rgba  = this->Table->WritePointer(4 * i, 4);

            if (this->Ramp == VTK_RAMP_SCURVE)
            {
                c_rgba[0] = static_cast<unsigned char>(127.5 * (1.0 + cos((1.0 - static_cast<double>(rgba[0])) * 3.141593)));
                c_rgba[1] = static_cast<unsigned char>(127.5 * (1.0 + cos((1.0 - static_cast<double>(rgba[1])) * 3.141593)));
                c_rgba[2] = static_cast<unsigned char>(127.5 * (1.0 + cos((1.0 - static_cast<double>(rgba[2])) * 3.141593)));
                c_rgba[3] = static_cast<unsigned char>(alpha * 255.0);
                /* same code, but with rounding
                c_rgba[0] = static_cast<unsigned char>
                  (127.5f*(1.0f + (float)cos(double((1.0f-rgba[0])*3.141593f)))+0.5f);
                c_rgba[1] = static_cast<unsigned char>
                  (127.5f*(1.0f + (float)cos(double((1.0f-rgba[1])*3.141593f)))+0.5f);
                c_rgba[2] = static_cast<unsigned char>
                  (127.5f*(1.0f + (float)cos(double((1.0f-rgba[2])*3.141593f)))+0.5f);
                c_rgba[3] = static_cast<unsigned char>(rgba[3]*255.0f + 0.5f);
                */
            }
            else
            {
                c_rgba[0] = static_cast<unsigned char>(rgba[0] * 255.0f + 0.5f);
                c_rgba[1] = static_cast<unsigned char>(rgba[1] * 255.0f + 0.5f);
                c_rgba[2] = static_cast<unsigned char>(rgba[2] * 255.0f + 0.5f);
                c_rgba[3] = static_cast<unsigned char>(rgba[3] * 255.0f + 0.5f);
            }
        }
        this->BuildTime.Modified();
    }
}

void mvLogColorTable::SetTransformValues(bool Value)
{
    m_TransformValues = Value;
};

void mvLogColorTable::SetColorScheme(int Value)
{
    if (m_ColorScheme != Value)
    {
        m_ColorScheme = Value;
        switch (m_ColorScheme)
        {
        case MV_CT_DEFAULT_C0LOR_SCHEME:
            m_TransformValues = false;
            SetRampToSCurve();
            SetHueRange(0, .6667);
            SetSaturationRange(1.0, 1.0);
            SetValueRange(1.0, 1.0);
            break;
        case MV_CT_REVERSED_DEFAULT_C0LOR_SCHEME:
            m_TransformValues = false;
            SetRampToSCurve();
            SetHueRange(.6667, 0);
            SetSaturationRange(1.0, 1.0);
            SetValueRange(1.0, 1.0);
            break;
        case MV_CT_MODIFIED_C0LOR_SCHEME:
            m_TransformValues = true;
            SetRampToLinear();
            SetHueRange(.6667, 0);
            SetSaturationRange(1.0, 1.0);
            SetValueRange(1.0, 1.0);
            break;
        case MV_CT_REVERSED_MODIFIED_C0LOR_SCHEME:
            m_TransformValues = true;
            SetRampToLinear();
            SetHueRange(0, .6667);
            SetSaturationRange(1.0, 1.0);
            SetValueRange(1.0, 1.0);
            break;
        case MV_CT_CUSTOM_C0LOR_SCHEME:
            SetCustomColorScheme();
            break;
        case MV_CT_REVERSED_CUSTOM_C0LOR_SCHEME:
            SetReversedCustomColorScheme();
            break;
        default:
            m_TransformValues = false;
            SetRampToSCurve();
            SetHueRange(0, .6667);
            SetSaturationRange(1.0, 1.0);
            SetValueRange(0.5, 0.5);
            break;
        }
    }
    Build();
    Modified();
};

void mvLogColorTable::SetDefaultColorScheme()
{
    SetColorScheme(MV_CT_DEFAULT_C0LOR_SCHEME);
};

void mvLogColorTable::SetReversedDefaultColorScheme()
{
    SetColorScheme(MV_CT_REVERSED_DEFAULT_C0LOR_SCHEME);
};

void mvLogColorTable::SetModifiedColorScheme()
{
    SetColorScheme(MV_CT_MODIFIED_C0LOR_SCHEME);
};

void mvLogColorTable::SetReversedModifiedColorScheme()
{
    SetColorScheme(MV_CT_REVERSED_MODIFIED_C0LOR_SCHEME);
};

void mvLogColorTable::SetFirstCustomColor(std::uint32_t value)
{
    m_FirstCustomColor = value;
};

void mvLogColorTable::SetLastCustomColor(std::uint32_t value)
{
    m_LastCustomColor = value;
};

void mvLogColorTable::FillColorTable(std::uint32_t lowercolor, std::uint32_t uppercolor)
{
    std::uint32_t LowerRed, LowerGreen, LowerBlue;
    std::uint32_t UpperRed, UpperGreen, UpperBlue;
    // red
    LowerRed                 = lowercolor;
    LowerRed                 = LowerRed << 24;
    LowerRed                 = LowerRed >> 24;

    // green
    LowerGreen               = lowercolor;
    LowerGreen               = LowerGreen << 16;
    LowerGreen               = LowerGreen >> 24;

    // blue
    LowerBlue                = lowercolor;
    LowerBlue                = LowerBlue << 8;
    LowerBlue                = LowerBlue >> 24;

    // red
    UpperRed                 = uppercolor;
    UpperRed                 = UpperRed << 24;
    UpperRed                 = UpperRed >> 24;

    // green
    UpperGreen               = uppercolor;
    UpperGreen               = UpperGreen << 16;
    UpperGreen               = UpperGreen >> 24;

    // blue
    UpperBlue                = uppercolor;
    UpperBlue                = UpperBlue << 8;
    UpperBlue                = UpperBlue >> 24;

    const int MaxColors      = 256;
    float     RedIncrement   = (float(UpperRed) - float(LowerRed)) / (float(MaxColors) - 1.0) / 256.0;
    float     GreenIncrement = (float(UpperGreen) - float(LowerGreen)) / (float(MaxColors) - 1.0) / 256.0;
    float     BlueIncrement  = (float(UpperBlue) - float(LowerBlue)) / (float(MaxColors) - 1.0) / 256.0;

    float     red, green, blue;
    SetNumberOfColors(MaxColors);
    for (int i = 0; i < MaxColors; i++)
    {
        red   = float(LowerRed) / 255 + i * RedIncrement;
        green = float(LowerGreen) / 255 + i * GreenIncrement;
        blue  = float(LowerBlue) / 255 + i * BlueIncrement;
        SetTableValue(i, red, green, blue);
    }
    Modified();
};

void mvLogColorTable::SetCustomColorScheme()
{
    FillColorTable(m_FirstCustomColor, m_LastCustomColor);
}

void mvLogColorTable::SetReversedCustomColorScheme()
{
    FillColorTable(m_LastCustomColor, m_FirstCustomColor);
};
