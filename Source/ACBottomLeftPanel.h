/*
  ==============================================================================

    ACBottomPanel.h
    Created: 2 Aug 2020 6:55:45pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "ACPanelBase.h"
#include "ACDisplayPresetButtons.h"

class ACBottomLeftPanel
:   public ACPanelBase,
    public Button::Listener,
    public ComboBox::Listener
{
public:

    ACBottomLeftPanel(ParametricEQ_ACAudioProcessor* inProcessor);
    ~ACBottomLeftPanel();

    void paint(Graphics& g) override;
    
    void resized() override;
    
    void buttonClicked (Button*) override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void loadImageFromFile(std::vector<std::unique_ptr<XmlElement>>& param);
    
//
//    DrawableButton db { "Button2", DrawableButton::ImageOnButtonBackground };
//    DrawableImage normal;
    

private:
    
    
    void displaySaveAsPopup();
    
    void updatePresetComboBox();
    
    std::unique_ptr<ComboBox> mPresetDisplay;
    
    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;
    
    std::unique_ptr<ACDisplayPresetButtons> mNewPresetImage, mSavePresetImage, mSaveAsPresetImage;
    
//    OwnedArray<Component> components;

    std::unique_ptr<ACDisplayPresetButtons> svg0;
    std::unique_ptr<ACDisplayPresetButtons> svg1;
//    std::unique_ptr<ACDisplayPresetButtons> svg2;
    
    std::vector<std::unique_ptr<ACDisplayPresetButtons>> svg4;
    std::vector<std::unique_ptr<XmlElement>> xmlImageFile;
    
//    template <typename ComponentType>
//    ComponentType* addToList (ComponentType* newComp)
//    {
//        components.add (newComp);
//        addAndMakeVisible (newComp);
//        return newComp;
//    }
    

    
};
