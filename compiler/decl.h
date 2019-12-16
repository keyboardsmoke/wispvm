#pragma once

namespace compiler
{
    namespace ast
    {

        class Decl
        {
        public:
            virtual ~Decl();

            bool IsValid() const
            {
                return m_invalid == 0;
            }

            void SetValid(bool valid)
            {
                m_invalid = !valid;
            }

            bool HasAttrs() const
            {
                return m_hasAttrs == 1;
            }

            bool IsUsed() const
            {
                return m_used == 1;
            }

            void SetUsed(bool used)
            {
                m_used = used;
            }

            bool IsReferenced() const
            {
                return m_referenced == 1;
            }

            void SetReferenced(bool referenced)
            {
                m_referenced = referenced;
            }

            uint32 GetStartIndex()
            {
                return m_range.sourceIndex;
            }

            uint32 GetEndIndex()
            {
                return m_range.sourceIndex + m_range.sourceLength;
            }

            Decl* GetTranslationUnit();
            Decl* GetParentFunctionOrMethod();

            virtual Statement* GetBody() const { return nullptr; }
            virtual bool HasBody() const { return GetBody() != nullptr; }

        private:
            /// The location of this decl.
            SourceRange m_range;

            /// InvalidDecl - This indicates a semantic error occurred.
            bool m_invalid : 1;

            /// HasAttrs - This indicates whether the decl has attributes or not.
            bool m_hasAttrs : 1;

            /// Whether this declaration was "used", meaning that a definition is
            /// required.
            bool m_used : 1;

            /// Whether this declaration was "referenced".
            /// The difference with 'm_used' is whether the reference appears in a
            /// evaluated context or not, e.g. functions used in other unreferenced functions
            /// are regarded as "referenced" but not "used".
            unsigned m_referenced : 1;

            // TODO: Store attributes


            LexicalToken m_token;
        };

        class NamedDecl : public Decl
        {
        public:
            std::string name;

        private:
            Visibility visibility;
        };

        class ValueDecl
        {

        };

        class DeclaratorDecl
        {

        };

        class FunctionDecl : public Node
        {

        };

        class VariableDecl : public Node
        {

        };



    }
}