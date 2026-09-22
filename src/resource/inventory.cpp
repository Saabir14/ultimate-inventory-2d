#include "inventory.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

#include "godot_cpp/core/error_macros.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot_template", "slot"), &Inventory::set_slot_template);
	ClassDB::bind_method(D_METHOD("get_slot_template"), &Inventory::get_slot_template);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot_template", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot_template", "get_slot_template");

	ClassDB::bind_method(D_METHOD("set_items", "items"), &Inventory::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Inventory::get_items);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_items", "get_items");

	ClassDB::bind_method(D_METHOD("set_item", "index", "item"), &Inventory::set_item);
	ClassDB::bind_method(D_METHOD("get_item", "index"), &Inventory::get_item);

	GDVIRTUAL_BIND(inventory_process, "delta");
}

void Inventory::set_slot_template(const Ref<InventorySlot> &p_slot) {
	if (slot_template == p_slot)
		return;

	slot_template = p_slot;
	ERR_FAIL_NULL(slot_template);

	for (int64_t i = 0; i < size(); i++) {
		// Replace current slot with duplicate of input slot keeping the item
		Ref<InventorySlot> new_slot = slot_template->duplicate(true);
		ERR_CONTINUE(new_slot.is_null());

		Ref<InventorySlot> current_slot = Object::cast_to<InventorySlot>(slots[i]);
		ERR_CONTINUE(current_slot.is_null());

		new_slot->set_item(current_slot->get_item());
		slots[i] = new_slot;
	}

	emit_changed();
}
Ref<InventorySlot> Inventory::get_slot_template() const { return slot_template; }

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> p_items) {
	ERR_FAIL_NULL(slot_template);

	const int64_t previous_size = size();
	const int64_t new_size = p_items.size();
	slots.resize(new_size);

	for (int64_t i = 0; i < new_size; i++) {
		Ref<InventorySlot> slot;
		if (i >= previous_size)
			slot = slot_template->duplicate(true);
		else
			slot = slots[i];

		slot->set_item(p_items[i]);
		slots[i] = slot;
	}

	emit_changed();
}
TypedArray<Ref<InventoryItem>> Inventory::get_items() const {
	TypedArray<Ref<InventoryItem>> items;
	items.resize(size());

	for (int64_t i = 0; i < size(); i++)
		items[i] = get_item(i);

	return items;
}

int64_t Inventory::size() const { return slots.size(); }

void Inventory::set_slot(int64_t p_index, const Ref<InventorySlot> &p_slot) {
	ERR_FAIL_INDEX(p_index, size());
	slots[p_index] = p_slot;
	emit_changed();
}
Ref<InventorySlot> Inventory::get_slot(int64_t p_index) const {
	ERR_FAIL_INDEX_V(p_index, size(), nullptr);
	return slots[p_index];
}

void Inventory::set_item(int64_t p_index, const Ref<InventoryItem> &p_item) {
	ERR_FAIL_INDEX(p_index, size());

	Ref<InventorySlot> slot = slots[p_index];
	ERR_FAIL_NULL(slot);

	slot->set_item(p_item);
}
Ref<InventoryItem> Inventory::get_item(int64_t p_index) const {
	ERR_FAIL_INDEX_V(p_index, size(), nullptr);

	Ref<InventorySlot> slot = slots[p_index];
	ERR_FAIL_NULL_V(slot, nullptr);

	return slot->get_item();
}

void Inventory::inventory_process(double delta) {
    GDVIRTUAL_CALL(inventory_process, delta);
}
