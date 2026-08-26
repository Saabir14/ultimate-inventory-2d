#include "inventory.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
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

	ClassDB::bind_method(D_METHOD("set_size", "size"), &Inventory::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &Inventory::get_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("attempt_resize", "size"), &Inventory::attempt_resize);
	ClassDB::bind_method(D_METHOD("resize", "size"), &Inventory::resize);

	ClassDB::bind_method(D_METHOD("place_item", "item", "index"), &Inventory::place_item, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("place_item_from_slot", "slot", "index"), &Inventory::place_item_from_slot, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("swap_item_from_slot", "slot", "index"), &Inventory::swap_item_from_slot);
}

void Inventory::set_slot(const Ref<InventorySlot> &p_slot) {
	if (slot.is_valid())
		slot->disconnect("changed", callable_mp(this, &Inventory::_update_slots));

	slot = p_slot;

	if (slot.is_valid())
		slot->connect("changed", callable_mp(this, &Inventory::_update_slots));

	// Set all slots to this slot
	_update_slots();
}
Ref<InventorySlot> Inventory::get_slot() const { return slot; }

void Inventory::_update_slots() {
	for (int64_t i = 0; i < slots.size(); i++) {
		Ref<InventoryItem> item = Ref<InventorySlot>(slots[i])->get_item();
		Ref<InventorySlot> new_slot = slot.is_valid() ? Ref<InventorySlot>(slot->duplicate()) : memnew(InventorySlot);
		new_slot->set_item(item);
		slots[i] = new_slot;
	}
}

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> &p_items) {
	slots.resize(p_items.size());
	Ref<InventorySlot> slot;
	for (int64_t i = 0; i < p_items.size(); i++) {
		slot = slots[i];
		if (slot.is_valid()) {
			slot->set_item(p_items[i]);
		} else {
			slot = memnew(InventorySlot);
			slot->set_item(p_items[i]);
			slots[i] = slot;
		}
	}
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

void Inventory::set_size(int64_t size) { slots.resize(size); }
int64_t Inventory::get_size() const { return slots.size(); }

bool Inventory::attempt_resize(int64_t size) {
    if (size >= get_size()) {
        set_size(size);
        return true;
    }

    // If size is smaller, pack items into new size
    // Check if fitting is pausible by finding the difference and counting empty slots
    int64_t d = get_size() - size;
    // Count for empty slots
    int64_t empty_slots = 0;
    for (int64_t i = 0; i < get_size(); i++) {
        Ref<InventorySlot> slot = slots[i];
        if (slot.is_null() || slot->get_item().is_null())
            empty_slots++;
    }

    if (empty_slots < d)
        return false;

    // Remove last empty slot in slots
    while (d) {
        for (int64_t i = get_size() - 1; i >= 0; i--) {
            Ref<InventorySlot> slot = slots[i];
            if (slot.is_null() || slot->get_item().is_null()) {
                slots.remove_at(i);
                d--;
                break;
            }
        }
    }

    return true;
}

TypedArray<Ref<InventoryItem>> Inventory::resize(int64_t size) {
    if (size >= get_size()) {
        set_size(size);
        return TypedArray<Ref<InventoryItem>>();
    }

    // Remove last empty slot in slots
    int64_t d = get_size() - size;
    while (d) {
        for (int64_t i = get_size() - 1; i >= 0; i--) {
            Ref<InventorySlot> slot = slots[i];
            if (slot.is_null() || slot->get_item().is_null()) {
                slots.remove_at(i);
                d--;
                break;
            }
        }
    }

    // If still to small, remove ending items and return them
    TypedArray<Ref<InventoryItem>> removed_items = TypedArray<Ref<InventoryItem>>();
    while (get_size() > size) {
        Ref<InventorySlot> slot = slots.pop_back();
        removed_items.append(slot->get_item());
    }

    return removed_items;
}

bool Inventory::place_item(const Ref<InventoryItem> &p_item, int64_t index) {
    // If index < 0, test all slots one by one
    if (index < 0) {
        for (int64_t i = 0; i < slots.size(); i++)
            if (Ref<InventorySlot>(slots[i])->place_item(p_item))
                return true;
        return false;
    }

    // If index >= 0, test only that slot
    return Ref<InventorySlot>(slots[index])->place_item(p_item);
}

bool Inventory::place_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index) {
	// If index < 0, test all slots one by one
	if (index < 0) {
		for (int64_t i = 0; i < slots.size(); i++)
			if (Ref<InventorySlot>(slots[i])->place_item_from_slot(p_slot))
				return true;
		return false;
	}

	// If index >= 0, test only that slot
	return Ref<InventorySlot>(slots[index])->place_item_from_slot(p_slot);
}

bool Inventory::swap_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index) {
	return Ref<InventorySlot>(slots[index])->swap_item_from_slot(p_slot);
}
