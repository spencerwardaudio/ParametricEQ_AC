/*
  ==============================================================================

    ACBottomPanel.cpp
    Created: 2 Aug 2020 6:55:45pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACBottomLeftPanel.h"

ACBottomLeftPanel::ACBottomLeftPanel(ParametricEQ_ACAudioProcessor* inProcessor)
:   ACPanelBase(inProcessor)
{
    int button_x = 15;
    int button_y = 20;
    int button_w = 30;
    int button_h = 25;
    
//    addAndMakeVisible(db);
//    db.setBounds (0, 0, 100, 40);
//    db.setImages (&normal, nullptr, nullptr);
    
    xmlImageFile.push_back (juce::parseXML (BinaryData::AC_New_svg));
    xmlImageFile.push_back (juce::parseXML (BinaryData::AC_Save_svg));
    xmlImageFile.push_back (juce::parseXML (BinaryData::AC_SaveAs_svg));
    
    loadImageFromFile(xmlImageFile);
    
    
    mNewPreset = std::make_unique<TextButton>();
    mNewPreset->setButtonText("");
    mNewPreset->setAlpha(0.9);
    mNewPreset->setBounds(button_x, button_y, button_w, button_h);

    mNewPreset->addListener(this);
    addAndMakeVisible(mNewPreset.get());
    
    
    button_x = button_x + button_w;

    mSavePreset = std::make_unique<TextButton>();
    mSavePreset->setButtonText("");
    mSavePreset->setBounds(button_x, button_y, button_w, button_h);
    mSavePreset->addListener(this);
    addAndMakeVisible(mSavePreset.get());
    button_x = button_x + button_w;
   
    mSaveAsPreset = std::make_unique<TextButton>();
    mSaveAsPreset->setButtonText("");
    mSaveAsPreset->setBounds(button_x, button_y, button_w, button_h);
    mSaveAsPreset->addListener(this);
    addAndMakeVisible(mSaveAsPreset.get());
    button_x = button_x + button_w;
    
//    svg[2]->setBounds(getLocalBounds().getX() + 60, getLocalBounds().getY() + 20, 20, 20);

    const int comboBox_w = 90;
    const int comboBox_x = 15;
    const int comboBox_y = button_y + 30;

    mPresetDisplay = std::make_unique<ComboBox>();
    mPresetDisplay->setBounds(comboBox_x, comboBox_y, comboBox_w, button_h);
    mPresetDisplay->addListener(this);
    addAndMakeVisible(mPresetDisplay.get());

    updatePresetComboBox();
    
    setSize(BOTTOM_PANEL_WIDTH, BOTTOM_PANEL_HEIGHT);

}

ACBottomLeftPanel::~ACBottomLeftPanel()
{
    
}

void ACBottomLeftPanel::paint(Graphics& g)
{
    ACPanelBase::paint(g);
    
    g.setColour(ACColour_1);
    g.setFont(font_2);
    
    
//    const int label_w = 220;

//    g.drawFittedText("PARAMETRIC EQ", BOTTOM_PANEL_WIDTH - (label_w + 50), 0, label_w, getHeight(), Justification::centredRight, 1);
}

void ACBottomLeftPanel::resized()
{
    svg0->setBounds(getLocalBounds().getX() + 20, getLocalBounds().getY() , 20, 20);
    svg1->setBounds(getLocalBounds().getX() + 40, getLocalBounds().getY(), 20, 20);
//    svg2->setBounds(getLocalBounds().getX() + 60, getLocalBounds().getY(), 20, 20);
    
//    svg4.at(0)->setBounds(getLocalBounds().getX() + 80, getLocalBounds().getY(), 20, 20);
//    if (svg[0] != nullptr)
//    {

//    }
//    if (svg[1] != nullptr)
//    {
//        svg[1]->setBounds(getLocalBounds().getX() + 24, getLocalBounds().getY() , 20, 20);
//    }
//    if (svg[2] != nullptr)
//    {
//        svg[2]->setBounds(getLocalBounds().getX() + 48 , getLocalBounds().getY() , 20, 20);
//    }
}


void ACBottomLeftPanel::buttonClicked (Button* b)
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
    
void ACBottomLeftPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == mPresetDisplay.get())
    {
        ACPresetManager* presetManager = mProcessor->getPresetManager();

        const int index = mPresetDisplay->getSelectedItemIndex();
        presetManager->loadPreset(index);  

    }
}

void ACBottomLeftPanel::displaySaveAsPopup()
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

void ACBottomLeftPanel::updatePresetComboBox()
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


void ACBottomLeftPanel::loadImageFromFile(std::vector<std::unique_ptr<XmlElement>>& param)
{

    std::cout << " image load is called " << std::endl;
    
//    auto svgImages[3] = { fileToLoad0, fileToLoad1, fileToLoad2 };
    
//    for (int i = 0; i < 3; ++i) {
    svg0 = std::make_unique<ACDisplayPresetButtons>(xmlImageFile.at(0));
    addAndMakeVisible(*svg0);
    svg1 = std::make_unique<ACDisplayPresetButtons>(xmlImageFile.at(1));
    addAndMakeVisible(*svg1);
//    svg2 = std::make_unique<ACDisplayPresetButtons>(xmlImageFile.at(2));
//    addAndMakeVisible(*svg2);
    
//    svg4[0] = std::make_unique<ACDisplayPresetButtons>(xmlImageFile.at(2));
//    addAndMakeVisible(*svg4[0]);
//    }

    
//    svg[1] = std::make_unique<ACDisplayPresetButtons>(fileToLoad1);
//    addAndMakeVisible(*svg[1]);
//
//    svg[1] = std::make_unique<ACDisplayPresetButtons>(fileToLoad0);
//    addAndMakeVisible(*svg[1]);
    
    resized();
}
