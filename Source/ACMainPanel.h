/*
  ==============================================================================

    ACCenterPanel.h
    Created: 2 Aug 2020 6:55:34pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once


#include "ACPanelBase.h"
#include "ACBottomLeftPanel.h"


class ACMainPanel
:   public ACPanelBase
{
    
public:
    
    ACMainPanel(MultiAvptslAudioProcessor* inProcessor);
    ~ACMainPanel();
    
private:
    
    std::unique_ptr<ACBottomLeftPanel> mBottomPanel;

};
