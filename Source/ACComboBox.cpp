/*
  ==============================================================================

    ACComboBox.cpp
    Created: 2 Aug 2020 3:26:30pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACComboBox.h"

ACParameterComboBox::ACParameterComboBox(AudioProcessorValueTreeState& stateToControl, const String& parameterID)
:   juce::ComboBox(parameterID)
{
    mAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(stateToControl, parameterID, *this);
}

ACParameterComboBox::~ACParameterComboBox()
{
    
}
