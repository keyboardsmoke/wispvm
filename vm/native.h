#pragma once

// Modules need to be defined in one big list
// It must contain the native, the name, and the index in the native list
// The name and the index will be used by the lexer/compiler to associate native calls with their index
// and the native function will be looked up by the index by the VM when a CALL_NATIVE instruction happens.

namespace wisp
{
    class Vm;
    class State;

    typedef std::function<void(Vm*, State*)> NativeDefinition;

    class Module;
    class NativeList
    {
    public:
        std::vector<NativeDefinition>& GetNatives()
        {
            return m_natives;
        }

        NativeDefinition GetNative(uint32 index)
        {
            return m_natives[index];
        }

        uint32 AddNative(NativeDefinition native)
        {
            uint32 idx = static_cast<uint32>(m_natives.size());
            m_natives.emplace_back(native);
            return idx;
        }

    private:
        std::vector<NativeDefinition> m_natives;
    };
}