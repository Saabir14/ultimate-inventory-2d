#include "slot_ui_node.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "node/item_ui_node.hpp"
#include "resource/inventory_item.hpp"

using namespace godot;

void SlotUiNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &SlotUiNode::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &SlotUiNode::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_item_ui_holder", "holder"), &SlotUiNode::set_item_ui_holder);
	ClassDB::bind_method(D_METHOD("get_item_ui_holder"), &SlotUiNode::get_item_ui_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_holder", PROPERTY_HINT_NODE_TYPE), "set_item_ui_holder", "get_item_ui_holder");
}

void SlotUiNode::set_slot(Ref<InventorySlot> p_slot) { slot = p_slot; }
Ref<InventorySlot> SlotUiNode::get_slot() const { return slot; }

void SlotUiNode::set_item_ui_holder(Node *p_holder) {
	item_ui_holder = p_holder;
	_update_item_ui();
}
Node *SlotUiNode::get_item_ui_holder() const { return item_ui_holder; }

void SlotUiNode::_update_item_ui() {
	if (item_ui_holder == nullptr)
		return;

	// Free children
	for (int32_t i = 0; i < item_ui_holder->get_child_count(); i++) {
		Node *child = item_ui_holder->get_child(i);
		item_ui_holder->remove_child(child);
		child->queue_free();
	}

	if (slot.is_null())
		return;

	const Ref<InventoryItem> item = slot->get_item();
	if (item.is_null())
		return;

	// Instantiate item ui scene
	ItemUiNode *item_ui = item->instantiate_item_ui();
	if (item_ui == nullptr)
		return;

	// Add item ui as child of item ui holder
	item_ui_holder->add_child(item_ui, false);
}
