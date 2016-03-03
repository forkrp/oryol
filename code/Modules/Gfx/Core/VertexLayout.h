#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::VertexLayout
    @ingroup Gfx
    @brief describes the data layout of a vertex in a vertex buffer
*/
#include "Core/Assertion.h"
#include "Core/Containers/StaticArray.h"
#include "Gfx/Core/Enums.h"
#include "Gfx/Core/GfxConfig.h"

namespace Oryol {
    
class VertexLayout {
public:
    /// a component in the vertex layout
    class Component {
    public:
        /// default constructor
        Component();
        /// construct from vertex attr and format
        Component(VertexAttr::Code attr, VertexFormat::Code format, uint8 slot=0);
        /// constructor for instanced component
        static Component Instanced(VertexAttr::Code attr, VertexFormat::Code format, uint8 slot=0);
        /// return true if valid (attr and format set)
        bool IsValid() const;
        /// clear the component (unset attr and format)
        void Clear();
        /// get byte size of component
        int32 ByteSize() const;

        VertexAttr::Code Attr;
        VertexFormat::Code Format;
        uint8 SlotIndex;
        VertexStepFunction::Code StepFunction;
        uint8 StepRate;
    };

    /// constructor
    VertexLayout();
    /// clear the vertex layout, chainable
    VertexLayout& Clear();
    /// return true if layout is empty
    bool Empty() const;
    /// add a component
    VertexLayout& Add(const Component& comp);
    /// add component by name and format
    VertexLayout& Add(VertexAttr::Code attr, VertexFormat::Code format, uint8 slot=0);
    /// add an instanced component by name and format
    VertexLayout& AddInstanced(VertexAttr::Code attr, VertexFormat::Code format, uint8 slot=1);
    /// append components from other vertex layout, fails hard on components collision
    VertexLayout& Append(const VertexLayout& other);
    /// get number of components
    int32 NumComponents() const;
    /// get component at index
    const Component& ComponentAt(int32 index) const;
    /// get component index by vertex attribute, return InvalidIndex if layout doesn't include attr
    int32 ComponentIndexByVertexAttr(VertexAttr::Code attr) const;
    /// get byte size of vertex
    int32 ByteSize() const;
    /// get byte offset of a component
    int32 ComponentByteOffset(int32 componentIndex) const;
    /// test if the layout contains a specific vertex attribute
    bool Contains(VertexAttr::Code attr) const;
    /// compute a hash value of a vertex layout
    uint64 Hash() const;

    /// compute a combined hash value of 2 vertex layout (used for mesh/vertex shader layout matching)
    static uint64 CombinedHash(const VertexLayout& l0, const VertexLayout& l1);

private:
    StaticArray<Component, GfxConfig::MaxNumVertexLayoutComponents> comps;
    StaticArray<uint8, GfxConfig::MaxNumVertexLayoutComponents> byteOffsets;
    StaticArray<int8, VertexAttr::NumVertexAttrs> attrCompIndices;  // maps vertex attributes to component indices
    int8 numComps;
    uint8 byteSize;
};

//------------------------------------------------------------------------------
inline
VertexLayout::Component::Component() :
Attr(VertexAttr::InvalidVertexAttr),
Format(VertexFormat::InvalidVertexFormat),
SlotIndex(0),
StepFunction(VertexStepFunction::PerVertex),
StepRate(0) {
    // empty
}

//------------------------------------------------------------------------------
inline
VertexLayout::Component::Component(VertexAttr::Code attr, VertexFormat::Code fmt, uint8 slot) :
Attr(attr),
Format(fmt),
SlotIndex(slot),
StepFunction(VertexStepFunction::PerVertex),
StepRate(0) {
    // empty
}

//------------------------------------------------------------------------------
inline VertexLayout::Component
VertexLayout::Component::Instanced(VertexAttr::Code attr, VertexFormat::Code fmt, uint8 slot) {
    Component comp(attr, fmt, slot);
    comp.StepFunction = VertexStepFunction::PerInstance;
    comp.StepRate = 1;
    return comp;
}

//------------------------------------------------------------------------------
inline bool
VertexLayout::Component::IsValid() const {
    return (VertexAttr::InvalidVertexAttr != this->Attr);
}

//------------------------------------------------------------------------------
inline void
VertexLayout::Component::Clear() {
    this->Attr = VertexAttr::InvalidVertexAttr;
    this->Format = VertexFormat::InvalidVertexFormat;
}

//------------------------------------------------------------------------------
inline int32
VertexLayout::Component::ByteSize() const {
    return VertexFormat::ByteSize(this->Format);
}

//------------------------------------------------------------------------------
inline int32
VertexLayout::ComponentIndexByVertexAttr(VertexAttr::Code attr) const {
    return this->attrCompIndices[attr];
}

//------------------------------------------------------------------------------
inline bool
VertexLayout::Contains(VertexAttr::Code attr) const {
    return InvalidIndex != this->ComponentIndexByVertexAttr(attr);
}

//------------------------------------------------------------------------------
inline bool
VertexLayout::Empty() const {
    return 0 == this->numComps;
}

//------------------------------------------------------------------------------
inline int32
VertexLayout::NumComponents() const {
    return this->numComps;
}

//------------------------------------------------------------------------------
inline const class VertexLayout::Component&
VertexLayout::ComponentAt(int32 index) const {
    return this->comps[index];
}

//------------------------------------------------------------------------------
inline int32
VertexLayout::ByteSize() const {
    return this->byteSize;
}

//------------------------------------------------------------------------------
inline int32
VertexLayout::ComponentByteOffset(int32 index) const {
    return this->byteOffsets[index];
}

} // namespace Oryol
