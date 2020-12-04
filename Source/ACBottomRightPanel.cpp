/*
  ==============================================================================

    ACBottomRightPanel.cpp
    Created: 3 Aug 2020 11:50:08am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACBottomRightPanel.h"

ACBottomRightPanel::ACBottomRightPanel(ParametricEQ_ACAudioProcessor* inProcessor)
:   ACPanelBase(inProcessor)
{
    
    label = std::make_unique<Label>("", "Active");
    addAndMakeVisible(label.get());
    label->setJustificationType(Justification::centred);

    int button_x = 60;
    int button_y = 26;
    int button_w = 15;
    int button_h = 15;

    
    EQ1 = std::make_unique<TextButton>();
    EQ1->setBounds(button_x, button_y, button_w, button_h);
    EQ1->addListener(this);
    addAndMakeVisible(EQ1.get());
    
    EQ1Attachments = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState1", *EQ1);
    
    button_x = button_x + button_w;
    
    EQ2 = std::make_unique<TextButton>();
    EQ2->setBounds(button_x, button_y, button_w, button_h);
    EQ2->addListener(this);
    addAndMakeVisible(EQ2.get());
    
    EQ2Attachments = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState2", *EQ2);
    
    button_x = button_x + button_w;
    
    EQ3 = std::make_unique<TextButton>();
    EQ3->setBounds(button_x, button_y, button_w, button_h);
    EQ3->addListener(this);
    addAndMakeVisible(EQ3.get());
    
    EQ3Attachments = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState3", *EQ3);
    
    button_x = button_x + button_w;
    
    EQ4 = std::make_unique<TextButton>();
    EQ4->setBounds(button_x, button_y, button_w, button_h);
    EQ4->addListener(this);
    addAndMakeVisible(EQ4.get());
    
    EQ4Attachments = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState4", *EQ4);
    
    button_x = button_x + button_w;
    
    EQ5 = std::make_unique<TextButton>();
    EQ5->setBounds(button_x, button_y, button_w, button_h);
    EQ5->addListener(this);
    addAndMakeVisible(EQ5.get());
    
    EQ5Attachments = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(mProcessor->getTreeState(), "ActiveState5", *EQ5);
    
    //Second Row
    button_x = 15;
    button_y = button_y + button_h + 8;
    button_w = 55;
    button_h = 30;
    
    FMode = std::make_unique<TextButton>();
    FMode->setButtonText("Mode1");
    FMode->setBounds(button_x, button_y, button_w, button_h);
    FMode->addListener(this);
    addAndMakeVisible(FMode.get());
    button_x = button_x + button_w;
    
    button_w = 35;
    
    LS = std::make_unique<TextButton>();
    LS->setButtonText("LS");
    LS->setBounds(button_x, button_y, button_w, button_h);
    LS->addListener(this);
    addAndMakeVisible(LS.get());
    button_x = button_x + button_w;
    
    HS = std::make_unique<TextButton>();
    HS->setButtonText("HS");
    HS->setBounds(button_x, button_y, button_w, button_h);
    HS->addListener(this);
    addAndMakeVisible(HS.get());
    button_x = button_x + button_w;
    
    
    EQ1->onClick = [this] { updateToggleState (EQ1.get(), "1"); };
    EQ2->onClick = [this] { updateToggleState (EQ2.get(), "2"); };
    EQ3->onClick = [this] { updateToggleState (EQ3.get(), "3"); };
    EQ4->onClick = [this] { updateToggleState (EQ4.get(), "4"); };
    EQ5->onClick = [this] { updateToggleState (EQ5.get(), "5"); };
    
    FMode->onClick = [this] { updateToggleState (FMode.get(), "Mode1"); };
    
    LS->onClick = [this] { updateToggleState (LS.get(), "LS"); };
    HS->onClick = [this] { updateToggleState (HS.get(), "HS"); };
    
    EQ1->setClickingTogglesState(true);
    EQ2->setClickingTogglesState(true);
    EQ3->setClickingTogglesState(true);
    EQ4->setClickingTogglesState(true);
    EQ5->setClickingTogglesState(true);
    
    FMode->setClickingTogglesState(true);
    
    LS->setClickingTogglesState(true);
    HS->setClickingTogglesState(true);
    
    setSize(BOTTOM_RIGHT_PANEL_WIDTH, BOTTOM_RIGHT_PANEL_HEIGHT);

}

ACBottomRightPanel::~ACBottomRightPanel()
{
    
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

}

void ACBottomRightPanel::buttonClicked (Button* b)
{

}

void ACBottomRightPanel::updateToggleState (Button* button, juce::String name)
{
    auto state = button->getToggleState();
    juce::String stateString = state ? "ON" : "OFF";
    
    auto baseColour = juce::Colours::lightblue;
    if (state)
    {
        EQ1->setColour(juce::TextButton::buttonOnColourId, baseColour);
        EQ2->setColour(juce::TextButton::buttonOnColourId, baseColour);
        EQ3->setColour(juce::TextButton::buttonOnColourId, baseColour);
        EQ4->setColour(juce::TextButton::buttonOnColourId, baseColour);
        EQ5->setColour(juce::TextButton::buttonOnColourId, baseColour);
 
        if (button == FMode.get())
        {
        FMode->setButtonText("Mode2");
        }
        
        LS->setColour(juce::TextButton::buttonOnColourId, baseColour);
        HS->setColour(juce::TextButton::buttonOnColourId, baseColour);
        
        mProcessor->setClickedBypass(0);
        mProcessor->setClickedBypass(1);
        mProcessor->setClickedBypass(2);
        mProcessor->setClickedBypass(3);
        mProcessor->setClickedBypass(4);
    }
    else if (button == FMode.get())
    {
//        if (FMode->clicked)
        FMode->setButtonText("Mode1");
    }

}
