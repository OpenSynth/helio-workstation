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

#include "Common.h"
#include "PianoTrackTreeItem.h"
#include "PianoSequence.h"
#include "ProjectTreeItem.h"
#include "TreeItemChildrenSerializer.h"
#include "Icons.h"
#include "Pattern.h"

#include "MainLayout.h"
#include "Instrument.h"
#include "OrchestraPit.h"

#include "Delta.h"
#include "PianoTrackDiffLogic.h"

PianoTrackTreeItem::PianoTrackTreeItem(const String &name) :
    MidiTrackTreeItem(name, Serialization::Core::pianoTrack)
{
    this->sequence = new PianoSequence(*this, *this);
    this->pattern = new Pattern(*this, *this);

    // this will be set by transport
    //this->layer->setInstrumentId(this->workspace.getDefaultInstrument()->getInstrumentID());

    /*this->vcsDiffLogic = new VCS::PianoTrackDiffLogic(*this);

    using namespace Serialization::VCS;
    this->deltas.add(new VCS::Delta({}, MidiTrackDeltas::trackPath));
    this->deltas.add(new VCS::Delta({}, MidiTrackDeltas::trackMute));
    this->deltas.add(new VCS::Delta({}, MidiTrackDeltas::trackColour));
    this->deltas.add(new VCS::Delta({}, MidiTrackDeltas::trackInstrument));
    this->deltas.add(new VCS::Delta({}, PianoSequenceDeltas::notesAdded));
    this->deltas.add(new VCS::Delta({}, PatternDeltas::clipsAdded));*/
}

Image PianoTrackTreeItem::getIcon() const noexcept
{
    return Icons::findByName(Icons::pianoTrack, HEADLINE_ICON_SIZE);
}

/*
int PianoTrackTreeItem::getNumDeltas() const
{
    return this->deltas.size();
}
*/

void PianoTrackTreeItem::selectAllPianoSiblings(PianoTrackTreeItem *layerItem)
{
    // select all layers in the project
    const auto pianoTreeItems = layerItem->getProject()->findChildrenOfType<PianoTrackTreeItem>();
    
    for (PianoTrackTreeItem *siblingPianoItem : pianoTreeItems)
    {
        if (siblingPianoItem != layerItem) // already selected, so don't blink twice
        {
            siblingPianoItem->setSelected(false, false, sendNotification);
            siblingPianoItem->setSelected(true, false, sendNotification);
        }
    }
    
    layerItem->setSelected(false, false, sendNotification);
    layerItem->setSelected(true, false, sendNotification);
}

//===----------------------------------------------------------------------===//
// VCS stuff
//===----------------------------------------------------------------------===//

/*
VCS::Delta *PianoTrackTreeItem::getDelta(int index) const
{
    using namespace Serialization::VCS;
    if (this->deltas[index]->hasType(PianoSequenceDeltas::notesAdded))
    {
        const int numEvents = this->getSequence()->size();

        if (numEvents == 0)
        {
            this->deltas[index]->setDescription(VCS::DeltaDescription("empty sequence"));
        }
        else
        {
            this->deltas[index]->setDescription(VCS::DeltaDescription("{x} notes", numEvents));
        }
    }
    else if (this->deltas[index]->hasType(PatternDeltas::clipsAdded))
    {
        const int numClips = this->getPattern()->size();

        if (numClips == 0)
        {
            this->deltas[index]->setDescription(VCS::DeltaDescription("empty pattern"));
        }
        else
        {
            this->deltas[index]->setDescription(VCS::DeltaDescription("{x} clips", numClips));
        }
    }

    return this->deltas[index];
}

ValueTree PianoTrackTreeItem::serializeDeltaData(int deltaIndex) const
{
    using namespace Serialization::VCS;
    if (this->deltas[deltaIndex]->hasType(MidiTrackDeltas::trackPath))
    {
        return this->serializePathDelta();
    }
    if (this->deltas[deltaIndex]->hasType(MidiTrackDeltas::trackMute))
    {
        return this->serializeMuteDelta();
    }
    else if (this->deltas[deltaIndex]->hasType(MidiTrackDeltas::trackColour))
    {
        return this->serializeColourDelta();
    }
    else if (this->deltas[deltaIndex]->hasType(MidiTrackDeltas::trackInstrument))
    {
        return this->serializeInstrumentDelta();
    }
    else if (this->deltas[deltaIndex]->hasType(PianoSequenceDeltas::notesAdded))
    {
        return this->serializeEventsDelta();
    }
    else if (this->deltas[deltaIndex]->hasType(PatternDeltas::clipsAdded))
    {
        return this->serializeClipsDelta();
    }

    jassertfalse;
    return {};
}

VCS::DiffLogic *PianoTrackTreeItem::getDiffLogic() const
{
    return this->vcsDiffLogic;
}

void PianoTrackTreeItem::resetStateTo(const VCS::TrackedItem &newState)
{
    using namespace Serialization::VCS;
    for (int i = 0; i < newState.getNumDeltas(); ++i)
    {
        const VCS::Delta *newDelta = newState.getDelta(i);
        const auto newDeltaData(newState.serializeDeltaData(i));
        
        if (newDelta->hasType(MidiTrackDeltas::trackPath))
        {
            this->resetPathDelta(newDeltaData);
        }
        else if (newDelta->hasType(MidiTrackDeltas::trackMute))
        {
            this->resetMuteDelta(newDeltaData);
        }
        else if (newDelta->hasType(MidiTrackDeltas::trackColour))
        {
            this->resetColourDelta(newDeltaData);
        }
        else if (newDelta->hasType(MidiTrackDeltas::trackInstrument))
        {
            this->resetInstrumentDelta(newDeltaData);
        }
        // the current layer state is supposed to have
        // a single note delta of type PianoSequenceDeltas::notesAdded
        else if (newDelta->hasType(PianoSequenceDeltas::notesAdded))
        {
            this->resetEventsDelta(newDeltaData);
        }
        // same rule applies to clips state:
        else if (newDelta->hasType(PatternDeltas::clipsAdded))
        {
            this->resetClipsDelta(newDeltaData);
        }
    }
}
*/

//===----------------------------------------------------------------------===//
// Serializable
//===----------------------------------------------------------------------===//

ValueTree PianoTrackTreeItem::serialize() const
{
    ValueTree tree(Serialization::Core::treeItem);

    // this->serializeVCSUuid(tree);

    tree.setProperty(Serialization::Core::treeItemType, this->type, nullptr);
    tree.setProperty(Serialization::Core::treeItemName, this->name, nullptr);

    this->serializeTrackProperties(tree);

    tree.appendChild(this->sequence->serialize(), nullptr);
    tree.appendChild(this->pattern->serialize(), nullptr);

    TreeItemChildrenSerializer::serializeChildren(*this, tree);

    return tree;
}

void PianoTrackTreeItem::deserialize(const ValueTree &tree)
{
    this->reset();

    // this->deserializeVCSUuid(tree);
    this->deserializeTrackProperties(tree);

    forEachValueTreeChildWithType(tree, e, Serialization::Midi::track)
    {
        this->sequence->deserialize(e);
    }

    forEachValueTreeChildWithType(tree, e, Serialization::Midi::pattern)
    {
        this->pattern->deserialize(e);
    }

    // Proceed with basic properties and children
    TreeItem::deserialize(tree);
}


//===----------------------------------------------------------------------===//
// Deltas
//===----------------------------------------------------------------------===//

/*
ValueTree PianoTrackTreeItem::serializePathDelta() const
{
    using namespace Serialization::VCS;
    ValueTree tree(MidiTrackDeltas::trackPath);
    tree.setProperty(delta, this->getTrackName(), nullptr);
    return tree;
}

ValueTree PianoTrackTreeItem::serializeMuteDelta() const
{
    using namespace Serialization::VCS;
    ValueTree tree(MidiTrackDeltas::trackMute);
    tree.setProperty(delta, this->getTrackMuteStateAsString(), nullptr);
    return tree;
}

ValueTree PianoTrackTreeItem::serializeColourDelta() const
{
    using namespace Serialization::VCS;
    ValueTree tree(MidiTrackDeltas::trackColour);
    tree.setProperty(delta, this->getTrackColour().toString(), nullptr);
    return tree;
}

ValueTree PianoTrackTreeItem::serializeInstrumentDelta() const
{
    using namespace Serialization::VCS;
    ValueTree tree(MidiTrackDeltas::trackInstrument);
    tree.setProperty(delta, this->getTrackInstrumentId(), nullptr);
    return tree;
}

ValueTree PianoTrackTreeItem::serializeEventsDelta() const
{
    ValueTree tree(Serialization::VCS::PianoSequenceDeltas::notesAdded);
    for (int i = 0; i < this->getSequence()->size(); ++i)
    {
        const MidiEvent *event = this->getSequence()->getUnchecked(i);
        tree.appendChild(event->serialize(), nullptr);
    }

    return tree;
}

void PianoTrackTreeItem::resetPathDelta(const ValueTree &state)
{
    jassert(state.hasType(Serialization::VCS::MidiTrackDeltas::trackPath));
    const String &path(state.getProperty(Serialization::VCS::delta));
    this->setXPath(path, false);
}

void PianoTrackTreeItem::resetMuteDelta(const ValueTree &state)
{
    jassert(state.hasType(Serialization::VCS::MidiTrackDeltas::trackMute));
    const String &muteState(state.getProperty(Serialization::VCS::delta));
    const bool willMute = MidiTrack::isTrackMuted(muteState);
    
    if (willMute != this->isTrackMuted())
    {
        this->setTrackMuted(willMute, false);
    }
}

void PianoTrackTreeItem::resetColourDelta(const ValueTree &state)
{
    jassert(state.hasType(Serialization::VCS::MidiTrackDeltas::trackColour));
    const String &colourString(state.getProperty(Serialization::VCS::delta));
    const Colour &colour(Colour::fromString(colourString));

    if (colour != this->getTrackColour())
    {
        this->setTrackColour(colour, false);
    }
}

void PianoTrackTreeItem::resetInstrumentDelta(const ValueTree &state)
{
    jassert(state.hasType(Serialization::VCS::MidiTrackDeltas::trackInstrument));
    const String &instrumentId(state.getProperty(Serialization::VCS::delta));
    this->setTrackInstrumentId(instrumentId, false);
}

void PianoTrackTreeItem::resetEventsDelta(const ValueTree &state)
{
    jassert(state.hasType(Serialization::VCS::PianoSequenceDeltas::notesAdded));

    this->getSequence()->reset();
    forEachValueTreeChildWithType(state, e, Serialization::Midi::note)
    {
        this->getSequence()->checkoutEvent<Note>(e);
    }

    this->getSequence()->updateBeatRange(false);
}
*/
