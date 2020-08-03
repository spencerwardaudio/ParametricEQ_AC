/*
  ==============================================================================

    ACComboBox.h
    Created: 2 Aug 2020 3:26:30pm
    Author:  Spencer Ward

  ==============================================================================
*/
#include "JuceHeader.h"

#pragma once

class KAPParameterComboBox
:   public ComboBox
{
public:
    
    KAPParameterComboBox(AudioProcessorValueTreeState& stateToControl, const String& parameterID);
    ~KAPParameterComboBox();
    
private:
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KAPParameterComboBox);
};
