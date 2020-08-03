/*
  ==============================================================================

    ACPresetManager.h
    Created: 2 Aug 2020 3:09:05pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define PRESET_FILE_EXTENTION ".acp"

class ACPresetManager
{
    
public:
    
    ACPresetManager(AudioProcessor* inProcessor);
    ~ACPresetManager();
    
    void getXmlForPreset(XmlElement* inElement);
    
    void loadPresetForXml(XmlElement* inElement);
    
    int getNumberOfPresets();
    
    String getPresetName(int inPresetIndex);
    
    void createNewPreset();
    
    void savePreset();
    
    void saveAsPreset(String inPresetName);
    
    void loadPreset(int inPresetIndex);
    
    bool getIsCurrentPresetSaved();
    
    String getCurrentPresetName();
    
    
private:
    
    void storeLocalPreset();
    
    bool mCurrentPresetIsSaved;
    
    File mCurrentlyLoadedPreset;
    
    Array<File> mLocalPresets;
    
    String mCurrentPresetName;
    
    String mPresetDirectory;
    
    XmlElement* mCurrentPresetXml;
    AudioProcessor* mProcessor;
};
