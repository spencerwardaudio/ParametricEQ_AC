/*
  ==============================================================================

    ACBottomRightPanel.h
    Created: 3 Aug 2020 11:50:08am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "ACPanelBase.h"

class ACBottomRightPanel
:   public ACPanelBase, public Button::Listener
{
public:

    ACBottomRightPanel(ParametricEQ_ACAudioProcessor* inProcessor);
    ~ACBottomRightPanel();

    void paint(Graphics& g) override;
    
    void resized() override;
    
    void buttonClicked (Button*) override;
    
    void updateToggleState (Button* button, juce::String name);
 
private:
    
    std::unique_ptr<Label> label;
    
    std::unique_ptr<TextButton> EQ1, EQ2, EQ3, EQ4, EQ5, LS, HS, FMode;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> EQ1Attachments, EQ2Attachments, EQ3Attachments, EQ4Attachments, EQ5Attachments;

};
