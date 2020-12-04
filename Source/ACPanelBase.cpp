/*
  ==============================================================================

    ACPanelBase.cpp
    Created: 2 Aug 2020 6:55:03pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACPanelBase.h"
#include "ACInterfaceDefines.h"

ACPanelBase::ACPanelBase(ParametricEQ_ACAudioProcessor* inProcessor)
:   mProcessor(inProcessor)
{
    
}

ACPanelBase::~ACPanelBase()
{
    
}

void ACPanelBase::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void ACPanelBase::mouseExit (const MouseEvent& event)
{
    repaint();
}

void ACPanelBase::paint (Graphics& g)
{
    if(isMouseOver()){
//        g.setColour(KAPColour_4);
//        g.fillAll();
    }
}
