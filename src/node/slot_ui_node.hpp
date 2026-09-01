#pragma once

#include "godot_cpp/classes/node.hpp"
#include "resource/inventory_slot.hpp"

namespace godot {

// SlotNode holds a slot
// handles instantiating the UI scene of the slot
class SlotUiNode : public Node {
	GDCLASS(SlotUiNode, Node)

private:
	Ref<InventorySlot> slot;
	Node *item_ui_holder;

protected:
	static void _bind_methods();

	void _update_item_ui();

public:
	void set_slot(Ref<InventorySlot> p_slot);
	Ref<InventorySlot> get_slot() const;

	void set_item_ui_holder(Node *p_holder);
	Node *get_item_ui_holder() const;
};
} //namespace godot
