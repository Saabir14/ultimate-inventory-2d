#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

namespace godot {

// Inventory resource to store inventory items in a list of slots
// Use this for any inentory containing more than one item
class Inventory : public Resource {
	GDCLASS(Inventory, Resource)

private:
    // Tempelate slot to use for all array slots
    Ref<InventorySlot> slot;
	// All inventory items stored in a TypedArray of Ref<InventoryItem>
	TypedArray<Ref<InventoryItem>> items;

protected:
	static void _bind_methods();

public:
    void set_slot(const Ref<InventorySlot> &p_slot);
    Ref<InventorySlot> get_slot() const;

    void set_items(const TypedArray<Ref<InventoryItem>> &p_items);
    TypedArray<Ref<InventoryItem>> get_items() const;
};

} //namespace godot
