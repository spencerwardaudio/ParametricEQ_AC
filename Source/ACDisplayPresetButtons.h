/*
  ==============================================================================

    ACDisplayPresetButtons.h
    Created: 22 Aug 2020 11:11:09pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ACDisplayPresetButtons : public Component
{
public:
     explicit ACDisplayPresetButtons(const std::unique_ptr<XmlElement>& fileToLoad)
    {
        image1 = Drawable::createFromSVG(*fileToLoad);

        if (image1 != nullptr)
            addAndMakeVisible(*image1);
        
    }


    void resized() override
    {
        if (image1 != nullptr)
            image1->setBounds(getLocalBounds());

    }
        
    std::unique_ptr<Drawable> image1;
};
