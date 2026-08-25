#include "inventory.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &Inventory::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &Inventory::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "template_slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_items", "items"), &Inventory::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Inventory::get_items);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_items", "get_items");
}

void Inventory::set_slot(const Ref<InventorySlot> &p_slot) {
	if (slot.is_valid()) {
		slot->disconnect("changed", Callable(this, "_update_slots"));
		print_line("disconnected");
	}

	slot = p_slot;

	if (slot.is_valid()) {
		slot->connect("changed", Callable(this, "_update_slots"));
		print_line("connected");
	}

	// Set all slots to this slot
	_update_slots();
}
Ref<InventorySlot> Inventory::get_slot() const { return slot; }

void Inventory::_update_slots() {
	for (int64_t i = 0; i < slots.size(); i++) {
		Ref<InventoryItem> item = slot.is_valid() ? slot->get_item() : nullptr;
		Ref<InventorySlot> new_slot = slot.is_valid() ? Ref<InventorySlot>(slot->duplicate()) : memnew(InventorySlot);
		new_slot->set_item(item);
		slots[i] = new_slot;
	}
	print_line("_update_slots");
}

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> &p_items) {
	slots.resize(p_items.size());
	Ref<InventorySlot> slot;
	for (int64_t i = 0; i < p_items.size(); i++) {
		slot = slots[i];
		if (slot.is_valid()) {
			slot->set_item(p_items[i]);
			print_line("valid");
		} else {
			slot = memnew(InventorySlot);
			slot->set_item(p_items[i]);
			slots[i] = slot;
			print_line("invalid");
		}
	}
	print_line(slots);
}
TypedArray<Ref<InventoryItem>> Inventory::get_items() const {
	TypedArray<Ref<InventoryItem>> items = TypedArray<Ref<InventoryItem>>();
	items.resize(slots.size());
	for (int64_t i = 0; i < slots.size(); i++) {
		Ref<InventorySlot> slot = slots[i];
		if (slot.is_valid())
			items[i] = slot->get_item();
		else
			items[i] = nullptr;
	}
	return items;
}
