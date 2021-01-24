/*
  ==============================================================================

    ACBottomRightPanel.cpp
    Created: 3 Aug 2020 11:50:08am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACBottomRightPanel.h"

Component* getChildComponentWithName(Component* cParent, const juce::String& name)
{
    for(auto&& component : cParent->getChildren())
    {
        if (component->getName() == name)
            return component;
    }
    return nullptr;
}

ACBottomRightPanel::ACBottomRightPanel(ParametricEQ_ACAudioProcessor* inProcessor)
:   ACPanelBase(inProcessor)
{
    
    label = std::make_unique<Label>("", "Active");
    addAndMakeVisible(label.get());
    label->setJustificationType(Justification::centred);
    
    setupBypassButton(1, "EQ", true);
        
    // For FMode, LS, HS, "Mode1", "Mode2"
    //    LS->setButtonText("LS");
//    setupBypassButton(3, "EQMode", false);

    setSize(BOTTOM_RIGHT_PANEL_WIDTH, BOTTOM_RIGHT_PANEL_HEIGHT);

}

ACBottomRightPanel::~ACBottomRightPanel(){}

void ACBottomRightPanel::setupBypassButton(int cntrlPoints, juce::String name, bool attachmentCnt)
{
    for( int control = 0; control < cntrlPoints; ++control )
    {
        auto bypassButton = std::make_unique<TextButton>(name + String(control));
        bypassButton->addListener(this);
        addAndMakeVisible(bypassButton.get());
        
        if(attachmentCnt)
        {
            auto buttonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState1" /*+ String(sizeof(bypassButtons))*/, *bypassButton);
            EQAttachment.push_back(std::move(buttonAttachment));
        }

        bypassButton->onClick = [&] { updateToggleState(bypassButton.get(), "1"); };
        bypassButton->setClickingTogglesState(true);
        
        std::cout << bypassButton->getName() << std::endl;
        
        bypassButtons.push_back(std::move(bypassButton));
    }
}

void ACBottomRightPanel::paint(Graphics& g)
{
    ACPanelBase::paint(g);
    
    g.setColour(ACColour_1);
    Rectangle<float> area (15.f, 22.f, BOTTOM_PANEL_ACTIVE_PANEL_WIDTH + 5, BOTTOM_PANEL_ACTIVE_PANEL_HEIGHT + 2);
    g.fillRoundedRectangle(area, 3.f);
    
    g.setFont(font_1);

    g.setColour (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.drawFittedText ("Active:", 20, 22, 80, 20, Justification::left, 1);
}

void ACBottomRightPanel::resized()
{
    Rectangle<int> bounds = getLocalBounds().removeFromTop(30);
    // margin1 = 60
    bounds.removeFromLeft(60);
//
    Grid grid;

    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
//
    grid.templateRows    = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };

//    for(int i = 0; i < 5; ++i)
//    {
        grid.items = juce::GridItem (bypassButtons[0].get());
//    }
//
    grid.performLayout (bounds.removeFromTop(49));
//
//    Grid grid2;
//
//    using Track = Grid::TrackInfo;
//    using Fr = Grid::Fr;
//
//    grid.templateRows    = { Track (Fr (1)) };
//    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
//
//    grid.items =
//    {
//            juce::GridItem (bypassButtons[0].get())
//    };
//
//    // margin2 = 15
//    bounds.removeFromLeft(15);
//
//    grid.performLayout (bounds.removeFromTop(49));
    
    
    
    
//    auto bounds = getLocalBounds();
//    auto rectTop = bounds.removeFromTop (40);
//    bounds.removeFromRight (40);
//    bounds.reduce (40, 40);
//
//    rectTop.reduce (10, 0);
//    lookAndFeelButton->setBounds (rectTop.removeFromRight(120).withSizeKeepingCentre(120, 24));
//
//    Grid grid;
//    using Track = Grid::TrackInfo;
//    using Fr = Grid::Fr;
//
//    grid.items.add(GridItem(lpfSlider.get()));
//    grid.items.add(GridItem (volumeSlider.get()));
//
//    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
//    grid.templateRows = { Track (Fr (1)), Track (Fr (1)) };
//    grid.columnGap = Grid::Px (10);
//    grid.rowGap = Grid::Px (10);
}

void ACBottomRightPanel::buttonClicked (Button* b){}

void ACBottomRightPanel::updateToggleState (Button* button, juce::String name)
{
    auto state = button->getToggleState();
    juce::String stateString = state ? "ON" : "OFF";
    
    auto baseColour = juce::Colours::lightblue;
    if (state)
    {
        //EQ1 to 5 + LS & HS
        for( auto&& bypass : bypassButtons)
            bypass->setColour(juce::TextButton::buttonOnColourId, baseColour);

//        if (button == getChildComponentWithName(this, "EQ1")) // bypassButton[5].get())
//            button->setButtonText("Mode2");
//
//        for(int i = 0; i < 5; ++i)
            mProcessor->setClickedBypass(0);
//    }
//    //        if (FMode->clicked)
//    else if (button == getChildComponentWithName(this, "EQMode1"))
//        button->setButtonText("Mode1");
    }
}
