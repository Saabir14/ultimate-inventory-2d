#include "inventory_ui.hpp"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "node/slot_ui.hpp"
#include "resource/inventory.hpp"

using namespace godot;

void InventoryUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_holder", "inventory_holder"), &InventoryUI::set_inventory_holder);
	ClassDB::bind_method(D_METHOD("get_inventory_holder"), &InventoryUI::get_inventory_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory_holder", PROPERTY_HINT_NODE_TYPE, "InventoryHolder"), "set_inventory_holder", "get_inventory_holder");
}

InventoryUI::InventoryUI() {
	set_process_internal(true);
}

void InventoryUI::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_UNPARENTED:
			_queue_free_slot_ui_nodes();
			break;

		case NOTIFICATION_PARENTED:
			_instantiate_slot_ui_nodes();
			index = get_index();
			break;

		case NOTIFICATION_INTERNAL_PROCESS:
			const int32_t new_index = get_index();
			if (new_index == index)
				break;
			index = new_index;
			_update_ui();
			break;
	}
}

void InventoryUI::set_inventory_holder(InventoryHolder *p_inventory_holder) {
	if (inventory_holder == p_inventory_holder)
		return;

	if (inventory_holder)
		inventory_holder->disconnect("inventory_set", callable_mp(this, &InventoryUI::_update_ui));

	inventory_holder = p_inventory_holder;

	if (inventory_holder)
		inventory_holder->connect("inventory_set", callable_mp(this, &InventoryUI::_update_ui));

	update_configuration_warnings();
	_update_ui();
}
InventoryHolder *InventoryUI::get_inventory_holder() const { return inventory_holder; }

PackedStringArray InventoryUI::_get_configuration_warnings() const {
	PackedStringArray warnings;

	if (!inventory_holder)
		warnings.push_back("An InventoryHolder node must be provided for InventoryUI to function. Please assign an InventoryHolder to it!");

	return warnings;
}

void InventoryUI::_queue_free_slot_ui_nodes() {
	if (slot_ui_nodes.is_empty())
		return;

	for (Variant slot_node : slot_ui_nodes) {
		if (Node *node = Object::cast_to<Node>(slot_node))
			node->queue_free();
	}

	slot_ui_nodes.clear();
}

void InventoryUI::_instantiate_slot_ui_nodes() {
	Node *parent = get_parent();
	ERR_FAIL_NULL(parent);

	if (!inventory_holder)
		return;

	Ref<Inventory> inventory = inventory_holder->get_inventory();
	if (inventory.is_null())
		return;

	slot_ui_nodes.resize(inventory->size());

	for (int64_t i = slot_ui_nodes.size() - 1; i >= 0; i--) {
		Ref<InventorySlot> slot = inventory->get_slot(i);
		if (slot.is_null())
			continue;

		SlotUI *node = slot->instantiate_slot_ui(PackedScene::GEN_EDIT_STATE_INSTANCE);
		slot_ui_nodes[i] = node;

		// Add slots right below (behind) this node
		if (node) {
			parent->add_child(node);
			parent->move_child(node, get_index() + 1);
		}
	}
}

void InventoryUI::_update_ui() {
	_queue_free_slot_ui_nodes();
	_instantiate_slot_ui_nodes();
}
