#pragma once

#include "godot_cpp/classes/node.hpp"
#include "resource/inventory_slot.hpp"


namespace godot {

class SlotNode : public Node {
    GDCLASS(SlotNode, Node)

private:
	Ref<InventorySlot> slot;

protected:
	static void _bind_methods();

public:
    void set_slot(Ref<InventorySlot> p_slot);
    Ref<InventorySlot> get_slot() const;
};
} //namespace godot
