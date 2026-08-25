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
    // Template slot to use for all array slots
    Ref<InventorySlot> slot;

	TypedArray<Ref<InventorySlot>> slots;
	void _update_slots();

protected:
	static void _bind_methods();

public:
    void set_slot(const Ref<InventorySlot> &p_slot);
    Ref<InventorySlot> get_slot() const;

    void set_items(const TypedArray<Ref<InventoryItem>> &p_items);
    TypedArray<Ref<InventoryItem>> get_items() const;

    int64_t get_size() const;

    bool place_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index = -1);
    bool swap_item_from_slot(const Ref<InventorySlot> &p_slot, int64_t index);
};

} //namespace godot
