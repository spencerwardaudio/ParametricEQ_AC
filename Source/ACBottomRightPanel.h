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
    
    void setupBypassButton(int cntrlPoints, juce::String name, bool attachment);

    void paint(Graphics& g) override;
    
    void resized() override;
    
    void buttonClicked (Button*) override;
    
    void updateToggleState (Button* button, juce::String name);
 
private:
    
    std::unique_ptr<Label> label;
    
    std::vector<std::unique_ptr<TextButton>> bypassButtons;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>> EQAttachment;
};
