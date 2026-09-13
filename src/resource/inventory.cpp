#include "inventory.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

#include "godot_cpp/core/error_macros.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot_template", "slot"), &Inventory::set_slot_template);
	ClassDB::bind_method(D_METHOD("get_slot_template"), &Inventory::get_slot_template);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot_template", "get_slot_template");

	ClassDB::bind_method(D_METHOD("set_items", "items"), &Inventory::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Inventory::get_items);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_items", "get_items");
}

void Inventory::set_slot_template(const Ref<InventorySlot> &p_slot) {
	if (slot_template == p_slot)
		return;

	if (p_slot.is_null()) {
		slot_template.unref();
		slots.clear();
		ERR_FAIL_MSG("Slot set to null, all items have been cleared");
	}

	slot_template = p_slot;

	if (slot_template.is_null())
		return;

	for (int64_t i = 0; i < slots.size(); i++) {
		// Replace current slot with duplicate of input slot keeping the item
		Ref<InventorySlot> new_slot = slot_template->duplicate(true);
		new_slot->set_item(Object::cast_to<InventorySlot>(slots[i])->get_item());
		slots[i] = new_slot;
	}

	emit_changed();
}
Ref<InventorySlot> Inventory::get_slot_template() const { return slot_template; }

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> p_items) {
	if (slot_template.is_null()) {
		ERR_FAIL_MSG("slot is null, can't set items");
	}

	const int64_t previous_size = slots.size();
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

	if (previous_size != new_size)
		emit_changed();
}
TypedArray<Ref<InventoryItem>> Inventory::get_items() const {
	TypedArray<Ref<InventoryItem>> items;
	items.resize(slots.size());

	for (int64_t i = 0; i < slots.size(); i++)
		items[i] = Object::cast_to<InventorySlot>(slots[i])->get_item();

	return items;
}

int64_t Inventory::size() { return slots.size(); }

Ref<InventorySlot> Inventory::get_slot(int64_t p_index) const { return slots[p_index]; }

void Inventory::set_slot(int64_t p_index, const Ref<InventorySlot> &p_slot) { slots[p_index] = p_slot; }
Ref<InventoryItem> Inventory::get_item(int64_t p_index) const { return Object::cast_to<InventorySlot>(slots[p_index])->get_item(); }
