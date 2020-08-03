/*
  ==============================================================================

    ACAudioHelpers.h
    Created: 2 Aug 2020 3:06:32pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

#define kParameterSmoothingCoeff_Generic 0.04
#define kParameterSmoothingCoeff_Fine 0.002
#define kMeterSmoothingCoeff 0.2

inline float kap_linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

