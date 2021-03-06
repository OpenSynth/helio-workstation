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

#pragma once

// belongs to ProjectTreeItem

class MidiRollEditMode : public ChangeBroadcaster
{
public:
    
    enum Mode
    {
        defaultMode                      = 0x01,
        drawMode                         = 0x02,
        selectionMode                    = 0x03,
        zoomMode                         = 0x04,
        dragMode                         = 0x05,
        insertSpaceMode                  = 0x06,
        wipeSpaceMode                    = 0x07,
        scissorsMode                     = 0x08,
    };
    
    MidiRollEditMode();
    
    bool forbidsViewportDragging() const;
    bool forcesViewportDragging() const;
    
    bool forbidsViewportZooming() const;
    bool forcesViewportZooming() const;
    
    bool forbidsSelectionMode() const;
    bool forcesSelectionMode() const;
    
    bool forbidsAddingEvents() const;
    bool forcesAddingEvents() const;

    bool forbidsSpaceWipe() const;
    bool forcesSpaceWipe() const;

    bool forbidsSpaceInsert() const;
    bool forcesSpaceInsert() const;

    bool shouldInteractWithChildren() const;
    MouseCursor getCursor() const;
    
    void unsetLastMode();
    void setMode(Mode newMode, bool force = false);
    bool isMode(Mode targetMode) const;
    
private:
    
    Mode mode;

    Mode previousMode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiRollEditMode)
    
};
