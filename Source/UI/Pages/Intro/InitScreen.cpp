/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

//[Headers]
#include "Common.h"
//[/Headers]

#include "InitScreen.h"

//[MiscUserDefs]
#include "App.h"
#include "LogoFader.h"
#include "MainWindow.h"
#include "HelioTheme.h"
#include "MainLayout.h"
#include "Workspace.h"
#include "DataEncoder.h"
#include "CommandIDs.h"
#include "App.h"
//[/MiscUserDefs]

InitScreen::InitScreen()
{
    addAndMakeVisible (bg = new PanelBackgroundA());
    addAndMakeVisible (logoImage = new LogoFader());


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);

    //[Constructor]
    this->setOpaque(true);
    this->rebound();
    this->toFront(false);
    this->setAlwaysOnTop(true);
    //[/Constructor]
}

InitScreen::~InitScreen()
{
    //[Destructor_pre]
    //const int animTime = SHORT_FADE_TIME(this);
    //Desktop::getInstance().getAnimator().fadeOut(this, animTime);
    //[/Destructor_pre]

    bg = nullptr;
    logoImage = nullptr;

    //[Destructor]
    //[/Destructor]
}

void InitScreen::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void InitScreen::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    bg->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
    logoImage->setBounds ((getWidth() / 2) - (300 / 2), (getHeight() / 2) + -50 - (300 / 2), 300, 300);
    //[UserResized] Add your own custom resize handling here..
    logoImage->setCentrePosition(this->getWidth() / 2, (this->getHeight() / 2) - 5);
    //[/UserResized]
}

void InitScreen::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    //[/UserCode_visibilityChanged]
}

void InitScreen::parentHierarchyChanged()
{
    //[UserCode_parentHierarchyChanged] -- Add your code here...
    this->rebound();
    //[/UserCode_parentHierarchyChanged]
}

void InitScreen::parentSizeChanged()
{
    //[UserCode_parentSizeChanged] -- Add your code here...
    if (this->isVisible())
    {
        this->rebound();

        #if HELIO_DESKTOP
        this->logoImage->startFade();
        #endif

        this->startTimer(1500);
    }
    //[/UserCode_parentSizeChanged]
}

void InitScreen::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    if (commandId == CommandIDs::InitWorkspace)
    {
        App::Workspace().init();
        App::Layout().init();
    }
    //[/UserCode_handleCommandMessage]
}


//[MiscUserCode]

void InitScreen::timerCallback()
{
    const bool isShiftPressed = Desktop::getInstance().getMainMouseSource().getCurrentModifiers().isShiftDown();
    if (! isShiftPressed)
    {
        this->stopTimer();
        this->postCommandMessage(CommandIDs::InitWorkspace);
    }
}

void InitScreen::rebound()
{
    //Logger::writeToLog("> " + String(this->getWidth()) + " : " + String(this->getHeight()));
    this->setBounds(0, 0, this->getParentWidth(), this->getParentHeight());
}

//[/MiscUserCode]

#if 0
/*
BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="InitScreen" template="../../Template"
                 componentName="" parentClasses="public Component, private Timer"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <METHODS>
    <METHOD name="parentHierarchyChanged()"/>
    <METHOD name="parentSizeChanged()"/>
    <METHOD name="visibilityChanged()"/>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="0"/>
  <JUCERCOMP name="" id="66a1a33f06322bfb" memberName="bg" virtualName=""
             explicitFocusOrder="0" pos="0 0 0M 0M" sourceFile="../Themes/PanelBackgroundA.cpp"
             constructorParams=""/>
  <GENERICCOMPONENT name="" id="ea1b592642055bdc" memberName="logoImage" virtualName=""
                    explicitFocusOrder="0" pos="0Cc -50Cc 300 300" class="LogoFader"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif