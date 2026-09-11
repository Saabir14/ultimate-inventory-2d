#include "slot_ui.hpp"

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "node/item_ui.hpp"

using namespace godot;

void InventorySlotUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventorySlotUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventorySlotUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");

	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &InventorySlotUI::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &InventorySlotUI::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_item_ui_holder", "holder"), &InventorySlotUI::set_item_ui_holder);
	ClassDB::bind_method(D_METHOD("get_item_ui_holder"), &InventorySlotUI::get_item_ui_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_holder", PROPERTY_HINT_NODE_TYPE), "set_item_ui_holder", "get_item_ui_holder");
}

void InventorySlotUI::set_slot(Ref<InventorySlot> p_slot) {
	if (slot.is_valid())
		slot->disconnect("changed", callable_mp(this, &InventorySlotUI::_update_ui));

	slot = p_slot;

	if (slot.is_valid())
		slot->connect("changed", callable_mp(this, &InventorySlotUI::_update_ui));

	_update_ui();
}
Ref<InventorySlot> InventorySlotUI::get_slot() const { return slot; }

void InventorySlotUI::set_item(Ref<InventoryItem> p_item) {
	ERR_FAIL_COND_MSG(slot.is_null(), "Can't set item of null slot");
	slot->set_item(p_item);
}
Ref<InventoryItem> InventorySlotUI::get_item() const {
	if (slot.is_null())
		return nullptr;
	return slot->get_item();
}

void InventorySlotUI::set_item_ui_holder(Node *p_holder) {
	item_ui_holder = p_holder;
	_update_ui();
}
Node *InventorySlotUI::get_item_ui_holder() const { return item_ui_holder; }

void InventorySlotUI::_update_ui() {
	if (item_ui_holder == nullptr)
		return;

	// Free item ui
	if (item_ui) {
		item_ui_holder->remove_child(item_ui);
		item_ui->queue_free();
		item_ui = nullptr;
	}

	if (slot.is_null())
		return;

	const Ref<InventoryItem> item = slot->get_item();
	if (item.is_null())
		return;

	// Instantiate item ui scene
	item_ui = item->instantiate_item_ui(PackedScene::GEN_EDIT_STATE_INSTANCE);
	if (item_ui == nullptr)
		return;

	// Add item ui as child of item ui holder
	item_ui_holder->add_child(item_ui, false);
}

Variant InventorySlotUI::_get_drag_data(const Vector2 &p_position) {
	if (slot.is_null() || slot->get_item().is_null())
		return Variant();

	set_drag_preview(_get_drag_preview());

	return slot;
}

Control *InventorySlotUI::_get_drag_preview() {
	const Vector2 size = Vector2(50, 50);
	Control *holder = memnew(Control);
	holder->add_child(duplicate());
	holder->set_size(size);
	holder->set_anchors_and_offsets_preset(PRESET_CENTER);
	return holder;
}

bool InventorySlotUI::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const {
	Ref<InventorySlot> other_slot = p_data;
	return other_slot.is_valid() && other_slot != slot;
}

void InventorySlotUI::_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
	Ref<InventorySlot> other_slot = p_data;
	if (other_slot.is_null() || slot.is_null())
		return;

	other_slot->interact_item_from_slot(slot);
}
