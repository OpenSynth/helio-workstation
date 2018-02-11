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

#include "DiffLogic.h"
#include "Delta.h"

namespace VCS
{
    class ProjectInfoDiffLogic : public DiffLogic
    {
    public:

        explicit ProjectInfoDiffLogic(TrackedItem &targetItem);

        //===--------------------------------------------------------------===//
        // DiffLogic
        //===--------------------------------------------------------------===//

        const Identifier getType() const override;
        void resetStateTo(const TrackedItem &newState) override;
        Diff *createDiff(const TrackedItem &initialState) const override;
        Diff *createMergedItem(const TrackedItem &initialState) const override;

    private:

        ValueTree mergePath(const ValueTree &state, const ValueTree &changes) const;
        ValueTree mergeFullName(const ValueTree &state, const ValueTree &changes) const;
        ValueTree mergeAuthor(const ValueTree &state, const ValueTree &changes) const;
        ValueTree mergeDescription(const ValueTree &state, const ValueTree &changes) const;

        DeltaDiff createPathDiff(const ValueTree &state, const ValueTree &changes) const;
        DeltaDiff createFullNameDiff(const ValueTree &state, const ValueTree &changes) const;
        DeltaDiff createAuthorDiff(const ValueTree &state, const ValueTree &changes) const;
        DeltaDiff createDescriptionDiff(const ValueTree &state, const ValueTree &changes) const;

    };
} // namespace VCS
