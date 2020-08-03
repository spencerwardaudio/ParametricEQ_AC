/*
  ==============================================================================

    ACBottomPanel.cpp
    Created: 2 Aug 2020 6:55:45pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACBottomPanel.h"

ACBottomPanel::ACBottomPanel(MultiAvptslAudioProcessor* inProcessor)
:   ACPanelBase(inProcessor)
{
    setSize(BOTTOM_PANEL_WIDTH, BOTTOM_PANEL_HEIGHT);
    
    int button_x = 15;
    int button_y = 20;
    int button_w = 30;
    int button_h = 25;

    mNewPreset = std::make_unique<TextButton>();
    mNewPreset->setButtonText("N");
    mNewPreset->setBounds(button_x, button_y, button_w, button_h);
    mNewPreset->addListener(this);
    addAndMakeVisible(mNewPreset.get());
    button_x = button_x + button_w;

    mSavePreset = std::make_unique<TextButton>();
    mSavePreset->setButtonText("S");
    mSavePreset->setBounds(button_x, button_y, button_w, button_h);
    mSavePreset->addListener(this);
    addAndMakeVisible(mSavePreset.get());
    button_x = button_x + button_w;

    mSaveAsPreset = std::make_unique<TextButton>();
    mSaveAsPreset->setButtonText("S*");
    mSaveAsPreset->setBounds(button_x, button_y, button_w, button_h);
    mSaveAsPreset->addListener(this);
    addAndMakeVisible(mSaveAsPreset.get());
    button_x = button_x + button_w;

    const int comboBox_w = 90;
    const int comboBox_x = 15;
    const int comboBox_y = button_y + 30;

    mPresetDisplay = std::make_unique<ComboBox>();
    mPresetDisplay->setBounds(comboBox_x, comboBox_y, comboBox_w, button_h);
    mPresetDisplay->addListener(this);
    addAndMakeVisible(mPresetDisplay.get());

    updatePresetComboBox();
}

ACBottomPanel::~ACBottomPanel()
{
    
}

void ACBottomPanel::paint(Graphics& g)
{
    ACPanelBase::paint(g);
    
    g.setColour(ACColour_1);
    g.setFont(font_2);
    
//    const int label_w = 220;

//    g.drawFittedText("PARAMETRIC EQ", BOTTOM_PANEL_WIDTH - (label_w + 50), 0, label_w, getHeight(), Justification::centredRight, 1);
}

void ACBottomPanel::buttonClicked (Button* b)
{
    ACPresetManager* presetManager = mProcessor->getPresetManager();

    if(b == mNewPreset.get()){
        presetManager->createNewPreset();

        updatePresetComboBox();
    }

    else if (b == mSavePreset.get()) {
        presetManager->savePreset();
    }

    else if(b == mSaveAsPreset.get()) {
        displaySaveAsPopup();
    }
}
    
void ACBottomPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == mPresetDisplay.get())
    {
        ACPresetManager* presetManager = mProcessor->getPresetManager();

        const int index = mPresetDisplay->getSelectedItemIndex();
        presetManager->loadPreset(index);  

    }
}

void ACBottomPanel::displaySaveAsPopup()
{
    ACPresetManager* presetManager = mProcessor->getPresetManager();

    String currentPresetName = presetManager->getCurrentPresetName();

    AlertWindow window ("Save As", "Please enter a name for your Preset", AlertWindow::NoIcon);

    window.centreAroundComponent(this, getWidth(), getHeight());

    window.addTextEditor("presetName", currentPresetName, "preset name: ");

    window.addButton("Confirm", 1);

    window.addButton("Cancel", 0);

    if(window.runModalLoop()){

        String presetName = window.getTextEditor("presetName")->getText();
        presetManager->saveAsPreset(presetName);

        updatePresetComboBox();
    }
}

void ACBottomPanel::updatePresetComboBox()
{
    ACPresetManager* presetManager = mProcessor->getPresetManager();

    String presetName = presetManager->getCurrentPresetName();

    mPresetDisplay->clear(dontSendNotification);

    const int numPresets = presetManager->getNumberOfPresets();

    for(int i = 0; i < numPresets; i++){
        mPresetDisplay->addItem(presetManager->getPresetName(i), (i+1));
    }

    mPresetDisplay->setText(presetManager->getCurrentPresetName());
}


