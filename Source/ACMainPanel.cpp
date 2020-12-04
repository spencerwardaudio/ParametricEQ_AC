/*
  ==============================================================================

    ACCenterPanel.cpp
    Created: 2 Aug 2020 6:55:34pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACMainPanel.h"
#include "ACParameters.h"

ACMainPanel::ACMainPanel(ParametricEQ_ACAudioProcessor* inProcessor)
:   ACPanelBase(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mBottomPanel = std::make_unique<ACBottomLeftPanel>(inProcessor);
    mBottomPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(mBottomPanel.get());
    
}

ACMainPanel::~ACMainPanel()
{
    
}
