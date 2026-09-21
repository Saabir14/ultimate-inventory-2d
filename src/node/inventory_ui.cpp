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

	ClassDB::bind_method(D_METHOD("_update_ui"), &InventoryUI::_update_ui);
}

InventoryUI::InventoryUI() {
	set_process_internal(true);
}

void InventoryUI::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_EXIT_TREE:
			_queue_free_slot_ui_nodes();
			break;

		case NOTIFICATION_ENTER_TREE:
			call_deferred("_update_ui");
			break;

		case NOTIFICATION_PARENTED:
			_update_ui();
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

	for (int64_t i = 0; i < slot_ui_nodes.size(); i++) {
		Node *node = Object::cast_to<Node>(slot_ui_nodes[i]);
		ERR_CONTINUE(!node);
		node->queue_free();
	}

	slot_ui_nodes.clear();
}

void InventoryUI::_instantiate_slot_ui_nodes() {
	Node *parent = get_parent();

	if (!inventory_holder || !parent)
		return;

	Ref<Inventory> inventory = inventory_holder->get_inventory();
	if (inventory.is_null())
		return;

	slot_ui_nodes.resize(inventory->size());

	for (int64_t i = 0; i < inventory->size(); i++) {
		Ref<InventorySlot> slot = inventory->get_slot(i);
		ERR_CONTINUE(slot.is_null());

		SlotUI *slot_ui = slot->instantiate_slot_ui();
		slot_ui_nodes[i] = slot_ui;
		ERR_CONTINUE(!slot_ui);

		// Add slots right below (behind) this node
		parent->add_child(slot_ui);
		parent->move_child(slot_ui, get_index() + 1 + i);
	}
}

void InventoryUI::_update_ui() {
	_queue_free_slot_ui_nodes();
	_instantiate_slot_ui_nodes();
}
