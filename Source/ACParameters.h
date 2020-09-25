/*
  ==============================================================================

    ACParameters.h
    Created: 3 Jun 2020 9:32:09pm
    Author:  Spencer Ward

  ==============================================================================
*/
#pragma once

enum ACParameter
{
    paramOutput = 0,

    paramFrequency1,
    paramQuality1,
    paramGain1,
    paramActive1,

    paramFrequency2,
    paramQuality2,
    paramGain2,
    paramActive2,
    
    paramFrequency3,
    paramQuality3,
    paramGain3,
    paramActive3,
    
    paramFrequency4,
    paramQuality4,
    paramGain4,
    paramActive4,
    
    paramFrequency5,
    paramQuality5,
    paramGain5,
    paramActive5,
    
    aParameter_TotalNumParameters
};

static String ACParameterID [aParameter_TotalNumParameters] =
{
    "Output",

    "Freq1",
    "Q1",
    "Gain1",
    "ActiveState1",

    "Freq2",
    "Q2",
    "Gain2",
    "ActiveState2",
    
    "Freq3",
    "Q3",
    "Gain3",
    "ActiveState3",
    
    "Freq4",
    "Q4",
    "Gain4",
    "ActiveState4",
    
    "Freq5",
    "Q5",
    "Gain5",
    "ActiveState5"

};
