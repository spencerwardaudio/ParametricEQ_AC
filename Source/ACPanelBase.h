/*
  ==============================================================================

    ACPanelBase.h
    Created: 2 Aug 2020 6:55:03pm
    Author:  Spencer Ward

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ACInterfaceDefines.h"


class ACPanelBase
:   public Component
{
public:
    
    ACPanelBase(MultiAvptslAudioProcessor* inProcessor);
    ~ACPanelBase();
    
    void mouseEnter (const MouseEvent& event) override;

    void mouseExit (const MouseEvent& event) override;
    
    void paint (Graphics& g) override;
    
protected:
    
    MultiAvptslAudioProcessor* mProcessor;
    
};
