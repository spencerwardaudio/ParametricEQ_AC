/*
  ==============================================================================

    ACPresetManager.cpp
    Created: 2 Aug 2020 3:09:05pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "ACPresetManager.h"

#if JUCE_WINDOWS
    static const String directorySeparator = "\\";
#elif JUCE_MAC
    static const String directorySeparator = "/";
#endif

ACPresetManager::ACPresetManager(AudioProcessor* inProcessor)
:   mCurrentPresetIsSaved(false),
    mCurrentPresetName("Untitled"),
    mProcessor(inProcessor)
{
    const String pluginName = (String) mProcessor->getName();
//    File& operator= (const String& newAbsolutePath);

    mPresetDirectory =
    File("/Users/Shared").getFullPathName()+directorySeparator+"AudioCrossover"+directorySeparator+pluginName;
    
//    mPresetDirectory =
//    File::getSpecialLocation(File::commonApplicationDataDirectory::).getFullPathName()+directorySeparator+pluginName;

    if(!File(mPresetDirectory).exists()){
        File(mPresetDirectory).createDirectory();
    }

    storeLocalPreset();
}
ACPresetManager::~ACPresetManager()
{

}

void ACPresetManager::getXmlForPreset(XmlElement* inElement)
{
    const int numParameters = mProcessor->getNumParameters();

    for(int i = 0; i < numParameters; i++)
    {
        inElement->setAttribute(mProcessor->getParameterName(i),
                                mProcessor->getParameter(i));
    }
}

void ACPresetManager::loadPresetForXml(XmlElement* inElement)
{
    mCurrentPresetXml = inElement;

    for(int i = 0; i < mCurrentPresetXml->getNumAttributes(); i++)
    {
        const String name = mCurrentPresetXml->getAttributeName(i);
        const float value = mCurrentPresetXml->getDoubleAttribute(name);

        for(int j = 0; j < mProcessor->getNumParameters(); j++)
        {
            if(mProcessor->getParameterName(j) == name){
                mProcessor->setParameterNotifyingHost(j, value);
            }
        }
    }
}

int ACPresetManager::getNumberOfPresets()
{
    return mLocalPresets.size();
}

String ACPresetManager::getPresetName(int inPresetIndex)
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void ACPresetManager::createNewPreset()
{
    const int numParameters = mProcessor->getNumParameters();

    for(int i = 0; i < numParameters; i++)
    {
        mProcessor->setParameterNotifyingHost(i, mProcessor->getParameterDefaultValue(i));
    }

    mCurrentPresetIsSaved = false;
    mCurrentPresetName = "Untitled";
}

void ACPresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    
    mCurrentlyLoadedPreset.deleteFile();

    mCurrentlyLoadedPreset.appendData(destinationData.getData(),
                                      destinationData.getSize());

    mCurrentPresetIsSaved = true;
}

void ACPresetManager::saveAsPreset(String inPresetName)
{
    File presetFile = File(mPresetDirectory + directorySeparator + inPresetName + PRESET_FILE_EXTENTION);

    if(!presetFile.exists()){
        presetFile.create();
    } else {
        presetFile.deleteFile();
    }

    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);

    presetFile.appendData(destinationData.getData(),
                          destinationData.getSize());

    mCurrentPresetIsSaved = true;
    mCurrentPresetName = inPresetName;

    storeLocalPreset();
}

void ACPresetManager::loadPreset(int inPresetIndex)
{
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];

    MemoryBlock presetBinary;

    if(mCurrentlyLoadedPreset.loadFileAsData(presetBinary)){
        mCurrentPresetIsSaved = true;
        mCurrentPresetName = getPresetName(inPresetIndex);
        mProcessor->setStateInformation(presetBinary.getData(),
                                        (int)presetBinary.getSize());
    }
}

bool ACPresetManager::getIsCurrentPresetSaved()
{
    return mCurrentPresetIsSaved;
}

String ACPresetManager::getCurrentPresetName()
{
    return mCurrentPresetName;
}

void ACPresetManager::storeLocalPreset()
{
    mLocalPresets.clear();

    for(DirectoryIterator di (File(mPresetDirectory),
                              false,
                              "*"+(String)PRESET_FILE_EXTENTION,
        File::TypesOfFileToFind::findFiles); di.next();)
    {
        File preset = di.getFile();
        mLocalPresets.add(preset);
    }
}
