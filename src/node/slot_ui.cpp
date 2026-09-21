#include "slot_ui.hpp"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "node/item_ui.hpp"

using namespace godot;

void SlotUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &SlotUI::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &SlotUI::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");

	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &SlotUI::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &SlotUI::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_item_ui_holder", "holder"), &SlotUI::set_item_ui_holder);
	ClassDB::bind_method(D_METHOD("get_item_ui_holder"), &SlotUI::get_item_ui_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_ui_holder", PROPERTY_HINT_NODE_TYPE, "Control"), "set_item_ui_holder", "get_item_ui_holder");
}

void SlotUI::set_slot(Ref<InventorySlot> p_slot) {
	if (slot == p_slot)
		return;

	if (slot.is_valid())
		slot->disconnect("item_changed", callable_mp(this, &SlotUI::_update_ui));

	slot = p_slot;

	if (slot.is_valid())
		slot->connect("item_changed", callable_mp(this, &SlotUI::_update_ui));

	_update_ui();
}
Ref<InventorySlot> SlotUI::get_slot() const { return slot; }

void SlotUI::set_item(Ref<InventoryItem> p_item) {
	if (slot.is_null() && !is_node_ready())
		return;

	ERR_FAIL_NULL(slot);

	if (slot->get_item() == p_item)
		return;

	slot->set_item(p_item);
}
Ref<InventoryItem> SlotUI::get_item() const {
	if (slot.is_null())
		return nullptr;
	return slot->get_item();
}

void SlotUI::set_item_ui_holder(Node *p_holder) {
	if (item_ui_holder == p_holder)
		return;

	item_ui_holder = p_holder ? p_holder : this;
	_update_ui();
}
Node *SlotUI::get_item_ui_holder() const { return item_ui_holder; }

void SlotUI::_update_ui() {
	// Free item ui
	int32_t index = 0;
	if (item_ui) {
		index = item_ui->get_index();
		item_ui->queue_free();
		item_ui = nullptr;
	}

	if (!item_ui_holder)
		return;

	ERR_FAIL_NULL(slot);

	const Ref<InventoryItem> item = slot->get_item();
	if (item.is_null())
		return;

	// Instantiate item ui scene
	item_ui = item->instantiate_item_ui();
	ERR_FAIL_NULL(item_ui);

	// Add item ui as child of item ui holder
	item_ui_holder->call_deferred("add_child", item_ui);
	item_ui_holder->call_deferred("move_child", item_ui, index);
}

Variant SlotUI::_get_drag_data(const Vector2 &p_position) {
	if (slot.is_null())
		return Variant();

	if (slot->get_item().is_null())
		return Variant();

	set_drag_preview(_get_drag_preview());

	return slot;
}

Control *SlotUI::_get_drag_preview() {
	const Vector2 size = get_custom_minimum_size();
	Control *holder = memnew(Control);
	holder->add_child(duplicate());
	holder->set_size(size);
	holder->set_anchors_and_offsets_preset(PRESET_CENTER);
	return holder;
}

bool SlotUI::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const {
	Ref<InventorySlot> other_slot = p_data;
	return other_slot.is_valid() && other_slot != slot;
}

void SlotUI::_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
	Ref<InventorySlot> other_slot = p_data;
	if (other_slot.is_null() || slot.is_null())
		return;

	other_slot->interact_item_from_slot(slot);
}
