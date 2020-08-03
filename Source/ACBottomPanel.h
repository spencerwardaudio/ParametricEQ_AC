/*
  ==============================================================================

    ACBottomPanel.h
    Created: 2 Aug 2020 6:55:45pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "ACPanelBase.h"

class ACBottomPanel
:   public ACPanelBase,
    public Button::Listener,
    public ComboBox::Listener
{
public:

    ACBottomPanel(MultiAvptslAudioProcessor* inProcessor);
    ~ACBottomPanel();

    void paint(Graphics& g) override;
    
    void buttonClicked (Button*) override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

private:
    
    void displaySaveAsPopup();
    
    void updatePresetComboBox();
    
    std::unique_ptr<ComboBox> mPresetDisplay;
    
    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;
};
