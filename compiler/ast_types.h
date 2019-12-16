#pragma once

namespace compiler
{
    namespace ast
    {
        enum class Linkage
        {
            /// No linkage, which means that the entity is unique and
            /// can only be referred to from within its scope.
            NoLinkage = 0,

            /// Internal linkage, which indicates that the entity can
            /// be referred to from within the translation unit (but not other
            /// translation units).
            InternalLinkage,

            /// External linkage, which indicates that the entity can
            /// be referred to from other translation units.
            ExternalLinkage
        };

        enum class Visibility
        {
            /// Objects with "hidden" visibility are not seen by the dynamic
            /// linker.
            HiddenVisibility,

            /// Objects with "protected" visibility are seen by the dynamic
            /// linker but always dynamically resolve to an object within this
            /// shared object.
            ProtectedVisibility,

            /// Objects with "default" visibility are seen by the dynamic linker
            /// and act like normal objects.
            DefaultVisibility
        };

        struct SourceRange
        {
            SourceRange() :
                line(0), col(0), sourceIndex(0), sourceLength(0) {}

            uint32 line;
            uint32 col;
            uint32 sourceIndex;
            uint32 sourceLength;
        };
    }
}