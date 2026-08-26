#include "slot_node.hpp"

using namespace godot;

void SlotNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &SlotNode::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &SlotNode::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");
}

void SlotNode::set_slot(Ref<InventorySlot> p_slot) { slot = p_slot; }
Ref<InventorySlot> SlotNode::get_slot() const { return slot; }
